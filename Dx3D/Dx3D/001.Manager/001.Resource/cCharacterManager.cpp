#include "stdafx.h"
#include "cCharacterManager.h"
#include "cPlayer.h"
#include "cMonster.h"

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
        cMonster* newMonster = new cMonster("Deathwing");
        g_pAutoReleasePool->AddObject(newMonster);
        m_mapMonster.insert(make_pair(i, newMonster));
        m_listMonster.push_back(newMonster);
    }

    count = 0;
}

void cCharacterManager::Destroy()
{
}

void cCharacterManager::PushMonster(cMonster* monster)
{
    //m_mapMonster.insert(make_pair(m_mapMonster.size() + 1, monster));
    m_listMonster.push_back(monster);
}

cMonster * cCharacterManager::GetMonster()
{
   /* if (count == 20)
        count = 0;
    return m_mapMonster.find(count++)->second;*/
    cMonster* returnMonster = m_listMonster.back();
    m_listMonster.pop_back();
    return returnMonster;
}
