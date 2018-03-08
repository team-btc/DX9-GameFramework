#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cMonster.h"

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

    for (int i = 0; i < 2; i++)
    {
        cMonster* m_pEnermy = new cMonster("Zelot", "Assets\\Zealot", "Zealot.X");
        m_pEnermy->SetPosition(GetRandomVector3(Vector3(0, 0, 0), Vector3(5, 0, 5)));
        m_vecMonster.push_back(m_pEnermy);
    } 
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update(&m_pPlayer->GetPosition());
    }

    m_pPlayer->Update();
    for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
    {
        (*iter)->Update();
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

    if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
    {
        cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
        BOOL isHit = false;
        float fDest;

       /* for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
        {
            ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)(*iter)->GetSkinnedMesh()->GetRootFrame();
            D3DXIntersectSubset((*pBoneMesh).pWorkingMesh, NULL, &ray.m_vDir, &ray.m_vDir, &isHit, NULL, NULL, NULL, &fDest, NULL, NULL);
            if (isHit)
            {
                cout << "dd" << endl;
            }
        }*/
        D3DXIntersectSubset(m_pSphere, 0, &ray.m_vDir, &ray.m_vDir, &isHit, 0, 0, 0, &fDest, NULL, NULL);
        if (isHit)
        {
            cout << "dd" << endl;
        }
    }

    if (g_pKeyManager->isOnceKeyDown(VK_SHIFT))
    {
        //나와 제일 가까운 몹을 판별하는 문법이 필요하다
        m_pPlayer->GetSkinnedMesh()->SetAnimationIndex(1);

        cMonster* nearMonster = NULL;
        float nearDist = 9999;
        for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
        {
            if (m_pPlayer->Distance((*iter)->GetPosition()) < nearDist)
            {
                nearDist = m_pPlayer->Distance((*iter)->GetPosition());
                nearMonster = *iter;
            }
        }

        if (nearMonster)
        {
            m_pPlayer->RayCast(nearMonster);
            nearMonster->RayCast(m_pPlayer);

            if (m_pPlayer->Distance(nearMonster->GetPosition()) < m_pPlayer->GetSphere().fRadius + nearMonster->GetSphere().fRadius)
            {
                m_pPlayer->Attack();
                m_pPlayer->SetAttack(true);
            }
            else
                m_pPlayer->SetMoveToTarget(true);
        }
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
    
    for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
    {
        (*iter)->Render();
    }

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