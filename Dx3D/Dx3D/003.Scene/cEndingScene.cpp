#include "stdafx.h"
#include "cEndingScene.h"


cEndingScene::cEndingScene()
{
    m_fTexPos = 0.0f;
    m_fTexPos2 = 850.0f;
}


cEndingScene::~cEndingScene()
{
    SAFE_DELETE(m_pBGLayer);
}

HRESULT cEndingScene::Start()
{
    m_isStart = false;
    m_pBGLayer = new cUILayer;
    m_pBGLayer->SetLayer("ending", Vector3(0, 0, 0), ST_SIZE(W_WIDTH , W_HEIGHT));
    string sz = (string)"Assets/EndingCredit/" + (string)"ending-credit.png";
    g_pTextureManager->AddTexture("ending-tex", sz, true);
    string sz2 = (string)"Assets/EndingCredit/" + (string)"ending-credit2.png";
    g_pTextureManager->AddTexture("ending-tex2", sz2, true);
    m_pUIBG = new cUIImageView;
    m_pUIBG->SetName("ending");
    m_pUIBG->SetLocalPos(Vector3(0, 0, 0));
    IMAGE_INFO imageInfo;
    m_pUIBG->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("ending-tex", &imageInfo));
    m_pUIBG->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));

    m_pUIBG2 = new cUIImageView;
    m_pUIBG2->SetName("ending");
    m_pUIBG2->SetLocalPos(Vector3(0, m_fTexPos2, 0));
    m_pUIBG2->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("ending-tex2", &imageInfo));
    m_pUIBG2->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    m_pBGLayer->AddUIObject(m_pUIBG2);
    m_pBGLayer->AddUIObject(m_pUIBG);

    m_pBGLayer->SetActive(true);

    m_pBGLayer->Setup();
    
    return S_OK;
}

HRESULT cEndingScene::Update()
{
    if (g_pKeyManager->isOnceKeyDown(VK_RETURN))
    {
        m_isStart = true;   
    }
    else if (g_pKeyManager->isOnceKeyDown(VK_SPACE))
    {
        m_isStart = false;
    }
    else if (g_pKeyManager->isStayKeyDown('U'))
    {
        m_fTexPos += SCROLL_SPD;
    }
    else if (g_pKeyManager->isStayKeyDown('D'))
    {
        m_fTexPos -= SCROLL_SPD;
    }
    //  SCROLL IMAGE
    if (m_isStart)
    {
        m_fTexPos -= SCROLL_SPD;
        if (m_fTexPos <= -11600.0f)
        {
            m_fTexPos = -11600.0f;
        }
        if (m_fTexPos <= -7300 && m_fTexPos > -11500.0f)
        {
            m_fTexPos2 -= SCROLL_SPD;
        }
    }



    m_pUIBG2->SetLocalPos(Vector3(0, m_fTexPos2, 0));
    m_pUIBG->SetLocalPos(Vector3(0, m_fTexPos, 0));

    m_pBGLayer->Update();
    return S_OK;
}

HRESULT cEndingScene::Render()
{
    //  ENDING CREDIT IMAGE RENDER
    m_pBGLayer->Render();
    return S_OK;
}

ULONG cEndingScene::Release()
{
    cObject::Release();
    return 0;
}

void cEndingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
