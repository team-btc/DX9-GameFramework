#pragma once
#include "cFrustum.h"

#define NUM_TEST 3

class cCamera;

class cMainGame
{
private:
    cCamera*                m_pCamera;

    LPEFFECT                m_pEffect;
    LPMESH                  m_pMesh;
    LPTEXTURE9              m_pDiffuseMap;
    LPTEXTURE9              m_pSpecularMap;

    cFrustum*               m_pFrustum;

    vector<cSkinnedMesh*>	m_vecSkinnedMesh;

    ST_NEWS*                m_pCurrNews;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};