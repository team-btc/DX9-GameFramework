#include "stdafx.h"
#include "cMapLoader.h"


cMapLoader::cMapLoader()
{
}

cMapLoader::~cMapLoader()
{
}

void cMapLoader::LoadMap(string szKey)
{
    // 만약에 로드 했던 맵이라면 로드하지 않고 현재 맵으로 셋팅
    if (g_pMapManager->IsLoadMapInfo(szKey))
    {
        g_pMapManager->SetCurrMap(szKey);

        return;
    }

    m_stMapInfo = new ST_MAP_INFO;
    m_szKey = szKey;

    json jLoad;
    ifstream iFile;
    iFile.open(MAP_PATH + szKey + "/" + szKey + ".json");
    iFile >> jLoad;
    iFile.close();
    
    // 지형 매쉬
    LoadMapMesh();

    // 텍스쳐 맵
    LoadTextureMap();

    // 텍스쳐
    LoadTexture(jLoad["texture"]);

    // 물
    LoadWater(jLoad["water"]);
    
    // 하늘
    LoadSky(jLoad["skybox"]);

    // 장애물
    LoadObstacle(jLoad["BLOCK_GROUP"]);

    // 이벤트 -> 여기서 플레이어 시작 위치 셋팅
    LoadEvent(jLoad["EVENT"]);

    // 맵 매니저에 셋팅 (현재 맵으로 설정됨)
    g_pMapManager->SetMapInfo(szKey, m_stMapInfo);
}

void cMapLoader::LoadMapMesh()
{
    g_pMeshManager->LoadMesh(m_szKey, MAP_PATH + m_szKey + "/" + m_szKey + ".x");
    m_stMapInfo->pTerrainMesh = g_pMeshManager->GetMesh(m_szKey);
}

void cMapLoader::LoadTextureMap()
{
    g_pTextureManager->AddTexture(m_szKey, MAP_PATH + m_szKey + "/" + m_szKey + ".png");
    m_stMapInfo->pTextureMap = (LPTEXTURE9)g_pTextureManager->GetTexture(m_szKey);
}

void cMapLoader::LoadTexture(json jTexture)
{
    json jTexBG = jTexture["BGtex"];
    string szBGTex = jTexBG["key"];
    szBGTex = TERRAIN_PATH + szBGTex;
    g_pTextureManager->AddTexture(jTexBG["key"], szBGTex);
    m_stMapInfo->arrTextureInfo[0].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTexBG["key"]);
    m_stMapInfo->arrTextureInfo[0].fDensity = jTexBG["density"];

    json jTex1 = jTexture["tex1"];
    string szTex1 = jTex1["key"];
    szTex1 = TERRAIN_PATH + szTex1;
    g_pTextureManager->AddTexture(jTex1["key"], szTex1);
    m_stMapInfo->arrTextureInfo[1].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTex1["key"]);
    m_stMapInfo->arrTextureInfo[1].fDensity = jTex1["density"];

    json jTex2 = jTexture["tex2"];
    string szTex2 = jTex2["key"];
    szTex2 = TERRAIN_PATH + szTex2;
    g_pTextureManager->AddTexture(jTex2["key"], szTex2);
    m_stMapInfo->arrTextureInfo[2].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTex2["key"]);
    m_stMapInfo->arrTextureInfo[2].fDensity = jTex2["density"];

    json jTex3 = jTexture["tex3"];
    string szTex3 = jTex3["key"];
    szTex3 = TERRAIN_PATH + szTex3;
    g_pTextureManager->AddTexture(jTex3["key"], szTex3);
    m_stMapInfo->arrTextureInfo[3].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTex3["key"]);
    m_stMapInfo->arrTextureInfo[3].fDensity = jTex3["density"];
}

void cMapLoader::LoadWater(json jWater)
{
    g_pMeshManager->LoadMesh(m_szKey + "Water", MAP_PATH + m_szKey + "/" + m_szKey + "Water.x");
    m_stMapInfo->pWaterMesh = g_pMeshManager->GetMesh(m_szKey + "Water");
    string szWaterTex = jWater["file_name"];
    szWaterTex = WATER_PATH + szWaterTex;
    g_pTextureManager->AddTexture(jWater["file_name"], szWaterTex);
    m_stMapInfo->pWaterTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jWater["file_name"]);
    m_stMapInfo->fWaterDensity = jWater["density"];
    m_stMapInfo->isEnableWater = jWater["enable"];
    m_stMapInfo->fWaterHeight = jWater["height"];
    m_stMapInfo->fWaterUVSpeed = jWater["uvspeed"];
    m_stMapInfo->fWaterWaveHeight = jWater["waveheight"];
    m_stMapInfo->fWaterHeightSpeed = jWater["heightspeed"];
    m_stMapInfo->fWaterfrequency = jWater["frequency"];
    m_stMapInfo->fWaterDensity = jWater["transparent"];
}

void cMapLoader::LoadSky(json jSky)
{
    string szSkyTex = jSky["key"];
    szSkyTex = SKY_PATH + szSkyTex;
    g_pTextureManager->AddCubeTexture(jSky["key"], szSkyTex);
    m_stMapInfo->pSkyTexture = (LPCUBETEXTURE9)g_pTextureManager->GetTexture(jSky["key"]);
}

