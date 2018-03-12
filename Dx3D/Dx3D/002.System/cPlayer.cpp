#include "stdafx.h"
#include "cPlayer.h"
#include "003.Object\cSkinnedMesh.h"

cPlayer::cPlayer(string szKey, string szFolder, string szFilename)
    :DestPoint(0,0,0)
    , isMoveToPoint(false)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);
    
    m_stSphere.fRadius = 1.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = *g_pMeshManager->GetBasicMesh("sphere");
    
}

cPlayer::cPlayer()
{
}


cPlayer::~cPlayer()
{
}

void cPlayer::Setup()
{
}

void cPlayer::Update()
{
    if (!isAttack)
    {
        if (g_pKeyManager->isOnceKeyDown('Q'))
        {
            // 바로 액션이 아니라 모션이 다지나야지 액션이가능함
            Action("Heal", "3");
        }

        if (g_pKeyManager->isOnceKeyDown('W'))
        {
            m_pMesh->SetAnimationIndex(1);
        }
        else if (g_pKeyManager->isStayKeyDown('W'))
        {
            MoveForword();
        }
        if (g_pKeyManager->isOnceKeyUp('W'))
        {
            m_pMesh->SetAnimationIndex(0);
        }

        if (g_pKeyManager->isOnceKeyDown('S'))
        {
            m_pMesh->SetAnimationIndex(1);
        }
        else if (g_pKeyManager->isStayKeyDown('S'))
        {
            MoveBackword();
        }
        if (g_pKeyManager->isOnceKeyUp('S'))
        {
            m_pMesh->SetAnimationIndex(0);
        }

        if (g_pKeyManager->isStayKeyDown('A'))
        {
            RotateLeft();
        }
        if (g_pKeyManager->isStayKeyDown('D'))
        {
            RotateRight();
        }
    }

    if (g_pKeyManager->isOnceKeyDown('1') && !isAttack)
    {
        AttackAnim();
    }

    if(isAttack && m_pMesh->GetCurPos() >= 1)
    {
        Action("Attack", "5");// 다시해야함
        cout << m_pTarget->GetHP() << endl;
        m_pTarget->RayCast(this); // 어그로 주고
        IdleAnim();
    }

    //마우스 이동
    if (isMoveToPoint)
    {
        Vector3 _Dir = DestPoint - m_vPosition;

        D3DXVec3Normalize(&_Dir, &_Dir);

        m_vPosition += _Dir * m_fSpeed;
        m_stSphere.vCenter = m_vPosition;

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &_Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

        D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

        //도착
        if (Distance(DestPoint) <m_fSpeed )
        {
            m_vPosition = DestPoint;
            IdleAnim();
            isMoveToPoint = false;
            DestPoint = Vector3(0, 0, 0);
        }
    }

    // 타게팅하고 때리러갈때
    if (isMoveToTarget)
    {
        Vector3 Dir = m_pTarget->GetPosition() - m_vPosition;
        float Distance = D3DXVec3Length(&Dir);
        D3DXVec3Normalize(&Dir, &Dir);

        // 공격모션으로 변경
        if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius )
        {
            isMoveToTarget = false;
            AttackAnim();
        }
        else
        {
            m_vPosition += Dir * m_fSpeed;
            m_stSphere.vCenter = m_vPosition;

            D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        }

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);
    }

    Matrix4 matR,matW;
    D3DXMatrixRotationY(&matR, D3DX_PI);
    matW = m_MatScale * m_MatRotate * matR * m_MatTrans ;
    m_stSphere.vCenter = m_vPosition;
    m_pMesh->SetWorldMatrix(matW);
}

void cPlayer::Render()
{
    m_pMesh->UpdateAndRender();

    Matrix4 matR,matT, matW;
    D3DXMatrixRotationY(&matR, D3DX_PI);
    D3DXMatrixTranslation(&matT, m_stSphere.vCenter.x, m_stSphere.vCenter.y + 0.5f, m_stSphere.vCenter.z);
    matW = m_MatScale * m_MatRotate * matR * matT;
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    m_pPikingMesh->DrawSubset(0);
}

void cPlayer::Destroy()
{
}
