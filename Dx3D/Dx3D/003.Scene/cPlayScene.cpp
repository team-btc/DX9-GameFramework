#include "stdafx.h"
#include "cPlayScene.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cUIProgressBar.h"

cPlayScene::cPlayScene()
    : m_pPlayerStatUILayer(NULL)
    , m_pTargetStatUILayer(NULL)
    , m_pMonsterHPUILayer(NULL)
	, m_stMapInfo(NULL)
    , m_pCamera(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_szMapKey("start")
    , m_pShop(NULL)
    , m_pInventory(NULL)
    , m_pGear(NULL)
    , m_pParticleFrost(NULL)
    , m_pPlayer(NULL)
    , m_isRoar(false)
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
    //  LOAD GEAR
    if (!m_pGear)
    {
        m_pGear = new cGear;
        m_pGear->Setup();
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
    if (m_pTargetStatUILayer)
    {
        m_pTargetStatUILayer->Setup();
    }
    if (m_pMonsterHPUILayer)
    {
        m_pMonsterHPUILayer->Setup();
    }

    //  파티클 세팅
    if (!m_pParticleFrost)
    {
        float power = 1.0f;
        Matrix4 mat;
        m_pPlayer->GetSwordMatrix(mat);
        Vector3 pos = m_pPlayer != NULL ? Vector3(mat._41, mat._42, mat._43) : Vector3(0, 0, 0);

        m_pParticleFrost = new cParticle(&pos, 1, 100);
        m_pParticleFrost->Init("snow");
        ST_PARTICLE_ATTR attr;
        attr.fGravity = 0.01f;
        attr.isLoop = true;
        attr.deltaAccelMin = Vector3(-power * 0.1f, -power * 0.5f, -power * 0.1f);
        attr.deltaAccelMax = Vector3(power * 0.1f, -power * 1.5f, power * 0.1f);
        attr.life = 3.0f;
        attr.fSpeed = 1.0f;
        attr.fMinLife = 5.0f;
        attr.fMaxLife = 10.0f;
        attr.color = XColor(1.0f, 1.0f, 1.0f, 1.0f);
        attr.isFade = false;
        m_pParticleFrost->SetSize(0.5f);
        m_pParticleFrost->SetGenTerm(1.0f);
        m_pParticleFrost->Reset(attr);
    }

    return S_OK;
}

HRESULT cPlayScene::Update()
{
    // == 수정 해야 하는 부분!!! 상점 활성화!!
    if (g_pKeyManager->isOnceKeyDown('O'))
    {
        m_pShop->OpenShop();

        // 인벤도 오픈
        if (m_pInventory && !m_pInventory->GetIsOpen())
        {
            m_pInventory->OpenInventory();
        }
    }
    // == 수정 해야 하는 부분!!! 인벤 활성화!!
    if (g_pKeyManager->isOnceKeyDown('I'))
    {
        m_pInventory->OpenInventory();
    }
    // == 수정 해야 하는 부분!!! 인벤 활성화!!
    if (g_pKeyManager->isOnceKeyDown('P'))
    {
        m_pGear->OpenGear();
    }

    // SHOP UPDATE -> 상점 지점을 픽킹 면제 시키기 위해서 가장 상단에서 실행
    if (m_pShop && m_pShop->GetIsOpen())
    {
        m_pShop->Update();
    }
    // INVENTORY UPDATE -> 인벤 지점을 픽킹 면제 시키기 위해서 가장 상단에서 실행
    if (m_pInventory && m_pInventory->GetIsOpen())
    {
        m_pInventory->Update();
    }
    // GEAR UPDATE -> 장비창 지점을 픽킹 면제 시키기 위해서 가장 상단에서 실행
    if (m_pGear && m_pGear->GetIsOpen())
    {
        m_pGear->Update();
    }

    //  UPDATE CAMERA
    if (m_pCamera)
    {
        if (m_pShop)
        {
            // 마우스 컨트롤 가능 여부 셋팅
            if (m_pShop->GetClickShop() || m_pInventory->GetIsClickInven() || m_pGear->GetIsClickGear())
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
                (*iter)->SetMoveSpeed(0.08f);
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
            m_pPlayer->SetMoveSpeed(0.3f);
        }
    }

    // UI 업데이트 (플레이어 스탯)
    UpdateUI();

    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Update();
    }
    if (m_pTargetStatUILayer)
    {
        m_pTargetStatUILayer->Update();
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
        if (m_pMonsterHPUILayer)
        {
            cUIObject* pObject;
            m_pMonsterHPUILayer->FindUIObject(&pObject, "default-hp");
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
            m_pMonsterHPUILayer->SetPosition(vScreenPos);
            pProgress->SetLocalPos(vScreenPos);
            m_pMonsterHPUILayer->Update();
            g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
            g_pDevice->SetRenderState(D3DRS_ZENABLE, false);
            m_pMonsterHPUILayer->Render();
            g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
            g_pDevice->SetRenderState(D3DRS_ZENABLE, true);
        }
    }

    // UI가 렌더 되기 때문에 하단에서 렌더!!
    if (m_pPlayer)
    {
        m_pPlayer->Render();
    }

    // UI 렌더
    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Render();
    }
    if (m_pTargetStatUILayer)
    {
        m_pTargetStatUILayer->Render();
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
    // GEAR RENDER
    if (m_pGear && m_pGear->GetIsOpen())
    {
        m_pGear->Render();
    }

    if (m_pParticleFrost)
    {
        m_pParticleFrost->Render();
    }

#ifdef _DEBUG

    // 장애물, 이벤트 트랩 출력
    m_pGameMap->RendObstacle();
    m_pGameMap->RendEventTrap();

#endif // _DEBUG

    return S_OK;
}

