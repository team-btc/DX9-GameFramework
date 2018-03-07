#pragma once
#include "cGameObject.h"

class cUnitObject : public cGameObject
{
private:
    cSkinnedMesh*   m_pMesh;

public:
    cUnitObject();
    ~cUnitObject();

    virtual HRESULT Init() override;
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Detroy() override;

    virtual ULONG Release() override;

    void Load(string szKey);
    cSkinnedMesh*   GetMesh() { return m_pMesh; }
};

