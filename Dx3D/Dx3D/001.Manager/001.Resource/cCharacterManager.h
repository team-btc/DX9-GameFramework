#pragma once

#define g_pCharacterManager cCharacterManager::GetInstance()
#define limitMonster 10

class cPlayer;
class cMonster;

class cCharacterManager
{
    SINGLETON(cCharacterManager);

private:

    cPlayer * m_pPlayer;
    //list<cMonster*>     m_listMonster;
    vector<cMonster*>     m_vMonster;

public:

    void Setup();
    void Destroy();

    void PushMonster(cMonster* monster);

    cPlayer* GetPlayer() { return m_pPlayer; }
    cMonster* GetMonster(string szMap);
};

