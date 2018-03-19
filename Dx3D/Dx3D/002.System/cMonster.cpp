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
            IdleAnim();
            m_fAggroTime = 9999.0f;
        }

        if (m_pTarget)
        {
            m_isMove = false;
            Vector3 Dir = m_pTarget->GetPosition() - m_vPosition;
            float Distance = D3DXVec3Length(&Dir);
            D3DXVec3Normalize(&Dir, &Dir);

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

                m_vPosition += Dir * 0.1f;
                m_stSphere.vCenter = m_vPosition;

                D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
            }
            D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
            D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

            if (Dir.z >0)
                m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
            else
                m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
            D3DXMatrixRotationY(&m_MatRotate, m_fRotY);

            D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        }
        else
        {
            m_fMoveCount += g_pTimerManager->GetDeltaTime();
            
            if (m_fMoveCount > 10.0f)
            {
                m_isMove = true;
                m_fMoveCount = 0.0f;
                m_vDest = m_vStartPoint + GetRandomVector3(Vector3(-m_fMoveRadius, m_vStartPoint.y, -m_fMoveRadius), Vector3(m_fMoveRadius, m_vStartPoint.y, m_fMoveRadius));
                WalkAnim();
            }
        }

        if (m_isMove)
        {
            Vector3 Dir = m_vDest - m_vPosition;
            D3DXVec3Normalize(&Dir, &Dir);

            m_vPosition += Dir * m_fMoveSpeed;
            m_stSphere.vCenter = m_vPosition;

            if (Distance(m_vDest) < m_fMoveSpeed)
            {
                m_isMove = false;
            }

            D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
            D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

            if (Dir.z >0)
                m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
            else
                m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
            D3DXMatrixRotationY(&m_MatRotate, m_fRotY);

            D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        }
    }
    else
    {
     
    }

    Matrix4 matR,matW;
    D3DXMatrixScaling(&m_MatScale, 2, 2, 2);
    D3DXMatrixRotationY(&matR, -D3DX_PI/2);
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    m_stSphere.vCenter = m_vPosition;
    matW = m_MatScale * m_MatRotate* matR * m_MatTrans;
    m_pMesh->SetWorldMatrix(matW);
}

void cMonster::Render()
{
    m_pMesh->UpdateAndRender();

#ifdef _DEBUG
    Matrix4 matS, matW;
    D3DXMatrixScaling(&matS, m_stSphere.fRadius, m_stSphere.fRadius, m_stSphere.fRadius);
    D3DXMatrixTranslation(&m_MatTrans, m_stSphere.vCenter.x, m_stSphere.vCenter.y + 5.0f, m_stSphere.vCenter.z);
    matW = matS * m_MatRotate * m_MatTrans;
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
