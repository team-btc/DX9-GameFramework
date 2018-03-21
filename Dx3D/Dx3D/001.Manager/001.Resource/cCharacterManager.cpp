#include "stdafx.h"
#include "cCharacterManager.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cBoss.h"

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

    m_pBoss = new cBoss("Deathwing");
    g_pAutoReleasePool->AddObject(m_pBoss);

    for (int i = 0; i < limitMonster; i++)
    {
        cMonster* newMonster = new cMonster("Boar");
        g_pAutoReleasePool->AddObject(newMonster);
        m_vMonster.push_back(newMonster);

        newMonster = new cMonster("Bear");
        g_pAutoReleasePool->AddObject(newMonster);
        m_vMonster.push_back(newMonster);
    }
}

void cCharacterManager::Destroy()
{
}

void cCharacterManager::PushMonster(iCharacterObject* monster)
{
    m_vMonster.push_back(monster);
}

cMonster* cCharacterManager::GetMonster(string szMap)
{
    cMonster* returnMonster = NULL;

    if (szMap == "start")
    {
        for (auto iter = m_vMonster.begin(); iter != m_vMonster.end(); iter++)
        {
            if ((*iter)->GetName() == "Boar")
            {
                returnMonster = (cMonster*)(*iter);
                returnMonster->Setup();
                m_vMonster.erase(iter);
                break;
            }
        }
    }
    else if (szMap == "badland")
    {
        int randNum = rand() % m_vMonster.size();
        returnMonster = (cMonster*)m_vMonster[randNum];
        returnMonster->Setup();
        m_vMonster.erase(m_vMonster.begin() + randNum);
    }
    else
    {
        int randNum = rand() % m_vMonster.size();
        returnMonster = (cMonster*)m_vMonster[randNum];
        returnMonster->Setup();
        m_vMonster.erase(m_vMonster.begin() + randNum);
    }

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

cBoss* cCharacterManager::GetBoss()
{
    return m_pBoss;
}