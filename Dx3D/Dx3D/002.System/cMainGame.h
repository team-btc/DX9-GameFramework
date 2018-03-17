#pragma once
#include "cCamera.h"
#include "cParticleExplosion.h"
#include "cParticleSnow.h"

class cMainGame
{
private:
    cCamera*                m_pCamera;
    cParticleExplosion*     m_pExplosion;
    cParticleSnow*          m_pSnow;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};