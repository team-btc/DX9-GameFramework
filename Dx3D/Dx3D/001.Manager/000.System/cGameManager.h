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
    int helmId;
    int chestPlateId;
    int swordId;

    ST_GEAR()
    {
        helmId = 0;
        chestPlateId = 2;
        swordId = 4;
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

private:
    int FindItem(int id);

public:
    // iSingletonManager��(��) ���� ��ӵ�
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

    void PushGear(int id);
    void PullGear(int id);

    void Pay(int gold);
    
    void LoadItemInfo();
    
    ST_ITEM_INFO* GetItemInfoById(int id);
    vector<ST_ITEM_INFO*> GetItemInfo() { return m_vecItemInfo; }
    int GetCurrGold() { return m_stInventory.gold; }
    vector<ST_ITEM> GetInvetoryInfo() { return m_stInventory.items; }
    ST_GEAR GetGearInfo() { return m_stGear; }
};

