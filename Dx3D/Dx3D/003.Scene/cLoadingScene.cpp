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
    , m_nCount(0)
    , m_nHeadIndex(0)
    , m_szHeadKey("map")
{
}


cLoadingScene::~cLoadingScene()
{
    
}

HRESULT cLoadingScene::Start()
{
    //  MAKE RESOURCE LIST
    ifstream i;
    i.open("Assets\\Data\\ResourceList.json", ios_base::in);
    i >> m_jResourceList;
    i.close();

    m_fMaxCount += (float)m_jResourceList["map"].size();
    m_fMaxCount += (float)m_jResourceList["texture"].size();
    m_fMaxCount += (float)m_jResourceList["json"].size();
    m_fMaxCount += (float)m_jResourceList["mesh"].size();
    m_fMaxCount += (float)m_jResourceList["sound"].size();

    // 맵 로더
    m_pMapLoader = new cMapLoader;
    //string szKey = "badland";
    //m_pMapLoader->SetKey(szKey);

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
    pUIBG->SetScale(W_WIDTH / (float)imageInfo.Width, W_HEIGHT / (float)imageInfo.Height);
    m_pBGLayer->AddUIObject(pUIBG);

    m_pBGLayer->SetActive(true);

    // 프로그래스 바
    m_pProgressBarLayer = new cUILayer;
    m_pProgressBarLayer->SetLayer("progressbar", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));

    ST_SIZE stSize = ST_SIZE(W_WIDTH * 0.8f, W_HEIGHT * 0.05f);
    Vector3 vPos = Vector3(W_WIDTH * 0.5f - stSize.w * 0.5f, W_HEIGHT - W_HEIGHT * 0.1f, 0);

    // 프로그래스바 배경
    cUIImageView* pUIProgressBG = new cUIImageView;
    string szbgPath = INTERFACE_PATH + (string)"Loading/loading-background.png";
    g_pTextureManager->AddTexture("loading-background", szbgPath, true);
    pUIProgressBG->SetName("progress-bg");
    pUIProgressBG->SetLocalPos(vPos);
    pUIProgressBG->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("loading-background", &imageInfo));
    pUIProgressBG->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pUIProgressBG->SetScale(stSize.w / (float)imageInfo.Width, stSize.h / (float)imageInfo.Height);
    m_pBGLayer->AddUIObject(pUIProgressBG);

    // 프로그래스바
    cUIProgressBar* pUIProgress = new cUIProgressBar;
    string szBackPath = INTERFACE_PATH + (string)"Loading/loading-frame.png";
    string szFrontPath = INTERFACE_PATH + (string)"Loading/loading-bar.png";
    pUIProgress->SetSize(Vector2(stSize.w, stSize.h));
    pUIProgress->AddGuageTexture(szFrontPath, 0, ST_SIZE(stSize.w - 80, stSize.h));
    pUIProgress->AddGuageTexture(szBackPath, 1, stSize);
    pUIProgress->SetMaxGuage(m_fMaxCount);
    pUIProgress->SetCurrentGuage(m_fCurrCount);
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
    m_fCurrCount = (float)m_nCount;
    //  LOAD RESOURCES
    //  map load
    if (m_szHeadKey == "map")
    {
        string key = m_jResourceList[m_szHeadKey][m_nHeadIndex]["name"];
        m_pMapLoader->SetKey(key);
        m_pMapLoader->LoadMap();
        m_nHeadIndex++;
        m_nCount++;
        if (m_nHeadIndex >= (int)m_jResourceList[m_szHeadKey].size())
        {
            m_nHeadIndex = 0;
            m_szHeadKey = "texture";
        }
    }
    else if (m_szHeadKey == "texture")
    {
        string key = m_jResourceList[m_szHeadKey][m_nHeadIndex]["key"];
        string path = m_jResourceList[m_szHeadKey][m_nHeadIndex]["path"];
        g_pTextureManager->AddTexture(key, path);

        m_nHeadIndex++;
        m_nCount++;
        if (m_nHeadIndex >= (int)m_jResourceList[m_szHeadKey].size())
        {
            m_szHeadKey = "json";
            m_nHeadIndex = 0;
        }
    }
    else if (m_szHeadKey == "json")
    {
        string key = m_jResourceList[m_szHeadKey][m_nHeadIndex]["key"];
        string path = m_jResourceList[m_szHeadKey][m_nHeadIndex]["path"];
        g_pMeshManager->LoadJSON(key, path);

        m_nHeadIndex++;
        m_nCount++;
        if (m_nHeadIndex >= (int)m_jResourceList[m_szHeadKey].size())
        {
            m_szHeadKey = "mesh";
            m_nHeadIndex = 0;
        }
    }
    else if (m_szHeadKey == "mesh")
    {
        string key = m_jResourceList[m_szHeadKey][m_nHeadIndex]["key"];
        string path = m_jResourceList[m_szHeadKey][m_nHeadIndex]["path"];
        string filename = m_jResourceList[m_szHeadKey][m_nHeadIndex]["filename"];
        g_pMeshManager->LoadSkinnedMesh(key, path, filename);

        m_nHeadIndex++;
        m_nCount++;
        if (m_nHeadIndex >= (int)m_jResourceList[m_szHeadKey].size())
        {
            m_nHeadIndex = 0;
            m_szHeadKey = "sound";
        }
    }
    else if (m_szHeadKey == "sound")
    {
        string key = m_jResourceList[m_szHeadKey][m_nHeadIndex]["key"];
        string path = m_jResourceList[m_szHeadKey][m_nHeadIndex]["path"];
        bool isBgm = m_jResourceList[m_szHeadKey][m_nHeadIndex]["bgm"].is_null() ? false : m_jResourceList[m_szHeadKey][m_nHeadIndex]["bgm"];
        bool isLoop = m_jResourceList[m_szHeadKey][m_nHeadIndex]["loop"].is_null() ? false : m_jResourceList[m_szHeadKey][m_nHeadIndex]["loop"];
        
        g_pSndManager->AddSound(key, "", path, isBgm, isLoop);

        m_nHeadIndex++;
        m_nCount++;
        if (m_nHeadIndex >= (int)m_jResourceList[m_szHeadKey].size())
        {
            m_nHeadIndex = 0;
            m_szHeadKey = "end";
        }
    }
    else
    {
        g_pCharacterManager->Setup();
        g_pGameManager->Setup();
        g_pFontManager->Setup();
        g_pMapManager->SetPrevMap("");
        g_pScnManager->ChangeScene("title");
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
            pProgress->SetCurrentGuage(m_fCurrCount);
        }
        m_pProgressBarLayer->Update();
    }

    return S_OK;
}

HRESULT cLoadingScene::Render()
{
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
