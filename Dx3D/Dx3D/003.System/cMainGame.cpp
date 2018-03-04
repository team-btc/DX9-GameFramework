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
    //  CUSTOM RESOURCE 해제
    g_pFontManager->Destory();
    g_pTextureManager->Destroy();
    g_pMeshManager->Destroy();
    g_pShaderManager->Destroy();

    //  SYSTEM RESOURCE 해제
    g_pBroadcastManager->Destroy();
    g_pAutoReleasePool->Drain();
    g_pObjectManager->Destory();
    g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
    HRESULT hr;
    srand((int)time(NULL));

    m_pCamera = new cCamera;
    hr = m_pCamera->Setup();
    g_pAutoReleasePool->AddObject(m_pCamera);
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update();
    }

    if (g_pKeyManager->isOnceKeyDown('Q'))
    {
    }

    if (g_pKeyManager->isOnceKeyDown('W'))
    {
    }

    if (g_pKeyManager->isOnceKeyDown('E'))
    {
    }

    if (g_pKeyManager->isOnceKeyDown('R'))
    {
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
    g_pTimerManager->Render();

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}