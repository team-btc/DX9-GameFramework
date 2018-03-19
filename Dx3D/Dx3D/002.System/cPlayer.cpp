#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cMonster.h"

cPlayer::cPlayer(string szKey, string szFolder, string szFilename)
    :DestPoint(0,0,0)
    , m_isMoveToPoint(false)
    , m_isPoint(false)
    , m_isPickMonster(false)
    , m_isMove(false)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_vecMonster = NULL;

    m_eTag = PLAYER;

    m_stStat.szName = "ChiChi";
    m_stStat.Level = 1;

    m_stStat.fSTR = 20.0f + m_stStat.Level * 5.0f;
    m_stStat.fDEX = 15.0f + m_stStat.Level * 5.0f;
    m_stStat.fINT = 15.0f + m_stStat.Level * 5.0f;

    m_stStat.fATK = 35.0f + m_stStat.Level * 20.0f;
    m_stStat.fDEF = 10.0f + m_stStat.Level * 5.0f;
    m_stStat.fCurHP = 500.0f + m_stStat.Level * 100.0f;
    m_stStat.fMaxHP = 500.0f + m_stStat.Level * 100.0f;
    m_stStat.fCurMP = 300.0f + m_stStat.Level * 50.0f;
    m_stStat.fMaxMP = 300.0f + m_stStat.Level * 50.0f;
    m_stStat.fSpeed = 1.0f;
    m_stStat.fCritical = 15.0f;
    m_stStat.fHPGen = m_stStat.fMaxHP * 0.01f + m_stStat.Level * 0.5f;
    m_stStat.fMPGen = m_stStat.fMaxMP * 0.01f + m_stStat.Level * 0.5f;
    m_stStat.nCoolTime = 0;
    m_stStat.nCurEXP = 0;
    m_stStat.nMaxEXP = 100;
    
    IdleAnim();

    m_stSphere.fRadius = 4.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
}

