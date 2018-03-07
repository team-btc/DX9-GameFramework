#pragma once
#include "cCamera.h"
#include "cPlayer.h"

class cMainGame
{
private:
    LPMESH      m_pSphere;
    cCamera*    m_pCamera;
    cPlayer*    m_pPlayer;
    cPlayer*    m_pEnermy;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};