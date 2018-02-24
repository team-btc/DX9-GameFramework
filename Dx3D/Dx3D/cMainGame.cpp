#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"

cMainGame::cMainGame()
    : m_pCamera(NULL)
{
    g_pLogManager->Setup("\\Log\\");
    g_pKeyManager->Setup();
    SetCursorPos(W_WIDTH / 2, W_HEIGHT / 2);
}


cMainGame::~cMainGame()
{
    SAFE_DELETE(m_pCamera);

    g_pFontManager->Destory();
    g_pTextureManager->Destroy();
    g_pMeshManager->Destroy();

    g_pAutoReleasePool->Drain();
    g_pObjectManager->Destory();
    g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
    srand((int)time(NULL));

    Vector3 dir(1.0f, -1.0f, 0.0f);
    D3DXVec3Normalize(&dir, &dir);
    XColor c = WHITE;
    LIGHT9 stLight = InitDirectional(&dir, &c);

    g_pDevice->SetLight(0, &stLight);
    g_pDevice->LightEnable(0, true);

    m_pCamera = new cCamera;
    m_pCamera->Setup();
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update();
    }
}


void cMainGame::Render()
{
    g_pDevice->Clear(NULL,
                     NULL,
                     D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                     D3DCOLOR_XRGB(47, 121, 112),
                     1.0f,
                     0);

    g_pDevice->BeginScene();

    g_pScnManager->Render();

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}