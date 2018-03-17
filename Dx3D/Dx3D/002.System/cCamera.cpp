#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
    : m_fDistance(3)
    , m_vEye(0, LOOKAT_POS, -m_fDistance)
    , m_vLookAt(0, LOOKAT_POS, 0)
    , m_vUp(0, 1, 0)
    , m_fRotX(0.0f)
    , m_fRotY(0.0f)
    , m_isRButtonDown(false)
    , m_isFocus(false)
{
}


cCamera::~cCamera()
{
}

HRESULT cCamera::Setup()
{
    RECT rc;
    GetClientRect(g_hWnd, &rc);

    Matrix4 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

    return S_OK;
}

ULONG cCamera::Release()
{
    return cObject::Release();
}

void cCamera::Update(Vector3* pTarget)
{
    // x축 회전은 -90 ~ 90 으로 고정
    if (m_fRotX < -LIMITED_ROT + D3DX_16F_EPSILON)
    {
        m_fRotX = -LIMITED_ROT + D3DX_16F_EPSILON;
    }
    else if (m_fRotX > LIMITED_ROT - D3DX_16F_EPSILON)
    {
        m_fRotX = LIMITED_ROT - D3DX_16F_EPSILON;
    }

    if (m_fRotY >= 360.0f)
    {
        m_fRotY -= 360.0f;
    }
    // 뷰 매트릭스 셋팅 - 타겟 == 큐브의 포지션
    m_vEye = Vector3(0, 0, -m_fDistance);

    Matrix4 matRotX, matRotY;
    D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fRotX));
    D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fRotY));

    D3DXVec3TransformCoord(&m_vEye, &m_vEye, &(matRotX * matRotY));

    Vector3 vDirZ, vDirX;
    D3DXVec3Normalize(&vDirZ, &m_vEye);
    D3DXVec3Cross(&vDirX, &vDirZ, &Vector3(0, 1, 0));
    D3DXVec3Cross(&vDirZ, &vDirX, &Vector3(0, 1, 0));

    if (pTarget)
    {
        m_vLookAt = *pTarget;
        m_vLookAt.y += LOOKAT_POS;
        m_vEye = m_vEye + *pTarget;
    }

    Matrix4 matView;
    D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &Vector3(0, 1, 0));
    g_pDevice->SetTransform(D3DTS_VIEW, &matView);

    g_vCameraPos = m_vEye;
}

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        m_ptPrevMouse.x = LOWORD(lParam);
        m_ptPrevMouse.y = HIWORD(lParam);
        m_isRButtonDown = true;
    }
    break;
    case WM_LBUTTONUP:
    {
        m_isRButtonDown = false;
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (m_isRButtonDown)
        {
            POINT ptCurrMouse;
            ptCurrMouse.x = LOWORD(lParam);
            ptCurrMouse.y = HIWORD(lParam);

            m_fRotY += (ptCurrMouse.x - m_ptPrevMouse.x) / 5.0f;
            m_fRotX += (ptCurrMouse.y - m_ptPrevMouse.y) / 5.0f;

            // x축 회전은 -90 ~ 90 으로 고정
            if (m_fRotX < -LIMITED_ROT + D3DX_16F_EPSILON)
            {
                m_fRotX = -LIMITED_ROT + D3DX_16F_EPSILON;
            }
            else if (m_fRotX > LIMITED_ROT - D3DX_16F_EPSILON)
            {
                m_fRotX = LIMITED_ROT - D3DX_16F_EPSILON;
            }

            m_ptPrevMouse = ptCurrMouse;
        }
    }
    break;
    case WM_MOUSEWHEEL:
    {
        if (m_fDistance > 2)
        {
            m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f;
        }
        else
        {
            m_fDistance = 2;
            if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)
                m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f;
        }
    }
    break;
    }
}
