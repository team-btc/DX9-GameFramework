#pragma once

#include <bitset>

#define g_pKeyManager cKeyManager::GetInstance()
#define KEYMAX 256

class cKeyManager
{
    SINGLETON(cKeyManager);
private:
    bitset<KEYMAX>  m_bitKeyUp;
    bitset<KEYMAX>  m_bitKeyDown;

public:
    void Setup();

    bool isOnceKeyDown(int key);
    bool isOnceKeyUp(int key);
    bool isStayKeyDown(int key);
    bool isToggleKy(int key);

};