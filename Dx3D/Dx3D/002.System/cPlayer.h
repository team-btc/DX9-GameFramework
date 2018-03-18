#pragma once
#include "003.Object\cCharacterObject.h"

#define Speed 0.3f

class cPlayer : public cCharacterObject
{
private:

    vector<cMonster*>* m_vecMonster;

    LPMESH m_pTerrain;

    Vector3 DestPoint;

    bool isMoveToPoint;
    bool isPoint;
    bool isPickMonster;
    bool isMove;

public:
    cPlayer(string szKey, string szFolder, string szFilename);
    cPlayer(string szKey);
    cPlayer();
    ~cPlayer();

    void Setup();
    void Update();
    void Render();
    //void Destroy();

    void SetMoveToPoint(bool MoveToPoint) { isMoveToPoint = MoveToPoint; }
    void SetDestPoint(Vector3 Dest) { DestPoint = Dest; }
    void SetVecMonster(vector<cMonster*>* monster) { m_vecMonster = monster; }
    void SetMove(bool Move) { isMove = Move; }
    void SetTerrain(LPMESH Terrain) { m_pTerrain = Terrain; }

    bool GetMoveToPoint() { return isMoveToPoint; }
    bool GetMove() { return isMove; }
};

