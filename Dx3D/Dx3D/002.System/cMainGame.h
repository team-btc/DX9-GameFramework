#pragma once
#include "cCamera.h"

class cMainGame
{
private:
    cCamera*                m_pCamera;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};