#include "stdafx.h"
#include "cPlayScene.h"

cPlayScene::cPlayScene()
    : m_stMapInfo(NULL)
    , m_pCamera(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_szMapKey("icecrown")
{
}


cPlayScene::~cPlayScene()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
}

HRESULT cPlayScene::Start()
{
    //  LOAD MAP
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.LoadMap(m_szMapKey);

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
  
    if (!m_vecMonster)
    {
        m_vecMonster = new vector<cMonster*>;
    }
    else
    {
        for (auto iter = m_vecMonster->begin(); iter != m_vecMonster->end(); iter++)
        {
            g_pCharacterManager->PushMonster(*iter);
        }
    }
    m_vecMonster->clear();

   
    for (int i = 0; i < m_stMapInfo->vecEventInfo.size(); i++)
    {
        if (m_stMapInfo->vecEventInfo[i].szName == "monster")
        {
            cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
            m_pEnermy->SetStartPoint(m_stMapInfo->vecEventInfo[i].vPos);
            m_pEnermy->SetActive(true);
            (*m_vecMonster).push_back(m_pEnermy);
        }
    }

    if (!m_pPlayer)
    {
        m_pPlayer = g_pCharacterManager->GetPlayer();
        m_pPlayer->SetPosition(m_stMapInfo->vStartPos);
        m_pPlayer->SetVecMonster(m_vecMonster);
        m_pPlayer->SetTerrain(m_stMapInfo->pTerrainMesh);
    }
    else
    {
        m_pPlayer->SetPosition(m_stMapInfo->vStartPos);
        m_pPlayer->SetVecMonster(m_vecMonster);
        m_pPlayer->SetTerrain(m_stMapInfo->pTerrainMesh);
    }

    //  CAMERA SETUP
    if (!m_pCamera)
    {
        m_pCamera = new cCamera;
        m_pCamera->TrackingEnable();
        m_pCamera->SetMaxDist(100.0f);
        m_pCamera->SetMinDist(5.0f);
        g_pCameraManager->AddCamera("play", m_pCamera);
        g_pCameraManager->SetCollisionMesh(m_stMapInfo->pTerrainMesh);
        g_pCameraManager->ColliderEnable();
    }
    m_pCamera->Setup();
    g_pCameraManager->SetCurrCamera("play");

    return S_OK;
}

HRESULT cPlayScene::Update()
{
    //  UPDATE CAMERA
    //if (m_pCamera)
    //{
    //    if (m_pPlayer)
    //    {
    //        m_pCamera->Update(&m_pPlayer->GetPosition());
    //    }
    //    else
    //    {
    //        m_pCamera->Update();
    //    }
    //}
    if (m_pCamera)
    {
        if (m_pPlayer)
        {
            m_pCamera->SetTargetPos(m_pPlayer->GetPosition());
        }
    }

    //  FRUSTUM CULL UPDATE
    m_pFrustum->Update();

    //  PLAYER UPDATE
    m_pPlayer->Update();

    // 위치 체크
    Vector3 Pos = m_pPlayer->GetPosition();
    m_pGameMap->GetHeight(Pos);
    m_pPlayer->SetPosition(Pos);

    if (m_vecMonster->size() == 0)
    {
        for (int i = 0; i < m_stMapInfo->vecEventInfo.size(); i++)
        {
            if (m_stMapInfo->vecEventInfo[i].szName == "monster")
            {
                cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
                m_pEnermy->SetStartPoint(m_stMapInfo->vecEventInfo[i].vPos);
                m_pEnermy->SetActive(true);
                (*m_vecMonster).push_back(m_pEnermy);
            }
        }
    }

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
            m_pPlayer->SetMove(false);
            m_pPlayer->SetMoveToPoint(false);
            m_pPlayer->SetMoveToTarget(false);
            m_pPlayer->IdleAnim();
        }
        else
        {
            m_pPlayer->SetMoveSpeed(0.5f);
        }
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

    Matrix4 matW, matV, matP;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    MATERIAL9 mtrl;
    mtrl.Ambient = WHITE;
    mtrl.Diffuse = WHITE;
    mtrl.Specular = WHITE;
    mtrl.Emissive = BLACK;
    mtrl.Power = 8.0f;
    g_pDevice->SetMaterial(&mtrl);
    g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->LightEnable(0, true);

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
    }

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
   // for (int i = 0; i < m_stMapInfo->vecObjectInfo.size(); ++i)
   // {
   //     m_stMapInfo->vecObjectInfo[i].pMesh->UpdateAndRender();
   // }
    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    //g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    //g_pDevice->SetRenderState(D3DRS_ZENABLE, true);

#ifdef _DEBUG

    // 장애물, 이벤트 트랩 출력
    //m_pGameMap->RendObstacle();
    //m_pGameMap->RendEventTrap();

#endif // _DEBUG

    return S_OK;
}

ULONG cPlayScene::Release()
{
    cObject::Release();
    return 0;
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_pCamera)
    {
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
    }
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
