#include "stdafx.h"
#include "cUIDraw.h"
#include "cUIImageView.h"
#include "cUITextView.h"


cUIDraw::cUIDraw()
    : m_pSprite(NULL)
    , m_pUIRoot(NULL)
{
    ZeroMemory(&m_stImageInfo, sizeof(IMAGE_INFO));
}


cUIDraw::~cUIDraw()
{
    SAFE_RELEASE(m_pSprite);
}

void cUIDraw::Setup()
{
    D3DXCreateSprite(g_pDevice, &m_pSprite);		// 스프라이트 생성

    g_pTextureManager->AddTexture("panel", "./UI/panel-info.png", true);
    g_pTextureManager->AddTexture("e-drink", "./icon/energy-drink.png", true);
    g_pTextureManager->AddTexture("c-drink", "./icon/cool-drink.png", true);
    g_pTextureManager->AddTexture("p-drink", "./icon/power-drink.png", true);

    D3DXIMAGE_INFO stImageInfo;

    cUIImageView* pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetName("e-drink");
    pImageView->SetTexture(*g_pTextureManager->GetTexture("panel", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetScale(150, 150);
    pImageView->SetLocalPos(Vector3(1150, 525, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot = pImageView;

    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetName("c-drink");
    pImageView->SetTexture(*g_pTextureManager->GetTexture("e-drink", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetScale(75, 75);
    pImageView->SetLocalPos(Vector3(-65, 45, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot->AddChild(pImageView);

    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetName("c-drink");
    pImageView->SetTexture(*g_pTextureManager->GetTexture("c-drink", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetScale(100, 100);
    pImageView->SetLocalPos(Vector3(25, 25, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot->AddChild(pImageView);

    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetName("p-drink");
    pImageView->SetTexture(*g_pTextureManager->GetTexture("p-drink", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetScale(75, 75);
    pImageView->SetLocalPos(Vector3(145, 45, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot->AddChild(pImageView);
}

void cUIDraw::Update(Vector3* pos)
{
    if (m_pUIRoot)
        m_pUIRoot->Update();
}

void cUIDraw::Render()
{
    if (m_pUIRoot)
        m_pUIRoot->Render(m_pSprite);
}

void cUIDraw::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_LBUTTONDOWN:
        {
            POINT point{ LOWORD(lParam), HIWORD(lParam) };
            cUIObject* target = m_pUIRoot->IsClicked(point, false);
            if (target)
            {
                string name = target->GetName();
                cout << name.c_str() << endl;;
            }
            break;
        }
    }
}
