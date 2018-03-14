#include "stdafx.h"
#include "cMapDataManager.h"


cMapDataManager::cMapDataManager()
{
}


cMapDataManager::~cMapDataManager()
{
}

void cMapDataManager::LoadMapData()
{
    // 텍스쳐 이미지 로드 하기
    g_pTextureManager->AddTexture("Soil.jpg", "Asste/Texture/Terrain/Soil.jpg");
    g_pTextureManager->AddTexture("Grass.jpg", "Asste/Texture/Terrain/Grass.jpg");
    g_pTextureManager->AddTexture("Stone.jpg", "Asste/Texture/Terrain/Stone.jpg");
    g_pTextureManager->AddTexture("Cement.jpg", "Asste/Texture/Terrain/Cement.jpg");
    g_pTextureManager->AddTexture("DryMud.jpg", "Asste/Texture/Terrain/DryMud.jpg");

    // 물 이미지 로드 하기
    g_pTextureManager->AddTexture("Water.jpg", "Asste/Texture/Water/Water.jpg");

    // 하늘 이미지 로드 하기
    g_pTextureManager->AddTexture("Cloudy.dds", "Asste/Texture/Sky/Cloudy.dds");
    g_pTextureManager->AddTexture("DeepNight.dds", "Asste/Texture/Sky/DeepNight.dds");
    g_pTextureManager->AddTexture("Interstellar.dds", "Asste/Texture/Sky/Interstellar.dds");
    g_pTextureManager->AddTexture("Isiands.dds", "Asste/Texture/Sky/Isiands.dds");
    g_pTextureManager->AddTexture("MidNight.dds", "Asste/Texture/Sky/MidNight.dds");
    g_pTextureManager->AddTexture("SkyBlue.dds", "Asste/Texture/Sky/SkyBlue.dds");

    // json 파일 로드 해오기 -> x파일, png파일 로드?
    ifstream i;
    json jLoad;
    // == 테스트맵 ==========
    i.open("Asste/Map/TestMap/TestMap.json");
    jLoad << i;
    i.close();
    m_mMapData.insert(make_pair("TestMap", jLoad));
    // 매쉬 로드
    m_mMesh.insert(make_pair("TestMap", LPMESH(NULL)));
    D3DXLoadMeshFromX("Asste/Map/TestMap/TestMap.x", NULL, g_pDevice, NULL, NULL, NULL, NULL, &m_mMesh["TestMap"]);
    jLoad.clear();
    // == 뉴맵 ==========
    i.open("Asste/Map/NewMap/NewMap.json");
    jLoad << i;
    i.close();
    m_mMapData.insert(make_pair("NewMap", jLoad));
    // 매쉬 로드
    m_mMesh.insert(make_pair("NewMap", LPMESH(NULL)));
    D3DXLoadMeshFromX("Asste/Map/NewMap/NewMap.x", NULL, g_pDevice, NULL, NULL, NULL, NULL, &m_mMesh["NewMap"]);
    jLoad.clear();
}

void cMapDataManager::Destroy()
{
    // 매쉬지우기
    // SAFE_RELEASE(m_mMesh[]->second);
}
