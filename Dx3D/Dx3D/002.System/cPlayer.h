#pragma once
#include "003.Object\cCharacterObject.h"

#define Speed 0.01f

class cPlayer : public cCharacterObject
{
private:

    vector<cMonster*>* m_vecMonster;

    Vector3 DestPoint;

    bool isMoveToPoint;
    bool isPoint;
    bool isPickMonster;

public:
    cPlayer(string szKey, string szFolder, string szFilename, string szJsonName);
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

    bool GetMoveToPoint() { return isMoveToPoint; }
};

