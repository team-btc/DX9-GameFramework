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
    bool        m_isMove;
public:
    cMonster(string szKey, string szFolder, string szFilename);
    cMonster(string szKey);
    cMonster();
    ~cMonster();

    void Setup();
    void Update();
    void Render();
    //void Destroy();

    void SetStartPoint(Vector3 startpos) { m_vStartPoint = startpos;
                                            m_vPosition = startpos; }
    void SetMove(bool move) { m_isMove = move; }
    void SetAggroTime(float Aggro) { m_fAggroTime = Aggro; }

    bool GetMove() { return m_isMove; }

};

