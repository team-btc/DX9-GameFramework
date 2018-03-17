#pragma once
#include "cCharacterObject.h"

class cNonPlayer : public cCharacterObject
{
public:
    cNonPlayer(string szKey);
    cNonPlayer();
    ~cNonPlayer();

    void Setup();
    void Update();
    void Render();
};

