#pragma once
#include "cCharacterObject.h"
class cBoss : public cCharacterObject
{
public:
    cBoss(string szKey);
    cBoss();
    ~cBoss();

    void Setup();
    void Update();
    void Render();
    virtual ULONG Release();
};

