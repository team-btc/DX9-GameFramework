#pragma once

#define g_pMapDataManager cMapDataManager::GetInstance()

class cMapDataManager
{
    SINGLETON(cMapDataManager);

private:
    map<string, json>       m_mMapData;     // 맵 데이터
    map<string, LPMESH>     m_mMesh;        // 지형 매쉬 데이터

public:

    void LoadMapData();                     // 모든 맵 데이터 로드 해오기
    void Destroy();
};

