#pragma once
#include "cCamera.h"
#include "cTitleScene.h"
#include "cLoadingScene.h"
#include "cPlayScene.h"
#include "cEndingScene.h"

class cMapLoad;

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