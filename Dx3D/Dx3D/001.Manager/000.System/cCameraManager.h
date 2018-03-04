#pragma once
#include "iSingletonManager.h"

#define g_pCameraManager    cCameraManager::GetInstance()

class cCameraManager : public iSingletonManager
{
    SINGLETON(cCameraManager);

private:
public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};

