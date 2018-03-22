#include "stdafx.h"
#include "cPlayScene.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIProgressBar.h"
#include "cShop.h"
#include "cInventory.h"

cPlayScene::cPlayScene()
    : m_pPlayerStatUILayer(NULL)
    , m_pHPUILayer(NULL)
	, m_stMapInfo(NULL)
    , m_pCamera(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_szMapKey("start")
    , m_pShop(NULL)
    , m_pInventory(NULL)
    , m_pParticleFrost(NULL)
    , m_pPlayer(NULL)
    , m_isRoar(false)
    , m_pParticleIce(NULL)
    , m_pParticleCherry(NULL)
    , m_pParticleEffect(NULL)
{
}


cPlayScene::~cPlayScene()
{
}

HRESULT cPlayScene::Start()
{
    //  LOAD MAP
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.SetKey(m_szMapKey);
        mapLoader.LoadMap();
        for (int i = 0; i < mapLoader.GetObjectMaxCnt(); ++i)
        {
            mapLoader.LoadObject(i);
        }

        m_stMapInfo = g_pMapManager->GetCurrMapInfo();

        // 현재 맵 셋팅
        if (!m_pGameMap)
        {
            m_pGameMap = new cGameMap;
        }

        m_pGameMap->SetCurrMapInfo(m_stMapInfo);

        // 텍스쳐 쉐이더 셋팅
        if (!m_pTextureShader)
        {
            m_pTextureShader = new cTextureShader;
        }

        m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
        m_pTextureShader->SetBGTexture(m_stMapInfo->arrTextureInfo[0].pTexture, m_stMapInfo->arrTextureInfo[0].fDensity);
        m_pTextureShader->SetTexture1(m_stMapInfo->arrTextureInfo[1].pTexture, m_stMapInfo->arrTextureInfo[1].fDensity);
        m_pTextureShader->SetTexture2(m_stMapInfo->arrTextureInfo[2].pTexture, m_stMapInfo->arrTextureInfo[2].fDensity);
        m_pTextureShader->SetTexture3(m_stMapInfo->arrTextureInfo[3].pTexture, m_stMapInfo->arrTextureInfo[3].fDensity);
        m_pTextureShader->SetAlphaDraw(m_stMapInfo->pTextureMap);

        // 스카이박스 쉐이더 셋팅
        if (!m_pSkyBoxShader)
        {
            m_pSkyBoxShader = new cSkyBoxShader;
        }

        m_pSkyBoxShader->SetBox(m_stMapInfo->pSkyTexture);

        // 물이 존재하면 웨이브 쉐이더 셋팅
        if (m_stMapInfo->isEnableWater)
        {
            if (!m_pWaveShader)
            {
                m_pWaveShader = new cWaveShader;
            }

            m_pWaveShader->SetMesh(m_stMapInfo->pWaterMesh);
            m_pWaveShader->SetWaveTexture(m_stMapInfo->pWaterTexture);
            m_pWaveShader->SetShader(m_stMapInfo->fWaterHeight, m_stMapInfo->fWaterWaveHeight, m_stMapInfo->fWaterHeightSpeed,
                m_stMapInfo->fWaterUVSpeed, m_stMapInfo->fWaterfrequency, m_stMapInfo->fWaterTransparent, m_stMapInfo->fWaterDensity);
        }
    }

    //  LOAD SHOP
    if (!m_pShop)
    {
        // 상점 셋팅
        m_pShop = new cShop;
        m_pShop->Setup();
    }
    //  LOAD INVENTORY
    if (!m_pInventory)
    {
        m_pInventory = new cInventory;
        m_pInventory->Setup();
    }

    if (!m_pFrustum)
    {
        m_pFrustum = new cFrustum;
        g_pAutoReleasePool->AddObject(m_pFrustum);
    }

    m_pFrustum->Setup();

    //  시작 지점 세팅
    string szPrevMap = "from-" + g_pMapManager->GetPrevMap();
    auto iter = m_stMapInfo->vecEventInfo.begin();
    for (; iter != m_stMapInfo->vecEventInfo.end(); iter++)
    {
        if (iter->szName == szPrevMap)
        {
            m_stMapInfo->vStartPos = iter->vPos;
            break;
        }

        if (szPrevMap == "from-" && iter->szName == "startpos")
        {
            m_stMapInfo->vStartPos = iter->vPos;
            break;
        }
    }
  
    //몬스터가 존재한다면 몬스터 회수
    if (!m_vecMonster)
    {
        m_vecMonster = new vector<iCharacterObject*>;
    }
    else
    {
        for (auto iter = m_vecMonster->begin(); iter != m_vecMonster->end(); iter++)
        {
            if ((*iter)->GetName() != "Deathwing")
            {
                g_pCharacterManager->PushMonster(*iter);
            }
            else
            {
                g_pCharacterManager->PushBoss((cBoss*)(*iter));
            }
        }
    }
    m_vecMonster->clear();

   //몬스터 생성
    for (int i = 0; i < m_stMapInfo->vecEventInfo.size(); i++)
    {
        if (m_stMapInfo->vecEventInfo[i].szName == "monster")
        {
            cMonster* Enermy = g_pCharacterManager->GetMonster(m_szMapKey);
            Enermy->SetStartPoint(m_stMapInfo->vecEventInfo[i].vPos);
            Enermy->SetActive(true);
            (*m_vecMonster).push_back(Enermy);
        }
        else if (m_stMapInfo->vecEventInfo[i].szName == "boss")
        {
            cBoss* Enermy = g_pCharacterManager->GetBoss();
            Enermy->SetStartPoint(m_stMapInfo->vecEventInfo[i].vPos);
            (*m_vecMonster).push_back(Enermy);
        }
    }

    if (!m_pPlayer)
    {
        m_pPlayer = g_pCharacterManager->GetPlayer();
    }

    if (!g_pGameManager->IsLoadData())
    {
        m_pPlayer->SetPosition(m_stMapInfo->vStartPos);
    }
    else
    {
        g_pGameManager->DisableLoadFlag();
    }

    m_pPlayer->SetVecMonster(m_vecMonster);
    m_pPlayer->SetTerrain(m_stMapInfo->pTerrainMesh);
    m_pPlayer->Setup();

    //  CAMERA SETUP
    if (!m_pCamera)
    {
        m_pCamera = new cCamera;
        m_pCamera->EnableFocus();
        m_pCamera->SetMaxDist(100.0f);
        m_pCamera->SetMinDist(5.0f);
        m_pCamera->SetLookatOffset(8.0f);
        g_pCameraManager->AddCamera("play", m_pCamera);
        g_pCameraManager->SetCollisionMesh(m_stMapInfo->pTerrainMesh);
        g_pCameraManager->EnableCollider();
    }
    m_pCamera->Setup();
    g_pCameraManager->SetCurrCamera("play");

    // UI 셋팅
    SetUI();

    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Setup();
    }

    //  파티클 세팅
    if (!m_pParticleFrost)
    {
        float power = 2.0f;
        Matrix4 mat;
        m_pPlayer->GetSwordMatrix(mat);
        Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleFrost = new cParticle(&pos, 20, 200, true, Vector3(-1.0f, 0, -0.5f), Vector3(0.5f, 1, 0.5f));
        m_pParticleFrost->Init("smog01");
        ST_PARTICLE_ATTR attr;
        attr.fGravity = -0.03f;
        attr.isLoop = true;
        attr.deltaAccelMin = Vector3(power * 0.5f, -power * 0.0f, power * 0.5f);
        attr.deltaAccelMax = Vector3(-power * 0.5f, -power * 1.5f, -power * 0.5f);
        attr.life = 0.5f;
        attr.fSpeed = 0.04f;
        attr.vAccel = Vector3(0.2f, 0.1f, 0.4f);
        attr.fMinLife = 0.1f;
        attr.fMaxLife = 0.9f;
        attr.color = D3DCOLOR_RGBA(0, 20, 250, 255);
        attr.isFade = true;
        attr.fadeColor = D3DCOLOR_RGBA(0, 25, 250, 180);
        m_pParticleFrost->SetSize(3.5f);
        m_pParticleFrost->SetGenTerm(0.4f);
        m_pParticleFrost->Reset(attr);
        m_pParticleFrost->RemoveBlack(true);
    }

    if (!m_pParticleIce)
    {
        float power = 1.0f;
        Vector3 pos = m_pPlayer->GetPosition();

        m_pParticleIce = new cParticle(&pos, 20, 100, true, Vector3(-3, 0, -3), Vector3(3, 0.5f, 3));
        m_pParticleIce->Init("smog04");
        ST_PARTICLE_ATTR attr;
        attr.fGravity = -0.08f;
        attr.isLoop = true;
        attr.deltaAccelMin = Vector3(power * 0.5f, -power * 0.5f, power * 0.5f); 
        attr.deltaAccelMax = Vector3(-power * 0.5f, -power * 0.5f, -power * 0.5f);
        attr.life = 2.3f;
        attr.fSpeed = 0.1f;
        attr.fMinLife = 2.3f;
        attr.fMaxLife = 3.7f;
        attr.isVariableSpeed = true;
        attr.color = D3DCOLOR_RGBA(187, 39, 5, 15);
        attr.isFade = false;
        m_pParticleIce->SetSize(4.5f);
        m_pParticleIce->SetGenTerm(0.5f);
        m_pParticleIce->Reset(attr);
        m_pParticleIce->RemoveBlack(true);
    }

    if (!m_pParticleCherry)
    {
        float power = 2.0f;
        Matrix4 mat;
        m_pPlayer->GetSwordMatrix(mat);
        Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleCherry = new cParticle(&pos, 40, 1000, true, Vector3(-60, 40, -60), Vector3(60, 120, 60));
        m_pParticleCherry->Init("leaf01");
        ST_PARTICLE_ATTR attr;
        attr.fGravity = 0.05f;
        attr.isLoop = true;
        attr.deltaAccelMin = Vector3(power * 0.5f, -power * 0.0f, power * 0.5f);
        attr.deltaAccelMax = Vector3(-power * 0.5f, -power * 1.5f, -power * 0.5f);
        attr.life = 15;
        attr.fSpeed = 0.2f;
        attr.vAccel = Vector3(0.2f, 0.1f, 0.4f);
        attr.fMinLife = 10.0f;
        attr.fMaxLife = 15;
        attr.color = D3DCOLOR_RGBA(245, 192, 247, 220);
        attr.isFade = false;
        m_pParticleCherry->SetSize(2);
        m_pParticleCherry->SetGenTerm(0.1f);
        m_pParticleCherry->Reset(attr);
    }

    // 이펙트 부분
    if (!m_pParticleEffect)
    {
        Vector3 pos = m_pPlayer->GetPosition();
        m_pParticleEffect = new cParticle(&pos, 30);
        m_pParticleEffect->Init("effect03");
    
        m_pParticleEffect->SetSize(5.0f);
        m_pParticleEffect->SetGenTerm(1);
      
        m_pParticleEffect->Reset();
        m_pParticleEffect->RemoveBlack(true);
    }
    return S_OK;
}

