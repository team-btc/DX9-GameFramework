#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cMapLoad.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cFrustum.h"

cMainGame::cMainGame()
    : m_pCamera(NULL)
{
    HRESULT hr = S_OK;
    g_pLogManager->Setup("\\Log\\");
    hr = g_pDbManager->Setup();
    hr = g_pKeyManager->Setup();
    hr = g_pTimerManager->Setup();
    hr = g_pDeviceManager->Setup();
    hr = g_pMaterialManager->Setup();
    g_pMeshManager->LoadBasicMesh();
    
    g_pScnManager->Setup();

    g_pMeshManager->LoadSkinnedMesh();
    g_pMeshManager->LoadJSON();
    g_pCharacterManager->Setup();
}


cMainGame::~cMainGame()
{
    SAFE_RELEASE(m_pSphere);
    SAFE_RELEASE(m_pTerain);

    HRESULT hr = S_OK;

    g_pScnManager->Destroy();

    SAFE_DELETE(m_pExplosion);
    SAFE_DELETE(m_pSnow);

    //  CUSTOM RESOURCE 해제
    g_pFontManager->Destroy();
    g_pTextureManager->Destroy();
    g_pMeshManager->Destroy();
    g_pShaderManager->Destroy();

    //  SYSTEM RESOURCE 해제
    g_pAutoReleasePool->Drain();
    g_pBroadcastManager->Destroy();
    g_pObjectManager->Destory();
    g_pMapManager->Destroy();
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

    m_pCamera = new cCamera;
    hr = m_pCamera->Setup();
    g_pAutoReleasePool->AddObject(m_pCamera);

    cMapLoad* map = new cMapLoad;

    g_pScnManager->AddScene("map", map);
    g_pScnManager->ChangeScene("map");

    m_pFrustum = new cFrustum;
    g_pAutoReleasePool->AddObject(m_pFrustum);
    m_pFrustum->Setup();

    m_pPlayer = g_pCharacterManager->GetPlayer();

    m_vecMonster = new vector<cMonster*>;
    for (int i = 0; i < 1; i++)
    {
        cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
        m_pEnermy->SetPosition(GetRandomVector3(Vector3(0, 0, 0), Vector3(5, 0, 5)));
        m_pEnermy->SetActive(true);
        (*m_vecMonster).push_back(m_pEnermy);
    } 

    m_pPlayer->SetVecMonster(m_vecMonster);
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update(&m_pPlayer->GetPosition());
    }

    g_pScnManager->Update();

    m_pFrustum->Update();

    m_pPlayer->Update();

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        (*iter)->Update();
    }

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end();)
    {
        if ((*iter)->GetAlive())
        {
            iter++;
        }
        else
        {
            iter = (*m_vecMonster).erase(iter);
        }
    }
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

        hr = g_pScnManager->Render();

        hr = g_pTimerManager->Render();

        g_pScnManager->Render();
        
        hr = g_pDevice->EndScene();
        hr = g_pDevice->Present(0, 0, 0, 0);
    }

    m_pPlayer->Render();
    
    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        bool result = false;
        m_pFrustum->IsInFrustum(result, &(*iter)->GetSphere());
        if (result)
            (*iter)->Render();
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
    if (m_pCamera)
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
}