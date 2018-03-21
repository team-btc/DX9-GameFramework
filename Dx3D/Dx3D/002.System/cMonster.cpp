#include "stdafx.h"
#include "cMonster.h"
#include "005.UI\\cUILayer.h"


cMonster::cMonster(string szKey, string szFolder, string szFilename)
    : m_vStartPoint(0, 0, 0)
    , m_vDest(0, 0, 0)
    , m_fMoveCount(0.0f)
    , m_fAggroTime(9999.0f)
    , m_fScale(0.0f)
    , m_fCenter(0.0f)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = MONSTER;

    m_stStat.szName = "Monster1";
    m_stStat.Level = 1;

    IdleAnim();

    m_vDest = Vector3(0, 0, 0);
    m_fMoveRadius = 30.0f;
    m_fMoveCount = 0.0f;
    m_fAggroTime = 9999.0f;
    m_stSphere.fRadius = 10.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cMonster::cMonster(string szKey)
    : m_vStartPoint(0,0,0)
    , m_vDest(0,0,0)
    , m_fMoveCount(0.0f)
    , m_fAggroTime(9999.0f)
    , m_fScale(0.0f)
    , m_fCenter(0.0f)
{
    m_pMesh = new cSkinnedMesh(szKey);
    //g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = MONSTER;

    m_szName = szKey;

    m_stStat.szName = szKey;
    m_stStat.Level = 1;

    SetLevelToStatus(m_stStat.szName, m_stStat.Level);

    IdleAnim();

    m_fMoveRadius = 30.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cMonster::cMonster()
{
}

cMonster::~cMonster()
{
    SAFE_RELEASE(m_pMesh);
    SAFE_DELETE(m_pUILayer);
}

void cMonster::Setup()
{
    m_pTarget = NULL;

    isAttack = false;
    isRun = false;
    isIdle = false;
    isHeal = false;
    isStatic = false;
    isActive = false;
    isAlive = true;
    isMoveToTarget = false;

    m_stStat.Level = 1;

    SetLevelToStatus(m_stStat.szName, m_stStat.Level);

    IdleAnim();

    m_stSphere.vCenter = m_vPosition;
}

void cMonster::Update()
{
    if (!isActive &&  m_pMesh->GetCurPos() >= 1.0f)
    {
        isAlive = false;
        g_pCharacterManager->PushMonster(this);
    }

    if (m_stStat.fCurHP <= 0 && isActive)
    {
        isActive = false;

        ST_STATUS TargetStatus = m_pTarget->GetStatus();
        TargetStatus.nCurEXP += m_stStat.nMaxEXP;
        m_pTarget->SetStatus(TargetStatus);

        m_pTarget->SetTarget(NULL);
        m_stSphere.fRadius = 0;
        DeadAnim();
    }


    if (isActive)
    {
        m_fAggroTime -= g_pTimerManager->GetDeltaTime();
        if (m_fAggroTime <= 0.0f)
        {
            m_pTarget = NULL;
            m_vDest = m_vStartPoint;
            //m_isMove = false;
            RunAnim();
            m_fAggroTime = 9999.0f;
        }

        if (m_pTarget)
        {
            isMove = true;
            m_vDir = m_pTarget->GetPosition() - m_vPosition;
            float Distance = D3DXVec3Length(&m_vDir);
            D3DXVec3Normalize(&m_vDir, &m_vDir);

            if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
            {
                //뛰는중이었는가?
                if (!isAttack)
                {
                    AttackAnim();
                    isAttack = true;
                }

                if (m_pMesh->GetCurPos() >= 1.0f)
                {
                    AttackAnim();
                    m_pMesh->SetDescZeroPos();
                    float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) <= m_pTarget->GetStatus().fDEF ? 1 : m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;
                    Action("Attack", ATK);
                }
            }
            else
            {
                //공격중이었는가?
                if (isAttack)
                {
                    RunAnim();
                    isAttack = false;
                }

                m_vPosition += m_vDir * m_fMoveSpeed;
                m_stSphere.vCenter = m_vPosition;

                D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
            }
            D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &m_vDir, &D3DXVECTOR3(0, 1, 0));
            D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

            if (m_vDir.z >0)
                m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
            else
                m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
            D3DXMatrixRotationY(&m_MatRotate, m_fRotY);

            D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        }
        else
        {
            m_fMoveCount += g_pTimerManager->GetDeltaTime();

            if (isMove)
            {
                m_vDir = m_vDest - m_vPosition;
                D3DXVec3Normalize(&m_vDir, &m_vDir);

                m_vPosition += m_vDir * m_fMoveSpeed;
                m_stSphere.vCenter = m_vPosition;

                if (Distance(m_vDest) < m_fMoveSpeed)
                {
                    m_vPosition = m_vDest;
                    isMove = false;
                    IdleAnim();
                }

                D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &m_vDir, &D3DXVECTOR3(0, 1, 0));
                D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

                if (m_vDir.z > 0)
                {
                    m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
                }
                else
                {
                    m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
                }
                D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
            }
            
            if (m_fMoveCount > 10.0f)
            {
                isMove = true;
                m_fMoveCount = 0.0f;
                m_vDest = m_vStartPoint + GetRandomVector3(Vector3(-m_fMoveRadius, 0, -m_fMoveRadius), Vector3(m_fMoveRadius, 0, m_fMoveRadius));
                //WalkAnim();
            }
        }
    }
    else
    {
    }

    m_pMesh->SetScale(m_fScale);
    m_pMesh->SetPosition(m_vPosition);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
}

void cMonster::Render()
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

ULONG cMonster::Release()
{
    return cObject::Release();
}

void cMonster::SetLevelToStatus(string szKey, int Level)
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
