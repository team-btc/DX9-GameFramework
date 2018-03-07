#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"

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
}


cMainGame::~cMainGame()
{
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

    LIGHT9 l = g_pLightManager->InitDirectional(&Vector3(1, -1, 1), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    g_pDevice->SetLight(0, &l);
    g_pDevice->LightEnable(0, true);

    g_pShaderManager->AddEffect("multi-animation", "Assets/MultiAnimation.hpp");
    g_pMeshManager->LoadSkinnedMesh();

    m_pCamera = new cCamera;
    hr = m_pCamera->Setup();
    g_pAutoReleasePool->AddObject(m_pCamera);

    cUnitObject* unit = new cUnitObject;
    unit->Load("banshee");
    g_pAutoReleasePool->AddObject(unit);
    m_vecUnit.push_back(unit);
}

void cMainGame::Update()
{
    if (g_pKeyManager->isStayKeyDown('Q'))
    {
        float l = m_pCamera->GetLength();
        l -= 0.1f;
        m_pCamera->SetLength(l);
    }

    if (g_pKeyManager->isStayKeyDown('W'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.x -= 0.1f;
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('E'))
    {
        float l = m_pCamera->GetLength();
        l += 0.1f;
        m_pCamera->SetLength(l);
    }

    if (g_pKeyManager->isOnceKeyDown('R'))
    {
        cUnitObject* unit = new cUnitObject;
        unit->Load("banshee");
        unit->GetMesh()->SetPosition(Vector3(m_vecUnit.size() * 0.5f, 0, 0));
        g_pAutoReleasePool->AddObject(unit);
        m_vecUnit.push_back(unit);
    }

    if (g_pKeyManager->isStayKeyDown('A'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.y -= 0.1f;
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('S'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.x += 0.1f;
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('D'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.y += 0.1f;
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isOnceKeyDown('F'))
    {
    }

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
    g_pTimerManager->Render();

    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    for (auto iter = m_vecUnit.begin(); iter != m_vecUnit.end(); iter++)
    {
        (*iter)->Render();
    }

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}