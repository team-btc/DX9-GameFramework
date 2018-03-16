#pragma once
#include "cSkinnedMesh.h"

#define g_pMeshManager cMeshManager::GetInstance()

class cMeshManager
{
    friend class cSkinnedMesh;

    SINGLETON(cMeshManager);
    
private:
    map<string, LPMESH*>        m_mapBasicMesh;
    map<string, cSkinnedMesh*>  m_mapSkinnedMesh;
    map<string, json>           m_mapJson;

public:
    HRESULT LoadBasicMesh();
    LPMESH* GetBasicMesh(string szKey);

    void LoadSkinnedMesh();
    void LoadJSON();
    cSkinnedMesh* GetMesh(string szKey);
    cSkinnedMesh* GetMesh(string szKey, string szDirectory, string szFilepath);
    json          GetJson(string szKey);
    json          GetJson(string szKey, string szDirectory, string szFilepath);
    void Destroy();
};

