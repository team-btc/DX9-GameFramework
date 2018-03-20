#pragma once
#include "cCharacterObject.h"

class cMonster : public cCharacterObject
{
private:
    Vector3     m_vStartPoint;
    Vector3     m_vDest;
    float       m_fMoveRadius;
    float       m_fMoveCount;
    float       m_fAggroTime;

public:
    cMonster(string szKey, string szFolder, string szFilename);
    cMonster(string szKey);
    cMonster();
    ~cMonster();

    void Setup();
    void Update();
    void Render();
    virtual ULONG Release();

    void SetStartPoint(Vector3 startpos) { m_vStartPoint = startpos;
                                            m_vPosition = startpos; }
    void SetAggroTime(float Aggro) { m_fAggroTime = Aggro; }

};

