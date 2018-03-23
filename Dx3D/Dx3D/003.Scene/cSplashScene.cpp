#include "stdafx.h"
#include "cSplashScene.h"


cSplashScene::cSplashScene()
    : m_pImg(NULL)
{
}


cSplashScene::~cSplashScene()
{
    SAFE_DELETE(m_pImg);
}

HRESULT cSplashScene::Start()
{
    m_pImg = m_pImg == NULL ? new cUILayer : m_pImg;
    g_pTextureManager->AddTexture("splash-img", "Assets\\Image\\BTC-PRESENTS.png", true);
    m_pImg->SetLayer("splash", Vector3(0.0f, 0.0f, 0.0f), ST_SIZE(W_WIDTH, W_HEIGHT),
        true, D3DCOLOR_RGBA(0, 0, 0, 255), "splash-img");
    m_pImg->SetTransparent(true);
    m_pImg->SetAlphaInterval(1);
    m_pImg->SetDeltaInterval(0.01f);
    m_pImg->SetActive(true);

    return S_OK;
}

HRESULT cSplashScene::Update()
{
    m_pImg->Update();

    if (m_pImg->GetTwinkleCount() == 1)
    {
        m_pImg->SetTransparent(false);
        m_pImg->SetBackGroundColor(D3DCOLOR_RGBA(255, 255, 255, 255));
        g_pScnManager->ChangeScene("loading");
    }

    return S_OK;
}

HRESULT cSplashScene::Render()
{
    m_pImg->Render();

    return S_OK;
}

void cSplashScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