HRESULT cPlayScene::Update()
{
    if (g_pKeyManager->isOnceKeyDown('T'))
    {
        m_pParticleEffect->Reset();
    }


    // == 수정 해야 하는 부분!!! 상점 활성화!!
    if (g_pKeyManager->isOnceKeyDown('O'))
    {
        m_pShop->OpenShop();
    }
    // == 수정 해야 하는 부분!!! 인벤 활성화!!
    if (g_pKeyManager->isOnceKeyDown('I'))
    {
        m_pInventory->OpenInventory();
    }

    // SHOP UPDATE -> 상점 지점을 픽킹 면제 시키기 위해서 가장 상단에서 실행
    if (m_pShop && m_pShop->GetIsOpen())
    {
        m_pShop->Update();

        // 아이템 구입을 했다면 인벤 오픈
        if (m_pShop->GetIsBuyItem() && m_pInventory)
        {
            m_pShop->SetIsBuyItem(false);
            m_pInventory->OpenInventory();
        }
    }
    // INVENTORY UPDATE -> 인벤 지점을 픽킹 면제 시키기 위해서 가장 상단에서 실행
    if (m_pInventory && m_pInventory->GetIsOpen())
    {
        m_pInventory->Update();
    }

    //  UPDATE CAMERA
    if (m_pCamera)
    {
        if (m_pShop)
        {
            // 마우스 컨트롤 가능 여부 셋팅
            if (m_pShop->GetClickShop() || m_pInventory->GetIsClickInven())
            {
                m_pCamera->SetControl(false);
            }
            else
            {
                m_pCamera->SetControl(true);
            }
        }

        if (m_pPlayer)
        {
            m_pCamera->SetTargetPos(m_pPlayer->GetPosition());
        }
    }

    //  FRUSTUM CULL UPDATE
    m_pFrustum->Update();

    //  PLAYER UPDATE
    if (m_pPlayer)
    {
        m_pPlayer->Update();
    }

    if (m_pParticleFrost)
    {
        Matrix4 mat;
        m_pPlayer->GetSwordMatrix(mat);
        Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleFrost->SetPosition(pos);
        m_pParticleFrost->Update();
    }

    if (m_pParticleIce)
    {
        Vector3 pos = m_pPlayer->GetPosition();
        //Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41 - 1, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleIce->SetPosition(pos);
        m_pParticleIce->Update();
    }


    if (m_pParticleCherry)
    {
        Matrix4 mat;
        m_pPlayer->GetSwordMatrix(mat);
        Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41 - 1, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleCherry->SetPosition(pos);
        m_pParticleCherry->Update();
    }

    if (m_pParticleEffect)
    {
        Matrix4 mat;
        m_pPlayer->GetSwordMatrix(mat);
        Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41 - 1, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleEffect->SetPosition(pos);
        m_pParticleEffect->UpdateCircle();
    }

    
    // 위치 체크
    Vector3 Pos = m_pPlayer->GetPosition();
    m_pGameMap->GetHeight(Pos);
    m_pPlayer->SetPosition(Pos);

    //REGEN
    if (m_vecMonster->size() == 0)
    {
        for (int i = 0; i < m_stMapInfo->vecEventInfo.size(); i++)
        {
            if (m_stMapInfo->vecEventInfo[i].szName == "monster")
            {
                cMonster* m_pEnermy = g_pCharacterManager->GetMonster(m_szMapKey);
                m_pEnermy->SetStartPoint(m_stMapInfo->vecEventInfo[i].vPos);
                m_pEnermy->SetActive(true);
                (*m_vecMonster).push_back(m_pEnermy);
            }
        }
    }

    //MONSTER SUMMON
    if (m_isRoar)
    {
        cBoss* Boss = NULL;
        for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
        {
            if ((*iter)->GetName() == "Deathwing")
                Boss = (cBoss*)(*iter);
        }

        if (Boss->GetIsRoar())
        {
            for (int i = 0; i < 1; i++)
            {
                Vector3 SummonsPos = Boss->GetPosition() + GetRandomVector3(Vector3(-10, 0, -10), Vector3(10, 0, 10));//임시좌표

                cMonster* m_pEnermy = g_pCharacterManager->GetMonster(m_szMapKey);
                m_pEnermy->SetStartPoint(SummonsPos);
                m_pEnermy->SetActive(true);
                m_pEnermy->SetTarget(m_pPlayer);
                m_pEnermy->RunAnim();
                (*m_vecMonster).push_back(m_pEnermy);
            }

            Boss->SetRoar(false);
            Boss->SetIsRoar(false);
            m_isRoar = false;
        }
    }

    //MONSTER UPDATE
    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        (*iter)->Update();

        Vector3 Pos = (*iter)->GetPosition();
        m_pGameMap->GetHeight(Pos);
        (*iter)->SetPosition(Pos);

        if ((*iter)->GetMove())
        {
            cRay ray;
            ray.m_vOrg = (*iter)->GetPosition();
            ray.m_vDir = (*iter)->GetDir();
            // 정면에 장애물이 없거나, 이동 예정 거리보다 먼곳에 장애물이 있으면
            float fDist = FLT_MAX;
            if (m_pGameMap->CheckObstacle(fDist, ray) == true
                && fDist < 3.0f)
            {
                // 문제가 있다.
                (*iter)->SetMoveSpeed(0.0f);
                (*iter)->SetMove(false);
                (*iter)->IdleAnim();
            }
            else
            {
                (*iter)->SetMoveSpeed(14.0f);
            }
        }

        if ((*iter)->GetRoar())
        {
            m_isRoar = true;
        }
    }

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end();)
    {
        if ((*iter)->GetAlive())
        {
            iter++;
        }
        else
        {
            iter = (*m_vecMonster).erase(iter);
        }
    }

    if (m_pPlayer->GetMove())
    {
        cRay ray;
        ray.m_vOrg = m_pPlayer->GetPosition();
        ray.m_vDir = m_pPlayer->GetDir();
        // 정면에 장애물이 없거나, 이동 예정 거리보다 먼곳에 장애물이 있으면
        float fDist = FLT_MAX;
        if (m_pGameMap->CheckObstacle(fDist, ray) == true
            && fDist < 3.0f)
        {
            // 문제가 있다.
            m_pPlayer->SetMoveSpeed(0.0f);
        }
        else
        {
            m_pPlayer->SetMoveSpeed(20.0f);
        }
    }

    // UI 업데이트 (플레이어 스탯)
    if (m_pPlayerStatUILayer)
    {
        UpdateUI();
        m_pPlayerStatUILayer->Update();
    }

    // 이벤트 체크
    string szEventName = "";
    if (m_pGameMap->CheckEvent(szEventName, m_pPlayer->GetPosition()))
    {
        // 이벤트 발동
        ParseEvent(szEventName);
    }

    return S_OK;
}

