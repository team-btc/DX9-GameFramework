#pragma once
#include "cCamera.h"
#include "cParticleExplosion.h"

class cMainGame
{
private:
    cCamera*                m_pCamera;
    cParticleExplosion*     m_pExplosion;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};