#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"

cMainGame::cMainGame()
    : m_pCamera(NULL)
    , m_pEffect(NULL)
    , m_pMesh(NULL)
    , m_pDiffuseMap(NULL)
    , m_pSpecularMap(NULL)
{
    g_pLogManager->Setup("\\Log\\");
    g_pKeyManager->Setup();
    SetCursorPos(W_WIDTH / 2, W_HEIGHT / 2);
}


cMainGame::~cMainGame()
{
    SAFE_RELEASE(m_pMesh);
    SAFE_RELEASE(m_pEffect);

    SAFE_DELETE(m_pCamera);

    g_pFontManager->Destory();
    g_pTextureManager->Destroy();
    g_pMeshManager->Destroy();

    g_pAutoReleasePool->Drain();
    g_pObjectManager->Destory();
    g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
    srand((int)time(NULL));

    Vector3 dir(1.0f, -1.0f, 0.0f);
    D3DXVec3Normalize(&dir, &dir);
    XColor c = WHITE;
    LIGHT9 stLight = InitDirectional(&dir, &c);

    g_pDevice->SetLight(0, &stLight);
    g_pDevice->LightEnable(0, false);
    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    m_pCamera = new cCamera;
    m_pCamera->Setup();

    if (FAILED(D3DXLoadMeshFromXA("./Shader/Model/Sphere.x",
                                  D3DXMESH_SYSTEMMEM,
                                  g_pDevice,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  &m_pMesh)))
    {
        OutputDebugStringA("모델 로딩 실패: ");
    };

    g_pShaderManager->AddEffect("specular-mapping", "./Shader/FX/SpecularMapping.fx");
    m_pEffect = g_pShaderManager->GetEffect("specular-mapping");

    g_pTextureManager->AddTexture("diffuse-map", "./Shader/Texture/Fieldstone_DM.tga");
    g_pTextureManager->AddTexture("specular-map", "./Shader/Texture/Fieldstone_SM.tga");
    m_pDiffuseMap = g_pTextureManager->GetTexture("diffuse-map");
    m_pSpecularMap = g_pTextureManager->GetTexture("specular-map");
}

void cMainGame::Update()
{
    if (m_pCamera)
    {
        m_pCamera->Update();
    }

    Matrix4 matWorld, matView, matProjection;
    Vector4 gWorldLightPosition(500.0f, 500.0f, -500.0f, 1.0f);
    Vector4 gWorldCameraPosition(0.0f, 0.0f, -200.0f, 1.0f);

    D3DXMatrixIdentity(&matWorld);
    g_pDevice->GetTransform(D3DTS_VIEW, &matView);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
    m_pEffect->SetMatrix("gWorldMatrix", &matWorld);
    m_pEffect->SetMatrix("gViewMatrix", &matView);
    m_pEffect->SetMatrix("gProjectionMatrix", &matProjection);
    m_pEffect->SetVector("gWorldCameraPosition", &gWorldCameraPosition);
    m_pEffect->SetVector("gWorldLightPosition", &gWorldLightPosition);

    Vector4 gLightColor(0.7f, 0.7f, 1.0f, 1.0f);
    m_pEffect->SetVector("gLightColor", &gLightColor);
    m_pEffect->SetTexture("DiffuseMap_Tex", m_pDiffuseMap);
    m_pEffect->SetTexture("SpecularMap_Tex", m_pSpecularMap);
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

    UINT numPasses = 0;
    m_pEffect->Begin(&numPasses, NULL);
    {
        for (UINT i = 0; i < numPasses; ++i)
        {
            m_pEffect->BeginPass(i);
            {
                if (m_pMesh)
                {
                    m_pMesh->DrawSubset(0);
                }
            }
            m_pEffect->EndPass();
        }
    }
    m_pEffect->End();
    
    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}