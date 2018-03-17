#pragma once
#include "cCamera.h"
#include "cPlayer.h"
#include "cMonster.h"

class cSkinnedMesh;
class cFrustum;

class cMainGame
{
private:
    cCamera*                m_pCamera;
    cPlayer*                m_pPlayer;
    cFrustum*               m_pFrustum;
    vector<cMonster*>*      m_vecMonster;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};