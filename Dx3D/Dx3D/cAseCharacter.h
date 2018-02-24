#pragma once
#include "cObject.h"
#include "cFrame.h"

class cAseCharacter : public cObject
{
private:
    ST_ASE_SCENE    m_stScene;
    cFrame*         m_pRootFrame;

public:
    cAseCharacter();
    ~cAseCharacter();

    void Load(char* szFullPath);
    void Update(Matrix4* matWorld = NULL);
    void Render();
};

