#include "stdafx.h"
#include "cTitleScene.h"

cTitleScene::cTitleScene()
    : m_stMapInfo(NULL)
    , m_pCamera(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_szMapKey("icecrown")
    , m_vSindraPos(229, 199, 242)
    , m_vArtuhsPos(Vector3(261, 88, 482))
    , m_vCameraPos(259.47f, 171.95f, 279.68f)
    , m_nCurrIndex(0)
    , m_isRoar(false)
    , m_isStart(false)
    , m_isArthusMove(false)
    , m_ft(0)
    , m_vStartPos(0,0,0)
    , m_fWorldTime(0)
    , m_isMovieStart(false)
{
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 199.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 199.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 400.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(280.0f, -88.0f, 540.0f));
   
    for (int i = 0; i < m_vecSindraJumpTarget.size(); ++i)
    {
        m_vecIsArriveSindra.push_back(false);
    }
}


cTitleScene::~cTitleScene()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
}

HRESULT cTitleScene::Start()
{
  
    //  LOAD MAP
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.LoadMap(m_szMapKey);

        m_stMapInfo = g_pMapManager->GetCurrMapInfo();

        // ���� �� ����
        if (!m_pGameMap)
        {
            m_pGameMap = new cGameMap;
        }

        m_pGameMap->SetCurrMapInfo(m_stMapInfo);

        // �ؽ��� ���̴� ����
        if (!m_pTextureShader)
        {
            m_pTextureShader = new cTextureShader;
        }

        m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
        m_pTextureShader->SetBGTexture(m_stMapInfo->arrTextureInfo[0].pTexture, m_stMapInfo->arrTextureInfo[0].fDensity);
        m_pTextureShader->SetTexture1(m_stMapInfo->arrTextureInfo[1].pTexture, m_stMapInfo->arrTextureInfo[1].fDensity);
        m_pTextureShader->SetTexture2(m_stMapInfo->arrTextureInfo[2].pTexture, m_stMapInfo->arrTextureInfo[2].fDensity);
        m_pTextureShader->SetTexture3(m_stMapInfo->arrTextureInfo[3].pTexture, m_stMapInfo->arrTextureInfo[3].fDensity);
        m_pTextureShader->SetAlphaDraw(m_stMapInfo->pTextureMap);

        // ��ī�̹ڽ� ���̴� ����
        if (!m_pSkyBoxShader)
        {
            m_pSkyBoxShader = new cSkyBoxShader;
        }

        m_pSkyBoxShader->SetBox(m_stMapInfo->pSkyTexture);

        // ���� �����ϸ� ���̺� ���̴� ����
        if (m_stMapInfo->isEnableWater)
        {
            if (!m_pWaveShader)
            {
                m_pWaveShader = new cWaveShader;
            }
            m_pWaveShader->SetMesh(m_stMapInfo->pWaterMesh);
            m_pWaveShader->SetWaveTexture(m_stMapInfo->pWaterTexture);
            m_pWaveShader->SetShader(m_stMapInfo->fWaterHeight, m_stMapInfo->fWaterWaveHeight, m_stMapInfo->fWaterHeightSpeed,
                m_stMapInfo->fWaterUVSpeed, m_stMapInfo->fWaterfrequency, m_stMapInfo->fWaterTransparent, m_stMapInfo->fWaterDensity);
        }
    }

    if (!m_pFrustum)
    {
        m_pFrustum = new cFrustum;
        g_pAutoReleasePool->AddObject(m_pFrustum);
    }

    m_pFrustum->Setup();

    m_pArthus = new cSkinnedMesh("arthaslichking");
    m_pArthus->SetPosition(m_vArtuhsPos);
    m_pArthus->SetScale(8.0f);
    m_pArthus->SetRotation(Vector3(0, -90.0f, 0));
    m_pArthus->SetAnimationIndex(1);
    g_pAutoReleasePool->AddObject(m_pArthus);

    // Setup �ŵ����
    SetSindragosa();

    //  CAMERA SETUP
    if (!m_pCamera)
    {
        m_pCamera = new cCamera;
        m_pCamera->TrackingEnable();
        m_pCamera->SetMaxDist(400.0f);
        m_pCamera->SetMinDist(5.0f);
        g_pCameraManager->AddCamera("title", m_pCamera);
        g_pCameraManager->SetCollisionMesh(m_stMapInfo->pTerrainMesh);
        g_pCameraManager->ColliderDisable();
        
    }
    m_pCamera->Setup();
    m_pCamera->SetPosition(m_vSindraPos);
    m_pCamera->SetTargetPos(m_vCameraPos);
    g_pCameraManager->SetCurrCamera("title");

   
    return S_OK;
}

