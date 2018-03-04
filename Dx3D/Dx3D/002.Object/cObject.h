#pragma once
#include "iObject.h"

class cObject : public iObject
{
private:
public:
    cObject();
    virtual ~cObject();

public:
    // iObject��(��) ���� ��ӵ�
    virtual ULONG   AddRef() override;
    virtual ULONG   Release(void) override;
    virtual HRESULT GetName(string& name) override;
    virtual HRESULT SetName(const string name) override;
    virtual HRESULT Destroy() override;

public:
    bool CompareName(const string name);
};