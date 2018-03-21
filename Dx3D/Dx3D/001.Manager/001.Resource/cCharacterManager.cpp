#include "stdafx.h"
#include "cCharacterManager.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cSindragosa.h"
cCharacterManager::cCharacterManager()
{
}


cCharacterManager::~cCharacterManager()
{
}

void cCharacterManager::Setup()
{
    m_pPlayer = new cPlayer("arthaslichking");
    g_pAutoReleasePool->AddObject(m_pPlayer);

    for (int i = 0; i < limitMonster; i++)
    {
        cMonster* newMonster = new cMonster("Frostwurmnorthlend");
        g_pAutoReleasePool->AddObject(newMonster);
       
        m_listMonster.push_back(newMonster);
    }
  
    //m_pSindragosa = new cSindragosa("Frostwurmnorthlend");
    //g_pAutoReleasePool->AddObject(m_pSindragosa);

}

void cCharacterManager::Destroy()
{
}

void cCharacterManager::PushMonster(cMonster* monster)
{
    m_listMonster.push_back(monster);
}

cMonster* cCharacterManager::GetMonster()
{
    cMonster* returnMonster = m_listMonster.back();
    returnMonster->Setup();
    m_listMonster.pop_back();
    return returnMonster;
}

json cCharacterManager::GetPlayerData()
{
    json jPlayerData;
    ST_STATUS stStatus = m_pPlayer->GetStatus();
    jPlayerData["name"] = stStatus.szName;
    jPlayerData["level"] = stStatus.Level;
    jPlayerData["exp"] = stStatus.nCurEXP;
    jPlayerData["hp"] = stStatus.fCurHP;
    jPlayerData["mp"] = stStatus.fCurMP;
    Vector3 pos = m_pPlayer->GetPosition();
    jPlayerData["pos"]["x"] = pos.x;
    jPlayerData["pos"]["y"] = pos.y;
    jPlayerData["pos"]["z"] = pos.z;

    return jPlayerData;
}

void cCharacterManager::SetPlayerData(json playerData)
{
    ST_STATUS stStatus;
    string szTemp = playerData["name"];
    stStatus.szName = szTemp;
    stStatus.Level = playerData["level"];
    stStatus.nCurEXP = playerData["exp"];
    stStatus.fCurHP = playerData["hp"];
    stStatus.fCurMP = playerData["mp"];
    m_pPlayer->SetStatus(stStatus);

    Vector3 pos;
    pos.x = playerData["pos"]["x"];
    pos.y = playerData["pos"]["y"];
    pos.z = playerData["pos"]["z"];
    m_pPlayer->SetPosition(pos);
}
