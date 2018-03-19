#include "stdafx.h"
#include "cBoss.h"


cBoss::cBoss(string szKey)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = MONSTER;

    m_stStat.szName = "Monster1";
    m_stStat.Level = 1;

    m_stStat.fSTR = 10.0f + m_stStat.Level * 3.0f;
    m_stStat.fDEX = 5.0f + m_stStat.Level * 3.0f;
    m_stStat.fINT = 5.0f + m_stStat.Level * 3.0f;

    m_stStat.fATK = 30.0f + m_stStat.Level * 10.0f;
    m_stStat.fDEF = 5.0f + m_stStat.Level * 3.0f;
    m_stStat.fCurHP = 300.0f + m_stStat.Level * 30.0f;
    m_stStat.fMaxHP = 300.0f + m_stStat.Level * 30.0f;
    m_stStat.fCurMP = 0.0f;
    m_stStat.fMaxMP = 0.0f;
    m_stStat.fSpeed = 1.0f;
    m_stStat.fCritical = 15.0f;
    m_stStat.fHPGen = 3.0f;
    m_stStat.fMPGen = 3.0f;
    m_stStat.nCoolTime = 0;
    m_stStat.nCurEXP = 0;
    m_stStat.nMaxEXP = 100;

    IdleAnim();

    m_stSphere.fRadius = 10.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cBoss::cBoss()
{
}


cBoss::~cBoss()
{
}

void cBoss::Setup()
{
}

void cBoss::Update()
{
}

void cBoss::Render()
{
}

ULONG cBoss::Release()
{
    return cObject::Release();
}
