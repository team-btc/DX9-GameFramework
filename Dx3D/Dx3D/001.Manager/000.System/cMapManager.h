#pragma once

#define g_pMapManager cMapManager::GetInstance()

class cMapManager
{
    SINGLETON(cMapManager);

private:
    map<string, ST_MAP*>            m_mapMapInfo;

    string                          m_szCurrMapKey;

public:
    // == ���� ==
    void SetMapInfo(string szKey, ST_MAP* stmap);
    void SetCurrMap(string szKey);

    // == ���� ==
    ST_MAP* GetCurrMapInfo();
    bool IsLoadMapInfo(string szKey);

    void Destroy();
};

