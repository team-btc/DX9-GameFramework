#pragma once

#define g_pCharacterManager cCharacterManager::GetInstance()
#define limitMonster 10

class cPlayer;
class cMonster;
class cBoss;
struct iCharacterObject;

class cCharacterManager
{
    SINGLETON(cCharacterManager);

private:

    cPlayer * m_pPlayer;
    cBoss*  m_pBoss;
    vector<iCharacterObject*>     m_vMonster;

public:

    void Setup();
    void Destroy();

    void PushMonster(iCharacterObject* monster);

    cPlayer* GetPlayer() { return m_pPlayer; }
    cMonster* GetMonster(string szMap);
    cBoss*  GetBoss();
};

