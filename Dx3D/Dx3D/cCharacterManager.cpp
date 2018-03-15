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
    m_pPlayer = new cPlayer("arthaslichking", "Assets\\Player\\ArthasLichking", "arthaslichking.X", "arthaslichking.json");
    g_pAutoReleasePool->AddObject(m_pPlayer);

    for (int i = 0; i < 20; i++)
    {
        cMonster* newMonster = new cMonster("Deathwing", "Assets\\Enemy\\Deathwing", "Deathwing.X", "Deathwing.json");
        g_pAutoReleasePool->AddObject(newMonster);
        m_mapMonster.insert(make_pair(i, newMonster));
    }

    count = 0;
}

void cCharacterManager::Destroy()
{
}

void cCharacterManager::PushMonster(cMonster* monster)
{
    m_mapMonster.insert(make_pair(m_mapMonster.size() + 1, monster));
}

cMonster * cCharacterManager::GetMonster()
{
    if (count == 20)
        count = 0;
    return m_mapMonster.find(count++)->second;
}
