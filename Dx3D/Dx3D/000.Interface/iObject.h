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

    virtual string GetName() PURE;
    virtual HRESULT SetName(const string name) PURE;

    virtual HRESULT Destroy() PURE;
};