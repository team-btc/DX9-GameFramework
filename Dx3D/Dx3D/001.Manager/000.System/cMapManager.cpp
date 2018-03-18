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

void cMapManager::SetMapInfo(string szKey, ST_MAP_INFO* stmap)
{
    m_mapMapInfo[szKey] = stmap;

    // ���� �� ����
    SetCurrMap(szKey);
}

ST_MAP_INFO* cMapManager::GetCurrMapInfo()
{
    ST_MAP_INFO* stMap = NULL;

    if (m_mapMapInfo.find(m_szCurrMapKey) != m_mapMapInfo.end())
    {
        stMap = m_mapMapInfo[m_szCurrMapKey];
    }

    return stMap;
}

bool cMapManager::IsLoadMapInfo(string szKey)
{
    bool isHave = false;

    if (m_mapMapInfo.find(szKey) != m_mapMapInfo.end())
    {
        isHave = true;
    }

    return isHave;
}

void cMapManager::Destroy()
{
    // �Ŵ����� �ִ� ������̶� �Ŵ����� ��Ʈ���� �� �� ����Ʈ ��.
    for (auto iter = m_mapMapInfo.begin(); iter != m_mapMapInfo.end(); )
    {
        auto objIter = iter->second->vecObjectInfo.begin();
        for (; objIter < iter->second->vecObjectInfo.end(); objIter++)
        {
            while (objIter->pMesh->Release());
        }
        SAFE_DELETE(iter->second);
        iter = m_mapMapInfo.erase(iter);
    }
}
