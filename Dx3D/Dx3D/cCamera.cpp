#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
    : m_fDistance(200)
    , m_vEye(0, LOOKAT_POS, -m_fDistance)
    , m_vLookAt(0, LOOKAT_POS, 0)
    , m_vUp(0, 1, 0)
    , m_fRotX(0)
    , m_fRotY(0)
    , m_isRButtonDown(false)
    , m_isFocus(false)
{
}


cCamera::~cCamera()
{
}

void cCamera::Setup()
{
    RECT rc;
    GetClientRect(g_hWnd, &rc);

    Matrix4 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Setup(bool focus)
{
    m_isFocus = focus;
    RECT rc;
    GetClientRect(g_hWnd, &rc);

    float fovy = 45.0f;
    if (m_isFocus)
    {
        fovy = 10.0f;
    }

    Matrix4 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(fovy), rc.right / (float)rc.bottom, 1, 1000);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
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
}