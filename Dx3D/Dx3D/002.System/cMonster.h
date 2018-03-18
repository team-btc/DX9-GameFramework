#pragma once
#include "cCharacterObject.h"

class cMonster : public cCharacterObject
{
private:
    Vector3     m_vecStartPoint;
    float       m_fMoveRadius;
    float       m_fMoveCount;

public:
    cMonster(string szKey, string szFolder, string szFilename);
    cMonster(string szKey);
    cMonster();
    ~cMonster();

    void Setup();
    void Update();
    void Render();
    //void Destroy();

    void SetStartPoint(Vector3 startpos) { m_vecStartPoint = startpos;
                                            m_vPosition = startpos; }
};

