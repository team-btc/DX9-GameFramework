#include "stdafx.h"
#include "cTitleScene.h"

cTitleScene::cTitleScene()
    : m_stMapInfo(NULL)
    , m_pCamera(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_szMapKey("icecrown")
    , m_vSindraPos(229, 199, 242)
    , m_vArtuhsPos(Vector3(261, 88, 482))
    , m_vCameraPos(259.47f, 171.95f, 279.68f)
    , m_nCurrIndex(0)
    , m_isRoar(false)
    , m_isStart(false)
    , m_isArthusMove(false)
    , m_isArthusRender(false)
    , m_isReady(false)
    , m_isChangeScene(false)
    , m_isTemp(false)
    , m_isFadeOut(false)
    , m_fFadeTime(FLT_MAX)
    , m_ft(0)
    , m_vStartPos(0,0,0)
    , m_fWorldTime(0)
    , m_isMovieStart(false)
    , m_vFlyRotation(0, -90, -75)
{
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 199.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 199.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 400.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(280.0f, -88.0f, 630.0f));
   
    for (int i = 0; i < m_vecSindraJumpTarget.size(); ++i)
    {
        m_vecIsArriveSindra.push_back(false);
    }
}


cTitleScene::~cTitleScene()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
    SAFE_DELETE(m_pBGLayer);
    SAFE_DELETE(m_pSGLayer);
}

HRESULT cTitleScene::Start()
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

    m_pArthus = new cSkinnedMesh("arthaslichking");
    m_pArthus->SetPosition(m_vArtuhsPos);
    m_pArthus->SetScale(8.0f);
    m_pArthus->SetRotation(Vector3(0, -90.0f, 0));
    m_pArthus->SetAnimationIndex(1);
    g_pAutoReleasePool->AddObject(m_pArthus);

    // Setup 신드라고사
    SetSindragosa();

    //  CAMERA SETUP
    if (!m_pCamera)
    {
        m_pCamera = new cCamera;
        m_pCamera->TrackingEnable();
        m_pCamera->SetMaxDist(400.0f);
        m_pCamera->SetMinDist(5.0f);
        g_pCameraManager->AddCamera("title", m_pCamera);
        g_pCameraManager->SetCollisionMesh(m_stMapInfo->pTerrainMesh);
        g_pCameraManager->ColliderDisable();
        
    }
    m_pCamera->Setup();
    m_pCamera->SetEye(Vector3(262.53f, 88.71f, 498.78f));
    m_pCamera->SetTargetPos(Vector3(259.47f, 172.95f, 279.68f));
    g_pCameraManager->SetCurrCamera("title");


    g_pTextureManager->AddTexture("Black", "./Assets/UI/black.png", true);
    m_pBGLayer = new cUILayer;
    m_pBGLayer->SetLayer("press", Vector3(150, 650, 0), ST_SIZE(1300, 150), true, D3DCOLOR_RGBA(0, 0, 0, 0), "Black");
    m_pBGLayer->SetTransparent(true);
    m_pBGLayer->SetDeltaInterval(0.01f);
    m_pBGLayer->SetAlphaInterval(2.5f);
    m_pBGLayer->SetActive(true);

    g_pTextureManager->AddTexture("press-tex", "Assets/Splash/press-space-to-start.png", true);    
    cUILayer* m_pBGLayer2 = new cUILayer;
    m_pBGLayer2->SetLayer("press_text", Vector3(0, 0, 0), ST_SIZE(1300, 150), true, D3DCOLOR_RGBA(0, 0, 0, 0), "press-tex");
    m_pBGLayer->AddUILayerChild(m_pBGLayer2);
    m_pBGLayer2->SetTransparent(true);
    m_pBGLayer2->SetDeltaInterval(0.01f);
    m_pBGLayer2->SetAlphaInterval(2.5f);

    g_pTextureManager->AddTexture("blue", "./Assets/UI/blue.png", true);
    m_pSGLayer = new cUILayer;
    m_pSGLayer->SetLayer("press", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT), true, D3DCOLOR_RGBA(0, 0, 0, 0), "blue");
    m_pSGLayer->SetTransparent(true);
    m_pSGLayer->SetDeltaInterval(0.01f);
    m_pSGLayer->SetAlphaInterval(1.0f);
    m_pSGLayer->SetBackGroundColor(D3DCOLOR_RGBA(0, 0, 0, 0));
    m_pSGLayer->SetActive(true);
    return S_OK;
}

HRESULT cTitleScene::Update()
{
    //  FRUSTUM CULL UPDATE
    m_pFrustum->Update();

    //  PLAYER UPDATE
    m_pArthus->SetPosition(m_vArtuhsPos);
    // 신드라고사 업데이트
    m_pSindragosa->Update();
    m_pSindragosa->SetPosition(m_vSindraPos);
    // 위치 체크

    //신드라곳아 움직임 셋팅
    if (g_pKeyManager->isOnceKeyDown('6'))
    {
        m_isArthusRender = false;
    }
    else if (g_pKeyManager->isOnceKeyDown('7'))
    {
        m_isArthusRender = true;
    }
    if (g_pKeyManager->isOnceKeyDown(VK_SPACE) &&  m_isReady)
    {
        m_isStart = true;
    }
    if (g_pKeyManager->isOnceKeyDown('0'))
    {
        m_isMovieStart = true;
    }
    else  if (g_pKeyManager->isOnceKeyDown('9'))
    {
        m_isChangeScene = false;
        m_isMovieStart = false;
        m_isStart = false;
        m_isArthusMove = false;
        m_isRoar = false;
        m_isReady = false;
        m_isTemp = false;
        m_vSindraPos = Vector3(229, 199, 242);
        m_pSindragosa->IdleAnim();
        m_nCurrIndex = 0;
        m_pArthus->SetAnimationIndex(1);
        m_vArtuhsPos = Vector3(261, 88, 482);
        m_ft = 0;     
        m_pSindragosa->SetRotation(Vector3(0, -90, 0));
        for (int n = 0; n <m_vecIsArriveSindra.size(); ++n)
        {
            m_vecIsArriveSindra[n] = false;
        }
    }
    if (m_isMovieStart)
    {
         MoveSindraAllRoute();
    }
    if (m_isStart)
    {
        MoveArthus();
    }
    if (m_pBGLayer && m_isReady)
    {
        m_pBGLayer->Update();
    }

    if (m_isFadeOut)
    {
        g_pScnManager->ChangeScene("play");
    }
    return S_OK;
}

