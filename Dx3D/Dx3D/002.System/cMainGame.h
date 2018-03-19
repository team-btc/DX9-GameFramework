#pragma once
#include "cCamera.h"
#include "cTitleScene.h"
#include "cLoadingScene.h"
#include "cPlayScene.h"
#include "cEndingScene.h"

class cMainGame
{
private:

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};