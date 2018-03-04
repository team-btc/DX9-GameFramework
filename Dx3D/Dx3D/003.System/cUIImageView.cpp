#include "stdafx.h"
#include "cUIImageView.h"


cUIImageView::cUIImageView()
    : m_dwColor(D3DCOLOR_XRGB(255, 255, 255))
    , m_pTexture(NULL)
{
}


cUIImageView::~cUIImageView()
{
    SAFE_RELEASE(m_pTexture);
}

void cUIImageView::Render(LPSPRITE pSprite)
{
    if (m_pTexture)
    {
        RECT rc;
        SetRect(&rc, 0, 0, (int)m_stSize.x, (int)m_stSize.y);
        pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

        pSprite->SetTransform(&m_matWorld);
        pSprite->Draw(m_pTexture, NULL, &Vector3(0, 0, 0),
                      &Vector3(0, 0, 0), m_dwColor);

        pSprite->End();
    }

    cUIObject::Render(pSprite);	// �θ��� ���� ȣ��
}

void cUIImageView::SetScale(int width, int height)
{
    m_matWorld._11 = (float)width / m_stSize.x;
    m_matWorld._22 = (float)height / m_stSize.y;
}
