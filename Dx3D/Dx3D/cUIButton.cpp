#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
{
}


cUIButton::~cUIButton()
{
}

void cUIButton::SetTexture(string sNormal, string sMouseOver, string sSelected)
{
    // 텍스쳐를 매니져에 추가 하고 텍스쳐 배열에 셋팅
    D3DXIMAGE_INFO stImageInfo;

    g_pTextureManager->AddTexture(sNormal, sNormal);
    m_aTexture[E_NORMAL] = *g_pTextureManager->GetTexture(sNormal);

    g_pTextureManager->AddTexture(sMouseOver, sMouseOver);
    m_aTexture[E_MOUSEOVER] = *g_pTextureManager->GetTexture(sMouseOver);

    g_pTextureManager->AddTexture(sSelected, sSelected, true);
    m_aTexture[E_SELECTED] = *g_pTextureManager->GetTexture(sSelected, &stImageInfo);

    // 이미지 사이즈 셋팅
    m_stSize.x = (float)stImageInfo.Width;
    m_stSize.y = (float)stImageInfo.Height;
}

void cUIButton::Update()
{
    cUIObject::UpdateWorldTM();

    RECT rc;	// 버튼 영역 렉트
    SetRect(&rc,
            (int)m_matWorld._41,
            (int)m_matWorld._42,
            (int)(m_matWorld._41 + m_stSize.x),
            (int)(m_matWorld._42 + m_stSize.y));

    //POINT ptCurrMouse;
    //GetCursorPos(&ptCurrMouse);               // 운영체제에서의 마우스 좌표
    //ScreenToClient(g_hWnd, &ptCurrMouse);     // 현재 프로그램에서의 좌표로 계산

    if (PtInRect(&rc, g_ptMouse))   // 버튼 위에 커서가 있는 경우
    {
        if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // 클릭중
        {
            if (m_eButtonState == E_MOUSEOVER)
            {
                m_ptClickPos = g_ptMouse;
                m_eButtonState = E_SELECTED;
            }
        }
        else // 클릭을 안하고 있을 때
        {
            if (m_eButtonState == E_SELECTED) // 이전에 클릭을 했던 상태라면
            {
                // 눌렀다 땜
                if (m_pButton)
                {
                    m_pButton->OnClick(this);
                    m_pButton->OnRelease(this);
                }
            }
            m_eButtonState = E_MOUSEOVER;   // 현재 상태는 마우스 오버 상태
        }
    }
    else // 버튼 밖에 커서가 있는 경우
    {
        if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // 클릭중
        {
        }
        else
        {
            if (m_pButton && m_eButtonState == E_SELECTED)
            {
                m_pButton->OnRelease(this);
            }
            m_eButtonState = E_NORMAL;
        }
    }

    if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // 클릭중
    {
        if (m_pButton && m_eButtonState == E_SELECTED)
        {
            m_pButton->OnDrag(this);
        }
    }

    cUIObject::UpdateChildren();
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
    RECT rc;
    SetRect(&rc, 0, 0, (int)m_stSize.x, (int)m_stSize.y);
    pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

    pSprite->SetTransform(&m_matWorld);
    if (m_aTexture[m_eButtonState])
    {
        pSprite->Draw(m_aTexture[m_eButtonState], &rc, &D3DXVECTOR3(0, 0, 0),
                      &D3DXVECTOR3(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
    }

    pSprite->End();

    cUIObject::Render(pSprite); // 부모의 렌더 호출
}
