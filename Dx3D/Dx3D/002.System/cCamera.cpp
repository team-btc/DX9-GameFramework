#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
    : m_fDistance(250)
    , m_vEye(0, LOOKAT_POS, -m_fDistance)
    , m_vLookAt(0, LOOKAT_POS, 0)
    , m_vUp(0, 1, 0)
    , m_fRotX(0.9f)
    , m_fRotY(0)
    , m_isRButtonDown(false)
    , m_isFocus(false)
    , m_vPosition(0, 0, 0)
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
    m_vEye = D3DXVECTOR3(0, LOOKAT_POS, -m_fDistance);

    // == 마우스 컨트롤 =======
    if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
    {
        m_isRButtonDown = true;
        m_ptPrevMouse = g_ptMouse;
    }
    if (g_pKeyManager->isOnceKeyUp(VK_RBUTTON))
    {
        m_isRButtonDown = false;
    }
    if (g_pKeyManager->isStayKeyDown(VK_RBUTTON))
    {
        if (m_isRButtonDown)
        {
            POINT ptCurrMouse;
            ptCurrMouse = g_ptMouse;

            m_fRotY += (ptCurrMouse.x - m_ptPrevMouse.x) / 100.0f;
            m_fRotX += (ptCurrMouse.y - m_ptPrevMouse.y) / 100.0f;

            // x축 회전은 -85 ~ 85 으로 고정
            if (m_fRotX < -D3DX_PI * LIMITED_ROT + D3DX_16F_EPSILON)
            {
                m_fRotX = -D3DX_PI * LIMITED_ROT + D3DX_16F_EPSILON;
            }
            else if (m_fRotX > D3DX_PI * LIMITED_ROT - D3DX_16F_EPSILON)
            {
                m_fRotX = D3DX_PI * LIMITED_ROT - D3DX_16F_EPSILON;
            }

            m_ptPrevMouse = ptCurrMouse;
        }
    }

    if (g_pKeyManager->isStayKeyDown('I')) 
    {
        m_fDistance -= 1.0f;

        if (m_fDistance < 5.0f)
        {
            m_fDistance = 5.0f;
        }
    }
    else if (g_pKeyManager->isStayKeyDown('O'))
    {
        m_fDistance += 1.0f;

        if (m_fDistance < 5.0f)
        {
            m_fDistance = 5.0f;
        }
    }

    //if (g_nWheelDelta != 0)
    //{
    //    m_fDistance -= g_nWheelDelta * 0.1f;
    //    g_nWheelDelta = 0;

    //    if (m_fDistance < 5.0f)
    //    {
    //        m_fDistance = 5.0f;
    //    }
    //}

    D3DXMATRIXA16 matRotX, matRotY;
    D3DXMatrixRotationX(&matRotX, m_fRotX);
    D3DXMatrixRotationY(&matRotY, m_fRotY);

    D3DXVec3TransformCoord(&m_vEye, &m_vEye, &(matRotX * matRotY));

    // 카메라가 고정으로 봐야하는 타겟이 있다면
    if (pTarget)
    {
        m_vLookAt = *pTarget;
        m_vLookAt.y += LOOKAT_POS;
        m_vPosition = *pTarget;
    }


    // 카메라를 컨트롤 해야 한다면
    else
    {
        if (g_pKeyManager->isStayKeyDown('A')
            || g_pKeyManager->isStayKeyDown('D')
            || g_pKeyManager->isStayKeyDown('W')
            || g_pKeyManager->isStayKeyDown('S')
            || g_pKeyManager->isStayKeyDown(VK_PRIOR)
            || g_pKeyManager->isStayKeyDown(VK_NEXT))
        {
            D3DXVECTOR3 dirX, dirZ;
            D3DXVec3Normalize(&dirZ, &m_vEye);
            D3DXVec3Cross(&dirX, &m_vEye, &D3DXVECTOR3(0, 1, 0));
            D3DXVec3Cross(&dirZ, &dirX, &D3DXVECTOR3(0, 1, 0));
            float fMovePower = 0.01f;//m_fDistance * 0.0001f;

            // == 키보드 컨트롤 ======= 
            if (g_pKeyManager->isStayKeyDown('A'))
            {
                m_vPosition -= dirX * fMovePower;
                m_vLookAt -= dirX * fMovePower;
            }
            if (g_pKeyManager->isStayKeyDown('D'))
            {
                m_vPosition += dirX * fMovePower;
                m_vLookAt += dirX * fMovePower;
            }
            if (g_pKeyManager->isStayKeyDown('W'))
            {
                m_vPosition += dirZ * fMovePower;
                m_vLookAt += dirZ * fMovePower;
            }
            if (g_pKeyManager->isStayKeyDown('S'))
            {
                m_vPosition -= dirZ * fMovePower;
                m_vLookAt -= dirZ * fMovePower;
            }
            if (g_pKeyManager->isStayKeyDown(VK_PRIOR)) //PGUP
            {
                Vector3 vDirY(0, 1, 0);
                m_vPosition += vDirY * 10;// * fMovePower;
                m_vLookAt += vDirY * 10;// * fMovePower;
            }
            if (g_pKeyManager->isStayKeyDown(VK_NEXT)) //PGDOWN
            {
                Vector3 vDirY(0, 1, 0);
                m_vPosition -= vDirY * 10;// *fMovePower;
                m_vLookAt -= vDirY * 10;// * fMovePower;
            }
        }
    }

    m_vEye += m_vPosition;

    // 뷰 메트릭스 셋팅
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &D3DXVECTOR3(0, 1, 0));
    g_pDevice->SetTransform(D3DTS_VIEW, &matView);

    g_vCameraPos = m_vEye;
}