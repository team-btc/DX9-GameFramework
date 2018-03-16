#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"

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
}


cMainGame::~cMainGame()
{
    HRESULT hr = S_OK;

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

    m_pCamera = new cCamera;
    hr = m_pCamera->Setup();
    g_pAutoReleasePool->AddObject(m_pCamera);

    g_pMeshManager->LoadBasicMesh();

    g_pTextureManager->AddTexture("test", "Assets/Texture/Particle/particle_circle.png");
    g_pTextureManager->AddTexture("snow", "Assets/Texture/Particle/particle_snow.png");

    m_pExplosion = new cParticleExplosion(&Vector3(0, 0, 0), 1000);
    m_pExplosion->Init("test");

    cBoundingBox* box = new cBoundingBox(Vector3(-100, 0, -100), Vector3(100, 10, 100));
    
    m_pSnow = new cParticleSnow(box, 1000);
    m_pSnow->Init("snow");
}

void cMainGame::Update()
{
    float speed = 5.0f;
    if (g_pKeyManager->isStayKeyDown('Q'))
    {
        float l = m_pCamera->GetLength();
        l -= speed * g_pTimerManager->GetDeltaTime();
        m_pCamera->SetLength(l);
    }

    if (g_pKeyManager->isStayKeyDown('W'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.x -= speed * g_pTimerManager->GetDeltaTime();
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('E'))
    {
        float l = m_pCamera->GetLength();
        l += speed * g_pTimerManager->GetDeltaTime();
        m_pCamera->SetLength(l);
    }

    if (g_pKeyManager->isOnceKeyDown('R'))
    {
        m_pExplosion->Reset();
    }

    if (g_pKeyManager->isStayKeyDown('A'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.y -= speed * g_pTimerManager->GetDeltaTime();
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('S'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.x += speed * g_pTimerManager->GetDeltaTime();
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('D'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.y += speed * g_pTimerManager->GetDeltaTime();
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isOnceKeyDown('F'))
    {
    }

    if (m_pCamera)
    {
        m_pCamera->Update();
    }

    if (m_pExplosion)
    {
        m_pExplosion->Update();
    }

    if (m_pSnow)
    {
        m_pSnow->Update();
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

        SAFE_RENDER(m_pExplosion);
        SAFE_RENDER(m_pSnow);

        hr = g_pTimerManager->Render();


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
}