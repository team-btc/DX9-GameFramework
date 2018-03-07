#pragma once
#include "cCamera.h"
#include "cMeshLoader.h"
class cMainGame
{
private:
    cCamera*    m_pCamera;
    cObjMeshLoader* m_pMeshLoader;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void RenderSubset(UINT i);

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};