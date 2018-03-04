#include "stdafx.h"
#include "cEventManager.h"


cEventManager::cEventManager()
    : m_fStandByTime(0.0f)
{
}


cEventManager::~cEventManager()
{
}

HRESULT cEventManager::Load(int nMapId)
{
    HRESULT hr = S_OK;
    json jEvent;
    ifstream i;
    i.open("data/event/event-" + to_string(nMapId) + ".json", ios_base::in);
    i >> jEvent;
    i.close();

    return hr;
}

cEventElem::cEventElem()
    : m_nId(-1)
    , m_szName("")
    , m_eType(cEvent::WAIT)
    , m_eStatus(STOP)
{
}

cEventElem::~cEventElem()
{
}

cEvent::cEvent()
{
}

cEvent::~cEvent()
{
    if (!m_listEventElem.empty())
    {
        Destroy();
    }
}

HRESULT cEvent::Destroy()
{
    HRESULT hr = S_OK;
    for (auto iter = m_listEventElem.begin(); iter != m_listEventElem.end();)
    {
        cEventElem* pElem = *iter;
        iter = m_listEventElem.erase(iter);
        SAFE_DELETE(pElem);
    }

    return hr;
}
