#pragma once

#define g_pCharacterManager cCharacterManager::GetInstance()
#define limitMonster 20

class cPlayer;
class cMonster;
class cSindragosa;

class cCharacterManager
{
    SINGLETON(cCharacterManager);

private:

    cPlayer*            m_pPlayer;
    list<cMonster*>     m_listMonster;
    cSindragosa * m_pSindragosa;

public:

    void Setup();
    void Destroy();

    void PushMonster(cMonster* monster);

    cPlayer* GetPlayer() { return m_pPlayer; }
    cMonster* GetMonster();
    cSindragosa* GetSindragosa() { return m_pSindragosa; };

    json GetPlayerData();
    void SetPlayerData(json playerData);
};

