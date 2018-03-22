#pragma once

#define g_pMapManager cMapManager::GetInstance()

class cMapManager
{
    SINGLETON(cMapManager);

private:
    map<string, ST_MAP_INFO*>       m_mapMapInfo;

    string                          m_szCurrMapKey;
    string                          m_szPrevMap;

public:
    // == ���� ==
    void SetMapInfo(string szKey, ST_MAP_INFO* stmap);
    void SetCurrMap(string szKey);
    string GetCurrKey() { return m_szCurrMapKey; }

    // == ���� ==
    ST_MAP_INFO* GetCurrMapInfo();
    bool IsLoadMapInfo(string szKey);

    void Destroy();

    void SetPrevMap(string szMap) { m_szCurrMapKey = szMap; }
    string GetPrevMap() { return m_szPrevMap; }

    json GetMapData();
    void SetMapData(json mapData);

    ST_EVENT_INFO GetEventByName(string szEvent);
};

