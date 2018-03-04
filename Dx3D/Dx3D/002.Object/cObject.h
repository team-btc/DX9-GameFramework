#pragma once
#include "iObject.h"

class cObject : public iObject
{
private:
    int m_nRefCount;

public:
    cObject();
    virtual ~cObject();

    // IUnknown��(��) ���� ��ӵ�
    virtual HRESULT QueryInterface(REFIID riid, void** ppvObject) override;
    virtual ULONG AddRef() override;
    virtual ULONG Release(void) override;

    // iObject��(��) ���� ��ӵ�
    virtual HRESULT GetName(string& name) override;
    virtual HRESULT SetName(const string name) override;
    virtual HRESULT Destroy() override;
};