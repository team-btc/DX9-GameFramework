#pragma once

#define g_pMapManager cMapManager::GetInstance()

class cMapManager
{
    SINGLETON(cMapManager);

private:
    map<string, ST_MAP*>            m_mapMapInfo;

    string                          m_szCurrMapKey;

public:
    // == ╪бем ==
    void SetMapInfo(string szKey, ST_MAP* stmap);
    void SetCurrMap(string szKey);

    // == ╟ыем ==
    ST_MAP* GetCurrMapInfo();
    bool IsLoadMapInfo(string szKey);

    void Destroy();
};

