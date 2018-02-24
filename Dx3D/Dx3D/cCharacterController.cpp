#include "stdafx.h"
#include "cCharacterController.h"


cCharacterController::cCharacterController()
    : m_vPosition(0, 0, 0)
    , m_fMoveSpeed(0.5f)
    , m_vDir(0, 0, 1)
    , m_vDest(0, 0, 0)
    , m_isDebugMode(false)
{
    D3DXMatrixIdentity(&m_matWorld);
    m_isMovable = false;
}


cCharacterController::~cCharacterController()
{
}

void cCharacterController::Update()
{
    // x, z 위치가 다른 경우 캐릭터 이동
    if (m_isMovable)
    {
        if (fabs(m_vDest.x - m_vPosition.x) > D3DX_16F_EPSILON ||
            fabs(m_vDest.z - m_vPosition.z) > D3DX_16F_EPSILON)
        {
            m_vDir = m_vDest - m_vPosition;
            m_vDir.y = 0.0f;
            float dist = D3DXVec3Length(&m_vDir);
            D3DXVec3Normalize(&m_vDir, &m_vDir);

            if (dist > m_fMoveSpeed)
                m_vPosition += m_vDir * m_fMoveSpeed;
            else
            {
                m_vPosition = m_vDest;

                if (!m_vecPath.empty())
                {
                    m_vDest = m_vecPath.back();
                    m_vecPath.pop_back();
                }
                else
                {
                    m_isMovable = false;
                }
            }
        }
    }

    Matrix4 matRotY;
    D3DXMatrixLookAtLH(&matRotY, &Vector3(0, 0, 0), &m_vDir, &Vector3(0, 1, 0));
    D3DXMatrixTranspose(&matRotY, &matRotY); // 역행렬을 만들기 위한 전치
    
    Matrix4 matTrans;
    D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    m_matWorld = matRotY * matTrans;

    if (GetAsyncKeyState(VK_F12) & 0x0001)
        m_isDebugMode = !m_isDebugMode;
}

void cCharacterController::Render()
{
    if (m_isDebugMode)
    {
    }
}

Vector3* cCharacterController::GetDestPosition()
{
    if (!m_vecPath.empty())
    {
        return &(*m_vecPath.begin());
    }
    return NULL;
}

bool cCharacterController::IsStop()
{
    if (m_vecPath.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}
