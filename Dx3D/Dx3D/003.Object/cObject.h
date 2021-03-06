#pragma once
#include "iObject.h"

class cObject : public iObject
{
private:
public:
    cObject();
    virtual ~cObject();

public:
    // iObject을(를) 통해 상속됨
    virtual HRESULT QueryInterface(REFIID riid, void ** ppvObject) override;
    virtual ULONG   AddRef() override;
    virtual ULONG   Release(void) override;
    virtual string  GetName() override;
    virtual HRESULT SetName(const string name) override;
    virtual HRESULT Destroy() override;
};