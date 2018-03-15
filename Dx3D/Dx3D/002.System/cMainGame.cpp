#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cFrustum.h"

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
    g_pMeshManager->LoadBasicMesh();
    g_pCharacterManager->Setup();
    D3DXCreateSphere(g_pDevice, 1, 10, 10, &m_pSphere, NULL);
}


cMainGame::~cMainGame()
{
    SAFE_RELEASE(m_pSphere);
    SAFE_RELEASE(m_pTerain);

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
    
    m_pFrustum = new cFrustum;
    g_pAutoReleasePool->AddObject(m_pFrustum);
    m_pFrustum->Setup();

    m_pPlayer = g_pCharacterManager->GetPlayer();

    for (int i = 0; i < 1; i++)
    {
        cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
        m_pEnermy->SetPosition(GetRandomVector3(Vector3(0, 0, 0), Vector3(5, 0, 5)));
        m_pEnermy->SetActive(true);
        m_pEnermy->SetHP(100);
        m_pEnermy->SetATK(5);
        m_vecMonster.push_back(m_pEnermy);
    } 

    m_pPlayer->SetVecMonster(&m_vecMonster);
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update(&m_pPlayer->GetPosition());
    }

    m_pFrustum->Update();

    m_pPlayer->Update();

    for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
    {
        (*iter)->Update();
    }

    for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end();)
    {
        if ((*iter)->GetAlive())
            iter++;
        else
            iter = m_vecMonster.erase(iter);
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

    Matrix4 matS;
    D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
    g_pDevice->SetTransform(D3DTS_WORLD, &matS);
    (*g_pMeshManager->GetBasicMesh("map"))->DrawSubset(0);

    m_pPlayer->Render();
    
    for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
    {
        bool result = false;
        m_pFrustum->IsInFrustum(result, &(*iter)->GetSphere());
        if (result)
            (*iter)->Render();
    }

    Matrix4 mat;
    D3DXMatrixIdentity(&mat);
    g_pDevice->SetTransform(D3DTS_WORLD, &mat);
    m_pSphere->DrawSubset(0);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_pCamera)
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
}