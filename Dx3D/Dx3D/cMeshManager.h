#pragma once
#include "cSkinnedMesh.h"

#define g_pMeshManager cMeshManager::GetInstance()

class cMeshManager
{
    friend class cSkinnedMesh;

    SINGLETON(cMeshManager);
    
private:
    map<string, cSkinnedMesh*> m_mapSkinnedMesh;

public:
    void Load();
    cSkinnedMesh* GetMesh(string szKey);
    cSkinnedMesh* GetMesh(string szKey, string szDirectory, string szFilepath);
    void Destroy();
};

