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
    if (m_pTexture && m_isAxtive)
    {
        pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

        pSprite->SetTransform(&m_matWorld);
        pSprite->Draw(m_pTexture, NULL, &Vector3(0, 0, 0),
            &Vector3(0, 0, 0), m_dwColor);

        pSprite->End();
    }

    cUIObject::Render(pSprite);	// 부모의 렌더 호출
}

void cUIImageView::SetScale(float width, float height)
{
    m_matWorld._11 = width;
    m_matWorld._22 = height;
}
