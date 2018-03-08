#include "stdafx.h"
#include "cMeshManager.h"


cMeshManager::cMeshManager()
{
}


cMeshManager::~cMeshManager()
{
}

HRESULT cMeshManager::LoadBasicMesh()
{
    LPMESH* mesh = new LPMESH;
    D3DXCreateBox(g_pDevice, 1.0f, 1.0f, 1.0f, mesh, NULL);
    m_mapBasicMesh.insert(make_pair("cube", mesh));

    mesh = new LPMESH;
    D3DXCreateSphere(g_pDevice, 1.0f, 10, 10, mesh, NULL);
    m_mapBasicMesh.insert(make_pair("sphere", mesh));

    mesh = new LPMESH;
    D3DXCreateTorus(g_pDevice, 1.0f - D3DX_16F_EPSILON, 1.0f, 10, 10, mesh, NULL);
    m_mapBasicMesh.insert(make_pair("torus", mesh));

    return S_OK;
}

LPMESH* cMeshManager::GetBasicMesh(string szKey)
{
    return nullptr;
}

void cMeshManager::LoadSkinnedMesh()
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
        return iter->second;
    }
}

void cMeshManager::Destroy()
{
    for (auto iter = m_mapBasicMesh.begin(); iter != m_mapBasicMesh.end();)
    {
        (*iter->second)->Release();
        iter = m_mapBasicMesh.erase(iter);
    }

    for (auto iter = m_mapSkinnedMesh.begin(); iter != m_mapSkinnedMesh.end();)
    {
        iter->second->Destroy();
        SAFE_RELEASE(iter->second);
        iter = m_mapSkinnedMesh.erase(iter);
    }
}