ULONG cPlayScene::Release()
{
    SAFE_DELETE(m_pPlayerStatUILayer);
    SAFE_DELETE(m_pTargetStatUILayer);
    SAFE_DELETE(m_pMonsterHPUILayer);
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
    SAFE_DELETE(m_pParticleFrost);
    SAFE_RELEASE(m_pShop);
    SAFE_RELEASE(m_pInventory);
    SAFE_RELEASE(m_pGear);

    return cObject::Release();
}

void cPlayScene::SetUI()
{
    ST_SIZE stPlayerStatSize = ST_SIZE(300, 120);
    Vector2 vPlayerHPSize = Vector2(stPlayerStatSize.w * 0.61f, stPlayerStatSize.h * 0.22f);
    Vector2 vPlayerMPSize = Vector2(stPlayerStatSize.w * 0.61f, stPlayerStatSize.h * 0.12f);
    Vector3 vPlayerStatPos = Vector3(15, 15, 0);

    ST_SIZE stTargetStatSize = ST_SIZE(200, 80);
    Vector2 vTargetHPSize = Vector2(stTargetStatSize.w * 0.58f, stTargetStatSize.h * 0.13f);
    Vector3 vTargetStatPos = Vector3(340, 30, 0);

    ST_SIZE stMonLayerSize = ST_SIZE(80, 15);
    Vector2 vMonHPSize = Vector2(stMonLayerSize.w, stMonLayerSize.h);

    string szRedPath = INTERFACE_PATH + (string)"progressBar/redgrad64.png";
    string szGreenPath = INTERFACE_PATH + (string)"progressBar/greengrad64.png";
    string szBluePath = INTERFACE_PATH + (string)"progressBar/bluegrad64.png";
    string szProgressBackPath = INTERFACE_PATH + (string)"progressBar/progress-back.png";

    LPFONTDX pFont = g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT);

    // 캐릭터 스탯 레이어 초기화
    if (!m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer = new cUILayer;
        m_pPlayerStatUILayer->SetLayer("player-stat", Vector3(0, 0, 0), stPlayerStatSize);
        m_pPlayerStatUILayer->SetActive(true);

        // 캐릭터 초상화
        cUIImageView* pCharacterImage = new cUIImageView;
        string szPath = INTERFACE_PATH + (string)"gear-character.png";
        g_pTextureManager->AddTexture("gear-character-img", szPath, true);
        pCharacterImage->SetName("gear-character-img");
        pCharacterImage->SetLocalPos(Vector3(vPlayerStatPos.x + 10, vPlayerStatPos.y + 5, 0));
        IMAGE_INFO imageInfo;
        pCharacterImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("gear-character-img", &imageInfo));
        pCharacterImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pCharacterImage->SetScale(100 / (float)imageInfo.Width, 100 / (float)imageInfo.Height);
        m_pPlayerStatUILayer->AddUIObject(pCharacterImage);

        // HP바
        cUIProgressBar* pUIProgressHP = new cUIProgressBar;
        pUIProgressHP->SetSize(vPlayerHPSize);
        pUIProgressHP->AddGuageTexture(szRedPath, 0, ST_SIZE(vPlayerHPSize.x, vPlayerHPSize.y));
        pUIProgressHP->AddGuageTexture(szProgressBackPath, 1, ST_SIZE(vPlayerHPSize.x, vPlayerHPSize.y));
        pUIProgressHP->SetMaxGuage(m_pPlayer->GetStatus().fMaxHP);
        pUIProgressHP->SetCurrentGuage(m_pPlayer->GetStatus().fCurHP);
        pUIProgressHP->SetLocalPos(Vector3(vPlayerStatPos.x + 111, vPlayerStatPos.y + 26, 0));
        pUIProgressHP->SetName("player-hp");
        pUIProgressHP->AddText(pFont, 0);
        m_pPlayerStatUILayer->AddUIObject(pUIProgressHP);

        // MP바
        cUIProgressBar* pUIProgressMP = new cUIProgressBar;
        pUIProgressMP->SetSize(vPlayerMPSize);
        pUIProgressMP->AddGuageTexture(szBluePath, 0, ST_SIZE(vPlayerMPSize.x, vPlayerMPSize.y));
        pUIProgressMP->AddGuageTexture(szProgressBackPath, 1, ST_SIZE(vPlayerMPSize.x, vPlayerMPSize.y));
        pUIProgressMP->SetMaxGuage(m_pPlayer->GetStatus().fMaxMP);
        pUIProgressMP->SetCurrentGuage(m_pPlayer->GetStatus().fCurMP);
        pUIProgressMP->SetLocalPos(Vector3(vPlayerStatPos.x + 111, vPlayerStatPos.y + 26 + 30, 0));
        pUIProgressMP->SetName("player-mp");
        pUIProgressMP->AddText(pFont, 0);
        m_pPlayerStatUILayer->AddUIObject(pUIProgressMP);

        // EXP바
        cUIProgressBar* pUIProgressEXP = new cUIProgressBar;
        pUIProgressEXP->SetSize(vPlayerMPSize);
        pUIProgressEXP->AddGuageTexture(szGreenPath, 0, ST_SIZE(vPlayerMPSize.x, vPlayerMPSize.y));
        pUIProgressEXP->AddGuageTexture(szProgressBackPath, 1, ST_SIZE(vPlayerMPSize.x, vPlayerMPSize.y));
        pUIProgressEXP->SetMaxGuage((float)m_pPlayer->GetStatus().nMaxEXP);
        pUIProgressEXP->SetCurrentGuage((float)m_pPlayer->GetStatus().nCurEXP);
        pUIProgressEXP->SetLocalPos(Vector3(vPlayerStatPos.x + 111, vPlayerStatPos.y + 26 + 47, 0));
        pUIProgressEXP->SetName("player-exp");
        pUIProgressEXP->AddText(pFont, 0);
        m_pPlayerStatUILayer->AddUIObject(pUIProgressEXP);

        // 플레이어 스탯 배경
        cUIImageView* pStatBGImage = new cUIImageView; 
        szPath = INTERFACE_PATH + (string)"progressBar/player-frame.png";
        g_pTextureManager->AddTexture("player-frame-img", szPath, true);
        pStatBGImage->SetName("player-frame-img");
        pStatBGImage->SetLocalPos(vPlayerStatPos);
        pStatBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("player-frame-img", &imageInfo));
        pStatBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pStatBGImage->SetScale(stPlayerStatSize.w / (float)imageInfo.Width, stPlayerStatSize.h / (float)imageInfo.Height);
        m_pPlayerStatUILayer->AddUIObject(pStatBGImage);
    }

    // 타겟 스탯 레이어 초기화
    if (!m_pTargetStatUILayer)
    {
        m_pTargetStatUILayer = new cUILayer;
        m_pTargetStatUILayer->SetLayer("Target-stat", Vector3(0, 0, 0), stTargetStatSize);
        m_pTargetStatUILayer->SetActive(false);
        string szPath;
        IMAGE_INFO imageInfo;

        // 캐릭터 초상화
        cUIImageView* pCharacterImage = new cUIImageView;
        pCharacterImage->SetName("target-face-img");
        pCharacterImage->SetLocalPos(Vector3(vTargetStatPos.x + 10, vTargetStatPos.y + 5, 0));
        m_pTargetStatUILayer->AddUIObject(pCharacterImage);

        // HP바
        cUIProgressBar* pUIProgressHP = new cUIProgressBar;
        pUIProgressHP->SetSize(vTargetHPSize);
        pUIProgressHP->AddGuageTexture(szRedPath, 0, ST_SIZE(vTargetHPSize.x, vTargetHPSize.y));
        pUIProgressHP->AddGuageTexture(szProgressBackPath, 1, ST_SIZE(vTargetHPSize.x, vTargetHPSize.y));
        pUIProgressHP->SetLocalPos(Vector3(vTargetStatPos.x + 78, vTargetStatPos.y + 35, 0));
        pUIProgressHP->SetName("target-hp");
        pUIProgressHP->AddText(pFont, 0);
        m_pTargetStatUILayer->AddUIObject(pUIProgressHP);

        // MP바
        cUIProgressBar* pUIProgressMP = new cUIProgressBar;
        pUIProgressMP->SetSize(vTargetHPSize);
        pUIProgressMP->AddGuageTexture(szRedPath, 0, ST_SIZE(vTargetHPSize.x, vTargetHPSize.y));
        pUIProgressMP->AddGuageTexture(szProgressBackPath, 1, ST_SIZE(vTargetHPSize.x, vTargetHPSize.y));
        pUIProgressMP->SetLocalPos(Vector3(vTargetStatPos.x + 78, vTargetStatPos.y + 50, 0));
        pUIProgressMP->SetName("target-mp");
        pUIProgressMP->AddText(pFont, 0);
        m_pTargetStatUILayer->AddUIObject(pUIProgressMP);

        // 타겟 스탯 배경
        cUIImageView* pStatBGImage = new cUIImageView;
        szPath = INTERFACE_PATH + (string)"progressBar/target-frame.png";
        g_pTextureManager->AddTexture("target-frame-img", szPath, true);
        pStatBGImage->SetName("target-frame-img");
        pStatBGImage->SetLocalPos(vTargetStatPos);
        pStatBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("target-frame-img", &imageInfo));
        pStatBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pStatBGImage->SetScale(stTargetStatSize.w / (float)imageInfo.Width, stTargetStatSize.h / (float)imageInfo.Height);
        m_pTargetStatUILayer->AddUIObject(pStatBGImage);
    }

    // 몬스터 따라다니는 UI레이어 초기화
    if (!m_pMonsterHPUILayer)
    {
        // 기본 HP 레이어 셋팅 (몬스터 따라다니는 용)
        m_pMonsterHPUILayer = new cUILayer;
        m_pMonsterHPUILayer->SetLayer("default-hp", Vector3(0, 0, 0), stMonLayerSize);
        m_pMonsterHPUILayer->SetActive(true);

        // HP바
        cUIProgressBar* pUIProgressHP = new cUIProgressBar;
        pUIProgressHP->SetSize(vMonHPSize);
        pUIProgressHP->AddGuageTexture(szRedPath, 0, ST_SIZE(vMonHPSize.x, vMonHPSize.y));
        pUIProgressHP->AddGuageTexture(szProgressBackPath, 1, ST_SIZE(vMonHPSize.x, vMonHPSize.y));
        pUIProgressHP->SetMaxGuage(100);
        pUIProgressHP->SetCurrentGuage(30);
        pUIProgressHP->SetName("default-hp");
        m_pMonsterHPUILayer->AddUIObject(pUIProgressHP);
    }
}

