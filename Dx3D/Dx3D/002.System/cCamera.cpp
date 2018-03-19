#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
    : m_fDistance(10.0f)
    , m_vEye(0.0f, 0.0f, m_fDistance)
    , m_vLookAt(0.0f, 0.0f, 0.0f)
    , m_vUp(0.0f, 1.0f, 0.0f)
    , m_fRotX(0.0f)
    , m_fRotY(0.0f)
    , m_isRButtonDown(false)
    , m_isFocus(false)
    , m_fFov(45.0f)
    , m_fMaxDist(10.0f)
    , m_fMinDist(2.0f)
    , m_vRotation(0.0f, 0.0f, 0.0f)
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
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(m_fFov), rc.right / (float)rc.bottom, 1, 1000);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

    return S_OK;
}

ULONG cCamera::Release()
{
    return cObject::Release();
}

HRESULT cCamera::Update()
{
    if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))
    {
        m_vRotation.y += (g_ptMouse.x - m_ptPrevMouse.x) * 0.2f;
        m_vRotation.x -= (g_ptMouse.y - m_ptPrevMouse.y) * 0.2f;
    }
    m_ptPrevMouse = g_ptMouse;

    // x축 회전은 -90 ~ 90 으로 고정
    m_vRotation.x = min(m_vRotation.x, LIMIT_ROTX);
    m_vRotation.x = max(m_vRotation.x, -LIMIT_ROTX);

    while (m_vRotation.y >= 360.0f)
    {
        m_vRotation.y -= 360.0f;
    }

    m_fDistance -= g_nWheelMouse * 0.01f;
    m_fDistance = min(m_fDistance, m_fMaxDist);
    m_fDistance = max(m_fDistance, m_fMinDist);

    // 뷰 매트릭스 셋팅 - 타겟 == 큐브의 포지션
    m_vEye = Vector3(0, 0, m_fDistance);

    Matrix4 matRotX, matRotY;
    D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_vRotation.x));
    D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_vRotation.y));

    D3DXVec3TransformCoord(&m_vEye, &m_vEye, &(matRotX * matRotY));

    if (m_isFocus)
    {
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

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        //m_ptPrevMouse.x = LOWORD(lParam);
        //m_ptPrevMouse.y = HIWORD(lParam);
        //m_isRButtonDown = true;
    }
    break;
    case WM_LBUTTONUP:
    {
        // = false;
    }
    break;
    case WM_MOUSEMOVE:
    {
        //if (m_isRButtonDown)
        //{
        //    POINT ptCurrMouse;
        //    ptCurrMouse.x = LOWORD(lParam);
        //    ptCurrMouse.y = HIWORD(lParam);
        //
        //    m_vRotation.y += (ptCurrMouse.x - m_ptPrevMouse.x) * 0.2f;
        //    m_vRotation.x -= (ptCurrMouse.y - m_ptPrevMouse.y) * 0.2f;
        //
        //    m_ptPrevMouse = ptCurrMouse;
        //}
    }
    break;
    case WM_MOUSEWHEEL:
    {
        //if (m_fDistance > 2)
        //{
        //    m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) * 0.01f;
        //}
        //else
        //{
        //    m_fDistance = 2;
        //    if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        //    {
        //        m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) * 0.01f;
        //    }
        //}
    }
    break;
    }
}
