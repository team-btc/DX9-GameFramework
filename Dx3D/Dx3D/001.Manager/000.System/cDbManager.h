#pragma once
#include "iSingletonManager.h"
#define g_pDbManager cDbManager::GetInstance()

#include "cAdo.h"

class cDbManager : public iSingletonManager
{
    SINGLETON(cDbManager);

private:
    cAdo*   m_pDatabase;

public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};

