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
    , m_vSpherePos(0, 0, 0)
    , m_vDirection(0, 0, 1)
    , m_fRotY(D3DX_PI)
{
    D3DXMatrixIdentity(&m_matWorld);
}


cMapLoad::~cMapLoad()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
}

HRESULT cMapLoad::Start()
{
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.LoadMap("iceCrown");

        m_stMapInfo = g_pMapManager->GetCurrMapInfo();

        // 플레이어 시작 위치 셋팅
        m_vSpherePos = m_stMapInfo->vStartPos;

        // 현재 맵 셋팅
        m_pGameMap = new cGameMap;
        m_pGameMap->SetCurrMapInfo(m_stMapInfo);

        // 텍스쳐 쉐이더 셋팅
        m_pTextureShader = new cTextureShader;

        m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
        m_pTextureShader->SetBGTexture(m_stMapInfo->arrTextureInfo[0].pTexture, m_stMapInfo->arrTextureInfo[0].fDensity);
        m_pTextureShader->SetTexture1(m_stMapInfo->arrTextureInfo[1].pTexture, m_stMapInfo->arrTextureInfo[1].fDensity);
        m_pTextureShader->SetTexture2(m_stMapInfo->arrTextureInfo[2].pTexture, m_stMapInfo->arrTextureInfo[2].fDensity);
        m_pTextureShader->SetTexture3(m_stMapInfo->arrTextureInfo[3].pTexture, m_stMapInfo->arrTextureInfo[3].fDensity);
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

        // 플레이어 대용 구
        m_pSphereMesh = g_pMeshManager->GetBasicMesh("sphere");
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

    if (g_pKeyManager->isStayKeyDown(VK_LEFT))
    {
        m_fRotY -= 0.1f;
    }
    if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
    {
        m_fRotY += 0.1f;
    }
    if (g_pKeyManager->isStayKeyDown(VK_UP))
    {
        cRay ray;
        ray.m_vOrg = m_vSpherePos;
        ray.m_vDir = m_vDirection;
        // 정면에 장애물이 없거나, 이동 예정 거리보다 먼곳에 장애물이 있으면
        float fDist = FLT_MAX;
        if (m_pGameMap->CheckObstacle(fDist, ray) == false
            || fDist > 3.0f)
        {
            m_vSpherePos += m_vDirection;
        }
    }
    if (g_pKeyManager->isStayKeyDown(VK_DOWN))
    {
        cRay ray;
        ray.m_vOrg = m_vSpherePos;
        ray.m_vDir = -m_vDirection;

        // 후면에 장애물이 없거나, 이동 예정 거리보다 먼곳에 장애물이 있으면
        float fDist = FLT_MAX;
        if (m_pGameMap->CheckObstacle(fDist, ray) == false
            || fDist > 3.0f)
        {
            m_vSpherePos -= m_vDirection;
        }
    }

    // 위치 체크
    m_pGameMap->GetHeight(m_vSpherePos);

    // 이벤트 체크
    string szEventName = "";
    if (m_pGameMap->CheckEvent(szEventName, m_vSpherePos))
    {
        // 이벤트 발동
    }
#ifdef _DEBUG

    //g_pLogManager->WriteLog(EL_INFO, szEventName);

#endif // _DEBUG
    
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
    if (m_pSphereMesh)
    {
        g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
        m_pSphereMesh->DrawSubset(0);
    }

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    //g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    //g_pDevice->SetRenderState(D3DRS_ZENABLE, true);

#ifdef _DEBUG

    // 장애물, 이벤트 트랩 출력
    m_pGameMap->RendObstacle();
    m_pGameMap->RendEventTrap();

#endif // _DEBUG

    return S_OK;
}
