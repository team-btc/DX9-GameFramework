#include "stdafx.h"
#include "cMapLoad.h"
#include "cMapLoader.h"
#include "cGameMap.h"
#include "002.Shader\cTextureShader.h"
#include "002.Shader\cSkyBoxShader.h"
#include "002.Shader\cWaveShader.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cFrustum.h"

cMapLoad::cMapLoad()
    : m_stMapInfo(NULL)
    , m_pGameMap(NULL)
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
    SAFE_DELETE(m_pGameMap);
}

HRESULT cMapLoad::Start()
{
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.LoadMap("badland");

        m_stMapInfo = g_pMapManager->GetCurrMapInfo();
        
        // ���� �� ����
        m_pGameMap = new cGameMap;
        m_pGameMap->SetCurrMapInfo(m_stMapInfo);

        // �ؽ��� ���̴� ����
        m_pTextureShader = new cTextureShader;

        m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
        m_pTextureShader->SetBGTexture(m_stMapInfo->arrTextureInfo[0].pTexture, m_stMapInfo->arrTextureInfo[0].fDensity);
        m_pTextureShader->SetTexture1(m_stMapInfo->arrTextureInfo[1].pTexture, m_stMapInfo->arrTextureInfo[1].fDensity);
        m_pTextureShader->SetTexture2(m_stMapInfo->arrTextureInfo[2].pTexture, m_stMapInfo->arrTextureInfo[2].fDensity);
        m_pTextureShader->SetTexture3(m_stMapInfo->arrTextureInfo[3].pTexture, m_stMapInfo->arrTextureInfo[3].fDensity);
        m_pTextureShader->SetAlphaDraw(m_stMapInfo->pTextureMap);

        // ��ī�̹ڽ� ���̴� ����
        m_pSkyBoxShader = new cSkyBoxShader;
        m_pSkyBoxShader->SetBox(m_stMapInfo->pSkyTexture);

        // ���� �����ϸ� ���̺� ���̴� ����
        if (m_stMapInfo->isEnableWater)
        {
            m_pWaveShader = new cWaveShader;
            m_pWaveShader->SetMesh(m_stMapInfo->pWaterMesh);
            m_pWaveShader->SetWaveTexture(m_stMapInfo->pWaterTexture);
            m_pWaveShader->SetShader(m_stMapInfo->fWaterHeight, m_stMapInfo->fWaterWaveHeight, m_stMapInfo->fWaterHeightSpeed,
                m_stMapInfo->fWaterUVSpeed, m_stMapInfo->fWaterfrequency, m_stMapInfo->fWaterTransparent);
        }
    }

    m_pFrustum = new cFrustum;
    g_pAutoReleasePool->AddObject(m_pFrustum);
    m_pFrustum->Setup();

    m_pPlayer = g_pCharacterManager->GetPlayer();
    m_pPlayer->SetPosition(m_stMapInfo->vStartPos);

    m_vecMonster = new vector<cMonster*>;
    for (int i = 0; i < 1; i++)
    {
        cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
        m_pEnermy->SetPosition(m_stMapInfo->vStartPos);
        m_pEnermy->SetActive(true);
        (*m_vecMonster).push_back(m_pEnermy);
    }

    m_pPlayer->SetVecMonster(m_vecMonster);
    m_pPlayer->SetTerrain(m_stMapInfo->pTerrainMesh);

    return S_OK;
}

HRESULT cMapLoad::Update()
{
    m_pFrustum->Update();

    m_pPlayer->Update();

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        (*iter)->Update();
    }

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end();)
    {
        if ((*iter)->GetAlive())
        {
            iter++;
        }
        else
        {
            iter = (*m_vecMonster).erase(iter);
        }
    }

    if (m_pPlayer->GetMove())
    {
        cRay ray;
        ray.m_vOrg = m_pPlayer->GetPosition();
        ray.m_vDir = m_pPlayer->GetDir();
        // ���鿡 ��ֹ��� ���ų�, �̵� ���� �Ÿ����� �հ��� ��ֹ��� ������
        float fDist = FLT_MAX;
        if (m_pGameMap->CheckObstacle(fDist, ray) == true
            || fDist < 3.0f)
        {
            m_pPlayer->SetMoveToPoint(false);
            m_pPlayer->SetMoveToTarget(false);
        }
    }
    

    // ��ġ üũ
    Vector3 Pos = m_pPlayer->GetPosition();
    m_pGameMap->GetHeight(Pos);
    m_pPlayer->SetPosition(Pos);

    // �̺�Ʈ üũ
    string szEventName = "";
    if (m_pGameMap->CheckEvent(szEventName, m_pPlayer->GetPosition()))
    {
        // �̺�Ʈ �ߵ�
    }
#ifdef _DEBUG

    //g_pLogManager->WriteLog(EL_INFO, szEventName);

#endif // _DEBUG
    
   /* Matrix4 matR, matT;
    D3DXMatrixRotationY(&matR, m_fRotY);
    D3DXMatrixTranslation(&matT, m_vSpherePos.x, m_vSpherePos.y, m_vSpherePos.z);

    m_matWorld = matR * matT;*/

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

    m_pPlayer->Render();

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        bool result = false;
        m_pFrustum->IsInFrustum(result, &(*iter)->GetSphere());
        if (result)
            (*iter)->Render();
    }

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    for (int i = 0; i < m_stMapInfo->vecObjectInfo.size(); ++i)
    {
        g_pDevice->SetTransform(D3DTS_WORLD, &m_stMapInfo->vecObjectInfo[i].matWorld);
        //m_stMapInfo->vecObjectInfo[i].pMesh->DrawSubset(0);
    }

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    //g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    //g_pDevice->SetRenderState(D3DRS_ZENABLE, true);

#ifdef _DEBUG

    // ��ֹ�, �̺�Ʈ Ʈ�� ���
    m_pGameMap->RendObstacle();
    m_pGameMap->RendEventTrap();

#endif // _DEBUG

    return S_OK;
}
