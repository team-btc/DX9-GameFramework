#pragma once
#include "stdafx.h"

interface iObject : public IUnknown
{
protected:
    string m_szName;

public:
    virtual HRESULT GetName(string& name) PURE;
    virtual HRESULT SetName(const string name) PURE;

    virtual HRESULT Destroy() PURE;
};