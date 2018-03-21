#pragma once
#include "cCharacterObject.h"
class cBoss : public cCharacterObject
{
private:
    Vector3 m_vStartPoint;

    float   m_fRoarTime;
    float   m_fScale;
    float   m_fCenter;

    bool m_isRoar;

public:
    cBoss(string szKey);
    cBoss();
    ~cBoss();

    void Setup();
    void Update();
    void Render();
    virtual ULONG Release();

    void Attack();
    void Roar();
    void MoveToTarget();

    void SetStartPoint(Vector3 startpos) {
        m_vStartPoint = startpos;
        m_vPosition = startpos;
    }
    void SetIsRoar(bool Roar) { m_isRoar = Roar; }
    void SetLevelToStatus(string szKey, int Level);

    bool GetIsRoar() { return m_isRoar; }
};