HRESULT cPlayScene::Render()
{
    if (m_stMapInfo == NULL ||
        m_stMapInfo->pTerrainMesh == NULL)
    {
        return E_FAIL;
    }

    Matrix4 matW;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    Vector4 vP(g_vCameraPos, 1.0f);

    if (m_pSkyBoxShader)
    {
        m_pSkyBoxShader->Render(vP);
    }

    m_pPlayer->Render();

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        bool result = false;
        m_pFrustum->IsInFrustum(result, &(*iter)->GetSphere());
        if (result)
        {
            (*iter)->Render();
        }

        // 몬스터를 따라다니는 UI!!
        cUIProgressBar* pProgress = NULL;
        if (m_pHPUILayer)
        {
            // HP
            cUIObject* pObject;
            m_pHPUILayer->FindUIObject(&pObject, "default-hp");
            if (pObject)
            {
                pProgress = (cUIProgressBar*)pObject;
            }
        }

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
        if (pProgress)
        {
            Vector3 vMonsterPos = (*iter)->GetPosition();
            vMonsterPos.y += 10.0f;

            Vector3 vScreenPos;
            D3DXVec3TransformCoord(&vScreenPos, &vMonsterPos, &(matView * matProj * matVP));

            pProgress->SetMaxGuage((*iter)->GetStatus().fMaxHP);
            pProgress->SetCurrentGuage((*iter)->GetStatus().fCurHP);
            vScreenPos.x -= pProgress->GetSize().x * 0.5f;
            m_pHPUILayer->SetPosition(vScreenPos);
            pProgress->SetLocalPos(vScreenPos);
            m_pHPUILayer->Update();
            m_pHPUILayer->Render();
        }
    }

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        Vector3 pos = m_pPlayer->GetPosition();
        pos = pos / m_stMapInfo->fMapSize;
        m_pTextureShader->SetPlayerPos(pos);
        if (m_pPlayer->GetTarget())
        {
            pos = m_pPlayer->GetTarget()->GetPosition();
            pos = pos / m_stMapInfo->fMapSize;
            m_pTextureShader->SetTargetPos(&pos);
        }
        else
        {
            m_pTextureShader->SetTargetPos(NULL);
        }
        m_pTextureShader->Render();
    }

    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    for (int i = 0; i < m_stMapInfo->vecObjectInfo.size(); ++i)
    {
        m_stMapInfo->vecObjectInfo[i].pMesh->UpdateAndRender();
    }
    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    // UI 렌더
    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Render();
    }

    // SHOP RENDER
    if (m_pShop && m_pShop->GetIsOpen())
    {
        m_pShop->Render();
    }
    // INVENTORY RENDER
    if (m_pInventory && m_pInventory->GetIsOpen())
    {
        m_pInventory->Render();
    }

    if (m_pParticleIce)
    {
        m_pParticleIce->Render();
    }


    if (m_pParticleFrost)
    {
        m_pParticleFrost->Render();
    }

    if (m_pParticleCherry)
    {
        m_pParticleCherry->Render();
    }

    if(m_pParticleEffect)
    {
        m_pParticleEffect->Render();
    }

