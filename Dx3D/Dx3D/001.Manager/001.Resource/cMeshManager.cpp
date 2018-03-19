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
    if (m_mapMesh.find(szKey) == m_mapMesh.end())
    {
        LPMESH* mesh = new LPMESH;
        D3DXLoadMeshFromXA(szPath.c_str(), NULL, g_pDevice, NULL, NULL, NULL, NULL, mesh);
        m_mapMesh.insert(make_pair(szKey, mesh));
    }
}

void cMeshManager::AddMesh(string szKey, LPMESH* mesh)
{
    if (m_mapMesh.find(szKey) != m_mapMesh.end())
    {
        SAFE_RELEASE(*m_mapMesh[szKey]);
    }

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

void cMeshManager::LoadStaticMesh(string szKey, string szPath)
{
    if (m_mapStaticMesh.find(szKey) == m_mapStaticMesh.end())
    {
        LPMESH mesh;
        HRESULT hr = D3DXLoadMeshFromXA(szPath.c_str(), D3DXMESH_32BIT | D3DXMESH_IB_MANAGED,
            g_pDevice, NULL, NULL, NULL, NULL, &mesh);
        m_mapStaticMesh.insert(make_pair(szKey, mesh));
    }
}

LPMESH cMeshManager::GetStaticMesh(string szKey)
{
    LPMESH mesh = NULL;

    if (m_mapStaticMesh.find(szKey) != m_mapStaticMesh.end())
    {
        mesh = m_mapStaticMesh[szKey];
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
    cSkinnedMesh* pNewMesh = new cSkinnedMesh;
    pNewMesh->Load("Assets\\Player\\ArthasLichking", "arthaslichking.X");
    if (GetJson("arthaslichking") != NULL)
    {
        pNewMesh->LoadJSON("arthaslichking");
    }
    m_mapSkinnedMesh.insert(make_pair("arthaslichking", pNewMesh));

    pNewMesh = new cSkinnedMesh;
    pNewMesh->Load("Assets\\Enemy", "Deathwing.X");
    if (GetJson("Deathwing") != NULL)
    {
        pNewMesh->LoadJSON("Deathwing");
    }
    m_mapSkinnedMesh.insert(make_pair("deathwing", pNewMesh));
}

void cMeshManager::LoadJSON()
{
    json newJson;
    ifstream m_fileJson;
    m_fileJson.open("Assets\\Player\\ArthasLichking\\ArthasLichking.json");
    m_fileJson >> newJson;
    m_mapJson.insert(make_pair("arthaslichking", newJson));
    m_fileJson.close();

    m_fileJson.open("Assets\\Enemy\\Deathwing.json");
    m_fileJson >> newJson;
    m_mapJson.insert(make_pair("deathwing", newJson));
    m_fileJson.close();
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
        SAFE_RELEASE(*iter->second);
        iter = m_mapBasicMesh.erase(iter);
    }

    for (auto iter = m_mapMesh.begin(); iter != m_mapMesh.end();)
    {
        SAFE_RELEASE(*iter->second);
        iter = m_mapMesh.erase(iter);
    }

    for (auto iter = m_mapStaticMesh.begin(); iter != m_mapStaticMesh.end();)
    {
        SAFE_RELEASE(iter->second); // 로드가 수정되면 문제 없는 코드!
        iter = m_mapStaticMesh.erase(iter);
    }

    for (auto iter = m_mapSkinnedMesh.begin(); iter != m_mapSkinnedMesh.end();)
    {
        iter->second->Destroy();
        while (iter->second->Release());
        iter = m_mapSkinnedMesh.erase(iter);
    }
}
