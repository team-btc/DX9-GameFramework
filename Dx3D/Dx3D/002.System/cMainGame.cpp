#include "stdafx.h"
#include "cMainGame.h"

cMainGame::cMainGame()
{
    HRESULT hr = S_OK;
    g_pLogManager->Setup("\\Log\\");
    hr = g_pDbManager->Setup();
    hr = g_pKeyManager->Setup();
    hr = g_pTimerManager->Setup();
    hr = g_pDeviceManager->Setup();
    hr = g_pMaterialManager->Setup();
    g_pMeshManager->LoadJSON();
    g_pMeshManager->LoadBasicMesh();
    g_pMeshManager->LoadSkinnedMesh();

    g_pScnManager->Setup();
    g_pCharacterManager->Setup();
}


cMainGame::~cMainGame()
{
    HRESULT hr = S_OK;

    g_pScnManager->Destroy();
    //  CUSTOM RESOURCE 해제
    g_pFontManager->Destroy();
    g_pTextureManager->Destroy();
    g_pMeshManager->Destroy();
    g_pShaderManager->Destroy();

    //  SYSTEM RESOURCE 해제
    g_pAutoReleasePool->Drain();
    g_pMapManager->Destroy();
    g_pObjectManager->Destory();
    hr = g_pDbManager->Destroy();
    hr = g_pDeviceManager->Destroy();
    g_pCharacterManager->Destroy();
    
    if (hr != S_OK)
    {
        assert(false && _TEXT("매니저 Destroy에 실패했음."));
    }
}

void cMainGame::Setup()
{
    HRESULT hr;
    srand((int)time(NULL));

    hr = g_pScnManager->AddScene("title", new cTitleScene);
    hr = g_pScnManager->AddScene("loading", new cLoadingScene);
    hr = g_pScnManager->AddScene("play", new cPlayScene);
    hr = g_pScnManager->AddScene("ending", new cEndingScene);

    hr = g_pScnManager->ChangeScene("title");
}

void cMainGame::Update()
{
    g_pScnManager->Update();
}

void cMainGame::Render()
{
    HRESULT hr;
    hr = g_pDevice->TestCooperativeLevel();
    //if (SUCCEEDED(hr))
    {
        hr = g_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);

        hr = g_pDevice->BeginScene();
#ifdef _DEBUG
        hr = g_pTimerManager->Render();
#endif // _DEBUG

        g_pScnManager->Render();
        
        hr = g_pDevice->EndScene();
        hr = g_pDevice->Present(0, 0, 0, 0);
    }

    //if (hr == D3DERR_DEVICELOST)
    //{
    //    assert(false && "DEVICE LOST");
    //    //LPDIRECT3DSURFACE9 pSurface9;
    //    //g_pDevice->GetDepthStencilSurface(&pSurface9);
    //    //pSurface9->Release();
    //}
    //else if (hr == D3DERR_DEVICENOTRESET)
    //{
    //    assert(false && "DEVICE NOT RESET");
    //    //g_pDeviceManager->Reset();
    //}
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g_pScnManager->WndProc(hWnd, message, wParam, lParam);
}