HRESULT cTitleScene::Render()
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
    if (m_isArthusRender)
    {
        m_pArthus->UpdateAndRender();
    }
    m_pSindragosa->Render();
  
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    if (m_isReady)
    {
        m_pBGLayer->Render();
    }
    if (!m_isFadeOut)
    {
        m_pSGLayer->Render();
    }
#ifdef _DEBUG

#endif // _DEBUG

    return S_OK;
}

ULONG cTitleScene::Release()
{
    cObject::Release();
    return 0;
}

void cTitleScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_pCamera)
    {
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
    }
}

void cTitleScene::MoveSindra(Vector3 vSpot, int n)
{
    if (!m_vecIsArriveSindra.at(n) && m_nCurrIndex == n)
    {
        if (m_ft == 0)
        {
            m_vStartPos = m_vSindraPos;
        }
        m_vSindraPos = (1 - m_ft) * m_vStartPos + m_ft * vSpot;
        m_ft += 0.006f;
        if (m_ft >= 1.0)
        {
            m_vecIsArriveSindra.at(n) = true;
            m_ft = 0.0f;
        }
    }
    
}

void cTitleScene::MoveArthus()
{
    if (!m_isArthusMove)
    {
        m_fArthusTime = g_pTimerManager->GetWorldTime() + 1.0f;
        m_isArthusMove = true;
    }
    if (m_fArthusTime <= g_pTimerManager->GetWorldTime() && m_isArthusMove)
    {
        m_isReady = false;
        if (m_ft == 0)
        {
            m_pArthus->SetAnimationIndex(11);
            m_vArthusStartPos = m_vArtuhsPos;
            m_vArthusLastPos = Vector3(261, 88, 542);
        }
        m_vArtuhsPos = (1 - m_ft) * m_vArthusStartPos + m_ft * m_vArthusLastPos;
        m_ft += 0.001f;
        if (m_ft >= 1.0)
        {
            m_ft = 1.0f;
        }
    }
}

void cTitleScene::MoveSindraAllRoute()
{
    if (!m_isMovieStart)
    {
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 10.0f;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 0)
    {
        m_pSindragosa->RoarAnim();
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.0f;
        m_nCurrIndex++;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 1)
    {
        m_pSindragosa->FlySitUpAnim();
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.0f;
        m_nCurrIndex++;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 2)
    {
        m_pSindragosa->FlyWalkAnim();
        m_fWorldTime = g_pTimerManager->GetWorldTime() + ARTHUS;
        m_nCurrIndex++;
        m_pSindragosa->SetRotation(m_vFlyRotation);
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 3 && !m_isArthusRender)
    {
        m_isArthusRender = true;
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 1.0f;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_isArthusRender && !m_isReady && !m_isTemp)
    {
        m_isReady = true;
        m_isTemp = true;
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 3.0f;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && !m_isChangeScene)
    {
        m_isChangeScene = true;
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 3.0f;
    }
    if (m_isChangeScene)
    {
        m_pSGLayer->Update();
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_isChangeScene)
    {
        m_fFadeTime = g_pTimerManager->GetWorldTime() + 3.6f;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime())
    {
        m_isFadeOut = true;
    }
    MoveSindra(m_vecSindraJumpTarget[0], 0);
    MoveSindra(m_vecSindraJumpTarget[1], 1);
    MoveSindra(m_vecSindraJumpTarget[2], 2);
    MoveSindra(m_vecSindraJumpTarget[3], 3);

}


void cTitleScene::SetSindragosa()
{
    Vector3 v = Vector3(0, -90, 0);
    Matrix4 matS;
    D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.5f);
    m_pSindragosa = new cSindragosa("Frostwurmnorthlend");
    g_pAutoReleasePool->AddObject(m_pSindragosa);
    m_pSindragosa->SetPosition(m_vSindraPos);
    m_pSindragosa->SetRotation(v);

    m_pSindragosa->SetScale(matS);
    m_pSindragosa->SetActive(true);
    m_pSindragosa->IdleAnim();
    
}
/*
    // 배경
    m_pBGLayer = new cUILayer;
    m_pBGLayer->SetLayer("bg", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    string sz = INTERFACE_PATH + (string)"Loading/loadscreen.png";
    g_pTextureManager->AddTexture("loadscreen", sz, true);
    cUIImageView* pUIBG = new cUIImageView;
    pUIBG->SetName("bg");
    pUIBG->SetLocalPos(Vector3(0, 0, 0));
    IMAGE_INFO imageInfo;
    pUIBG->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("loadscreen", &imageInfo));
    pUIBG->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pUIBG->SetScale(W_WIDTH, W_HEIGHT);
    m_pBGLayer->AddUIObject(pUIBG);
    
    m_pBGLayer->SetActive(true);
*/