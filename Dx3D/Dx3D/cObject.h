#pragma once
#include "iObject.h"

class cObject : public iObject
{
private:
    int m_nRefCount;

public:
    cObject();
    virtual ~cObject();

    // IUnknown을(를) 통해 상속됨
    virtual HRESULT QueryInterface(REFIID riid, void** ppvObject) override;
    virtual ULONG AddRef() override;
    virtual ULONG Release(void) override;

    // iObject을(를) 통해 상속됨
    virtual HRESULT GetName(string& name) override;
    virtual HRESULT SetName(const string name) override;
    virtual HRESULT Destroy() override;
};