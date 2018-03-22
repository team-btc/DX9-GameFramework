#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"
#include "cMonster.h"

cPlayer::cPlayer(string szKey, string szFolder, string szFilename)
    :DestPoint(0,0,0)
    , m_isMoveToPoint(false)
    , m_isPoint(false)
    , m_isPickMonster(false)
    , m_isMouse(false)
    , m_fScale(0.0f)
    , m_fCenter(0.0f)
    , m_fWalkTime(0.36f)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_vecMonster = NULL;

    m_eTag = PLAYER;

    m_stStat.szName = szKey;
    m_stStat.Level = 1;

    SetLevelToStatus(m_stStat.szName, m_stStat.Level);

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
    , m_isMouse(false)
    , m_fScale(0.0f)
    , m_fCenter(0.0f)
    , m_nPrevIndex(0)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_vecMonster = NULL;

    m_eTag = PLAYER;

    m_stStat.szName = szKey;
    m_stStat.Level = 1;

    SetLevelToStatus(m_stStat.szName, m_stStat.Level);

    IdleAnim();

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
    SetPlayerAtkSound();
    srand(time(NULL));
}

void cPlayer::Update()
{
    m_nRandIndex = rand() % 10;
    if (g_pKeyManager->isOnceKeyDown(VK_ESCAPE))
    {
        m_pTarget = NULL;
    }

    if (g_pKeyManager->isOnceKeyDown('C'))
    {
        cout << "���ݷ�:" << m_stStat.fATK << endl;
    }

    //������
    if (m_stStat.nCurEXP >= m_stStat.nMaxEXP)
    {
        m_stStat.nCurEXP -= m_stStat.nMaxEXP;
        m_stStat.Level++;
        SetLevelToStatus(m_stStat.szName,m_stStat.Level);
        cout << "������ " << endl;
        cout << "���� ����:" << m_stStat.Level << endl;
    }

    if (!isAttack)
    {
        //������ ��ư ������
        if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
        {
            m_isMouse = true;;
            cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
            PickMonster(ray);
            PickGround(ray);
        }

        //�⺻ ����
        if (g_pKeyManager->isOnceKeyDown('1'))
        {
            m_isPoint = true;
            AttackAnim();
            if (g_pSndManager->IsPlay(m_vecPlayerAtkSound[m_nPrevIndex]))
            {
                g_pSndManager->Stop(m_vecPlayerAtkSound[m_nPrevIndex]);
                g_pSndManager->Play(m_vecPlayerAtkSound[m_nRandIndex], 0.7f);
                m_nPrevIndex = m_nRandIndex;
            }
            else
            {
                g_pSndManager->Play(m_vecPlayerAtkSound[m_nRandIndex], 0.7f);
                m_nPrevIndex = m_nRandIndex;
            }
        }

        //�������̾ƴ϶�� �̵��Ҽ��ְ� ȸ����ų�� �����ִ�.
        if (g_pKeyManager->isOnceKeyDown('Q'))
        {
            // �ٷ� �׼��� �ƴ϶� ����� ���������� �׼��̰�����
            Action("Heal", 50.0f);
        }

        
        if (g_pKeyManager->isOnceKeyDown('W'))
        {
            m_isMoveToPoint = false;
            isMoveToTarget = false;
            m_isMouse = false;
            isMove = true;
            RunAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('W') && !m_isMouse)
        {
            MoveForword();
        }
        if (g_pKeyManager->isOnceKeyUp('W'))
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
        //�������� ���ϸ��̼��� �������̶��
        //���� Ÿ�ֿ̹� �������� ����
        if (m_pMesh->GetAnimName() == "Attack0")
        {
            if (m_isPoint && m_pMesh->GetdescPos() >= m_pMesh->GetStateInfo()["Attack"].mapPosition["attack"])
            {
                m_isPoint = false;
                if (m_pTarget)
                {
                    //������ ������ �־����
                    Action("Attack", m_stStat.fATK + (m_stStat.fSTR * 2)-m_pTarget->GetStatus().fDEF);

                    m_pTarget->RayCast(this); // ��׷� �ְ�
                    if (m_pTarget->GetTag() == MONSTER)
                    {
                        cMonster* Target = (cMonster*)m_pTarget;
                        Target->SetAggroTime(AggroTime);
                    }
                }
            }
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                IdleAnim();
            }
        }
        //���� Ÿ�̹��̾��ٸ� �׳� �������� ����
        else
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                if (m_pTarget)
                {
                    //������ ������ �־����
                    float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) <= m_pTarget->GetStatus().fDEF ? 1 : m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;
                    Action("Attack", ATK);

                    m_pTarget->RayCast(this); // ��׷� �ְ�
                    if (m_pTarget->GetTag() == MONSTER)
                    {
                        cMonster* Target = (cMonster*)m_pTarget;
                        Target->SetAggroTime(AggroTime);
                    }
                }
                IdleAnim();
            }
        }
    }

    // ����Ʈ ������
    // Ÿ���� �����ش�
    if (g_pKeyManager->isOnceKeyDown(VK_TAB))
    {
        if ((*m_vecMonster).size() > 0)
        {
            NearestSearch((*m_vecMonster));
        }
    }

    //���콺 �̵�
    if (m_isMoveToPoint)
    {
        Move();
    }

    // Ÿ�����ϰ� ����������
    if (isMoveToTarget)
    {
        Attack();
    }

   

    m_pMesh->SetScale(m_fScale);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
    m_pMesh->SetPosition(m_vPosition);
}

