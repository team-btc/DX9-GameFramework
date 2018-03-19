#include "stdafx.h"
#include "cLoadingScene.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUIProgressBar.h"
#include "cMapLoader.h"

cLoadingScene::cLoadingScene()
    : m_pMapLoader(NULL)
    , m_pBGLayer(NULL)
    , m_pProgressBarLayer(NULL)
    , m_isMapDefLoad(false)
    , m_nObjectMaxCnt(0)
    , m_nObjectCurrCnt(0)
    , m_fMaxCount(1.0f)
    , m_fCurrCount(0.0f)
{
}


cLoadingScene::~cLoadingScene()
{
    
}

HRESULT cLoadingScene::Start()
{
    //  MAKE RESOURCE LIST
    
    // 맵 로더
    m_pMapLoader = new cMapLoader;
    string szKey = "badland";
    m_pMapLoader->SetKey(szKey);

    // 배경
    m_pBGLayer = new cUILayer;
    m_pBGLayer->SetLayer("bg", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT), false);
    string sz = TERRAIN_PATH + (string)"Lava.jpg";
    g_pTextureManager->AddTexture("lava", sz, true);
    cUIImageView* pUIBG = new cUIImageView;
    pUIBG->SetName("bg");
    pUIBG->SetLocalPos(Vector3(0, 0, 0));
    IMAGE_INFO imageInfo;
    pUIBG->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("lava", &imageInfo));
    pUIBG->SetSize(Vector2(imageInfo.Width, imageInfo.Height));
    pUIBG->SetScale(W_WIDTH, W_HEIGHT);
    m_pBGLayer->AddUIObject(pUIBG);

    m_pBGLayer->SetActive(true);

    // 프로그래스 바
    m_pProgressBarLayer = new cUILayer;
    m_pProgressBarLayer->SetLayer("progressbar", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT), false);

    ST_SIZE stSize = ST_SIZE(W_WIDTH * 0.8f, W_HEIGHT * 0.05f);
    Vector3 vPos = Vector3(W_WIDTH * 0.5f - stSize.w * 0.5f, W_HEIGHT - W_HEIGHT * 0.1f, 0);

    cUIProgressBar* pUIProgress = new cUIProgressBar;
    string szBluePath = INTERFACE_PATH + (string)"buttons/bluegrad64.png";
    string szRedPath = INTERFACE_PATH + (string)"buttons/redgrad64.png";
    pUIProgress->SetSize(Vector2(stSize.w, stSize.h));
    pUIProgress->AddGuageTexture(szBluePath, 0);
    pUIProgress->AddGuageTexture(szRedPath, 1);
    pUIProgress->SetMaxGuage(m_fMaxCount * 100.0f);
    pUIProgress->SetCurrentGuage(m_fCurrCount * 100.0f);
    pUIProgress->SetLocalPos(vPos);
    pUIProgress->SetName("progressbar");
    LPFONTDX pFont = g_pFontManager->GetFont(g_pFontManager->E_QUEST);
    pUIProgress->AddText(pFont, 0);
    m_pProgressBarLayer->AddUIObject(pUIProgress);
        
    m_pProgressBarLayer->SetActive(true);

    if (m_pBGLayer)
    {
        m_pBGLayer->Setup();
    }

    if (m_pProgressBarLayer)
    {
        m_pProgressBarLayer->Setup();
    }

    return S_OK;
}

HRESULT cLoadingScene::Update()
{
    //  LOAD RESOURCES

    if (m_pMapLoader && m_isMapDefLoad == false)
    {
        m_pMapLoader->LoadMap();
        m_nObjectMaxCnt = m_pMapLoader->GetObjectMaxCnt();
        m_fCurrCount = 1.0f / ((float)m_nObjectMaxCnt + 1.0f);
        m_isMapDefLoad = true;
    }
    else if (m_pMapLoader && m_nObjectCurrCnt < m_nObjectMaxCnt)
    {
        m_pMapLoader->LoadObject(m_nObjectCurrCnt);
        ++m_nObjectCurrCnt;
        m_fCurrCount = (1.0f + (float)m_nObjectCurrCnt) / ((float)m_nObjectMaxCnt + 1.0f);
    }
    else
    {
        // 로딩이 끝났으면!
    }

    if (m_pBGLayer)
    {
        m_pBGLayer->Update();
    }

    if (m_pProgressBarLayer)
    {
        cUIObject* pObject;
        m_pProgressBarLayer->FindUIObject(&pObject, "progressbar");
        if (pObject)
        {
            cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
            pProgress->SetCurrentGuage(m_fCurrCount * 100.0f);
        }
        m_pProgressBarLayer->Update();
    }

    return S_OK;
}

HRESULT cLoadingScene::Render()
{
    //  BACKGROUND IMAGE
    //  PROGRESS BAR

    if (m_pBGLayer)
    {
        m_pBGLayer->Render();
    }

    if (m_pProgressBarLayer)
    {
        m_pProgressBarLayer->Render();
    }

    return S_OK;
}

ULONG cLoadingScene::Release()
{
    SAFE_DELETE(m_pProgressBarLayer);
    SAFE_DELETE(m_pBGLayer);
    SAFE_DELETE(m_pMapLoader);

    cObject::Release();
    
    return 0;
}

void cLoadingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
