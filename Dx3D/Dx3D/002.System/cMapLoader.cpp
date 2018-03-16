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

    json jLoad;
    ifstream iFile;
    iFile.open(MAP_PATH + szKey + "/" + szKey + ".json");
    iFile >> jLoad;
    iFile.close();

    ST_MAP* stMap = new ST_MAP;

    // 플레이어 스타트 위치
    stMap->vStartPos.x = jLoad["startPos"]["x"];
    stMap->vStartPos.y = jLoad["startPos"]["y"];
    stMap->vStartPos.z = jLoad["startPos"]["z"];

    // 지형 매쉬
    g_pMeshManager->LoadMesh(szKey, MAP_PATH + szKey + "/" + szKey + ".x");
    stMap->pTerrainMesh = g_pMeshManager->GetBasicMesh(szKey);

    // 텍스쳐 맵
    g_pTextureManager->AddTexture(szKey, MAP_PATH + szKey + "/" + szKey + ".png");
    stMap->pTextureMap = (LPTEXTURE9)g_pTextureManager->GetTexture(szKey);

    // 텍스쳐
    string szBGTex = jLoad["texture"]["BGtex"]["key"];
    szBGTex = TERRAIN_PATH + szBGTex;
    g_pTextureManager->AddTexture(jLoad["texture"]["BGtex"]["key"], szBGTex);
    stMap->pTerBGTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jLoad["texture"]["BGtex"]["key"]);
    stMap->fBGTexDensity = jLoad["texture"]["BGtex"]["density"];
    string szTex1 = jLoad["texture"]["tex1"]["key"];
    szTex1 = TERRAIN_PATH + szTex1;
    g_pTextureManager->AddTexture(jLoad["texture"]["tex1"]["key"], szTex1);
    stMap->pTerTexture1 = (LPTEXTURE9)g_pTextureManager->GetTexture(jLoad["texture"]["tex1"]["key"]);
    stMap->fTex1Density = jLoad["texture"]["tex1"]["density"];
    string szTex2 = jLoad["texture"]["tex2"]["key"];
    szTex2 = TERRAIN_PATH + szTex2;
    g_pTextureManager->AddTexture(jLoad["texture"]["tex2"]["key"], szTex2);
    stMap->pTerTexture2 = (LPTEXTURE9)g_pTextureManager->GetTexture(jLoad["texture"]["tex2"]["key"]);
    stMap->fTex2Density = jLoad["texture"]["tex2"]["density"];
    string szTex3 = jLoad["texture"]["tex3"]["key"];
    szTex3 = TERRAIN_PATH + szTex3;
    g_pTextureManager->AddTexture(jLoad["texture"]["tex3"]["key"], szTex3);
    stMap->pTerTexture3 = (LPTEXTURE9)g_pTextureManager->GetTexture(jLoad["texture"]["tex3"]["key"]);
    stMap->fTex3Density = jLoad["texture"]["tex3"]["density"];

    // 물
    g_pMeshManager->LoadMesh(szKey + "Water", MAP_PATH + szKey + "/" + szKey + "Water.x");
    stMap->pWaterMesh = g_pMeshManager->GetBasicMesh(szKey + "Water");
    string szWaterTex = jLoad["water"]["file_name"];
    szWaterTex = WATER_PATH + szWaterTex;
    g_pTextureManager->AddTexture(jLoad["water"]["file_name"], szWaterTex);
    stMap->pWaterTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jLoad["water"]["file_name"]);
    stMap->fWaterDensity = jLoad["water"]["density"];
    stMap->isEnableWater = jLoad["water"]["enable"];
    stMap->fWaterHeight = jLoad["water"]["height"];
    stMap->fWaterUVSpeed = jLoad["water"]["uvspeed"];
    stMap->fWaterWaveHeight = jLoad["water"]["waveheight"];
    stMap->fWaterHeightSpeed = jLoad["water"]["heightspeed"];
    stMap->fWaterfrequency = jLoad["water"]["frequency"];
    stMap->fWaterDensity = jLoad["water"]["transparent"];

    // 하늘
    string szSkyTex = jLoad["skybox"]["key"];
    szSkyTex = SKY_PATH + szSkyTex;
    g_pTextureManager->AddCubeTexture(jLoad["skybox"]["key"], szSkyTex);
    stMap->pSkyTexture = (LPCUBETEXTURE9)g_pTextureManager->GetTexture(jLoad["skybox"]["key"]);

    // 장애물
    vector<Vector3> vecObstacle;
    for (int nBG = 0; nBG < jLoad["BLOCK_GROUP"].size(); ++nBG)
    {
        for (int nBG_Point = 0; nBG_Point < jLoad["BLOCK_GROUP"][nBG]["BLOCK_GROUP_POINT"].size(); ++nBG_Point)
        {
            Vector3 vPos;
            vPos.x = (float)jLoad["BLOCK_GROUP"][nBG]["BLOCK_GROUP_POINT"][nBG_Point]["BLOCK_POINT_X"];
            vPos.y = (float)jLoad["BLOCK_GROUP"][nBG]["BLOCK_GROUP_POINT"][nBG_Point]["BLOCK_POINT_Y"];
            vPos.z = (float)jLoad["BLOCK_GROUP"][nBG]["BLOCK_GROUP_POINT"][nBG_Point]["BLOCK_POINT_Z"];
            vecObstacle.push_back(vPos);
        }
        for (int i = 0; i < vecObstacle.size() - 1; ++i)
        {
            // 버텍스
            vector<ST_PC_VERTEX> vecPCVertex;
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i].x,        0,      vecObstacle[i].z),      Color(RED)));       // 0
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i].x,        255,    vecObstacle[i].z),      Color(RED)));	    // 1
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i + 1].x,    255,    vecObstacle[i + 1].z),  Color(RED)));	    // 2
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i + 1].x,    0,      vecObstacle[i + 1].z),  Color(RED)));	    // 3

            // 인덱스
            vector<DWORD> vecIndex;

            vecIndex.push_back(0);
            vecIndex.push_back(1);
            vecIndex.push_back(2);
            vecIndex.push_back(0);
            vecIndex.push_back(2);
            vecIndex.push_back(3);

            vecIndex.push_back(3);
            vecIndex.push_back(2);
            vecIndex.push_back(1);
            vecIndex.push_back(3);
            vecIndex.push_back(1);
            vecIndex.push_back(0);

            // 매쉬
            LPMESH* mesh = new LPMESH;

            D3DXCreateMeshFVF((DWORD)vecIndex.size() / 3, (DWORD)vecPCVertex.size(),
                D3DXMESH_MANAGED | D3DXMESH_32BIT, ST_PC_VERTEX::FVF, g_pDevice, mesh);

            // 버텍스 버퍼 기록
            Vector3* pV = NULL;
            (*mesh)->LockVertexBuffer(NULL, (LPVOID*)&pV);
            memcpy(pV, &vecPCVertex[0], vecPCVertex.size() * sizeof(ST_PC_VERTEX));
            (*mesh)->UnlockVertexBuffer();

            // 인덱스 버퍼 기록
            DWORD* pI = NULL;
            (*mesh)->LockIndexBuffer(NULL, (LPVOID*)&pI);
            memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
            (*mesh)->UnlockIndexBuffer();

            // 속성 버퍼 기록
            DWORD* pA = NULL;
            (*mesh)->LockAttributeBuffer(NULL, &pA);
            for (int i = 0; i < vecIndex.size() / 3; ++i) // 페이스별로 하나씩 기록
                pA[i] = (DWORD)0;
            (*mesh)->UnlockAttributeBuffer();

            // 메쉬 최적화 : 버텍스 개수 만큼 인접정보를 담을 공간을 확보
            vector<DWORD> vecAdjBuf2((*mesh)->GetNumFaces() * 3);

            (*mesh)->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf2[0]);

            (*mesh)->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, &vecAdjBuf2[0], 0, 0, 0);

            // 매쉬 저장
            char buffer[10];
            snprintf(buffer, 10, "%d_%d", nBG, i);
            string szMeshKey = szKey + "Obstacle" + buffer;
            g_pMeshManager->AddMesh(szMeshKey, mesh);
            stMap->vecObstacleMesh.push_back(g_pMeshManager->GetBasicMesh(szMeshKey));
        }
    }

    // 맵 매니저에 셋팅 (현재 맵으로 설정됨)
    g_pMapManager->SetMapInfo(szKey, stMap);
}