void cMapLoader::LoadObstacle(json jObstacle)
{
    vector<Vector3> vecObstacle;
    // 버텍스
    vector<ST_PC_VERTEX> vecPCVertex;
    // 인덱스
    vector<DWORD> vecIndex;
    // 인덱스 넘버
    int nIndex = 0;
    for (int nBG = 0; nBG < jObstacle.size(); ++nBG)
    {
        for (int nBG_Point = 0; nBG_Point < jObstacle[nBG]["BLOCK_GROUP_POINT"].size(); ++nBG_Point)
        {
            json jBGPoint = jObstacle[nBG]["BLOCK_GROUP_POINT"][nBG_Point];
            Vector3 vPos;
            vPos.x = (float)jBGPoint["BLOCK_POINT_X"];
            vPos.y = (float)jBGPoint["BLOCK_POINT_Y"];
            vPos.z = (float)jBGPoint["BLOCK_POINT_Z"];
            vecObstacle.push_back(vPos);
        }
        for (int i = 0; i < vecObstacle.size() - 1; ++i)
        {
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i].x, 0, vecObstacle[i].z), Color(RED)));       // 0
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i].x, 255, vecObstacle[i].z), Color(RED)));	    // 1
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i + 1].x, 255, vecObstacle[i + 1].z), Color(RED)));	    // 2
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i + 1].x, 0, vecObstacle[i + 1].z), Color(RED)));	    // 3

            vecIndex.push_back(nIndex + 0);
            vecIndex.push_back(nIndex + 1);
            vecIndex.push_back(nIndex + 2);
            vecIndex.push_back(nIndex + 0);
            vecIndex.push_back(nIndex + 2);
            vecIndex.push_back(nIndex + 3);

            vecIndex.push_back(nIndex + 3);
            vecIndex.push_back(nIndex + 2);
            vecIndex.push_back(nIndex + 1);
            vecIndex.push_back(nIndex + 3);
            vecIndex.push_back(nIndex + 1);
            vecIndex.push_back(nIndex + 0);

            nIndex += 4;
        }
    }

    // 장애물 매쉬
    D3DXCreateMeshFVF((DWORD)vecIndex.size() / 3, (DWORD)vecPCVertex.size(),
        D3DXMESH_MANAGED | D3DXMESH_32BIT, ST_PC_VERTEX::FVF, g_pDevice, &m_stMapInfo->pObstacleMesh);

    // 버텍스 버퍼 기록
    Vector3* pV = NULL;
    m_stMapInfo->pObstacleMesh->LockVertexBuffer(NULL, (LPVOID*)&pV);
    memcpy(pV, &vecPCVertex[0], vecPCVertex.size() * sizeof(ST_PC_VERTEX));
    m_stMapInfo->pObstacleMesh->UnlockVertexBuffer();

    // 인덱스 버퍼 기록
    DWORD* pI = NULL;
    m_stMapInfo->pObstacleMesh->LockIndexBuffer(NULL, (LPVOID*)&pI);
    memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
    m_stMapInfo->pObstacleMesh->UnlockIndexBuffer();

    // 속성 버퍼 기록
    DWORD* pA = NULL;
    m_stMapInfo->pObstacleMesh->LockAttributeBuffer(NULL, &pA);
    for (int i = 0; i < vecIndex.size() / 3; ++i) // 페이스별로 하나씩 기록
        pA[i] = (DWORD)0;
    m_stMapInfo->pObstacleMesh->UnlockAttributeBuffer();

    // 메쉬 최적화 : 버텍스 개수 만큼 인접정보를 담을 공간을 확보
    vector<DWORD> vecAdjBuf2(m_stMapInfo->pObstacleMesh->GetNumFaces() * 3);

    m_stMapInfo->pObstacleMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf2[0]);

    m_stMapInfo->pObstacleMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, &vecAdjBuf2[0], 0, 0, 0);

    // 매쉬 매니저에 저장
    g_pMeshManager->AddMesh(m_szKey + "Obstacle", &m_stMapInfo->pObstacleMesh);
}

void cMapLoader::LoadEvent(json jEvent)
{
    for (int i = 0; i < jEvent.size(); ++i)
    {
        ST_EVENT_INFO stEvent;

        string szName = jEvent[i]["EVENT_NAME"];
        stEvent.szName = szName;
        stEvent.vPos.x = jEvent[i]["EVENT_POSITION_X"];
        stEvent.vPos.y = jEvent[i]["EVENT_POSITION_Y"];
        stEvent.vPos.z = jEvent[i]["EVENT_POSITION_Z"];
        stEvent.fRadius = 10.0f;

        // 맵의 Y값에 맞게 설정
        cRay ray;
        ray.m_vOrg = stEvent.vPos;
        ray.m_vOrg.y = 255.0f;
        ray.m_vDir = Vector3(0, -1, 0);
        BOOL isHit = false;
        float fDist;

        D3DXIntersectSubset(m_stMapInfo->pTerrainMesh, NULL, &ray.m_vOrg, &ray.m_vDir, &isHit,
            NULL, NULL, NULL, &fDist, NULL, NULL);

        if (isHit)
        {
            stEvent.vPos.y = 255.0f - fDist;
        }

        // 시작포지션이면 값 넣기
        if (!strcmp(szName.c_str(), "startpos"))
        {
            m_stMapInfo->vStartPos.x = stEvent.vPos.x;
            m_stMapInfo->vStartPos.y = stEvent.vPos.y;
            m_stMapInfo->vStartPos.z = stEvent.vPos.z;
        }

        m_stMapInfo->vecEventInfo.push_back(stEvent);
    }
}