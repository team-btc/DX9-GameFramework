#pragma once
#include "003.Object\cCharacterObject.h"

#define AggroTime 10.0f

class cPlayer : public cCharacterObject
{
private:

    vector<iCharacterObject*>* m_vecMonster;

    LPMESH m_pTerrain;

    Vector3 DestPoint;

    float m_fScale;
    float m_fCenter;

    bool m_isMoveToPoint;
    bool m_isPoint;
    bool m_isPickMonster;
    bool m_isMouse;

public:
    cPlayer(string szKey, string szFolder, string szFilename);
    cPlayer(string szKey);
    cPlayer();
    ~cPlayer();

    void Setup();
    void Update();
    void Render();
    
    void PickMonster(cRay ray);
    void PickGround(cRay ray);
    void Attack();
    void Move();

    void SetMoveToPoint(bool MoveToPoint) { m_isMoveToPoint = MoveToPoint; }
    void SetDestPoint(Vector3 Dest) { DestPoint = Dest; }
    void SetVecMonster(vector<iCharacterObject*>* monster) { m_vecMonster = monster; }
    void SetTerrain(LPMESH Terrain) { m_pTerrain = Terrain; }
    void SetLevelToStatus(string szKey, int Level);

    bool GetMoveToPoint() { return m_isMoveToPoint; }

    ULONG Release();
};

