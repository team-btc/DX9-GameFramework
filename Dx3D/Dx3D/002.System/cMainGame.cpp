#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cPlayer.h"

cMainGame::cMainGame()
    : m_pCamera(NULL)
{
    SetCursorPos(W_WIDTH / 2, W_HEIGHT / 2);
    HRESULT hr = S_OK;
    g_pLogManager->Setup("\\Log\\");
    hr = g_pDbManager->Setup();
    hr = g_pKeyManager->Setup();
    hr = g_pTimerManager->Setup();
    hr = g_pDeviceManager->Setup();
    hr = g_pMaterialManager->Setup();
    D3DXCreateBox(g_pDevice, 1, 1, 1, &m_pSphere, NULL);
}


cMainGame::~cMainGame()
{
    SAFE_RELEASE(m_pSphere);

    HRESULT hr = S_OK;

    //  CUSTOM RESOURCE 해제
    g_pFontManager->Destroy();
    g_pTextureManager->Destroy();
    g_pMeshManager->Destroy();
    g_pShaderManager->Destroy();

    //  SYSTEM RESOURCE 해제
    g_pAutoReleasePool->Drain();
    g_pBroadcastManager->Destroy();
    g_pObjectManager->Destory();
    hr = g_pDbManager->Destroy();
    hr = g_pDeviceManager->Destroy();

    if (hr != S_OK)
    {
        assert(false && _TEXT("매니저 Destroy에 실패했음."));
    }
}

void cMainGame::Setup()
{
    HRESULT hr;
    srand((int)time(NULL));

    m_pCamera = new cCamera;
    hr = m_pCamera->Setup();
    g_pAutoReleasePool->AddObject(m_pCamera);

    m_pPlayer = new cPlayer("Ghost", "Assets\\Unit\\Ghost", "Ghost.X");
    m_pEnermy = new cPlayer("Zelot", "Assets\\Zealot", "Zealot.X");
    
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update(&m_pPlayer->GetPosition());
    }

    m_pPlayer->Update();
    //m_pEnermy->Update();

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

    m_pPlayer->Render();
    m_pEnermy->Render();

    m_pSphere->DrawSubset(0);

    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_pCamera)
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
}