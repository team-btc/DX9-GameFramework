#include "stdafx.h"
#include "cPicking.h"


cPicking::cPicking()
    : m_pSphere(NULL)
    , m_vPickedPos(0, 0, 0)
    , m_pSphere2(NULL)
{
}


cPicking::~cPicking()
{
    SAFE_RELEASE(m_pSphere);
    SAFE_RELEASE(m_pSphere2);
}

void cPicking::Setup()
{
    D3DXCreateSphere(g_pDevice, 1.0f, 10, 10, &m_pSphere, NULL);

    Color c = D3DCOLOR_XRGB(128, 50, 50);
    m_vecPCVertex.push_back(ST_PC_VERTEX(Vector3(-15, 0, -15), c));
    m_vecPCVertex.push_back(ST_PC_VERTEX(Vector3(-15, 0, 15), c));
    m_vecPCVertex.push_back(ST_PC_VERTEX(Vector3(15, 0, 15), c));
    m_vecPCVertex.push_back(ST_PC_VERTEX(Vector3(-15, 0, -15), c));
    m_vecPCVertex.push_back(ST_PC_VERTEX(Vector3(15, 0, 15), c));
    m_vecPCVertex.push_back(ST_PC_VERTEX(Vector3(15, 0, -15), c));

    D3DXCreateSphere(g_pDevice, 2.0f, 10, 10, &m_pSphere2, NULL);

    for (int i = 0; i < 50; ++i)
    {
        ST_SPHERE stSphere;
        stSphere.fRadius = 2.0f;
        stSphere.isPicked = false;
        stSphere.vCenter = Vector3(rand() % 501 / 10.0f - 25,
                                   rand() % 501 / 10.0f - 25,
                                   rand() % 501 / 10.0f - 25);
        m_vecSphere.push_back(stSphere);
    }
}

void cPicking::Render()
{
    // == 충돌 용 바닥
    Matrix4 matI;
    D3DXMatrixIdentity(&matI);

    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    g_pDevice->SetTransform(D3DTS_WORLD, &matI);
    g_pDevice->SetFVF(ST_PC_VERTEX::FVF);
    g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
                                  (UINT)(m_vecPCVertex.size() / 3),
                                  &m_vecPCVertex[0],
                                  (UINT)sizeof(ST_PC_VERTEX));

    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

    // == 바닥과 충돌 지점에 구 그리기
    Matrix4 matT;
    D3DXMatrixTranslation(&matT, m_vPickedPos.x, m_vPickedPos.y, m_vPickedPos.z);

    if (m_pSphere)
    {
        g_pDevice->SetTransform(D3DTS_WORLD, &matT);
        g_pDevice->SetMaterial(&BLUE_MTRL);
        m_pSphere->DrawSubset(0);
    }

    // == 픽킹용 구 그리기
    for (int i = 0; i < (int)m_vecSphere.size(); ++i)
    {
        D3DXMatrixTranslation(&matT,
                              m_vecSphere[i].vCenter.x,
                              m_vecSphere[i].vCenter.y,
                              m_vecSphere[i].vCenter.z);

        if (m_pSphere2)
        {
            g_pDevice->SetTransform(D3DTS_WORLD, &matT);
            if (m_vecSphere[i].isPicked)
                g_pDevice->SetMaterial(&RED_MTRL);
            else
                g_pDevice->SetMaterial(&WHITE_MTRL);
            m_pSphere2->DrawSubset(0);
        }
    }
}

void cPicking::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_MBUTTONDOWN:
        {
            // 마우스 좌표를 받아서 월드 공간에서의 레이를 만든다.
            cRay ray = cRay::RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
            for (int i = 0; i < (int)m_vecPCVertex.size(); i += 3)
            {
                float dist;
                // 모든 면에 대해서 충돌 검사
                if (D3DXIntersectTri(&m_vecPCVertex[i + 0].p, &m_vecPCVertex[i + 1].p,
                                     &m_vecPCVertex[i + 2].p, &ray.m_vOrg, &ray.m_vDir, 0, 0, &dist))
                {
                    // 충돌 위치 계산
                    m_vPickedPos = ray.m_vOrg + ray.m_vDir * dist;
                    break;
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            // 마우스 좌표를 받아서 월드 공간에서의 레이를 만든다.
            cRay ray = cRay::RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
            for (int i = 0; i < (int)m_vecSphere.size(); ++i)
            {
                m_vecSphere[i].isPicked = ray.IsPicked(&m_vecSphere[i]);
            }
        }
        break;
    }
}
