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
    json jLoad;
    ifstream iFile;
    iFile.open(MAP_PATH + szKey + "/" + szKey + ".json");
    iFile >> jLoad;
    iFile.close();

    ST_MAP* stMap = new ST_MAP;

    // 지형 매쉬
    g_pMeshManager->LoadMapMesh(szKey);
    stMap->pTerrainMesh = *g_pMeshManager->GetBasicMesh(szKey);

    // 텍스쳐 맵
    g_pTextureManager->AddTexture(szKey, MAP_PATH + szKey + "/" + szKey + ".png");
    stMap->pTextureMap = (LPTEXTURE9)g_pTextureManager->GetTexture(szKey);

    // 텍스쳐
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
    //g_pMeshManager->LoadWaterMesh(szKey + "Water", szKey);
    //stMap->pWaterMesh = *g_pMeshManager->GetBasicMesh(szKey + "Water");
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

    // 맵 매니저에 셋팅 (현재 맵으로 설정됨)
    g_pMapManager->SetMapInfo(szKey, stMap);
}
