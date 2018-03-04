#include "stdafx.h"
#include "cKeyManager.h"

cKeyManager::cKeyManager()
{

}

cKeyManager::~cKeyManager()
{

}

HRESULT cKeyManager::Setup()
{
    for (int i = 0; i < KEYMAX; i++)
    {
        m_bitKeyUp.set(i, false);
        m_bitKeyDown.set(i, true);
    }

    return S_OK;
}

HRESULT cKeyManager::Update()
{
    return E_NOTIMPL;
}

HRESULT cKeyManager::Render()
{
    return E_NOTIMPL;
}

HRESULT cKeyManager::Destroy()
{
    return E_NOTIMPL;
}

bool cKeyManager::isOnceKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (!m_bitKeyDown[key])
        {
            m_bitKeyDown.set(key, true);

            return true;
        }
    }
    else
    {
        m_bitKeyDown.set(key, false);
    }

    return false;
}

bool cKeyManager::isOnceKeyUp(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        m_bitKeyUp.set(key, true);
    }
    else
    {
        if (m_bitKeyUp[key])
        {
            m_bitKeyUp.set(key, false);

            return true;
        }
    }

    return false;
}

bool cKeyManager::isStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        return true;
    }
    return false;
}

bool cKeyManager::isToggleKy(int key)
{
    if (GetKeyState(key) & 0x0001)
    {
        return true;
    }
    return false;
}
