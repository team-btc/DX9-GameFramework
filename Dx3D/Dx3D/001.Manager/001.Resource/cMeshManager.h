#pragma once
#include "cSkinnedMesh.h"

#define g_pMeshManager cMeshManager::GetInstance()

class cMeshManager
{
    friend class cSkinnedMesh;

    SINGLETON(cMeshManager);
    
private:
    map<string, LPMESH*>        m_mapBasicMesh;
    map<string, LPMESH*>        m_mapMesh;
    map<string, cSkinnedMesh*>  m_mapSkinnedMesh;

public:
    HRESULT LoadBasicMesh();
    LPMESH GetBasicMesh(string szKey);

    void LoadMesh(string szKey, string szPath);
    void AddMesh(string szKey, LPMESH* mesh);
    LPMESH GetMesh(string szKey);

    void LoadSkinnedMesh();
    cSkinnedMesh* GetSkinnedMesh(string szKey);
    cSkinnedMesh* GetSkinnedMesh(string szKey, string szDirectory, string szFilepath);
    void Destroy();
};

