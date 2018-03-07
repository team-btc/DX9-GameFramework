#pragma once
#include "003.Object\cCharacterObject.h"

class cPlayer : public cCharacterObject
{
public :

public:
    cPlayer(string szKey, string szFolder, string szFilename);
    cPlayer();
    ~cPlayer();

    void Setup();
    void Update();
    void Render();
    void Destroy();
};