HRESULT cTitleScene::Update()
{
    //  FRUSTUM CULL UPDATE
    m_pFrustum->Update();

    //  PLAYER UPDATE
    m_pArthus->SetPosition(m_vArtuhsPos);
    // �ŵ���� ������Ʈ
    m_pSindragosa->Update();
    m_pSindragosa->SetPosition(m_vSindraPos);
    // ��ġ üũ

    //�ŵ����� ������ ����
    if (g_pKeyManager->isOnceKeyDown('8'))
    {
        m_isStart = true;
    }
    if (g_pKeyManager->isOnceKeyDown('0'))
    {
        m_isMovieStart = true;
    }
    else  if (g_pKeyManager->isOnceKeyDown('9'))
    {
        m_isMovieStart = false;
        m_isStart = false;
        m_isArthusMove = false;
        m_vSindraPos = Vector3(229, 199, 242);
        m_isRoar = false;
        m_pSindragosa->IdleAnim();
        m_nCurrIndex = 0;
        m_pArthus->SetAnimationIndex(1);
        m_vArtuhsPos = Vector3(261, 88, 482);
        m_ft = 0;      
        for (int n = 0; n <m_vecIsArriveSindra.size(); ++n)
        {
            m_vecIsArriveSindra[n] = false;
        }
    }
    if (m_isMovieStart)
    {
         MoveSindraAllRoute();
    }
    if (m_isStart)
    {
        MoveArthus();
    }
    return S_OK;
}

HRESULT cTitleScene::Render()
{
    if (m_stMapInfo == NULL ||
        m_stMapInfo->pTerrainMesh == NULL)
    {
        return E_FAIL;
    }

    Matrix4 matW, matV, matP;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    MATERIAL9 mtrl;
    mtrl.Ambient = WHITE;
    mtrl.Diffuse = WHITE;
    mtrl.Specular = WHITE;
    mtrl.Emissive = BLACK;
    mtrl.Power = 8.0f;
    g_pDevice->SetMaterial(&mtrl);
    g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->LightEnable(0, true);

    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    Vector4 vP(g_vCameraPos, 1.0f);

    if (m_pSkyBoxShader)
    {
        m_pSkyBoxShader->Render(vP);
    }

    m_pArthus->UpdateAndRender();
    m_pSindragosa->Render();
  
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    


#ifdef _DEBUG

#endif // _DEBUG

    return S_OK;
}

ULONG cTitleScene::Release()
{
    cObject::Release();
    return 0;
}

void cTitleScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_pCamera)
    {
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
    }
}

void cTitleScene::SetSindraAni(int n)
{
    switch (n)
    {
        case 0:
            m_pSindragosa->FlySitDownAnim();
        break;
        case 1:
            m_pSindragosa->FlyWalkAnim();
        break;

        case 2:
            m_pSindragosa->FlySitUpAnim();
        break;

        case 3:
            m_pSindragosa->FlySitAnim();
        break;

        case 7:
            m_pSindragosa->RoarAnim();
        break;

        case 8:
            m_pSindragosa->DeadAnim();
        break;

        case 9:
            m_pSindragosa->RunAnim();
        break;

        case 10:
            m_pSindragosa->WalkAnim();
        break;

        case 11:
            m_pSindragosa->IdleAnim();
        break;

        default:
        break;

    }

}

void cTitleScene::MoveSindra(Vector3 vSpot, int n)
{
    if (!m_vecIsArriveSindra.at(n) && m_nCurrIndex == n)
    {
        if (m_ft == 0)
        {
            m_vStartPos = m_vSindraPos;
        }
        m_vSindraPos = (1 - m_ft) * m_vStartPos + m_ft * vSpot;
        m_ft += 0.006f;
        if (m_ft >= 1.0)
        {
            m_vecIsArriveSindra.at(n) = true;
            m_ft = 0.0f;
        }
    }
    
}

void cTitleScene::MoveArthus()
{
    if (!m_isArthusMove)
    {
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 3.0f;
        m_isArthusMove = true;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_isArthusMove)
    {
        if (m_ft == 0)
        {
            m_pArthus->SetAnimationIndex(11);
            m_vArthusStartPos = m_vArtuhsPos;
            m_vArthusLastPos = Vector3(261, 88, 542);
        }
        m_vArtuhsPos = (1 - m_ft) * m_vArthusStartPos + m_ft * m_vArthusLastPos;
        m_ft += 0.001f;
        if (m_ft >= 1.0)
        {
            m_ft = 1.0f;
        }
    }
}

void cTitleScene::MoveSindraAllRoute()
{
    if (!m_isMovieStart)
    {
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 10.0f;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 0)
    {
        m_pSindragosa->RoarAnim();
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.0f;
        m_nCurrIndex++;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 1)
    {
        m_pSindragosa->FlySitUpAnim();
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.0f;
        m_nCurrIndex++;
    }
    if (m_fWorldTime <= g_pTimerManager->GetWorldTime() && m_nCurrIndex == 2)
    {
        m_pSindragosa->FlyWalkAnim();
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.0f;
        m_nCurrIndex++;
    }
    MoveSindra(m_vecSindraJumpTarget[0], 0);
    MoveSindra(m_vecSindraJumpTarget[1], 1);
    MoveSindra(m_vecSindraJumpTarget[2], 2);
    MoveSindra(m_vecSindraJumpTarget[3], 3);

}


void cTitleScene::SetSindragosa()
{
    Vector3 v = Vector3(0, -90, 0);
    Matrix4 matS;
    D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.5f);
    m_pSindragosa = new cSindragosa("Frostwurmnorthlend");
    g_pAutoReleasePool->AddObject(m_pSindragosa);
    m_pSindragosa->SetPosition(m_vSindraPos);
    m_pSindragosa->SetRotation(v);

    m_pSindragosa->SetScale(matS);
    m_pSindragosa->SetActive(true);
    m_pSindragosa->IdleAnim();
    
}
