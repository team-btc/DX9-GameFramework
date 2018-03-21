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
    cPlayer* m_pPlayer;
    cBoss*  m_pBoss;
    vector<iCharacterObject*>     m_vMonster;

public:

    void Setup();
    void Destroy();

    void PushMonster(iCharacterObject* monster);
    void PushBoss(cBoss* boss) { m_pBoss = boss; }

    cPlayer* GetPlayer() { return m_pPlayer; }

    json GetPlayerData();
    void SetPlayerData(json playerData);

    cMonster* GetMonster(string szMap);
    cBoss*  GetBoss();
    int     GetVectorSize() { return (int)m_vMonster.size(); }
};

