#include "stdafx.h"
#include "cMapLoad.h"
#include "cMapLoader.h"
#include "cGameMap.h"
#include "002.Shader\cTextureShader.h"
#include "002.Shader\cSkyBoxShader.h"
#include "002.Shader\cWaveShader.h"

cMapLoad::cMapLoad()
    : m_stMapInfo(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_pSphereMesh(NULL)
    , m_vSpherePos(10, 128, 256)
    , m_vDirection(0, 0, 1)
    , m_fRotY(90.0f / 180.0f * D3DX_PI)
{
    D3DXMatrixIdentity(&m_matWorld);
}


cMapLoad::~cMapLoad()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
    SAFE_RELEASE(m_pSphereMesh);
}

HRESULT cMapLoad::Start()
{
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.LoadMap("iceCrown");

        m_stMapInfo = g_pMapManager->GetCurrMapInfo();

        // 현재 맵 셋팅
        m_pGameMap = new cGameMap;
        m_pGameMap->SetCurrMapMesh(m_stMapInfo->pTerrainMesh);
        m_pGameMap->SetCurrObstacleMesh(m_stMapInfo->vecObstacleMesh);

        // 텍스쳐 쉐이더 셋팅
        m_pTextureShader = new cTextureShader;

        m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
        m_pTextureShader->SetBGTexture(m_stMapInfo->pTerBGTexture, m_stMapInfo->fBGTexDensity);
        m_pTextureShader->SetTexture1(m_stMapInfo->pTerTexture1, m_stMapInfo->fTex1Density);
        m_pTextureShader->SetTexture2(m_stMapInfo->pTerTexture2, m_stMapInfo->fTex2Density);
        m_pTextureShader->SetTexture3(m_stMapInfo->pTerTexture3, m_stMapInfo->fTex3Density);
        m_pTextureShader->SetAlphaDraw(m_stMapInfo->pTextureMap);

        // 스카이박스 쉐이더 셋팅
        m_pSkyBoxShader = new cSkyBoxShader;
        m_pSkyBoxShader->SetBox(m_stMapInfo->pSkyTexture);

        // 물이 존재하면 웨이브 쉐이더 셋팅
        if (m_stMapInfo->isEnableWater)
        {
            m_pWaveShader = new cWaveShader;
            m_pWaveShader->SetMesh(m_stMapInfo->pWaterMesh);
            m_pWaveShader->SetWaveTexture(m_stMapInfo->pWaterTexture);
            m_pWaveShader->SetShader(m_stMapInfo->fWaterHeight, m_stMapInfo->fWaterWaveHeight, m_stMapInfo->fWaterHeightSpeed,
                m_stMapInfo->fWaterUVSpeed, m_stMapInfo->fWaterfrequency, m_stMapInfo->fWaterTransparent);
        }

        D3DXCreateSphere(g_pDevice, 1, 10, 10, &m_pSphereMesh, NULL);
    }

    // 라이트
    Vector3 dir(1.0f, -1.0f, 0.0f);
    D3DXVec3Normalize(&dir, &dir);
    LIGHT9 stLight = g_pLightManager->InitDirectional(&dir, &WHITE);
    g_pDevice->SetLight(0, &stLight);

    return S_OK;
}

HRESULT cMapLoad::Update()
{
    Vector3 vDir(0, 0, 1);
    Matrix4 matRotY;
    D3DXMatrixRotationY(&matRotY, m_fRotY);
    D3DXVec3TransformNormal(&m_vDirection, &vDir, &matRotY);

    Vector3 vMovePos = m_vSpherePos;

    if (g_pKeyManager->isStayKeyDown(VK_UP))
    {
        vMovePos += m_vDirection;
    }
    if (g_pKeyManager->isStayKeyDown(VK_DOWN))
    {
        vMovePos -= m_vDirection;
    }
    if (g_pKeyManager->isStayKeyDown(VK_LEFT))
    {
        m_fRotY -= 0.1f;
    }
    if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
    {
        m_fRotY += 0.1f;
    }

    // 장애물 체크
    cRay ray;
    ray.m_vOrg = vMovePos;
    ray.m_vDir = m_vDirection;
    if (m_pGameMap->CheckObstacle(ray) == false)
    {
        m_vSpherePos = vMovePos;
    }

    // 위치 체크
    m_pGameMap->GetHeight(m_vSpherePos);

    Matrix4 matR, matT;
    D3DXMatrixRotationY(&matR, m_fRotY);
    D3DXMatrixTranslation(&matT, m_vSpherePos.x, m_vSpherePos.y, m_vSpherePos.z);

    m_matWorld = matR * matT;

    return S_OK;
}

HRESULT cMapLoad::Render()
{
    if (!m_stMapInfo || !m_stMapInfo->pTerrainMesh)
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

    // 구 출력
    g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
    m_pSphereMesh->DrawSubset(0);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    g_pDevice->SetRenderState(D3DRS_ZENABLE, true);

    // 장애물 출력
    m_pGameMap->RendObstacle();

    return S_OK;
}
