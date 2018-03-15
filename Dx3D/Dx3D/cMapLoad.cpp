#include "stdafx.h"
#include "cMapLoad.h"
#include "cMapLoader.h"
#include "002.Shader\cTextureShader.h"
#include "002.Shader\cSkyBoxShader.h"
#include "002.Shader\cWaveShader.h"

cMapLoad::cMapLoad()
    : m_stMapInfo(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
{
}


cMapLoad::~cMapLoad()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
}

HRESULT cMapLoad::Start()
{
    cMapLoader mapLoader; 
    mapLoader.LoadMap("magam");

    m_stMapInfo = g_pMapManager->GetCurrMapInfo();

    //// 텍스쳐 쉐이더 셋팅
    //m_pTextureShader = new cTextureShader;

    //m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
    //m_pTextureShader->SetTexture1(m_stMapInfo->pTerTexture1, m_stMapInfo->fTex1Density);
    //m_pTextureShader->SetTexture2(m_stMapInfo->pTerTexture2, m_stMapInfo->fTex2Density);
    //m_pTextureShader->SetTexture3(m_stMapInfo->pTerTexture3, m_stMapInfo->fTex3Density);
    //m_pTextureShader->SetAlphaDraw(m_stMapInfo->pTextureMap);

    //// 스카이박스 쉐이더 셋팅
    //m_pSkyBoxShader = new cSkyBoxShader;
    //m_pSkyBoxShader->SetBox(m_stMapInfo->pSkyTexture);

    //// 물이 존재하면 웨이브 쉐이더 셋팅
    //if (m_stMapInfo->isEnableWater)
    //{
    //    m_pWaveShader = new cWaveShader;
    //    m_pWaveShader->SetMesh(m_stMapInfo->pWaterMesh);
    //    m_pWaveShader->SetWaveTexture(m_stMapInfo->pWaterTexture);
    //    m_pWaveShader->SetShader(m_stMapInfo->fWaterHeight, m_stMapInfo->fWaterWaveHeight, m_stMapInfo->fWaterHeightSpeed,
    //        m_stMapInfo->fWaterUVSpeed, m_stMapInfo->fWaterfrequency, m_stMapInfo->fWaterTransparent);
    //}

    // 라이트
    Vector3 dir(1.0f, -1.0f, 0.0f);
    D3DXVec3Normalize(&dir, &dir);
    LIGHT9 stLight = g_pLightManager->InitDirectional(&dir, &WHITE);
    g_pDevice->SetLight(0, &stLight);

    return S_OK;
}

HRESULT cMapLoad::Update()
{


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

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    g_pDevice->SetRenderState(D3DRS_ZENABLE, true);
    
    return S_OK;
}
