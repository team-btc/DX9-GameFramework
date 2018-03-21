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
        if (m_fRoarTime > 20.0f && g_pCharacterManager->GetVectorSize() > 0)
        {
            if (!isRoar)
            {
                m_fRoarTime = 0.0f;
                RoarAnim();
                isAttack = true;
                isMove = false;
                m_fMoveSpeed = 0.0f;
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

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &m_vDir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);
        if (m_vDir.z >0)
            m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
        else
            m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
    }

    if (isAttack)
    {
        Attack();
    }

    if (isMoveToTarget && !isRoar)
    {
        MoveToTarget();
    }

    if (isRoar)
    {
        if (m_pMesh->GetCurPos() > 1.0f)
        {
            m_isRoar = true;
            isMove = true;
            isRoar = false;
            isAttack = false;
            isMoveToTarget = false;
        }
    }

    m_pMesh->SetScale(6.0f);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
    m_pMesh->SetPosition(m_vPosition);
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
    if (m_pMesh->GetCurPos() >= 1.0f)
    {
        //데미지 계산식을 넣어야함
        float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) <= m_pTarget->GetStatus().fDEF ? 1 : m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;
        Action("Attack", ATK);
        m_pMesh->SetDescZeroPos();
    }
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
