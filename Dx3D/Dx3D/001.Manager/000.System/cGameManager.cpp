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
    LoadQuestInfo();

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
    savefileStream.open(PLAYER_SAVE_PATH, ios_base::out);
    savefileStream << m_jPlayerInfo.dump(4);
    savefileStream.close();
}

void cGameManager::LoadPlayerInfo()
{
    ifstream loadfileStream;
    loadfileStream.open(PLAYER_SAVE_PATH, ios_base::in);
    if (loadfileStream.is_open())
    {
        loadfileStream >> m_jPlayerInfo;
        loadfileStream.close();

        g_pCharacterManager->SetPlayerData(m_jPlayerInfo[JSON_PLAYER_KEY]);
        g_pMapManager->SetMapData(m_jPlayerInfo[JSON_MAP_KEY]);
        SetGearData(m_jPlayerInfo[JSON_GEAR_KEY]);
        SetInvenData(m_jPlayerInfo[JSON_INVENTORY_KEY]);

        m_isLoadData = true;
    }
}

void cGameManager::MakeSaveData()
{
    json playerData = g_pCharacterManager->GetPlayerData();
    json mapData = g_pMapManager->GetMapData();
    json invenData = GetInvenData();
    json gearData = GetGearData();

    m_jPlayerInfo[JSON_PLAYER_KEY] = playerData;
    m_jPlayerInfo[JSON_MAP_KEY] = mapData;
    m_jPlayerInfo[JSON_INVENTORY_KEY] = invenData;
    m_jPlayerInfo[JSON_GEAR_KEY] = gearData;
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

        // 아이템 사용하기(장착아이템이면)
        if (GetItemInfoById(id)->isWear)
        {
            PushGear(id);
        }
    }
    // 새로운 아이템이면
    else
    {
        return;
    }
}

void cGameManager::PushGear(int id)
{
    switch (GetItemInfoById(id)->eItemType)
    {
    case ST_ITEM_INFO::E_HELM:
    {
        // 장착한 아이템이 있으면 인벤토리로 돌리기
        if (m_stGear.helmId != -1)
        {
            PushItem(m_stGear.helmId);
        }
        m_stGear.helmId = id;
    }
    break;
    case ST_ITEM_INFO::E_CHEST_PLATE:
    {
        if (m_stGear.chestPlateId != -1)
        {
            PushItem(m_stGear.chestPlateId);
        }
        m_stGear.chestPlateId = id;
    }
    break;
    case ST_ITEM_INFO::E_SWORD:
    {
        if (m_stGear.swordId != -1)
        {
            PushItem(m_stGear.swordId);
        }
        m_stGear.swordId = id;
    }
    break;
    }
}

