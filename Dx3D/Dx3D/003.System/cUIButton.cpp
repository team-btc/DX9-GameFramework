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
    // �ؽ��ĸ� �Ŵ����� �߰� �ϰ� �ؽ��� �迭�� ����
    D3DXIMAGE_INFO stImageInfo;

    g_pTextureManager->AddTexture(sNormal, sNormal);
    m_aTexture[E_NORMAL] = g_pTextureManager->GetTexture(sNormal);

    g_pTextureManager->AddTexture(sMouseOver, sMouseOver);
    m_aTexture[E_MOUSEOVER] = g_pTextureManager->GetTexture(sMouseOver);

    g_pTextureManager->AddTexture(sSelected, sSelected, true);
    m_aTexture[E_SELECTED] = g_pTextureManager->GetTexture(sSelected, &stImageInfo);

    // �̹��� ������ ����
    m_stSize.x = (float)stImageInfo.Width;
    m_stSize.y = (float)stImageInfo.Height;
}

void cUIButton::Update()
{
    cUIObject::UpdateWorldTM();

    RECT rc;	// ��ư ���� ��Ʈ
    SetRect(&rc,
            (int)m_matWorld._41,
            (int)m_matWorld._42,
            (int)(m_matWorld._41 + m_stSize.x),
            (int)(m_matWorld._42 + m_stSize.y));

    //POINT ptCurrMouse;
    //GetCursorPos(&ptCurrMouse);               // �ü�������� ���콺 ��ǥ
    //ScreenToClient(g_hWnd, &ptCurrMouse);     // ���� ���α׷������� ��ǥ�� ���

    if (PtInRect(&rc, g_ptMouse))   // ��ư ���� Ŀ���� �ִ� ���
    {
        if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // Ŭ����
        {
            if (m_eButtonState == E_MOUSEOVER)
            {
                m_ptClickPos = g_ptMouse;
                m_eButtonState = E_SELECTED;
            }
        }
        else // Ŭ���� ���ϰ� ���� ��
        {
            if (m_eButtonState == E_SELECTED) // ������ Ŭ���� �ߴ� ���¶��
            {
                // ������ ��
                if (m_pButton)
                {
                    m_pButton->OnClick(this);
                    m_pButton->OnRelease(this);
                }
            }
            m_eButtonState = E_MOUSEOVER;   // ���� ���´� ���콺 ���� ����
        }
    }
    else // ��ư �ۿ� Ŀ���� �ִ� ���
    {
        if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // Ŭ����
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

    if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // Ŭ����
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

    cUIObject::Render(pSprite); // �θ��� ���� ȣ��
}
