#pragma once
#include "cUIObject.h"

class cUIButton;

interface iUIButton
{
    virtual void OnClick(cUIButton* pSender) PURE;
    virtual void OnDrag(cUIButton* pSender) PURE;
    virtual void OnRelease(cUIButton* pSender) PURE;
};

class cUIButton : public cUIObject
{
private:
    enum eButtonState
    {
        E_NORMAL,
        E_MOUSEOVER,
        E_SELECTED,
        E_COUNT
    };

    eButtonState		m_eButtonState;         // 현재 버튼 상태
    LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];    // 버튼 텍스쳐 배열
    SYNTHESIZE(iUIButton*, m_pButton, UIButton);
    SYNTHESIZE(POINT, m_ptClickPos, ClickPos);

public:
    cUIButton();
    virtual ~cUIButton();

    void SetTexture(string sNormal, string sMouseOver, string sSelected);
    eButtonState GetButtonState() { return m_eButtonState; }
    void SetButtonState(int state) { m_eButtonState = (eButtonState)E_NORMAL; }
    void SetScale(float width, float height);

    // cUIObject override
    void SetSize(Vector2 size) override;
    virtual void Update() override;
    virtual void Render(LPD3DXSPRITE pSprite) override;
};