void cGameManager::PullGear(int id)
{
    switch (GetItemInfoById(id)->eItemType)
    {
    case ST_ITEM_INFO::E_HELM:
    {
        // 인벤토리로 돌리기
        PushItem(m_stGear.helmId);
        m_stGear.helmId = -1;
    }
    break;
    case ST_ITEM_INFO::E_CHEST_PLATE:
    {
        PushItem(m_stGear.chestPlateId);
        m_stGear.chestPlateId = -1;
    }
    break;
    case ST_ITEM_INFO::E_SWORD:
    {
        PushItem(m_stGear.swordId);
        m_stGear.swordId = -1;
    }
    break;
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
    iFile.open(SHOP_PATH);
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
        stItem->fPlusValue = jItem[i]["item-plus-value"];
        stItem->isWear = jItem[i]["item-wear"];
        stItem->eItemType = jItem[i]["item-type"];

        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_HP_CUR, jItem[i]["hp-cur"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_HP_MAX, jItem[i]["hp-max"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_MP_CUR, jItem[i]["mp-cur"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_MP_MAX, jItem[i]["mp-max"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_ATT, jItem[i]["att"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_DEF, jItem[i]["def"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_STR, jItem[i]["str"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_DEX, jItem[i]["dex"]);
        LoadItemPlusStat(&stItem->vecPlusStat, E_PLAYER_INT, jItem[i]["int"]);

        m_vecItemInfo.push_back(stItem);
    }
}

void cGameManager::LoadItemPlusStat(vector<ST_PLUS_STAT_INFO>* vecPlusStat, E_PLAYER_STAT eStat, float fValue)
{
    if (fValue != 0)
    {
        ST_PLUS_STAT_INFO stStat;
        stStat.nType = (int)eStat;
        stStat.fPlusValue = fValue;
        vecPlusStat->push_back(stStat);
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

void cGameManager::LoadQuestInfo()
{
    ifstream i;
    i.open(QUEST_LIST_PATH, ios_base::in);
    i >> m_jQuestInfo;
    i.close();
}

json cGameManager::GetQuest(string map)
{
    return m_jQuestInfo[map];
}

string cGameManager::GetStatName(E_PLAYER_STAT eStat)
{
    string szName = "";

    switch (eStat)
    {
    case E_PLAYER_HP_CUR:
    {
        szName = "HP";
    }
        break;
    case E_PLAYER_HP_MAX:
    {
        szName = "Health";
    }
        break;
    case E_PLAYER_MP_CUR:
    {
        szName = "MP";
    }
        break;
    case E_PLAYER_MP_MAX:
    {
        szName = "MP";
    }
        break;
    case E_PLAYER_ATT:
    {
        szName = "Att";
    }
        break;
    case E_PLAYER_DEF:
    {
        szName = "Def";
    }
        break;
    case E_PLAYER_STR:
    {
        szName = "Str";
    }
        break;
    case E_PLAYER_DEX:
    {
        szName = "Dex";
    }
        break;
    case E_PLAYER_INT:
    {
        szName = "Int";
    }
        break;
    }

    return szName;
}

json cGameManager::GetInvenData()
{
    json jInven;
    jInven[JSON_GOLD] = m_stInventory.gold;
    for (int i = 0; i < (int)m_stInventory.items.size(); i++)
    {
        jInven[JSON_ITEMS][i][JSON_ITEM_ID] = m_stInventory.items[i].id;
        jInven[JSON_ITEMS][i][JSON_ITEM_COUNT] = m_stInventory.items[i].count;
    }

    return jInven;
}

json cGameManager::GetGearData()
{
    json jGear;
    jGear[JSON_GEAR_HELM] = m_stGear.helmId;
    jGear[JSON_GEAR_CHEST] = m_stGear.chestPlateId;
    jGear[JSON_GEAR_WEAPON] = m_stGear.swordId;

    return jGear;
}

void cGameManager::SetInvenData(json data)
{
    m_stInventory.gold = data[JSON_GOLD].is_null() ? 0 : data[JSON_GOLD];
    if (!data[JSON_ITEMS].is_null())
    {
        for (int i = 0; i < (int)data[JSON_ITEMS].size(); i++)
        {
            ST_ITEM item;
            item.id = data[JSON_ITEMS][i][JSON_ITEM_ID].is_null() ? -1 : data[JSON_ITEMS][i][JSON_ITEM_ID];
            item.count = data[JSON_ITEMS][i][JSON_ITEM_COUNT].is_null() ? -1 : data[JSON_ITEMS][i][JSON_ITEM_COUNT];
            if (item.id == -1)
            {
                continue;
            }
            m_stInventory.items.push_back(item);
        }
    }
}

void cGameManager::SetGearData(json data)
{
    m_stGear.helmId = data[JSON_GEAR_HELM].is_null() ? -1 : data[JSON_GEAR_HELM];
    m_stGear.helmId = data[JSON_GEAR_CHEST].is_null() ? -1 : data[JSON_GEAR_CHEST];
    m_stGear.helmId = data[JSON_GEAR_WEAPON].is_null() ? -1 : data[JSON_GEAR_WEAPON];
}
