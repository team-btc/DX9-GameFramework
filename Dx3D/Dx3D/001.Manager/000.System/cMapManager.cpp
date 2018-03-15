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

    // ���� �� ����
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
    // �Ŵ����� �ִ� ������̶� �Ŵ����� ��Ʈ���� �� �� ����Ʈ ��.
    for (auto iter = m_mapMapInfo.begin(); iter != m_mapMapInfo.end(); )
    {
        SAFE_DELETE(iter->second);
        iter = m_mapMapInfo.erase(iter);
    }
}