cPlayer::cPlayer(string szKey)
    :DestPoint(0, 0, 0)
    , m_isMoveToPoint(false)
    , m_isPoint(false)
    , m_isPickMonster(false)
    , m_isMove(false)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_vecMonster = NULL;

    m_eTag = PLAYER;

    m_stStat.szName = "ChiChi";
    m_stStat.Level = 1;

    m_stStat.fSTR = 20.0f + m_stStat.Level * 5.0f;
    m_stStat.fDEX = 15.0f + m_stStat.Level * 5.0f;
    m_stStat.fINT = 15.0f + m_stStat.Level * 5.0f;

    m_stStat.fATK = 35.0f + m_stStat.Level * 20.0f;
    m_stStat.fDEF = 10.0f + m_stStat.Level * 5.0f;
    m_stStat.fCurHP = 500.0f + m_stStat.Level * 100.0f;
    m_stStat.fMaxHP = 500.0f + m_stStat.Level * 100.0f;
    m_stStat.fCurMP = 300.0f + m_stStat.Level * 50.0f;
    m_stStat.fMaxMP = 300.0f + m_stStat.Level * 50.0f;
    m_stStat.fSpeed = 1.0f;
    m_stStat.fCritical = 15.0f;
    m_stStat.fHPGen = m_stStat.fMaxHP * 0.01f + m_stStat.Level * 0.5f;
    m_stStat.fMPGen = m_stStat.fMaxMP * 0.01f + m_stStat.Level * 0.5f;
    m_stStat.nCoolTime = 0;
    m_stStat.nCurEXP = 0;
    m_stStat.nMaxEXP = 100;

    IdleAnim();

    m_stSphere.fRadius = 4.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = g_pMeshManager->GetBasicMesh("sphere");
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
    if (!isAttack)
    {
        //������ ��ư ������
        if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
        {
            m_isMove = true;
            cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
            BOOL isHit = false;
            float _dist = 0.0f;
            m_isPickMonster = false;

            //�޽� ����
            for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
            {
                if (ray.IsPicked(&(*iter)->GetSphere()))
                {
                    m_isPickMonster = true;
                    m_isMoveToPoint = false;
                    isMoveToTarget = true;
                    RayCast(*iter);
                    if (!isRun && Distance((*iter)->GetPosition()) >= m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
                    {
                        RunAnim();
                    }
                }
            }

            //���̵� ���߿� ���� ���� ���̸��� �ٲ�����
            if (!m_isPickMonster)
            {
                D3DXIntersectSubset(m_pTerrain, 0, &ray.m_vOrg, &ray.m_vDir, &isHit, 0, 0, 0, &_dist, NULL, NULL);
                if (isHit)
                {
                    if (!isRun)
                    {
                        RunAnim();
                    }
                    Vector3 _Dest = ray.m_vOrg + ray.m_vDir*_dist;
                    isAttack = false;
                    isMoveToTarget = false;
                    m_isMoveToPoint = true;
                    DestPoint = _Dest;
                }
            }

        }

        //�⺻ ����
        if (g_pKeyManager->isOnceKeyDown('1'))
        {
            m_isPoint = true;
            AttackAnim();
        }

        //�������̾ƴ϶�� �̵��Ҽ��ְ� ȸ����ų�� �����ִ�.
        if (g_pKeyManager->isOnceKeyDown('Q'))
        {
            // �ٷ� �׼��� �ƴ϶� ����� ���������� �׼��̰�����
            Action("Heal", 50.0f);
        }

        if (g_pKeyManager->isOnceKeyDown('W'))
        {
            m_isMove = true;
            RunAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('W'))
        {
            MoveForword();
        }
        if (g_pKeyManager->isOnceKeyUp('W'))
        {
            IdleAnim();
        }

        if (g_pKeyManager->isOnceKeyDown('S'))
        {
            m_isMove = true;
            RunAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('S'))
        {
            MoveBackword();
        }
        if (g_pKeyManager->isOnceKeyUp('S'))
        {
            IdleAnim();
        }

        if (g_pKeyManager->isOnceKeyDown('A') && !isRun)
        {
            LeftAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('A'))
        {
            RotateLeft();
        }
        if (g_pKeyManager->isOnceKeyUp('A') && !isRun)
        {
            IdleAnim();
        }
        if (g_pKeyManager->isOnceKeyDown('D') && !isRun)
        {
            RightAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('D'))
        {
            RotateRight();
        }
        if (g_pKeyManager->isOnceKeyUp('D') && !isRun)
        {
            IdleAnim();
        }
    }
    else
    {
        //�������̶��
        //���ϴ� ��ġ�϶� �������� ����.
        if (m_pMesh->GetAnimName() == "Attack")
        {
            if (m_isPoint && m_pMesh->GetdescPos() >= m_pMesh->GetStateInfo()["Attack"].mapPosition["attack"])
            {
                m_isPoint = false;
                //������ ������ �־����
                Action("Attack", m_stStat.fATK);// �ٽ��ؾ���
                if (m_pTarget)
                {
                    m_pTarget->RayCast(this); // ��׷� �ְ�
                    cMonster* Target = (cMonster*)m_pTarget;
                    Target->SetAggroTime(AggroTime);
                }
            }
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                IdleAnim();
            }
        }
        else
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                Action("Attack", m_stStat.fATK);// �ٽ��ؾ���
                if (m_pTarget)
                {
                    m_pTarget->RayCast(this); // ��׷� �ְ�
                    cMonster* Target = (cMonster*)m_pTarget;
                    Target->SetAggroTime(AggroTime);
                }
                IdleAnim();
            }
        }
    }

    //����Ʈ ������
    // Ÿ�ϸ� �����ش�.
    if (g_pKeyManager->isOnceKeyDown(VK_SHIFT))
    {
        if ((*m_vecMonster).size() > 0)
        {
            NearestSearch((*m_vecMonster));
        }
    }

    //���콺 �̵�
    if (m_isMoveToPoint)
    {
        Vector3 _Dir = DestPoint - m_vPosition;

        D3DXVec3Normalize(&_Dir, &_Dir);
        m_vDir = _Dir;

        m_vPosition += _Dir * m_fMoveSpeed;
        m_stSphere.vCenter = m_vPosition;

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &_Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

        if (_Dir.z > 0)
        {
            m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
        }
        else
        {
            m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
        }
        D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
        D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

        //����
        if (Distance(DestPoint) < m_fMoveSpeed)
        {
            m_vPosition = DestPoint;
            IdleAnim();
            m_isMoveToPoint = false;
            DestPoint = Vector3(0, 0, 0);
        }
    }

    // Ÿ�����ϰ� ����������
    if (isMoveToTarget)
    {
        Vector3 Dir = m_pTarget->GetPosition() - m_vPosition;
        float Distance = D3DXVec3Length(&Dir);
        D3DXVec3Normalize(&Dir, &Dir);
        m_vDir = Dir;

        // ���ݸ������ ����
        if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius )
        {
            isMoveToTarget = false;
            m_isPoint = true;
            AttackAnim();
        }
        else
        {
            m_vPosition += Dir * m_fMoveSpeed;
            m_stSphere.vCenter = m_vPosition;
        }
        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);
        if (Dir.z > 0)
        {
            m_fRotY = atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
        }
        else
        {
            m_fRotY = D3DX_PI - atan2(m_MatRotate._31, sqrt(pow(m_MatRotate._32, 2) + pow(m_MatRotate._33, 2)));
        }
        D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
    }

    m_pMesh->SetScale(8.0f);
    m_pMesh->SetPosition(m_vPosition);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
}

void cPlayer::Render()
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
