#pragma once
#include "cCharacterObject.h"

class cMonster : public cCharacterObject
{
private:
    Vector3     m_vecStartPoint;
    float       m_fMoveRadius;
    float       m_fMoveCount;
    float       m_fScale;
public:
    cMonster(string szKey, string szFolder, string szFilename);
    cMonster(string szKey);
    cMonster();
    ~cMonster();

    void Setup();
    void Update();
    void Render();
   
    virtual ULONG Release();

    void SetStartPoint(Vector3 startpos) { m_vecStartPoint = startpos;
                                            m_vPosition = startpos; }
};

