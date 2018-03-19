#include "stdafx.h"
#include "cMonster.h"
#include "005.UI\\cUILayer.h"


cMonster::cMonster(string szKey, string szFolder, string szFilename)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
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

    m_vDest = Vector3(0, 0, 0);
    m_fMoveRadius = 30.0f;
    m_fMoveCount = 0.0f;
    m_fAggroTime = 9999.0f;
    m_stSphere.fRadius = 10.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cMonster::cMonster(string szKey)
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

    m_vDest = Vector3(0, 0, 0);
    m_fMoveRadius = 30.0f;
    m_fMoveCount = 0.0f;
    m_fAggroTime = 9999.0f;
    m_stSphere.fRadius = 10.0f;
    m_stSphere.vCenter = m_vPosition;


    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cMonster::cMonster()
{
}


cMonster::~cMonster()
{
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
    m_isMove = false;

    m_stStat.Level = 1;

    m_stStat.fSTR = 10.0f + m_stStat.Level * 3.0f;
    m_stStat.fDEX = 5.0f + m_stStat.Level * 3.0f;
    m_stStat.fINT = 5.0f + m_stStat.Level * 3.0f;

    m_stStat.fATK = 30.0f + m_stStat.Level * 10.0f;
    m_stStat.fDEF = 5.0f + m_stStat.Level * 3.0f;
    m_stStat.fCurHP = 300.0f + m_stStat.Level * 30.0f;
    m_stStat.fMaxHP = 300.0f + m_stStat.Level * 30.0f;

    IdleAnim();

    m_stSphere.fRadius = 10.0f;
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

        int EXP = m_pTarget->GetStatus().nCurEXP + m_stStat.nMaxEXP;

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
            m_isMove = true;
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
                    Action("Attack", m_stStat.fATK);
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

            if (m_isMove)
            {
                m_vDir = m_vDest - m_vPosition;
                D3DXVec3Normalize(&m_vDir, &m_vDir);

                m_vPosition += m_vDir * m_fMoveSpeed;
                m_stSphere.vCenter = m_vPosition;

                if (Distance(m_vDest) < m_fMoveSpeed)
                {
                    m_vPosition = m_vDest;
                    m_isMove = false;
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
                m_isMove = true;
                m_fMoveCount = 0.0f;
                m_vDest = m_vStartPoint + GetRandomVector3(Vector3(-m_fMoveRadius, 0, -m_fMoveRadius), Vector3(m_fMoveRadius, 0, m_fMoveRadius));
                WalkAnim();
            }
        }
    }
    else
    {
    }

    m_pMesh->SetScale(8.0f);
    m_pMesh->SetPosition(m_vPosition);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
}

void cMonster::Render()
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

ULONG cMonster::Release()
{
    return cObject::Release();
}
