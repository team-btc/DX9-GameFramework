#pragma once
#include "cCharacterObject.h"
#define AggroTime 10.0f

class cPlayer : public cCharacterObject
{
private:
    vector<cMonster*>* m_vecMonster;

    LPMESH m_pTerrain;

    Vector3 DestPoint;

    bool m_isMoveToPoint;
    bool m_isPoint;
    bool m_isPickMonster;
    bool m_isMove;

public:
    cPlayer(string szKey, string szFolder, string szFilename);
    cPlayer(string szKey);
    cPlayer();
    ~cPlayer();

    void Setup();
    void Update();
    void Render();
    ULONG Release() override;

    void SetMoveToPoint(bool MoveToPoint) { m_isMoveToPoint = MoveToPoint; }
    void SetDestPoint(Vector3 Dest) { DestPoint = Dest; }
    void SetVecMonster(vector<cMonster*>* monster) { m_vecMonster = monster; }
    void SetMove(bool Move) { m_isMove = Move; }
    void SetTerrain(LPMESH Terrain) { m_pTerrain = Terrain; }

    bool GetMoveToPoint() { return m_isMoveToPoint; }
    bool GetMove() { return m_isMove; }

    void GetSwordMatrix(Matrix4& mat);
};

