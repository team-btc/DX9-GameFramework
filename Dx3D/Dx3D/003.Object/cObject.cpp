#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
{
    g_pObjectManager->AddObject(this);
    m_nRefCount = 1;
}


cObject::~cObject()
{
    if (m_nRefCount <= 0)
    {
        g_pObjectManager->RemoveObejct(this);
    }
    else
    {
        assert(false && "릴리즈를 사용해서 객체를 해제하세요");
    }
}

HRESULT cObject::QueryInterface(REFIID riid, void ** ppvObject)
{
    return E_NOTIMPL;
}

ULONG cObject::AddRef()
{
    return ++m_nRefCount;
}

ULONG cObject::Release()
{
    ULONG ul = --m_nRefCount;
    if (ul <= 0)
    {
        delete this;
    }

    return ul;
}

string cObject::GetName()
{
    return m_szName;
}

HRESULT cObject::SetName(const string name)
{
    if (name != "")
    {
        m_szName = name;

        return S_OK;
    }
    else
    {
        return E_INVALIDARG;
    }
}

HRESULT cObject::Destroy()
{
    return E_NOTIMPL;
}