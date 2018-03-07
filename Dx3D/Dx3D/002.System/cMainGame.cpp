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

    m_pMeshLoader->Destroy();
    m_pMesh->Release();

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

    D3DXCreateSphere(g_pDevice, 10, 10, 10, &m_pMesh, NULL);
    g_pShaderManager->AddEffect("mesh", ".\\Assets\\Shader\\FX\\MeshFromObj.fx");
    m_pEffect = g_pShaderManager->GetEffect("mesh");

    m_pMeshLoader = new cMeshLoader;
    hr = m_pMeshLoader->Create(g_pDevice, L"Assets\\WatertowerOBJ\\tanariswatertower.obj");


    // Cache the effect handles
    g_hAmbient = m_pEffect->GetParameterBySemantic(0, "Ambient");
    g_hDiffuse = m_pEffect->GetParameterBySemantic(0, "Diffuse");
    g_hSpecular = m_pEffect->GetParameterBySemantic(0, "Specular");
    g_hOpacity = m_pEffect->GetParameterBySemantic(0, "Opacity");
    g_hSpecularPower = m_pEffect->GetParameterBySemantic(0, "SpecularPower");
    g_hLightColor = m_pEffect->GetParameterBySemantic(0, "LightColor");
    g_hLightPosition = m_pEffect->GetParameterBySemantic(0, "LightPosition");
    g_hCameraPosition = m_pEffect->GetParameterBySemantic(0, "CameraPosition");
    g_hTexture = m_pEffect->GetParameterBySemantic(0, "Texture");
    g_hTime = m_pEffect->GetParameterBySemantic(0, "Time");
    g_hWorld = m_pEffect->GetParameterBySemantic(0, "World");
    g_hWorldViewProjection = m_pEffect->GetParameterBySemantic(0, "WorldViewProjection");

    for (UINT i = 0; i < m_pMeshLoader->GetNumMaterials(); i++)
    {
        Material* pMaterial = m_pMeshLoader->GetMaterial(i);

        const char* strTechnique = NULL;

        if (pMaterial->pTexture && pMaterial->bSpecular)
            strTechnique = "TexturedSpecular";
        else if (pMaterial->pTexture && !pMaterial->bSpecular)
            strTechnique = "TexturedNoSpecular";
        else if (!pMaterial->pTexture && pMaterial->bSpecular)
            strTechnique = "Specular";
        else if (!pMaterial->pTexture && !pMaterial->bSpecular)
            strTechnique = "NoSpecular";

        pMaterial->hTechnique = m_pEffect->GetTechniqueByName(strTechnique);
    }
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
        r.x -= 1.0f;
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
    }

    if (g_pKeyManager->isStayKeyDown('A'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.y -= 1.0f;
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('S'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.x += 1.0f;
        m_pCamera->SetRotation(r);
    }

    if (g_pKeyManager->isStayKeyDown('D'))
    {
        Vector3 r = m_pCamera->GetRotation();
        r.y += 1.0f;
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

    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

    Matrix4 mWorld, mView, mProj, mWorldViewProjection;
    g_pDevice->GetTransform(D3DTS_WORLD, &mWorld);
    g_pDevice->GetTransform(D3DTS_VIEW, &mView);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &mProj);

    mWorldViewProjection = mWorld * mView * mProj;

    // Update the effect's variables. 
    m_pEffect->SetMatrix(g_hWorldViewProjection, &mWorldViewProjection);
    m_pEffect->SetMatrix(g_hWorld, &mWorld);
    m_pEffect->SetFloat(g_hTime, g_pTimerManager->GetWorldTime());
    m_pEffect->SetValue(g_hCameraPosition, m_pCamera->GetEye(), sizeof(D3DXVECTOR3));

    UINT iCurSubset = -1;//(UINT)(INT_PTR)g_SampleUI.GetComboBox(IDC_SUBSET)->GetSelectedData();

    // A subset of -1 was arbitrarily chosen to represent all subsets
    if (iCurSubset == -1)
    {
        // Iterate through subsets, changing material properties for each
        for (UINT iSubset = 0; iSubset < m_pMeshLoader->GetNumMaterials(); iSubset++)
        {
            RenderSubset(iSubset);
        }
    }

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::RenderSubset(UINT iSubset)
{
    HRESULT hr;
    UINT iPass, cPasses;

    // Retrieve the ID3DXMesh pointer and current material from the MeshLoader helper
    ID3DXMesh* pMesh = m_pMeshLoader->GetMesh();
    Material* pMaterial = m_pMeshLoader->GetMaterial(iSubset);

    // Set the lighting variables and texture for the current material
    m_pEffect->SetValue(g_hAmbient, pMaterial->vAmbient, sizeof(D3DXVECTOR3));
    m_pEffect->SetValue(g_hDiffuse, pMaterial->vDiffuse, sizeof(D3DXVECTOR3));
    m_pEffect->SetValue(g_hSpecular, pMaterial->vSpecular, sizeof(D3DXVECTOR3));
    m_pEffect->SetTexture(g_hTexture, pMaterial->pTexture);
    m_pEffect->SetFloat(g_hOpacity, pMaterial->fAlpha);
    m_pEffect->SetInt(g_hSpecularPower, pMaterial->nShininess);

    m_pEffect->SetTechnique(pMaterial->hTechnique);
    m_pEffect->Begin(&cPasses, 0);

    for (iPass = 0; iPass < cPasses; iPass++)
    {
        m_pEffect->BeginPass(iPass);

        // The effect interface queues up the changes and performs them 
        // with the CommitChanges call. You do not need to call CommitChanges if 
        // you are not setting any parameters between the BeginPass and EndPass.
        // V( g_pEffect->CommitChanges() );

        // Render the mesh with the applied technique
        pMesh->DrawSubset(iSubset);

        m_pEffect->EndPass();
    }
    m_pEffect->End();
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}