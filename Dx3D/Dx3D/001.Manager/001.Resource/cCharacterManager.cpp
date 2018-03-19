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
        cMonster* newMonster = new cMonster("Boar");
        g_pAutoReleasePool->AddObject(newMonster);
        //m_listMonster.push_back(newMonster);
        m_vMonster.push_back(newMonster);

        newMonster = new cMonster("Bear");
        g_pAutoReleasePool->AddObject(newMonster);
        //m_listMonster.push_back(newMonster);
        m_vMonster.push_back(newMonster);

        /*newMonster = new cMonster("Deathwing");
        g_pAutoReleasePool->AddObject(newMonster);
        m_listMonster.push_back(newMonster);*/
    }
}

void cCharacterManager::Destroy()
{
}

void cCharacterManager::PushMonster(cMonster* monster)
{
    //m_listMonster.push_back(monster);
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
                returnMonster = (*iter);
                returnMonster->Setup();
                //m_listMonster.pop_back(); // 이거도 잘못됨
                m_vMonster.erase(iter);
                break;
            }
        }
    }
    else if (szMap == "badland")
    {
        int randNum = rand() % m_vMonster.size();
        returnMonster = m_vMonster[randNum];
        returnMonster->Setup();
       // m_listMonster.pop_back(); 
        m_vMonster.erase(m_vMonster.begin() + randNum);
    }

    return returnMonster;
}
