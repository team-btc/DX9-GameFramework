#pragma once
#include "stdafx.h"

interface iObject
{
protected:
    int     m_nRefCount;
    string  m_szName;

public:
    virtual ULONG AddRef() PURE;
    virtual ULONG Release() PURE;

    virtual HRESULT GetName(string& name) PURE;
    virtual HRESULT SetName(const string name) PURE;

    virtual HRESULT Destroy() PURE;
};