#ifdef _DEBUG

    // 장애물, 이벤트 트랩 출력
    //m_pGameMap->RendObstacle();
    //m_pGameMap->RendEventTrap();

#endif // _DEBUG

    return S_OK;
}

ULONG cPlayScene::Release()
{
    SAFE_DELETE(m_pPlayerStatUILayer);
    SAFE_DELETE(m_pHPUILayer);
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
    SAFE_DELETE(m_pPlayerStatUILayer);
    SAFE_DELETE(m_pHPUILayer);
    SAFE_DELETE(m_pParticleFrost);
    SAFE_DELETE(m_pParticleIce);
    SAFE_DELETE(m_pParticleCherry);
    SAFE_DELETE(m_pParticleEffect);
    SAFE_RELEASE(m_pShop);
    SAFE_RELEASE(m_pInventory);

    return cObject::Release();
}

void cPlayScene::SetUI()
{
    ST_SIZE stLayerSize = ST_SIZE(W_WIDTH / 4.0f, W_HEIGHT / 10.0f);
    Vector2 vHP_MPSize = Vector2(stLayerSize.w * 0.8f, stLayerSize.h * 0.3f);

    ST_SIZE stMonLayerSize = ST_SIZE(80, 15);
    Vector2 vMonHPSize = Vector2(stMonLayerSize.w, stMonLayerSize.h);

    string szRedPath = INTERFACE_PATH + (string)"progressBar/redgrad64.png";
    string szGreenPath = INTERFACE_PATH + (string)"progressBar/greengrad64.png";
    string szBluePath = INTERFACE_PATH + (string)"progressBar/bluegrad64.png";
    string szGreyPath = INTERFACE_PATH + (string)"progressBar/greyscaleramp64.png";

    LPFONTDX pFont = g_pFontManager->GetFont(g_pFontManager->E_QUEST);

    // 캐릭터 스탯 레이어 초기화
    if (!m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer = new cUILayer;
        m_pPlayerStatUILayer->SetLayer("player-stat", Vector3(0, 0, 0), stLayerSize);
        m_pPlayerStatUILayer->SetActive(true);

        // HP바
        cUIProgressBar* pUIProgressHP = new cUIProgressBar;
        pUIProgressHP->SetSize(vHP_MPSize);
        pUIProgressHP->AddGuageTexture(szGreenPath, 0, ST_SIZE(vHP_MPSize.x, vHP_MPSize.y));
        pUIProgressHP->AddGuageTexture(szRedPath, 1, ST_SIZE(vHP_MPSize.x, vHP_MPSize.y));
        pUIProgressHP->SetMaxGuage(m_pPlayer->GetStatus().fMaxHP);
        pUIProgressHP->SetCurrentGuage(m_pPlayer->GetStatus().fCurHP);
        pUIProgressHP->SetLocalPos(Vector3(80, 15, 0));
        pUIProgressHP->SetName("player-hp");
        pUIProgressHP->AddText(pFont, 0);
        m_pPlayerStatUILayer->AddUIObject(pUIProgressHP);

        // MP바
        cUIProgressBar* pUIProgressMP = new cUIProgressBar;
        pUIProgressMP->SetSize(vHP_MPSize);
        pUIProgressMP->AddGuageTexture(szBluePath, 0, ST_SIZE(vHP_MPSize.x, vHP_MPSize.y));
        pUIProgressMP->AddGuageTexture(szRedPath, 1, ST_SIZE(vHP_MPSize.x, vHP_MPSize.y));
        pUIProgressMP->SetMaxGuage(m_pPlayer->GetStatus().fMaxMP);
        pUIProgressMP->SetCurrentGuage(m_pPlayer->GetStatus().fCurMP);
        pUIProgressMP->SetLocalPos(Vector3(80, 50, 0));
        pUIProgressMP->SetName("player-mp");
        pUIProgressMP->AddText(pFont, 0);
        m_pPlayerStatUILayer->AddUIObject(pUIProgressMP);
    }

    if (!m_pHPUILayer)
    {
        // 기본 HP 레이어 셋팅 (몬스터용)
        m_pHPUILayer = new cUILayer;
        m_pHPUILayer->SetLayer("default-hp", Vector3(0, 0, 0), stMonLayerSize);
        m_pHPUILayer->SetActive(true);

        // HP바
        cUIProgressBar* pUIProgressHP = new cUIProgressBar;
        pUIProgressHP->SetSize(vMonHPSize);
        pUIProgressHP->AddGuageTexture(szRedPath, 0, ST_SIZE(vMonHPSize.x, vMonHPSize.y));
        pUIProgressHP->AddGuageTexture(szGreyPath, 1, ST_SIZE(vMonHPSize.x, vMonHPSize.y));
        pUIProgressHP->SetMaxGuage(100);
        pUIProgressHP->SetCurrentGuage(30);
        pUIProgressHP->SetName("default-hp");
        m_pHPUILayer->AddUIObject(pUIProgressHP);
    }
}

void cPlayScene::UpdateUI()
{
    cUIObject* pObject;

    // HP
    m_pPlayerStatUILayer->FindUIObject(&pObject, "player-hp");
    if (pObject)
    {
        cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
        pProgress->SetCurrentGuage(m_pPlayer->GetStatus().fCurHP);
        pProgress->SetMaxGuage(m_pPlayer->GetStatus().fMaxHP);
    }
    // MP
    m_pPlayerStatUILayer->FindUIObject(&pObject, "player-mp");
    if (pObject)
    {
        cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
        pProgress->SetCurrentGuage(m_pPlayer->GetStatus().fCurMP);
        pProgress->SetMaxGuage(m_pPlayer->GetStatus().fMaxMP);
    }
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cPlayScene::ParseEvent(string szCommand)
{
    string szPrefix = szCommand.substr(0, szCommand.find('-'));
    if (szPrefix == "to")
    {
        string szPostfix = szCommand.substr(szCommand.find('-') + 1);
        TransportMap(szPostfix);
    }
}

void cPlayScene::TransportMap(string szMap)
{
    m_szMapKey = szMap;
    m_stMapInfo = NULL;
    Start();
    Update();
}
