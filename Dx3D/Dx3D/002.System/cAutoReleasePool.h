#pragma once
#include "iObject.h"

#define g_pAutoReleasePool cAutoReleasePool::GetInstance()

class cAutoReleasePool
{
    SINGLETON(cAutoReleasePool);

private:
    list<IUnknown*> m_lstObject;

public:
    void AddObject(IUnknown* pObject);
    void Drain();
};