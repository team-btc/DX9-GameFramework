#pragma once
#include "iSingletonManager.h"

#define g_pGameManager cGameManager::GetInstance()


enum E_PLAYER_STAT
{
    E_PLAYER_LEVEL,
    E_PLAYER_NAME,
    E_PLAYER_HP_CUR,
    E_PLAYER_HP_MAX,
    E_PLAYER_MP_CUR,
    E_PLAYER_MP_MAX,
    E_PLAYER_ATT,
    E_PLAYER_DEF,
    E_PLAYER_STR,
    E_PLAYER_DEX,
    E_PLAYER_INT
};

class cGameManager : public iSingletonManager
{
private:
    json                        m_jPlayerInfo;
    bool                        m_isLoadData;

    vector<ST_ITEM_INFO*>       m_vecItemInfo;
    ST_INVENTORY                m_stInventory;
    ST_GEAR                     m_stGear;

    //  QUEST
    json                        m_jQuestInfo;

private:
    SINGLETON(cGameManager);

private:
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

    void PushGear(int id);
    void PullGear(int id);

    void Pay(int gold);
    
    void LoadItemInfo();
    void LoadItemPlusStat(vector<ST_PLUS_STAT_INFO>* vecPlusStat, E_PLAYER_STAT eStat, float fValue);
    
    ST_ITEM_INFO* GetItemInfoById(int id);
    vector<ST_ITEM_INFO*> GetItemInfo() { return m_vecItemInfo; }
    int GetCurrGold() { return m_stInventory.gold; }
    vector<ST_ITEM> GetInvetoryInfo() { return m_stInventory.items; }
    ST_GEAR GetGearInfo() { return m_stGear; }
    string GetStatName(E_PLAYER_STAT eStat);
    
#pragma region QUEST
    void LoadQuestInfo();
    json GetQuest(string map);
#pragma endregion
};