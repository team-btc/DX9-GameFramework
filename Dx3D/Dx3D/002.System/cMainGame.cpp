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
    g_pMeshManager->LoadBasicMesh();
    D3DXCreateSphere(g_pDevice, 1, 10, 10, &m_pSphere, NULL);
    D3DXLoadMeshFromX(L"Assets\\HeightMapData\\Plane.X", D3DXMESH_MANAGED, g_pDevice, NULL, NULL, NULL, NULL, &m_pTerain);
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
    m_pPlayer->SetHP(100);
    m_pPlayer->SetATK(100);

    for (int i = 0; i < 2; i++)
    {
        cMonster* m_pEnermy = new cMonster("Zelot", "Assets\\Zealot", "Zealot.X");
        m_pEnermy->SetPosition(GetRandomVector3(Vector3(0, 0, 0), Vector3(5, 0, 5)));
        m_pEnermy->SetHP(100);
        m_pEnermy->SetATK(5);
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

    if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON) && !m_pPlayer->GetAttak())
    {
        cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
        BOOL isHit = false;
        float _dist = 0.0f;

        //맵이동
        D3DXIntersectSubset(m_pTerain, 0, &ray.m_vOrg, &ray.m_vDir, &isHit, 0, 0, 0, &_dist, NULL, NULL);
        if (isHit)
        {
            if (!m_pPlayer->GetRun())
                m_pPlayer->RunAnim();
            Vector3 _Dest = ray.m_vOrg + ray.m_vDir*_dist;
            m_pPlayer->SetAttack(false);
            m_pPlayer->SetMoveToTarget(false);
            m_pPlayer->SetMoveToPoint(true);
            m_pPlayer->SetDestPoint(_Dest);
        }
      
      
        //메쉬 공격
        for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
        {
            if (ray.IsPicked(&(*iter)->GetSphere()))
            {
                if(!m_pPlayer->GetRun())
                    m_pPlayer->RunAnim();
                m_pPlayer->SetMoveToPoint(false);
                m_pPlayer->SetMoveToTarget(true);
                m_pPlayer->RayCast((*iter));
            }
        }
        
    }

    // 타켓만 정해준다.
    if (g_pKeyManager->isOnceKeyDown(VK_SHIFT))
    {
        if (m_vecMonster.size() > 0)
        {
            m_pPlayer->NearestSearch(m_vecMonster);
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

    Matrix4 matS;
    D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
    g_pDevice->SetTransform(D3DTS_WORLD, &matS);
    m_pTerain->DrawSubset(0);

    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pPlayer->Render();
    
    for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
    {
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