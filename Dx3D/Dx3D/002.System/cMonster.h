#pragma once
#include "cCharacterObject.h"

class cMonster : public cCharacterObject
{
private:

public:
    cMonster(string szKey, string szFolder, string szFilename);
    cMonster(string szKey);
    cMonster();
    ~cMonster();

    void Setup();
    void Update();
    void Render();
    //void Destroy();
};

