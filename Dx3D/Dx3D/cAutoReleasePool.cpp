#include "stdafx.h"
#include "cAutoReleasePool.h"
#include "cObject.h"


cAutoReleasePool::cAutoReleasePool()
{
    m_lstObject.clear();
}


cAutoReleasePool::~cAutoReleasePool()
{
}

void cAutoReleasePool::AddObject(cObject* pObject)
{
    m_lstObject.push_back(pObject);
}

void cAutoReleasePool::Drain()
{
    for each (auto p in m_lstObject)
    {
        int count = p->GetRefCount();

        for (int i = 0; i < count; ++i)
        {
            SAFE_RELEASE(p);
        }
    }

    m_lstObject.clear();
}
