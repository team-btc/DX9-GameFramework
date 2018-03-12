#pragma once
#include "003.Object\cCharacterObject.h"

class cPlayer : public cCharacterObject
{
private:

    Vector3 DestPoint;

    bool isMoveToPoint;

public:
    cPlayer(string szKey, string szFolder, string szFilename);
    cPlayer();
    ~cPlayer();

    void Setup();
    void Update();
    void Render();
    void Destroy();

    void SetMoveToPoint(bool MoveToPoint) { isMoveToPoint = MoveToPoint; }
    void SetDestPoint(Vector3 Dest) { DestPoint = Dest; }

    bool GetMoveToPoint() { return isMoveToPoint; }
};

