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
    m_fMoveSpeed = 0.0f;
}

void cPlayer::Update()
{
    if (g_pKeyManager->isOnceKeyDown(VK_ESCAPE))
    {
        m_pTarget = NULL;
    }

    if (g_pKeyManager->isOnceKeyDown('C'))
    {
        cout << "공격력:" << m_stStat.fATK << endl;
        cout << "현재경험치:" << m_stStat.nCurEXP << endl;
    }

    //레벨업
    if (m_stStat.nCurEXP >= m_stStat.nMaxEXP)
    {
        m_stStat.nCurEXP -= m_stStat.nMaxEXP;
        m_stStat.Level++;
        SetLevelToStatus(m_stStat.szName,m_stStat.Level);
        cout << "레벨업 " << endl;
        cout << "현재 레벨:" << m_stStat.Level << endl;
    }

    if (!isAttack)
    {
        //오른쪽 버튼 누를시
        if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
        {
            m_isMouse = true;;
            cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
            PickMonster(ray);
            PickGround(ray);
        }

        //기본 공격
        if (g_pKeyManager->isOnceKeyDown('1'))
        {
            m_isPoint = true;
            AttackAnim();
        }

        //공격중이아니라면 이동할수있고 회복스킬을 쓸수있다.
        if (g_pKeyManager->isOnceKeyDown('Q'))
        {
            // 바로 액션이 아니라 모션이 다지나야지 액션이가능함
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
        //공격중인 에니메이션이 실행중이라면
        //일정 타이밍에 데미지가 들어가게
        if (m_pMesh->GetAnimName() == "Attack0")
        {
            if (m_isPoint && m_pMesh->GetdescPos() >= m_pMesh->GetStateInfo()["Attack"].mapPosition["attack"])
            {
                m_isPoint = false;
                if (m_pTarget)
                {
                    //데미지 계산식을 넣어야함
                    Action("Attack", m_stStat.fATK + (m_stStat.fSTR * 2)-m_pTarget->GetStatus().fDEF);

                    m_pTarget->RayCast(this); // 어그로 주고
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
        //일정 타이밍이없다면 그냥 데미지가 들어가게
        else
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                if (m_pTarget)
                {
                    //데미지 계산식을 넣어야함
                    float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) <= m_pTarget->GetStatus().fDEF ? 1 : m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;
                    Action("Attack", ATK);

                    m_pTarget->RayCast(this); // 어그로 주고
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

    // 쉬프트 누를시
    // 타켓을 정해준다
    if (g_pKeyManager->isOnceKeyDown(VK_TAB))
    {
        if ((*m_vecMonster).size() > 0)
        {
            NearestSearch((*m_vecMonster));
        }
    }

    //마우스 이동
    if (m_isMoveToPoint)
    {
        Move();
    }

    // 타게팅하고 때리러갈때
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

void cPlayer::PickMonster(cRay ray)
{
    isMove = true;
    m_isPickMonster = false;
    //메쉬 공격
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

    //맵이동 나중에 씬에 따라서 맵이름이 바뀌어야함
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

    // 공격모션으로 변경
    if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
    {
        isMoveToTarget = false;
        m_isPoint = true;
        AttackAnim();
    }
    else
    {
        m_vPosition += m_vDir * m_fMoveSpeed * g_pTimerManager->GetDeltaTime();
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
   
    m_vPosition += m_vDir * m_fMoveSpeed * g_pTimerManager->GetDeltaTime();
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

    //도착
    if (Distance(DestPoint) < m_fMoveSpeed * g_pTimerManager->GetDeltaTime())
    {
        m_vPosition = DestPoint;
        IdleAnim();
        m_isMoveToPoint = false;
        DestPoint = Vector3(0, 0, 0);
    }
}

void cPlayer::SetLevelToStatus(string szKey, int Level)
{
    json status = g_pMeshManager->GetJson("Status");

    m_stStat.szName = szKey;
    m_stStat.Level = Level;
    m_stStat.fSTR = (float)status[szKey]["STR"] + Level * (float)status[szKey]["UPSTR"];
    m_stStat.fDEX = (float)status[szKey]["DEX"] + Level * (float)status[szKey]["UPDEX"];
    m_stStat.fINT = (float)status[szKey]["INT"] + Level * (float)status[szKey]["UPINT"];
                                                  
    m_stStat.fATK = (float)status[szKey]["ATK"] + Level * (float)status[szKey]["UPATK"];
    m_stStat.fDEF = (float)status[szKey]["DEF"] + Level * (float)status[szKey]["UPDEF"];
    m_stStat.fCurHP = (float)status[szKey]["HP"] + Level * (float)status[szKey]["UPHP"];
    m_stStat.fMaxHP = (float)status[szKey]["HP"] + Level * (float)status[szKey]["UPHP"];
    m_stStat.fCurMP = (float)status[szKey]["MP"] + Level * (float)status[szKey]["UPMP"];
    m_stStat.fMaxMP = (float)status[szKey]["MP"] + Level * (float)status[szKey]["UPMP"];

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