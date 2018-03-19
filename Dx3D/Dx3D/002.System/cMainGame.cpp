#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cMapLoad.h"


cMainGame::cMainGame()
{
    HRESULT hr = S_OK;
    g_pLogManager->Setup("\\Log\\");
    hr = g_pDbManager->Setup();
    hr = g_pKeyManager->Setup();
    hr = g_pTimerManager->Setup();
    hr = g_pDeviceManager->Setup();
    hr = g_pCameraManager->Setup();
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
    g_pCameraManager->Destroy();
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

    //m_pCamera = new cCamera;
    //hr = m_pCamera->Setup();
    //g_pAutoReleasePool->AddObject(m_pCamera);

    //map = new cMapLoad;

    hr = g_pScnManager->AddScene("title", new cTitleScene);
    hr = g_pScnManager->AddScene("loading", new cLoadingScene);
    hr = g_pScnManager->AddScene("play", new cPlayScene);
    hr = g_pScnManager->AddScene("ending", new cEndingScene);

    hr = g_pScnManager->ChangeScene("title");
}

void cMainGame::Update()
{
    g_ptDeltaMouse.x = g_ptMouse.x - g_ptDeltaMouse.x;
    g_ptDeltaMouse.y = g_ptMouse.y - g_ptDeltaMouse.y;
    g_pCameraManager->Update();
    g_pScnManager->Update();
    g_ptDeltaMouse = g_ptMouse;

    ResetWMParam();
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
        hr = g_pCameraManager->Render();
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

void cMainGame::ResetWMParam()
{
    g_nWheelMouse = 0;
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEMOVE:
    {
        g_ptMouse.x = LOWORD(lParam);
        g_ptMouse.y = HIWORD(lParam);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        g_nWheelMouse = GET_WHEEL_DELTA_WPARAM(wParam);
        break;
    }
    case WM_LBUTTONDBLCLK:
    {
        g_ptLDoubleClick.x = LOWORD(lParam);
        g_ptLDoubleClick.y = HIWORD(lParam);
        break;
    }
    case WM_RBUTTONDBLCLK:
    {
        g_ptRDoubleClick.x = LOWORD(lParam);
        g_ptRDoubleClick.y = HIWORD(lParam);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        g_ptLClick.x = LOWORD(lParam);
        g_ptLClick.y = HIWORD(lParam);
        break;
    }
    case WM_LBUTTONUP:
    {
        g_ptLClick.x = -1;
        g_ptLClick.y = -1;
        break;
    }
    case WM_RBUTTONDOWN:
    {
        g_ptRClick.x = LOWORD(lParam);
        g_ptRClick.y = HIWORD(lParam);
        break;
    }
    case WM_RBUTTONUP:
    {
        g_ptRClick.x = -1;
        g_ptRClick.y = -1;
        break;
    }
    }

    g_pScnManager->WndProc(hWnd, message, wParam, lParam);
}