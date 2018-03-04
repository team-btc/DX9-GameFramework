#pragma once
#include "iSingletonManager.h"
#include <bitset>

#define g_pKeyManager cKeyManager::GetInstance()
#define KEYMAX 256

class cKeyManager : public iSingletonManager
{
    SINGLETON(cKeyManager);

private:
    bitset<KEYMAX>  m_bitKeyUp;
    bitset<KEYMAX>  m_bitKeyDown;

public:
    bool isOnceKeyDown(int key);
    bool isOnceKeyUp(int key);
    bool isStayKeyDown(int key);
    bool isToggleKy(int key);

    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};