#pragma once
#include "cCamera.h"

class cMapLoad;
class cSkinnedMesh;

class cMainGame
{
private:
    cCamera*                m_pCamera;
    cMapLoad*                map;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};