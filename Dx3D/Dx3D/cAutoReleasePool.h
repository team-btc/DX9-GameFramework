#pragma once

class cObject;

#define g_pAutoReleasePool cAutoReleasePool::GetInstance()

class cAutoReleasePool
{
    SINGLETON(cAutoReleasePool);

private:
    list<cObject*> m_lstObject;

public:
    void AddObject(cObject* pObject);
    void Drain();
};