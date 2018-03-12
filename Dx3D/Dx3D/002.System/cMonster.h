#pragma once
#include "003.Object\cCharacterObject.h"

class cMonster : public cCharacterObject
{
private:

public:
    cMonster(string szKey, string szFolder, string szFilename);
    cMonster();
    ~cMonster();

    void Setup();
    void Update();
    void Render();
    void Destroy();
};