void cPlayer::Render()
{
    m_pMesh->UpdateAndRender();

#ifdef _DEBUG
    Matrix4 matT, matW;
    D3DXMatrixScaling(&m_MatScale, m_stSphere.fRadius, m_stSphere.fRadius, m_stSphere.fRadius);
    D3DXMatrixTranslation(&matT, m_stSphere.vCenter.x, m_stSphere.vCenter.y + m_fCenter, m_stSphere.vCenter.z);
    matW = m_MatScale * matT;
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //m_pPikingMesh->DrawSubset(0);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif // _DEBUG
}

ULONG cPlayer::Release()
{
    return cObject::Release();
}

void cPlayer::GetSwordMatrix(Matrix4 & mat)
{
    m_pMesh->GetMatrixByName(mat, "creature_arthaslichking_arthaslichking_bone_43");
}

void cPlayer::PlayWalkSound()
{
    if (g_pMapManager->GetCurrKey() == "start")
    {
        m_fWalkTime += g_pTimerManager->GetDeltaTime();
        if (m_fWalkTime >= 0.45f)
        {
            if (g_pSndManager->IsPlay("start-walk"))
            {
                g_pSndManager->Stop("start-walk");
                g_pSndManager->Play("start-walk");
            }
            else
            {
                g_pSndManager->Play("start-walk");
            }
            m_fWalkTime = 0.0f;
        }
    }
    else if (g_pMapManager->GetCurrKey() == "badland")
    {
        m_fWalkTime += g_pTimerManager->GetDeltaTime();
        if (m_fWalkTime >= 0.45f)
        {
            if (g_pSndManager->IsPlay("badland-walk"))
            {
                g_pSndManager->Stop("badland-walk");
                g_pSndManager->Play("badland-walk");
            }
            else
            {
                g_pSndManager->Play("badland-walk");
            }
            m_fWalkTime = 0.0f;
        }
    }
    else if (g_pMapManager->GetCurrKey() == "tempdun")
    {
        m_fWalkTime += g_pTimerManager->GetDeltaTime();
        if (m_fWalkTime >= 0.45f)
        {
            if (g_pSndManager->IsPlay("start-walk"))
            {
                g_pSndManager->Stop("tempdun-walk");
                g_pSndManager->Play("tempdun-walk");
            }
            else
            {
                g_pSndManager->Play("tempdun-walk");
            }
            m_fWalkTime = 0.0f;
        }
    }
}

void cPlayer::SetPlayerAtkSound()
{
    m_vecPlayerAtkSound.push_back("attack1");
    m_vecPlayerAtkSound.push_back("attack2");
    m_vecPlayerAtkSound.push_back("attack3");
    m_vecPlayerAtkSound.push_back("attack4");
    m_vecPlayerAtkSound.push_back("attack5");
    m_vecPlayerAtkSound.push_back("attack6");
    m_vecPlayerAtkSound.push_back("attack7");
    m_vecPlayerAtkSound.push_back("attack8");
    m_vecPlayerAtkSound.push_back("attack9");
    m_vecPlayerAtkSound.push_back("attack10");
}

void cPlayer::PickMonster(cRay ray)
{
    isMove = true;
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
}

void cPlayer::PickGround(cRay ray)
{
    BOOL isHit = false;
    float _dist = 0.0f;

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

void cPlayer::Attack()
{
    m_vDir = m_pTarget->GetPosition() - m_vPosition;
    float Distance = D3DXVec3Length(&m_vDir);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    // ���ݸ������ ����
    if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
    {
        isMoveToTarget = false;
        m_isPoint = true;
        AttackAnim();
        if (g_pSndManager->IsPlay(m_vecPlayerAtkSound[m_nPrevIndex]))
        {
            g_pSndManager->Stop(m_vecPlayerAtkSound[m_nPrevIndex]);
            g_pSndManager->Play(m_vecPlayerAtkSound[m_nRandIndex], 0.7f);
            m_nPrevIndex = m_nRandIndex;
        }
        else
        {
            g_pSndManager->Play(m_vecPlayerAtkSound[m_nRandIndex], 0.7f);
            m_nPrevIndex = m_nRandIndex;
        }
    }
    else
    {
        PlayWalkSound();
        m_vPosition += m_vDir * m_fMoveSpeed;
        m_stSphere.vCenter = m_vPosition;
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

void cPlayer::Move()
{
    m_vDir = DestPoint - m_vPosition;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
   
    m_vPosition += m_vDir * m_fMoveSpeed;
    m_stSphere.vCenter = m_vPosition;

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
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    PlayWalkSound();
    //����
    if (Distance(DestPoint) < m_fMoveSpeed)
    {
        m_vPosition = DestPoint;
        IdleAnim();
        m_isMoveToPoint = false;
        DestPoint = Vector3(0, 0, 0);
        m_fWalkTime = 0.36f;
    }
}

void cPlayer::SetLevelToStatus(string szKey, int Level)
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