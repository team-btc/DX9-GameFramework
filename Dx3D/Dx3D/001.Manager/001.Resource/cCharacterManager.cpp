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
  
    m_pSindragosa = new cSindragosa("Frostwurmnorthlend");
    g_pAutoReleasePool->AddObject(m_pSindragosa);

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

