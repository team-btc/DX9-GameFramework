#include "stdafx.h"
#include "cAseCharacter.h"
#include "cAseLoader.h"


cAseCharacter::cAseCharacter()
    : m_pRootFrame(NULL)
{
}


cAseCharacter::~cAseCharacter()
{
}

void cAseCharacter::Load(char* szFullPath)
{
    cAseLoader* pAseLoader = new cAseLoader;
    m_pRootFrame = pAseLoader->Load("woman/woman_01_all.ase", &m_stScene);
    SAFE_RELEASE(pAseLoader);
}

void cAseCharacter::Update(Matrix4* matWorld)
{
    /*
    Ű������ ���
    */
    int nFirstTick = m_stScene.nFirstFrame * m_stScene.nTicksPerFrame;
    int nLastTick = m_stScene.nLastFrame * m_stScene.nTicksPerFrame;
    int nTickSpeed = m_stScene.nFrameSpeed * m_stScene.nTicksPerFrame;
    int nKeyFrame = (nTickSpeed * GetTickCount() / 1000) % (nLastTick - nFirstTick) + nFirstTick;
    // 640 ~ 3199 �ݺ�

    // ĳ���� ũ��, �ʱ� �ٶ󺸴� ���� ������ �ʿ��ϸ� ���⼭ �Ѵ�.
    Matrix4 matW, matS, matR;
    D3DXMatrixScaling(&matS, 10, 10, 10);
    D3DXMatrixRotationY(&matR, D3DX_PI);

    matW = matS * matR * *matWorld;

    if (m_pRootFrame)
        m_pRootFrame->Update(nKeyFrame, &matW);
}

void cAseCharacter::Render()
{
    if (m_pRootFrame)
        m_pRootFrame->Render();
}
