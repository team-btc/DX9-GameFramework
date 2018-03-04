#pragma once
#define DEBUG_RENDER (true)
#include "cUIObject.h"

class cUIProgressBar : public cUIObject
{
private:
    enum E_PROGRESSBAR_INNER_TYPE
    {
        E_PROGRESSBAR_TEXTURE_FRONT = 0, E_PROGRESSBAR_TEXTURE_BACK, E_PROGRESSBAR_TEXT
    };
private:
    float	m_fMaxGuage;
    float	m_fCurGuage;        // 포인터로 할까 어떻게 할까 흠...

public:
    cUIProgressBar();
    virtual ~cUIProgressBar();

    HRESULT SetMaxGuage(IN float fMaxGuage);        // 포인터로 할지는 의논
    HRESULT SetCurrentGuage(IN float fCurGuage);    // 포인터로 할지는 의논

    HRESULT AddText(IN LPFONT pFont, IN DWORD TextFormat,
        IN D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
    HRESULT AddGuageTexture(IN string GuageImgPath,
        IN int nImgType = 0, IN D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
    HRESULT Setup(IN string strProgressBarName, IN Vector3 vPosition, IN Vector2 vSize,
        IN float fGuageMax);

    void Update(IN float fCurrentGuage,
        IN Vector3* vFollowPosition = NULL);    // 오버라이드로 했어야 오브젝트에 막들어가는데..
    virtual void Update() override;
    virtual void Render(LPSPRITE pSprite) override;
};
