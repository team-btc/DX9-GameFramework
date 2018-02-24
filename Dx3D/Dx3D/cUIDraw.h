#pragma once
#include "cObject.h"

class cUIObject;

class cUIDraw : public cObject
{
private:
    LPSPRITE            m_pSprite;
    IMAGE_INFO           m_stImageInfo;
    cUIObject*          m_pUIRoot;

public:
    cUIDraw();
    ~cUIDraw();

    void Setup();
    void Update(Vector3* pos = NULL);
    void Render();
    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

