#include "stdafx.h"
#include "cMeshManager.h"


cMeshManager::cMeshManager()
{
}


cMeshManager::~cMeshManager()
{
}

void cMeshManager::Load()
{
}

cSkinnedMesh* cMeshManager::GetMesh(string szKey)
{
    auto iter = m_mapSkinnedMesh.find(szKey);
    if (iter != m_mapSkinnedMesh.end())
    {
        return iter->second;
    }
    else
    {
        return NULL;
    }
}

cSkinnedMesh* cMeshManager::GetMesh(string szKey, string szDirectory, string szFilepath)
{
    auto iter = m_mapSkinnedMesh.find(szKey);
    if (iter == m_mapSkinnedMesh.end())
    {
        cSkinnedMesh* pNewMesh = new cSkinnedMesh;
        pNewMesh->Load(szDirectory, szFilepath);
        m_mapSkinnedMesh.insert(make_pair(szKey, pNewMesh));
        return m_mapSkinnedMesh[szKey];
    }
    else
    {
        return NULL;
    }
}

void cMeshManager::Destroy()
{
    for (auto iter = m_mapSkinnedMesh.begin(); iter != m_mapSkinnedMesh.end(); iter++)
    {
        iter->second->Destroy();
        SAFE_RELEASE(iter->second);
    }
}
