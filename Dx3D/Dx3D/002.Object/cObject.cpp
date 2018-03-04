#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
    : m_nRefCount(1)
{
    g_pObjectManager->AddObject(this);
}


cObject::~cObject()
{
    if (m_nRefCount <= 0)
        g_pObjectManager->RemoveObejct(this);
    else
        assert(false && "����� ����ؼ� ��ü�� �����ϼ���");
}

HRESULT cObject::QueryInterface(REFIID riid, void** ppvObject)
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

HRESULT cObject::GetName(string& name)
{
    if (name != "")
    {
        m_szName = name;
        return E_INVALIDARG;
    }
    else
    {
        m_szName = name;
        return S_OK;
    }
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
