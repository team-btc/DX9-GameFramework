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
        gold = 0;
    }
};

class cGameManager : public iSingletonManager
{
private:
    json                        m_jPlayerInfo;
    bool                        m_isLoadData;
    vector<ST_ITEM_INFO*>       m_vecItemInfo;

private:
    SINGLETON(cGameManager);

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

    void Pay(int gold);
    
    void LoadItemInfo();
    ST_ITEM_INFO* GetItemInfoById(int nId);
    vector<ST_ITEM_INFO*> GetItemInfo() { return m_vecItemInfo; }

};

