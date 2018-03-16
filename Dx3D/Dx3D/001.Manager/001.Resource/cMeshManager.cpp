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

void cMeshManager::LoadMesh(string szKey, string szPath)
{
    LPMESH* mesh = new LPMESH;
    D3DXLoadMeshFromXA(szPath.c_str(), NULL, g_pDevice, NULL, NULL, NULL, NULL, mesh);
    m_mapMesh.insert(make_pair(szKey, mesh));
}

void cMeshManager::AddMesh(string szKey, LPMESH* mesh)
{
    m_mapMesh.insert(make_pair(szKey, mesh));
}

LPMESH cMeshManager::GetMesh(string szKey)
{
    LPMESH mesh = NULL;

    if (m_mapMesh.find(szKey) != m_mapMesh.end())
    {
        mesh = *m_mapMesh[szKey];
    }

    return mesh;
}

LPMESH cMeshManager::GetBasicMesh(string szKey)
{
    LPMESH mesh = NULL;

    if (m_mapBasicMesh.find(szKey) != m_mapBasicMesh.end())
    {
        mesh = *m_mapBasicMesh[szKey];
    }

    return mesh;
}

void cMeshManager::LoadSkinnedMesh()
{
}

cSkinnedMesh* cMeshManager::GetSkinnedMesh(string szKey)
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

cSkinnedMesh* cMeshManager::GetSkinnedMesh(string szKey, string szDirectory, string szFilepath)
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
    for (auto iter = m_mapBasicMesh.begin(); iter != m_mapBasicMesh.end();)
    {
        SAFE_RELEASE(*iter->second);
        iter = m_mapBasicMesh.erase(iter);
    }

    for (auto iter = m_mapMesh.begin(); iter != m_mapMesh.end();)
    {
        SAFE_RELEASE(*iter->second);
        iter = m_mapMesh.erase(iter);
    }

    for (auto iter = m_mapSkinnedMesh.begin(); iter != m_mapSkinnedMesh.end();)
    {
        iter->second->Destroy();
        SAFE_RELEASE(iter->second);
        iter = m_mapSkinnedMesh.erase(iter);
    }
}
