#pragma once
#include "iSingletonManager.h"

#define g_pGameManager cGameManager::GetInstance()

struct ST_ITEM
{
    int id;
    int count;

    ST_ITEM()
    {
        id = -1;
        count = -1;
    }
};

struct ST_INVENTORY
{
    int     gold;
    vector<ST_ITEM> items;

    ST_INVENTORY()
    {
        gold = 123456;
    }
};

struct ST_GEAR
{
    int swordId;
    int helmId;
    int chestPlateId;

    ST_GEAR()
    {
        swordId = -1;
        helmId = -1;
        chestPlateId = -1;
    }
};

class cGameManager : public iSingletonManager
{
private:
    json                        m_jPlayerInfo;
    bool                        m_isLoadData;

    vector<ST_ITEM_INFO*>       m_vecItemInfo;
    ST_INVENTORY                m_stInventory;
    ST_GEAR                     m_stGear;

private:
    SINGLETON(cGameManager);

    int FindItem(int id);

public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    void SavePlayerInfo();
    void LoadPlayerInfo();
    void MakeSaveData();

    bool IsLoadData() { return m_isLoadData; }
    void DisableLoadFlag() { m_isLoadData = false; }
    void EnableLoadFlag() { m_isLoadData = true; }

    void PushItem(int id);
    void PullItem(int id);

    void Pay(int gold);
    
    void LoadItemInfo();
    
    ST_ITEM_INFO* GetItemInfoById(int id);
    vector<ST_ITEM_INFO*> GetItemInfo() { return m_vecItemInfo; }
    int GetCurrGold() { return m_stInventory.gold; }
    vector<ST_ITEM> GetInvetoryInfo() { return m_stInventory.items; }
};

