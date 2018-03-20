#include "stdafx.h"
#include "cBoss.h"


cBoss::cBoss(string szKey)
    : m_vStartPoint(0,0,0)
    , m_fRoarTime(0.0f)
    , m_isRoar(false)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = BOSS;

    m_szName = szKey;

    m_stStat.szName = szKey;
    m_stStat.Level = 1;

    m_stStat.fSTR = 50.0f + m_stStat.Level * 10.0f;
    m_stStat.fDEX = 30.0f + m_stStat.Level * 10.0f;
    m_stStat.fINT = 30.0f + m_stStat.Level * 10.0f;

    m_stStat.fATK = 150.0f + m_stStat.Level * 30.0f;
    m_stStat.fDEF = 120.0f + m_stStat.Level * 10.0f;
    m_stStat.fCurHP = 2000.0f + m_stStat.Level * 100.0f;
    m_stStat.fMaxHP = 2000.0f + m_stStat.Level * 100.0f;
    m_stStat.fCurMP = 500.0f + m_stStat.Level * 50.0f;;
    m_stStat.fMaxMP = 500.0f + m_stStat.Level * 50.0f;;
    m_stStat.fSpeed = 1.0f;
    m_stStat.fCritical = 30.0f;
    m_stStat.fHPGen = 20.0f;
    m_stStat.fMPGen = 10.0f;
    m_stStat.nCoolTime = 0;
    m_stStat.nCurEXP = 0;
    m_stStat.nMaxEXP = 80;

    IdleAnim();

    m_stSphere.fRadius = 20.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cBoss::cBoss()
    : m_vStartPoint(0,0,0)
    , m_fRoarTime(0.0f)
    , m_isRoar(false)
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
    //죽음정의


    //공격부터 스킬까지 정의
    if (m_pTarget)
    {
        m_fRoarTime += g_pTimerManager->GetDeltaTime();
        if (m_fRoarTime > 10.0f)
        {
            if (!m_isRoar)
            {
                m_fRoarTime = 0.0f;
                m_isRoar = true;
                RoarAnim();
                //playSyene에서 구현해야함
            }
        }

        if (Distance(m_pTarget->GetPosition()) < m_pTarget->GetSphere().fRadius + m_stSphere.fRadius)
        {
            if (!isAttack)
            {
                isMoveToTarget = false;
                isAttack = true;
                AttackAnim();
            }
        }
        else
        {
            if (!isMoveToTarget)
            {
                isMove = true;
                isAttack = false;
                isMoveToTarget = true;
                RunAnim();
            }
        }
    }

    if (isAttack)
    {
        Attack();
    }

    if (isMoveToTarget && !isRoar)
    {
        MoveToTarget();
    }

    m_pMesh->SetScale(6.0f);
    m_pMesh->SetPosition(m_vPosition);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 180.0f, 0));
}

void cBoss::Render()
{
    m_pMesh->UpdateAndRender();

#ifdef _DEBUG
    Matrix4 matT, matW;
    D3DXMatrixScaling(&m_MatScale, m_stSphere.fRadius, m_stSphere.fRadius, m_stSphere.fRadius);
    D3DXMatrixTranslation(&matT, m_stSphere.vCenter.x, m_stSphere.vCenter.y + 4.0f, m_stSphere.vCenter.z);
    matW = m_MatScale * matT;
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pPikingMesh->DrawSubset(0);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif // _DEBUG
}

ULONG cBoss::Release()
{
    return cObject::Release();
}

void cBoss::Attack()
{
}

void cBoss::Roar()
{
}

void cBoss::MoveToTarget()
{
    m_vDir = m_pTarget->GetPosition() - m_vPosition;
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition += m_vDir * m_fMoveSpeed;
    m_stSphere.vCenter = m_vPosition;
}
