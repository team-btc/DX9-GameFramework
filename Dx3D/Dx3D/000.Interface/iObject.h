#pragma once
#include "stdafx.h"

interface iObject : public IUnknown
{
protected:
    int     m_nRefCount;
    string  m_szName;

public:
    virtual ULONG AddRef() PURE;
    virtual ULONG Release() PURE;

    virtual string GetName() PURE;
    virtual HRESULT SetName(const string name) PURE;

    virtual HRESULT Destroy() PURE;

    // IUnknown을(를) 통해 상속됨
    virtual HRESULT QueryInterface(REFIID riid, void ** ppvObject) PURE;
};