void cPlayScene::UpdateUI()
{
    cUIObject* pObject;

    if (m_pPlayerStatUILayer)
    {
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
        // EXP
        m_pPlayerStatUILayer->FindUIObject(&pObject, "player-exp");
        if (pObject)
        {
            cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
            pProgress->SetCurrentGuage((float)m_pPlayer->GetStatus().nCurEXP);
            pProgress->SetMaxGuage((float)m_pPlayer->GetStatus().nMaxEXP);
        }
    }

    if (m_pTargetStatUILayer)
    {
        iCharacterObject* pTarget = m_pPlayer->GetTarget();

        if (pTarget)
        {
            m_pTargetStatUILayer->SetActive(true);

            // 캐릭터 초상화
            m_pTargetStatUILayer->FindUIObject(&pObject, "target-face-img");
            if (pObject)
            {
                string szName = pTarget->GetName() + "-face.png";
                string szPath = INTERFACE_PATH + szName;
                g_pTextureManager->AddTexture(szName, szPath, true);

                cUIImageView* pCharacterImage = (cUIImageView*)pObject;
                IMAGE_INFO imageInfo;
                pCharacterImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szName, &imageInfo));
                pCharacterImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
                pCharacterImage->SetScale(70 / (float)imageInfo.Width, 70 / (float)imageInfo.Height);
            }
            // HP
            m_pTargetStatUILayer->FindUIObject(&pObject, "target-hp");
            if (pObject)
            {
                cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
                pProgress->SetCurrentGuage(pTarget->GetStatus().fCurHP);
                pProgress->SetMaxGuage(pTarget->GetStatus().fMaxHP);
            }
            // MP
            m_pTargetStatUILayer->FindUIObject(&pObject, "target-mp");
            if (pObject)
            {
                cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
                pProgress->SetCurrentGuage(pTarget->GetStatus().fCurMP);
                pProgress->SetMaxGuage(pTarget->GetStatus().fMaxMP);
            }
        }
        else
        {
            m_pTargetStatUILayer->SetActive(false);
        }
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
