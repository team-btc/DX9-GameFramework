#pragma once
#include "cObject.h"
#include "cUIButton.h"

class cUIObject;
enum { E_BUTTON1, E_BUTTON2, E_TEXTVIEW, E_TEXTVIEW2, E_TITLEBAR };
class cUIMenu : public cObject, public iUIButton
{
private:
    LPSPRITE            m_pSprite;
    IMAGE_INFO           m_stImageInfo;
    cUIObject*          m_pUIRoot;
    SYNTHESIZE(string, m_szText, Text);
    SYNTHESIZE(string, m_szText2, Text2);
    SYNTHESIZE(bool, m_btnMove, BtnMove);

public:
    cUIMenu();
    ~cUIMenu();

    void Setup();
    void Update(Vector3* pos = NULL);
    void Render();
    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // iUIButton을(를) 통해 상속됨
    virtual void OnClick(cUIButton* pSender) override;
    virtual void OnDrag(cUIButton* pSender) override;
    virtual void OnRelease(cUIButton* pSender) override;
};

