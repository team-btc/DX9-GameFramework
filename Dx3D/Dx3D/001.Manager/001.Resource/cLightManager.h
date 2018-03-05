#pragma once
#include "iSingletonManager.h"

#define g_pLightManager     cLightManager::GetInstance()

class cLightManager : public iSingletonManager
{
    SINGLETON(cLightManager);

private:
public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    D3DLIGHT9 InitDirectional(Vector3* dir, XColor* c);
    D3DLIGHT9 InitPoint(Vector3* pos, XColor* c);
    D3DLIGHT9 InitSpot(Vector3* dir, Vector3* pos, XColor* c);
};

