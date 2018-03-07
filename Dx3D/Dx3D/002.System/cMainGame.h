#pragma once
#include "cCamera.h"
#include "cUnitObject.h"

class cMainGame
{
private:
    cCamera*    m_pCamera;

    vector<cUnitObject*>    m_vecUnit;
public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};