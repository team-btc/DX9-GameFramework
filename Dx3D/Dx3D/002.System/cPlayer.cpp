#include "stdafx.h"
#include "cPlayer.h"
#include "003.Object\cSkinnedMesh.h"

cPlayer::cPlayer(string szKey, string szFolder, string szFilename)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);
    m_stSphere.fRadius = 1.0f;
    
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
        m_pMesh->SetAnimationIndex(2);
        isAttack = true;
    }

    if(isAttack && m_pMesh->GetCurPos() >= 1)
    {
        isAttack = false;
        m_pMesh->SetAnimationIndex(0);
    }

    if (isMoveToTarget)
    {
        Vector3 Dir = m_pTarget->GetPosition() - m_vPosition;
        float Distance = D3DXVec3Length(&Dir);

        if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius )
        {
            isMoveToTarget = false;
            Attack();
            isAttack = true;
        }

        D3DXVec3Normalize(&Dir, &Dir);

        m_vPosition += Dir * m_fSpeed;

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

        D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    }

    Matrix4 matR,matW;
    D3DXMatrixRotationY(&matR, D3DX_PI);
    matW = m_MatScale * m_MatRotate * matR * m_MatTrans ;
    m_pMesh->SetWorldMatrix(matW);
}

void cPlayer::Render()
{
    m_pMesh->UpdateAndRender();
}

void cPlayer::Destroy()
{
}
