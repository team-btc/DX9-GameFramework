#pragma once
#include "iObject.h"

#define g_pAutoReleasePool cAutoReleasePool::GetInstance()

class cAutoReleasePool
{
    SINGLETON(cAutoReleasePool);

private:
    list<iObject*> m_lstObject;

public:
    void AddObject(iObject* pObject);
    void Drain();
};