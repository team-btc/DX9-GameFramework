#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
{}


cUIButton::~cUIButton()
{}

void cUIButton::SetTexture(string sNormal, string sMouseOver, string sSelect)
{
    // �ؽ��ĸ� �Ŵ����� �߰� �ϰ� �ؽ��� �迭�� ����
    IMAGE_INFO stImageInfo;
    stImageInfo.Width = 1;
    stImageInfo.Height = 1;

    //g_pTextureManager->AddTexture(sNormal, sNormal, true);
    m_aTexture[E_NORMAL] = (LPTEXTURE9)g_pTextureManager->GetTexture(sNormal);

    //g_pTextureManager->AddTexture(sMouseOver, sMouseOver, true);
    m_aTexture[E_MOUSEOVER] = (LPTEXTURE9)g_pTextureManager->GetTexture(sMouseOver);

    //g_pTextureManager->AddTexture(sSelect, sSelect, true);
    m_aTexture[E_SELECT] = (LPTEXTURE9)g_pTextureManager->GetTexture(sSelect, &stImageInfo);

    // �̹��� ������ ����
    m_stSize.x = (float)stImageInfo.Width;
    m_stSize.y = (float)stImageInfo.Height;

    m_isDebugRender = true;
}

void cUIButton::SetScale(float width, float height)
{
    m_matWorld._11 = width / m_stSize.x;
    m_matWorld._22 = height / m_stSize.y;
}

void cUIButton::SetSize(Vector2 size)
{
    SetScale(size.x, size.y);
}

void cUIButton::Update()
{
    cUIObject::UpdateWorldTM();
    RECT rc;	// ��ư ���� ��Ʈ
    SetRect(&rc,
        (int)m_matWorld._41,
        (int)m_matWorld._42,
        (int)(m_matWorld._41 + (m_stSize.x * m_matWorld._11)),
        (int)(m_matWorld._42 + m_stSize.y * m_matWorld._22));

    if (PtInRect(&rc, g_ptMouse))   // ��ư ���� Ŀ���� �ִ� ���
    {
        if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // Ŭ����
        {
            if (m_eButtonState == E_MOUSEOVER)
            {
                m_ptClickPos = g_ptMouse;
                m_eButtonState = E_SELECT;
            }
        }
        else // Ŭ���� ���ϰ� ���� ��
        {
            if (m_eButtonState == E_SELECT) // ������ Ŭ���� �ߴ� ���¶��
            {
                // ������ ��
                if (m_pButton)
                {
                    m_pButton->OnClick(this);
                    //m_pButton->OnRelease(this);
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
            if (m_pButton && m_eButtonState == E_SELECT)
            {
                m_pButton->OnRelease(this);
            }
            m_eButtonState = E_NORMAL;
        }
    }

    if (g_pKeyManager->isStayKeyDown(VK_LBUTTON))   // Ŭ����
    {
        if (m_pButton && m_eButtonState == E_SELECT)
        {
            m_pButton->OnDrag(this);
        }
    }

    cUIObject::UpdateChildren();
}

void cUIButton::Render(LPSPRITE pSprite)
{
    RECT rc;
    SetRect(&rc, 0, 0, (int)m_stSize.x, (int)m_stSize.y);
    pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

    pSprite->SetTransform(&m_matWorld);
    if (m_aTexture[m_eButtonState])
    {
        pSprite->Draw(m_aTexture[m_eButtonState], &rc, &Vector3(0, 0, 0),
            &Vector3(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
    }

    pSprite->End();

    cUIObject::Render(pSprite); // �θ��� ���� ȣ��
}
