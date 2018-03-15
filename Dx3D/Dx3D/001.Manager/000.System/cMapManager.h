#pragma once

#define g_pMapManager cMapManager::GetInstance()

class cMapManager
{
    SINGLETON(cMapManager);

private:
    map<string, ST_MAP*>            m_mapMapInfo;

    string                          m_szCurrMapKey;

private:
    void SetCurrMap(string szKey);

public:
    // == ╪бем ==
    void SetMapInfo(string szKey, ST_MAP* stmap);

    // == ╟ыем ==
    ST_MAP* GetCurrMapInfo();

    void Destroy();
};

