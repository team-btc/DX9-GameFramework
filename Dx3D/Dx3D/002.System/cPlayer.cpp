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
    , m_fRecoveryCount(0.0f)
    , m_nPrevIndex(0)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_vecMonster = NULL;

    m_eTag = PLAYER;

    m_stStat.szName = szKey;
    m_stStat.Level = g_pMeshManager->GetJson("Status")[m_stStat.szName]["LEVEL"];

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
    UISetup();
    m_fMoveSpeed = 0.0f;
    SetPlayerAtkSound();
    m_isMoveToPoint = false;
    IdleAnim();
}

void cPlayer::Update()
{
    if (m_stStat.fCurHP < 0.0f)
    {
        m_stStat.fCurHP = 1.0f;
        //PostQuitMessage(1);
    }

    m_nRandIndex = rand() % 10;
    if (g_pKeyManager->isOnceKeyDown(VK_ESCAPE))
    {
        m_pTarget = NULL;
    }

    if (g_pKeyManager->isOnceKeyDown('G'))
    {
        m_stStat.Level--;
        SetLevelToStatus(m_stStat.szName, m_stStat.Level);
    }

    if (g_pKeyManager->isOnceKeyDown('H'))
    {
        m_stStat.Level++;
        SetLevelToStatus(m_stStat.szName, m_stStat.Level);
    }

    if (g_pKeyManager->isOnceKeyDown('J'))
    {
        g_pGameManager->Pay(1000);
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

    //레벨업
    if (m_stStat.nCurEXP >= m_stStat.nMaxEXP)
    {
        m_stStat.nCurEXP -= m_stStat.nMaxEXP;
        m_stStat.Level++;
        SetLevelToStatus(m_stStat.szName,m_stStat.Level);

        if (m_isUsingRoar)
        {
            m_stStat.fCurHP += 500.0f;
            m_stStat.fMaxHP += 500.0f;
            m_stStat.fATK += 100.0f;
            m_stStat.fDEF += 50.0f;
            m_fScale = 12.0f;
        }
    }

    if (m_isUsingRoar)
    {
        m_fRoartime -= g_pTimerManager->GetDeltaTime();
        if (m_fRoartime <= 0.0f)
        {
            m_stStat.fMaxHP -= 500.0f;
            m_stStat.fCurHP > m_stStat.fMaxHP ? m_stStat.fCurHP = m_stStat.fMaxHP : m_stStat.fCurHP;
            m_stStat.fATK -= 100.0f;
            m_stStat.fDEF -= 50.0f;
            m_fScale = 8.0f;
            m_isUsingRoar = false;
        }
    }
    
    if (!isAttack && !isAction)
    {
        //오른쪽 버튼 누를시
        if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON))
        {
            isRecovery = false;
            m_fRecoveryCount = 0.0f;
            m_isMouse = true;
            cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
            PickMonster(ray);
            PickGround(ray);
        }
        
        //기본 공격
        if (g_pKeyManager->isOnceKeyDown('1'))
        {
            isRecovery = false;
            m_fRecoveryCount = 0.0f;
            m_isPoint = true;
            AttackAnim();
            g_pSndManager->Play(m_vecPlayerAtkSound[m_nRandIndex], 0.7f);
            m_nPrevIndex = m_nRandIndex;
        }

        //원거리 스킬
        if (g_pKeyManager->isOnceKeyDown('2'))
        {
            if (m_stStat.fCurMP >= 200.0f)
            {
                isRecovery = false;
                m_fRecoveryCount = 0.0f;
                m_isPoint = true;
                CastingAnim();
                g_pSndManager->Play("skill");
                isCasting = true;
                isAction = true;
                m_isMoveToPoint = false;
                m_fMoveSpeed = 0.0f;
            }
        }

        //위협
        if (g_pKeyManager->isOnceKeyDown('3')&& !isRoar)
        {
            if (m_stStat.fCurMP >= 50.0f)
            {
                isRecovery = false;
                m_fRecoveryCount = 0.0f;
                RoarAnim();
                isAction = true;
                m_isMoveToPoint = false;
                m_fMoveSpeed = 0.0f;

                if (!m_isUsingRoar)
                {
                    m_stStat.fCurHP += 500.0f;
                    m_stStat.fMaxHP += 500.0f;
                    m_stStat.fATK += 100.0f;
                    m_stStat.fDEF += 50.0f;
                    m_fScale = 12.0f;
                    m_isUsingRoar = true;
                }
                m_fRoartime = 50.0f;
            }
        }

        //공격중이아니라면 이동할수있고 회복스킬을 쓸수있다.
        if (g_pKeyManager->isOnceKeyDown('Q'))
        {
            // 바로 액션이 아니라 모션이 다지나야지 액션이가능함
            if (m_stStat.fCurMP >= 50.0f && m_stStat.fCurHP < m_stStat.fMaxHP)
            {
                g_pSndManager->Play("heal");
                HealAnim();
                isAction = true;
            }
        }

        
        if (g_pKeyManager->isOnceKeyDown('W'))
        {
            isRecovery = false;
            m_fRecoveryCount = 0.0f;
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

        //체젠마젠
        if (isRecovery)
        {
            m_stStat.fCurHP < m_stStat.fMaxHP ? m_stStat.fCurHP += m_stStat.fHPGen * g_pTimerManager->GetDeltaTime() : m_stStat.fCurHP = m_stStat.fMaxHP;
            m_stStat.fCurMP < m_stStat.fMaxMP ? m_stStat.fCurMP += m_stStat.fMPGen * g_pTimerManager->GetDeltaTime() : m_stStat.fCurMP = m_stStat.fMaxMP;
        }
        else
        {
            m_fRecoveryCount += g_pTimerManager->GetDeltaTime();
            if (m_fRecoveryCount > 5.0f)
            {
                isRecovery = true;
            }
        }
        
    }
    else
    {
        //공격중인 에니메이션이 실행중이라면
        //일정 타이밍에 데미지가 들어가게
        if (m_pMesh->GetAnimName() == "Attack0")
        {
            if (m_isPoint && m_pMesh->GetdescPos() >= m_pMesh->GetStateInfo()["Attack0"].mapPosition["attack"])
            {
                m_isPoint = false;
                if (m_pTarget)
                {
                    if (Distance(m_pTarget->GetPosition()) < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
                    {
                        //데미지 계산식을 넣어야함
                        float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;

                        Action("Attack", ATK);

                        // UI 셋팅
                        AddAttUI((int)ATK);

                        m_pTarget->RayCast(this); // 어그로 주고

                        if (m_pTarget->GetTag() == MONSTER)
                        {
                            cMonster* Target = (cMonster*)m_pTarget;
                            Target->SetAggroTime(AggroTime);
                        }
                    }
                }
            }
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                IdleAnim();
            }
        }
        else if (m_pMesh->GetAnimName() == "Attack1")
        {
            if (m_isPoint && m_pMesh->GetdescPos() >= m_pMesh->GetStateInfo()["Attack1"].mapPosition["attack"])
            {
                m_isPoint = false;
                if (m_pTarget)
                {
                    if (Distance(m_pTarget->GetPosition()) < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
                    {
                        float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;
                        //데미지 계산식을 넣어야함
                        Action("Attack", ATK);

                        AddAttUI((int)ATK);

                        m_pTarget->RayCast(this); // 어그로 주고
                        if (m_pTarget->GetTag() == MONSTER)
                        {
                            cMonster* Target = (cMonster*)m_pTarget;
                            Target->SetAggroTime(AggroTime);
                        }
                    }
                }
            }
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                IdleAnim();
            }
        }
        //일정 타이밍이없다면 그냥 데미지가 들어가게
        else if (isRoar)
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                isAction = false;
                isRoar = false;
                IdleAnim();
            }
        }
        else if (isHeal)
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                Action("Heal", 50.0f + m_stStat.fINT * 4);
                m_stStat.fCurMP -= 50.0f;
                isAction = false;
                isHeal = false;
                IdleAnim();
            }
        }
        else if (isCasting)
        {
            if (m_pMesh->GetCurPos() >= 3.0f)
            {
                isCasting = false;
                FireAnim();
            }
        }
        else if (isFire)
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                isAction = false;
                isFire = false;
                if (m_pTarget)
                {
                    //데미지 계산식을 넣어야함 조금수정해야함
                    float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) <= m_pTarget->GetStatus().fDEF ? 1 : m_stStat.fATK + (m_stStat.fSTR * 5) + (m_stStat.fINT*8);
                    Action("Attack", ATK);
                    m_stStat.fCurMP -= 200.0f;

                    // UI 셋팅
                    AddAttUI((int)ATK);

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
        else
        {
            if (m_pMesh->GetCurPos() >= 1.0f)
            {
                if (m_pTarget)
                {
                    if (Distance(m_pTarget->GetPosition()) < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
                    {
                        //데미지 계산식을 넣어야함
                        float ATK = m_stStat.fATK + (m_stStat.fSTR * 2) <= m_pTarget->GetStatus().fDEF ? 1 : m_stStat.fATK + (m_stStat.fSTR * 2) - m_pTarget->GetStatus().fDEF;
                        Action("Attack", ATK);

                        // UI 셋팅
                        AddAttUI((int)ATK);

                        m_pTarget->RayCast(this); // 어그로 주고
                        if (m_pTarget->GetTag() == MONSTER)
                        {
                            cMonster* Target = (cMonster*)m_pTarget;
                            Target->SetAggroTime(AggroTime);
                        }
                    }
                }
                IdleAnim();
            }
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

    UIUpdate();

    m_pMesh->SetScale(m_fScale);
    m_pMesh->SetRotation(Vector3(0, D3DXToDegree(m_fRotY) - 90.0f, 0));
    m_pMesh->SetPosition(m_vPosition);
}

void cPlayer::Render()
{
    m_pMesh->UpdateAndRender();

    UIRender();

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
    for (auto iter = m_vecDamageText.begin(); iter != m_vecDamageText.end();)
    {
        SAFE_DELETE(iter->pUILayer);
        iter = m_vecDamageText.erase(iter);
    }

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
            g_pSndManager->Play("start-walk");
            m_fWalkTime = 0.0f;
        }
    }
    else if (g_pMapManager->GetCurrKey() == "badland")
    {
        m_fWalkTime += g_pTimerManager->GetDeltaTime();
        if (m_fWalkTime >= 0.45f)
        {
            g_pSndManager->Play("badland-walk");
            m_fWalkTime = 0.0f;
        }
    }
    else if (g_pMapManager->GetCurrKey() == "tempdun")
    {
        m_fWalkTime += g_pTimerManager->GetDeltaTime();
        if (m_fWalkTime >= 0.45f)
        {
            g_pSndManager->Play("tempdun-walk");
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
    //메쉬 공격
    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        if (ray.IsPicked(&(*iter)->GetSphere()))
        {
            isRecovery = false;
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
        g_pSndManager->Play(m_vecPlayerAtkSound[m_nRandIndex], 0.7f);
        m_nPrevIndex = m_nRandIndex;
    }
    else
    {
        m_vPosition += m_vDir * m_fMoveSpeed * g_pTimerManager->GetDeltaTime();
        PlayWalkSound();
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
    PlayWalkSound();
    //도착
    if (Distance(DestPoint) < m_fMoveSpeed * g_pTimerManager->GetDeltaTime())
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

    m_stStat.fHPGen = (float)status[szKey]["HPGEN"] + Level * (float)status[szKey]["UPHPGEN"];
    m_stStat.fMPGen = (float)status[szKey]["MPGEN"] + Level * (float)status[szKey]["UPMPGEN"];

    m_stStat.fSpeed = (float)status[szKey]["SPEED"];
    m_stStat.fCritical = (float)status[szKey]["CRITICAL"];
    m_stStat.nCoolTime = status[szKey]["COOLTIME"];
    m_stStat.nCurEXP = 0;
    m_stStat.nMaxEXP = status[szKey]["EXP"];

    m_fScale = (float)status[szKey]["SIZE"];
    m_stSphere.fRadius = (float)status[szKey]["PICKINGRADIUS"];
    m_fCenter = (float)status[szKey]["CENTERPOS"];
}

void cPlayer::UISetup()
{
    // 공격 값 UI
    if (m_vecDamageText.empty())
    {
        m_vecDamageText.resize(5);
        for (int i = 0; i < m_vecDamageText.size(); ++i)
        {
            m_vecDamageText[i] = MakeDamageText();
        }
    }

    if (!m_vecDamageText.empty())
    {
        for (int i = 0; i < m_vecDamageText.size(); ++i)
        {
            if (m_vecDamageText[i].pUILayer)
            {
                m_vecDamageText[i].pUILayer->Setup();
            }
        }
    }
}

void cPlayer::UIUpdate()
{
    if (!m_vecDamageText.empty())
    {
        // 텍스트 위치 관리
        for (int i = 0; i < m_vecDamageText.size(); ++i)
        {
            if (m_vecDamageText[i].isAxtive && m_vecDamageText[i].pTextUI)
            {
                m_vecDamageText[i].vCurrPos.y += m_vecDamageText[i].fPlusY;

                Matrix4 matView, matProj, matVP;
                g_pDevice->GetTransform(D3DTS_VIEW, &matView);
                g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

                D3DVIEWPORT9 vp;
                g_pDevice->GetViewport(&vp);
                D3DXMatrixIdentity(&matVP);
                matVP._11 = vp.Width / 2.0f;
                matVP._22 = -(vp.Height / 2.0f);
                matVP._33 = vp.MaxZ - vp.MinZ;
                matVP._41 = vp.X + vp.Width / 2.0f;
                matVP._42 = vp.Y + vp.Height / 2.0f;
                matVP._43 = vp.MinZ;

                Vector3 vScreenPos;
                D3DXVec3TransformCoord(&vScreenPos, &m_vecDamageText[i].vCurrPos, &(matView * matProj * matVP));

                vScreenPos.x -= m_vecDamageText[i].pTextUI->GetSize().x * 0.5f;
                vScreenPos.y -= m_vecDamageText[i].pTextUI->GetSize().y * 0.5f;

                m_vecDamageText[i].pTextUI->SetLocalPos(vScreenPos);

                if (m_vecDamageText[i].vCurrPos.y >= m_vecDamageText[i].fMaxY)
                {
                    m_vecDamageText[i].isAxtive = false;
                }
            }
        }
    }

    if (!m_vecDamageText.empty())
    {
        for (int i = 0; i < m_vecDamageText.size(); ++i)
        {
            if (m_vecDamageText[i].pUILayer && m_vecDamageText[i].isAxtive)
            {
                m_vecDamageText[i].pUILayer->Update();
            }
        }
    }
}

void cPlayer::UIRender()
{
    if (!m_vecDamageText.empty())
    {
        g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
        g_pDevice->SetRenderState(D3DRS_ZENABLE, false);
        for (int i = 0; i < m_vecDamageText.size(); ++i)
        {
            if (m_vecDamageText[i].pUILayer && m_vecDamageText[i].isAxtive)
            {
                m_vecDamageText[i].pUILayer->Render();
            }
        }
        g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
        g_pDevice->SetRenderState(D3DRS_ZENABLE, true);
    }
}

ST_DAMAGE_TEXT cPlayer::MakeDamageText()
{
    ST_DAMAGE_TEXT stDamage;
    stDamage.pUILayer = new cUILayer;
    stDamage.pUILayer->SetLayer("att-num-layer", Vector3(0, 0, 0), ST_SIZE(50, 50));
    stDamage.pUILayer->SetActive(true);

    stDamage.pTextUI = new cUITextView;
    char Buf[20];
    sprintf_s(Buf, sizeof(Buf), "att-num-%zd", m_vecDamageText.size());
    stDamage.pTextUI->SetName(Buf);
    stDamage.pTextUI->SetLocalPos(stDamage.vCurrPos);
    stDamage.pTextUI->SetSize(Vector2(100, 50));
    stDamage.pTextUI->SetFont(g_pFontManager->GetFont(g_pFontManager->E_DAMAGE));
    stDamage.pTextUI->SetText(stDamage.szDamage);
    stDamage.pTextUI->SetColor(D3DCOLOR_XRGB(255, 255, 0));

    stDamage.fPlusY = 0.05f;

    stDamage.pUILayer->AddUIObject(stDamage.pTextUI);

    return stDamage;
}

void cPlayer::AddAttUI(int nAttValue)
{
    bool isSet = false;

    for (int i = 0; i < m_vecDamageText.size(); ++i)
    {
        // 텍스트 UI레이어가 비어 있으면
        if (!m_vecDamageText[i].isAxtive)
        {
            m_vecDamageText[i].isAxtive = true;
            m_vecDamageText[i].vCurrPos = GetTarget()->GetPosition();
            m_vecDamageText[i].vCurrPos.y += 10;
            m_vecDamageText[i].fMaxY = m_vecDamageText[i].vCurrPos.y + 2;
            char Buf[10];
            sprintf_s(Buf, sizeof(Buf), "%d", nAttValue);
            m_vecDamageText[i].szDamage = Buf;
            m_vecDamageText[i].pTextUI->SetText(m_vecDamageText[i].szDamage);

            isSet = true;

            break;
        }
    }

    // 빈 UI레이어가 없으면
    if (!isSet)
    {
        m_vecDamageText.push_back(MakeDamageText());
    }
}
