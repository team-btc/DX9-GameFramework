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
    LoadItemInfo();

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

    for (auto iter = m_vecItemInfo.begin(); iter != m_vecItemInfo.end();)
    {
        SAFE_DELETE(*iter);
        iter = m_vecItemInfo.erase(iter);
    }

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

int cGameManager::FindItem(int id)
{
    int nIndex = -1;

    for (int i = 0; i < m_stInventory.items.size(); ++i)
    {
        if (m_stInventory.items[i].id == id)
        {
            nIndex = i;
            break;
        }
    }

    return nIndex;
}

void cGameManager::PushItem(int id)
{
    int nIndex = FindItem(id);

    // 있는 아이템이면
    if (nIndex != -1)
    {
        m_stInventory.items[nIndex].count += 1;
    }
    // 새로운 아이템이면
    else
    {
        ST_ITEM stItem;
        stItem.id = id;
        stItem.count = 1;
        m_stInventory.items.push_back(stItem);
    }
}

void cGameManager::PullItem(int id)
{
    int nIndex = FindItem(id);

    // 있는 아이템이면
    if (nIndex != -1)
    {
        m_stInventory.items[nIndex].count -= 1;
        if (m_stInventory.items[nIndex].count <= 0)
        {
            m_stInventory.items.erase(m_stInventory.items.begin() + nIndex);
        }
    }
    // 새로운 아이템이면
    else
    {
        return;
    }
}

void cGameManager::Pay(int gold)
{
    m_stInventory.gold += gold;
}

void cGameManager::LoadItemInfo()
{
    json jLoad;
    ifstream iFile;
    iFile.open(SHOP_PATH + (string)"shop.json");
    iFile >> jLoad;
    iFile.close();

    json jItem = jLoad["item"];
    for (int i = 0; i < jItem.size(); ++i)
    {
        ST_ITEM_INFO* stItem = new ST_ITEM_INFO;
        string szName = jItem[i]["item-name"];
        stItem->szName = szName;
        string szPath = jItem[i]["item-path"];
        stItem->szPath = szPath;
        stItem->nPrice = jItem[i]["item-price"];
        int nStatNum = jItem[i]["item-plus-stat"];
        char szBuf[10];
        sprintf_s(szBuf, sizeof(szBuf), "%d", nStatNum);
        stItem->stStat.szName = szBuf;
        stItem->fPlusValue = jItem[i]["item-plus-value"];

        m_vecItemInfo.push_back(stItem);
    }
}

ST_ITEM_INFO* cGameManager::GetItemInfoById(int id)
{
    ST_ITEM_INFO* stItemInfo = NULL;

    if (id < m_vecItemInfo.size())
    {
        stItemInfo = m_vecItemInfo[id];
    }

    return stItemInfo;
}
