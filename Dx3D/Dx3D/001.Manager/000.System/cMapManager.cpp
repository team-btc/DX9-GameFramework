#include "stdafx.h"
#include "cMapManager.h"


cMapManager::cMapManager()
{
}


cMapManager::~cMapManager()
{
}

void cMapManager::SetCurrMap(string szKey)
{
    m_szCurrMapKey = szKey;
}

void cMapManager::SetMapInfo(string szKey, ST_MAP* stmap)
{
    m_mapMapInfo[szKey] = stmap;

    // 현재 맵 설정
    SetCurrMap(szKey);
}

ST_MAP* cMapManager::GetCurrMapInfo()
{
    ST_MAP* stMap = NULL;

    if (m_mapMapInfo.find(m_szCurrMapKey) != m_mapMapInfo.end())
    {
        stMap = m_mapMapInfo[m_szCurrMapKey];
    }

    return stMap;
}

void cMapManager::Destroy()
{
    // 매니저에 있는 내용들이라 매니저가 디스트로이 될 때 딜리트 됨.
    for (auto iter = m_mapMapInfo.begin(); iter != m_mapMapInfo.end(); )
    {
        SAFE_DELETE(iter->second);
        iter = m_mapMapInfo.erase(iter);
    }
}
