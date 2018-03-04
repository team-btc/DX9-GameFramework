#pragma once

class cObject;

#define g_pObjectManager cObjectManager::GetInstance()

class cObjectManager
{
    SINGLETON(cObjectManager);

private:
    set<cObject*> m_setObject;

public:
    void AddObject(cObject* pObject);
    void RemoveObejct(cObject* pObject);
    void Destory();
};

