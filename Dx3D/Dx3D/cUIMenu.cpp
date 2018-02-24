#include "stdafx.h"
#include "cUIMenu.h"
#include "cUIImageView.h"
#include "cUITextView.h"


cUIMenu::cUIMenu()
    : m_pSprite(NULL)
    , m_pUIRoot(NULL)
{
    ZeroMemory(&m_stImageInfo, sizeof(IMAGE_INFO));
    m_szText = "";
    m_szText2 = "";
    m_btnMove = false;
}


cUIMenu::~cUIMenu()
{
    SAFE_RELEASE(m_pSprite);
}

void cUIMenu::Setup()
{
    D3DXCreateSprite(g_pDevice, &m_pSprite);		// 스프라이트 생성

    g_pTextureManager->AddTexture("panel-info", "./UI/panel-info.png", true);
    IMAGE_INFO stImageInfo;

    cUIImageView* pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetTexture(*g_pTextureManager->GetTexture("panel-info", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetLocalPos(Vector3(100, 100, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot = pImageView;

    g_pTextureManager->AddTexture("title_back", "./UI/btn-med-up.png", true);

    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetTexture(*g_pTextureManager->GetTexture("title_back", &stImageInfo));
    pImageView->SetSize(Vector2((float)stImageInfo.Width, (float)stImageInfo.Height));
    pImageView->SetLocalPos(Vector3(55, 75, 0));
    pImageView->SetDebugRender(true);
    m_pUIRoot->AddChild(pImageView);

    cUITextView* pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("");
    pTextView->SetSize(Vector2(450, 250));
    pTextView->SetLocalPos(Vector3(55, 75, 0));
    pTextView->SetFont(g_pFontManager->GetFont(cFontManager::E_QUEST));
    pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    pTextView->SetTag(E_TEXTVIEW);
    m_pUIRoot->AddChild(pTextView);

    pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("");
    pTextView->SetSize(Vector2(450, 450));
    pTextView->SetLocalPos(Vector3(55, 275, 0));
    pTextView->SetFont(g_pFontManager->GetFont(cFontManager::E_QUEST));
    pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    pTextView->SetTag(E_TEXTVIEW2);
    m_pUIRoot->AddChild(pTextView);

    cUIButton* pButton = NULL;
    
    pButton = new cUIButton;
    g_pAutoReleasePool->AddObject(pButton);
    pButton->SetTexture("./UI/btn-med-up.png", "./UI/btn-med-over.png", "./UI/btn-med-down.png");
    pButton->SetLocalPos(Vector3(135, 400, 0));
    pButton->SetDebugRender(true);
    pButton->SetUIButton(this);
    pButton->SetTag(E_BUTTON1);
    m_pUIRoot->AddChild(pButton);

    pButton = new cUIButton;
    g_pAutoReleasePool->AddObject(pButton);
    pButton->SetLocalPos(Vector3(0, 0, 0));
    pButton->SetDebugRender(true);
    pButton->SetUIButton(this);
    pButton->SetTag(E_TITLEBAR);
    pButton->SetSize(m_pUIRoot->GetSize());
    m_pUIRoot->AddChild(pButton);
}

void cUIMenu::Update(Vector3* pos)
{
    if (m_pUIRoot)
    {
        if (pos)
        {
            Vector3 screenPos = *pos;
            Matrix4 matView, matProj, matVP;

            g_pDevice->GetTransform(D3DTS_VIEW, &matView);
            g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

            VIEWPORT9 vp;
            g_pDevice->GetViewport(&vp);
            D3DXMatrixIdentity(&matVP);
            matVP._11 = vp.Width / 2.0f;
            matVP._22 = -(vp.Height / 2.0f);
            matVP._33 = vp.MaxZ - vp.MinZ;
            matVP._41 = vp.X + vp.Width / 2.0f;
            matVP._42 = vp.Y + vp.Height / 2.0f;
            matVP._43 = vp.MinZ;

            D3DXVec3TransformCoord(&screenPos, &screenPos, &(matView * matProj * matVP));

            m_pUIRoot->SetLocalPos(Vector3(screenPos.x, screenPos.y, 0.0));
        }

        cUITextView* pTextView = (cUITextView*)m_pUIRoot->GetChildByTag(E_TEXTVIEW);
        pTextView->SetText(m_szText);

        pTextView = (cUITextView*)m_pUIRoot->GetChildByTag(E_TEXTVIEW2);
        pTextView->SetText(m_szText2);

        m_pUIRoot->Update();
    }
}

void cUIMenu::Render()
{
    if (m_pUIRoot)
        m_pUIRoot->Render(m_pSprite);
}

void cUIMenu::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cUIMenu::OnClick(cUIButton* pSender)
{
    if (pSender->GetTag() == E_BUTTON1)
    {
        m_btnMove = !m_btnMove;
    }
    else if (pSender->GetTag() == E_BUTTON2)
    {
    }
}

void cUIMenu::OnDrag(cUIButton* pSender)
{
    if (pSender->GetTag() == E_TITLEBAR)
    {
        POINT dtPoint;
        POINT clickedPos = pSender->GetClickPos();
        pSender->SetClickPos(g_ptMouse);
        dtPoint.x = g_ptMouse.x - clickedPos.x;
        dtPoint.y = g_ptMouse.y - clickedPos.y;
        Vector3 pos = m_pUIRoot->GetLocalPos();
        pos.x += (float)dtPoint.x;
        pos.y += (float)dtPoint.y;
        m_pUIRoot->SetLocalPos(pos);
    }
}

void cUIMenu::OnRelease(cUIButton* pSender)
{
    
}