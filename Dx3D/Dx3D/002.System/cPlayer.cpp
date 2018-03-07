#include "stdafx.h"
#include "cPlayer.h"
#include "003.Object\cSkinnedMesh.h"

cPlayer::cPlayer(string szKey, string szFolder, string szFilename)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);
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
    if (g_pKeyManager->isStayKeyDown('W'))
    {
        MoveForword();
    }
    if (g_pKeyManager->isStayKeyDown('S'))
    {
        MoveBackword();
    }
    if (g_pKeyManager->isStayKeyDown('A'))
    {
        RotateLeft();
    }
    if (g_pKeyManager->isStayKeyDown('D'))
    {
        RotateRight();
    }
    if (g_pKeyManager->isOnceKeyDown('1'))
    {
        Attack();
        isAttack = true;
    }

    if(isAttack && m_pMesh->GetCurPos() >= 1)
    {
        isAttack = false;
        Idle();
        
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
