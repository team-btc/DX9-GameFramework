#pragma once
#include "cUIObject.h"


class cUIImageView : public cUIObject
{
private:
    SYNTHESIZE(Color, m_dwColor, Color);
    SYNTHESIZE_ADD_REF(LPTEXTURE9, m_pTexture, Texture);

public:
    cUIImageView();
    virtual ~cUIImageView();

    virtual void Render(LPSPRITE pSprite) override;
    void SetScale(int width, int height);
};

