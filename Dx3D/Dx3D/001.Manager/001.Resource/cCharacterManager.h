#pragma once

#define g_pCharacterManager cCharacterManager::GetInstance()
#define limitMonster 20

class cPlayer;
class cMonster;

class cCharacterManager
{
    SINGLETON(cCharacterManager);

private:

    cPlayer * m_pPlayer;
    list<cMonster*>     m_listMonster;

public:

    void Setup();
    void Destroy();

    void PushMonster(cMonster* monster);

    cPlayer* GetPlayer() { return m_pPlayer; }
    cMonster* GetMonster();
};

