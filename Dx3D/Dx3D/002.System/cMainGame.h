#pragma once
#include "cCamera.h"
#include "cParticleExplosion.h"
#include "cParticleSnow.h"
#include "cParticle.h"

class cMainGame
{
private:
    cCamera*                m_pCamera;
    cParticleExplosion*     m_pExplosion;
    cParticleSnow*          m_pSnow;
    cParticle*              m_pParticle;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};