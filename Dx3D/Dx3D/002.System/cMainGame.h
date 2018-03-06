#pragma once
#include "cCamera.h"

#include "cGameObject.h"

class cMainGame
{
private:
    cCamera*    m_pCamera;

#pragma region EmitMessageTest
    cGameObject*    m_pGObj1;
#pragma endregion

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};