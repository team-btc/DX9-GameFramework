#pragma once
#include "iSingletonManager.h"

#define g_pGameManager cGameManager::GetInstance()

class cGameManager : public iSingletonManager
{
private:
    json        m_jPlayerInfo;
    bool        m_isLoadData;

private:
    SINGLETON(cGameManager);

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
};

