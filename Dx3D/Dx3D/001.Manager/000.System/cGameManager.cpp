#include "stdafx.h"
#include "cGameManager.h"


cGameManager::cGameManager()
    : m_isLoadData(false)
{
}


cGameManager::~cGameManager()
{
}

HRESULT cGameManager::Setup()
{
    LoadPlayerInfo();

    return S_OK;
}

HRESULT cGameManager::Update()
{
    return E_NOTIMPL;
}

HRESULT cGameManager::Render()
{
    return E_NOTIMPL;
}

HRESULT cGameManager::Destroy()
{
    SavePlayerInfo();

    return S_OK;
}

void cGameManager::SavePlayerInfo()
{
    MakeSaveData();

    ofstream savefileStream;
    savefileStream.open("Save\\Player.json", ios_base::out);
    savefileStream << m_jPlayerInfo.dump(4);
    savefileStream.close();
}

void cGameManager::LoadPlayerInfo()
{
    ifstream loadfileStream;
    loadfileStream.open("Save\\Player.json", ios_base::in);
    if (loadfileStream.is_open())
    {
        loadfileStream >> m_jPlayerInfo;
        loadfileStream.close();

        g_pCharacterManager->SetPlayerData(m_jPlayerInfo["player"]);
        g_pMapManager->SetMapData(m_jPlayerInfo["map"]);

        m_isLoadData = true;
    }
}

void cGameManager::MakeSaveData()
{
    json playerData = g_pCharacterManager->GetPlayerData();
    json mapData = g_pMapManager->GetMapData();

    m_jPlayerInfo["player"] = playerData;
    m_jPlayerInfo["map"] = mapData;
}
