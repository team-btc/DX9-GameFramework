#pragma once
#include "cCharacterObject.h"

class cSindragosa : public cCharacterObject
{
private:
    Vector3     m_vStartPoint;
    Vector3     m_vDest;
    float       m_fMoveRadius;
    float       m_fMoveCount;
    float       m_fScale;
    float       m_fAggroTime;
    bool        m_isMove;
    float       m_fRotation;
    Vector3     m_vRotation;

public:
    cSindragosa(string szKey, string szFolder, string szFilename);
    cSindragosa(string szKey);
    cSindragosa();
    ~cSindragosa();

    void Setup();
    void Update();
    void Render();

    virtual ULONG Release();

    void SetRotation(Vector3 v) { m_vRotation = v; }
    void SetStartPoint(Vector3 startpos) {
        m_vStartPoint = startpos;
        m_vPosition = startpos;
    }
    void SetMove(bool move) { m_isMove = move; }
    void SetAggroTime(float Aggro) { m_fAggroTime = Aggro; }

    bool GetMove() { return m_isMove; }
    
};

