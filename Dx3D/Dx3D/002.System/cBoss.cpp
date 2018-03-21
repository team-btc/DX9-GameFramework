#include "stdafx.h"
#include "cBoss.h"


cBoss::cBoss(string szKey)
    : m_vStartPoint(0,0,0)
    , m_fRoarTime(0.0f)
    , m_isRoar(false)
    , m_fScale(0.0f)
    , m_fCenter(0.0f)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = BOSS;

    m_szName = szKey;

    m_stStat.szName = szKey;
    m_stStat.Level = 1;

    SetLevelToStatus(m_stStat.szName, m_stStat.Level);

    IdleAnim();

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

        m_vDir = m_pTarget->GetPosition() - m_vPosition;
        float Distance = D3DXVec3Length(&m_vDir);
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        if (Distance < m_pTarget->GetSphere().fRadius + m_stSphere.fRadius)
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
    D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &m_vDir, &D3DXVECTOR3(0, 1, 0));
    D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);
    if (m_vDir.z >0)
        m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
    else
        m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));

    m_pMesh->SetScale(m_fScale);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
    m_pMesh->SetPosition(m_vPosition);
}

void cBoss::Render()
{
    m_pMesh->UpdateAndRender();

#ifdef _DEBUG
    Matrix4 matT, matW;
    D3DXMatrixScaling(&m_MatScale, m_stSphere.fRadius, m_stSphere.fRadius, m_stSphere.fRadius);
    D3DXMatrixTranslation(&matT, m_stSphere.vCenter.x, m_stSphere.vCenter.y + m_fCenter, m_stSphere.vCenter.z);
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

void cBoss::SetLevelToStatus(string szKey, int Level)
{
    json status = g_pMeshManager->GetJson("Status");

    m_stStat.fSTR = (float)status[szKey]["STR"] + m_stStat.Level * (float)status[szKey]["UPSTR"];
    m_stStat.fDEX = (float)status[szKey]["DEX"] + m_stStat.Level * (float)status[szKey]["UPDEX"];
    m_stStat.fINT = (float)status[szKey]["INT"] + m_stStat.Level * (float)status[szKey]["UPINT"];

    m_stStat.fATK = (float)status[szKey]["ATK"] + m_stStat.Level * (float)status[szKey]["UPATK"];
    m_stStat.fDEF = (float)status[szKey]["DEF"] + m_stStat.Level * (float)status[szKey]["UPDEF"];
    m_stStat.fCurHP = (float)status[szKey]["HP"] + m_stStat.Level * (float)status[szKey]["UPHP"];
    m_stStat.fMaxHP = (float)status[szKey]["HP"] + m_stStat.Level * (float)status[szKey]["UPHP"];
    m_stStat.fCurMP = (float)status[szKey]["MP"] + m_stStat.Level * (float)status[szKey]["UPMP"];
    m_stStat.fMaxMP = (float)status[szKey]["MP"] + m_stStat.Level * (float)status[szKey]["UPMP"];

    m_stStat.fHPGen = (float)status[szKey]["HPGEN"];
    m_stStat.fMPGen = (float)status[szKey]["MPGEN"];

    m_stStat.fSpeed = (float)status[szKey]["SPEED"];
    m_stStat.fCritical = (float)status[szKey]["CRITICAL"];
    m_stStat.nCoolTime = status[szKey]["COOLTIME"];
    m_stStat.nCurEXP = 0;
    m_stStat.nMaxEXP = status[szKey]["EXP"];

    m_fScale = (float)status[szKey]["SIZE"];
    m_stSphere.fRadius = (float)status[szKey]["PICKINGRADIUS"];
    m_fCenter = (float)status[szKey]["CENTERPOS"];
}
