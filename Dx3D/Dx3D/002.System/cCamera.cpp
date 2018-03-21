#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
    : m_fDistance(10.0f)
    , m_vEye(0.0f, 0.0f, m_fDistance)
    , m_vLookAt(0.0f, 0.0f, 0.0f)
    , m_vUp(0.0f, 1.0f, 0.0f)
    , m_isFocus(false)
    , m_isControl(true)
    , m_isMovable(false)
    , m_fFov(45.0f)
    , m_fMaxDist(10.0f)
    , m_fMinDist(2.0f)
    , m_vTargetPos(0.0f, 0.0f, 0.0f)
    , m_fLookatOffset(LOOKAT_POS)
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
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(m_fFov), rc.right / (float)rc.bottom, 1.0f, 1000.0f);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

    return S_OK;
}

ULONG cCamera::Release()
{
    return cObject::Release();
}

HRESULT cCamera::Update()
{
    if (m_isControl)
    {
        if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
        {
            m_vRotation.y += (g_ptMouse.x - m_ptPrevMouse.x) * 0.2f;
            m_vRotation.x -= (g_ptMouse.y - m_ptPrevMouse.y) * 0.2f;
        }
        m_fDistance -= g_nWheelMouse * 0.01f;
    }
    m_ptPrevMouse = g_ptMouse;

    // x축 회전은 -90 ~ 90 으로 고정
    m_vRotation.x = min(m_vRotation.x, LIMIT_ROTX);
    m_vRotation.x = max(m_vRotation.x, -LIMIT_ROTX);

    while (m_vRotation.y >= 360.0f)
    {
        m_vRotation.y -= 360.0f;
    }

    m_fDistance = min(m_fDistance, m_fMaxDist);
    m_fDistance = max(m_fDistance, m_fMinDist);

    if (m_isMovable)
    {
        m_isFocus = false;
        if (g_pKeyManager->isStayKeyDown(VK_LEFT))
        {
            m_vLookAt.x -= 1.0f;
        }
        else if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
        {
            m_vLookAt.x += 1.0f;
        }

        if (g_pKeyManager->isStayKeyDown(VK_UP))
        {
            m_vLookAt.z += 1.0f;
        }
        else if (g_pKeyManager->isStayKeyDown(VK_DOWN))
        {
            m_vLookAt.z -= 1.0f;
        }

        if (g_pKeyManager->isStayKeyDown('Z'))
        {
            m_vLookAt.y -= 1.0f;
        }
        else if (g_pKeyManager->isStayKeyDown('X'))
        {
            m_vLookAt.y += 1.0f;
        }

        Vector3 pos = Vector3(0, 0, m_fDistance);
        Matrix4 matR;
        D3DXMatrixRotationYawPitchRoll(&matR,
            D3DXToRadian(m_vRotation.y),
            D3DXToRadian(m_vRotation.x),
            D3DXToRadian(m_vRotation.z));
        D3DXVec3TransformCoord(&pos, &pos, &matR);
        m_vEye = m_vLookAt + pos;
    }

    if (m_isFocus)
    {
        // 뷰 매트릭스 셋팅 - 타겟 == 큐브의 포지션
        m_vEye = Vector3(0, 0, m_fDistance);

        Matrix4 matRotX, matRotY;
        D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_vRotation.x));
        D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_vRotation.y));

        D3DXVec3TransformCoord(&m_vEye, &m_vEye, &(matRotX * matRotY));

        m_vLookAt = m_vTargetPos;
        m_vLookAt.y += m_fLookatOffset;
        m_vEye += m_vTargetPos;
    }

    Matrix4 matView;
    D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &Vector3(0, 1, 0));
    g_pDevice->SetTransform(D3DTS_VIEW, &matView);

    g_vCameraPos = m_vEye;

    return S_OK;
}

void cCamera::SetFov(float fov)
{
    m_fFov = fov;
    Setup();
}
