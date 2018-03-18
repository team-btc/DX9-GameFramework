#include "stdafx.h"
#include "cAutoReleasePool.h"


cAutoReleasePool::cAutoReleasePool()
{
    m_lstObject.clear();
}


cAutoReleasePool::~cAutoReleasePool()
{
}

void cAutoReleasePool::AddObject(IUnknown* pObject)
{
    m_lstObject.push_back(pObject);
}

void cAutoReleasePool::Drain()
{
    for each (auto vPos in m_lstObject)
    {
        while (vPos->Release());
    }

    m_lstObject.clear();
}
