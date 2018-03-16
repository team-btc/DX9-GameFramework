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

    mesh = new LPMESH;
    D3DXLoadMeshFromX(L"Assets\\HeightMapData\\Plane.X", D3DXMESH_MANAGED, g_pDevice, NULL, NULL, NULL, NULL, mesh);
    m_mapBasicMesh.insert(make_pair("map", mesh));

    return S_OK;
}


LPMESH* cMeshManager::GetBasicMesh(string szKey)
{
    auto iter = m_mapBasicMesh.find(szKey);
    if (iter != m_mapBasicMesh.end())
    {
        return iter->second;
    }
    else
    {
        return NULL;
    }
}

void cMeshManager::LoadSkinnedMesh()
{
    cSkinnedMesh* pNewMesh = new cSkinnedMesh;
    pNewMesh->Load("Assets\\Player\\ArthasLichking", "arthaslichking.X");
    m_mapSkinnedMesh.insert(make_pair("arthaslichking", pNewMesh));

    pNewMesh = new cSkinnedMesh;
    pNewMesh->Load("Assets\\Enemy\\Deathwing", "Deathwing.X");
    m_mapSkinnedMesh.insert(make_pair("Deathwing", pNewMesh));
}

void cMeshManager::LoadJSON()
{
    json newJson;
    ifstream m_fileJson;
    m_fileJson.open("Assets\\Player\\ArthasLichking\\arthaslichking.json");
    m_fileJson >> newJson;
    m_mapJson.insert(make_pair("arthaslichking", newJson));
    m_fileJson.close();

    m_fileJson.open("Assets\\Enemy\\Deathwing\\Deathwing.json");
    m_fileJson >> newJson;
    m_mapJson.insert(make_pair("Deathwing", newJson));
    m_fileJson.close();
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

json cMeshManager::GetJson(string szKey)
{
    auto iter = m_mapJson.find(szKey);
    if (iter != m_mapJson.end())
    {
        return iter->second;
    }
    else
    {
        return NULL;
    }
}

json cMeshManager::GetJson(string szKey, string szDirectory, string szFilepath)
{
    auto iter = m_mapJson.find(szKey);
    if (iter == m_mapJson.end())
    {
        json newJson;
        ifstream m_fileJson;
        m_fileJson.open(szDirectory + "\\" + szFilepath);

        m_fileJson >> newJson;
        m_mapJson.insert(make_pair(szKey, newJson));
        m_fileJson.close();
        return m_mapJson[szKey];
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
