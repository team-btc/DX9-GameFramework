#pragma once
#include "cObject.h"

class cGroup;
class cMtlTex;

class cObjLoader : public cObject
{
private:
    map<string, cMtlTex*> m_mapMtlTex;

    void LoadMtlLib(char* szFilePath);

public:
    cObjLoader();
    ~cObjLoader();

    void Load(IN char* szFilePath, IN Matrix4* pMat,
              OUT vector<cGroup*>& vecGroup);
    LPMESH LoadMesh(IN char* szFilePath, IN Matrix4* pMat,
                        OUT vector<cMtlTex*>& vecMtlTex);
};

