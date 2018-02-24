#include "stdafx.h"
#include "cHPDraw.h"
#include "cUIImageView.h"


cHPDraw::cHPDraw()
    : m_pSprite(NULL)
    , m_pUIRoot(NULL)
    , m_nHp(0)
{
    ZeroMemory(&m_stImageInfo, sizeof(IMAGE_INFO));
}


cHPDraw::~cHPDraw()
{
    SAFE_RELEASE(m_pSprite);
}

void cHPDraw::Setup()
{
    D3DXCreateSprite(g_pDevice, &m_pSprite);		// 스프라이트 생성

    g_pTextureManager->AddTexture("hp-bar", "./UI/hp-bar.png", true);

    IMAGE_INFO stImageInfo;

    cUIImageView* pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetName("hp-bar");
    pImageView->SetTexture(*g_pTextureManager->GetTexture("hp-bar", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetScale(500, 15);
    pImageView->SetLocalPos(Vector3(25, 25, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot = pImageView;
}

void cHPDraw::Update(Vector3* pos)
{
    if (m_pUIRoot)
    {
        ((cUIImageView*)m_pUIRoot)->SetScale(100 * m_nHp, 15);
        m_pUIRoot->Update();
    }
}

void cHPDraw::Render()
{
    if (m_pUIRoot)
        m_pUIRoot->Render(m_pSprite);
}

void cHPDraw::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
