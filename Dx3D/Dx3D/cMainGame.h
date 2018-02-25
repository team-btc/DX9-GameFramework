#pragma once

class cCamera;

class cMainGame
{
private:
    cCamera*                m_pCamera;

    LPEFFECT                m_pEffect;
    LPMESH                  m_pMesh;
    LPTEXTURE9              m_pDiffuseMap;
    LPTEXTURE9              m_pSpecularMap;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

