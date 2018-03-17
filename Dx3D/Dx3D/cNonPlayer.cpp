#include "stdafx.h"
#include "cNonPlayer.h"


cNonPlayer::cNonPlayer(string szKey)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = MONSTER;
    
}

cNonPlayer::cNonPlayer()
{
}


cNonPlayer::~cNonPlayer()
{
}

void cNonPlayer::Setup()
{
}

void cNonPlayer::Update()
{
}

void cNonPlayer::Render()
{
}
