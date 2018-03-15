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
    map<int, cMonster*> m_mapMonster;
    list<cMonster*>     m_listMonster;
    int count;

public:

    void Setup();
    void Destroy();

    void PushMonster(cMonster* monster);

    cPlayer* GetPlayer() { return m_pPlayer; }
    cMonster* GetMonster();
};

