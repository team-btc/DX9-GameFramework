#pragma once
#include "cCamera.h"
#include "cParticle.h"

class cMainGame
{
private:
    cCamera*                m_pCamera;
    cParticle*              m_pParticle;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};