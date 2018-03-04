#pragma once
#include "cUIObject.h"

class cUITextView : public cUIObject
{
private:
    SYNTHESIZE(string, m_sText, Text);
    SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
    SYNTHESIZE(DWORD, m_dwColor, Color);
    SYNTHESIZE_ADD_REF(LPFONT, m_pFont, Font);

public:
    cUITextView();
    virtual ~cUITextView();

    virtual void Render(LPSPRITE pSprite) override;
};