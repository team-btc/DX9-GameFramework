#pragma once
#include "iSingletonManager.h"

#define g_pDeviceManager cDeviceManager::GetInstance()
#define g_pDevice cDeviceManager::GetInstance()->GetDevice()

class cDeviceManager : public iSingletonManager
{
    SINGLETON(cDeviceManager);

private:
    LPDIRECT3D9         m_pD3D;         // D3D9 인터페이스로의 포인터를 담는다.
    LPDEVICE9           m_pD3DDevice;   // 장치로 부터 D3D9 인터페이스를 만드는데 사용 된다.

public:

    LPDEVICE9 GetDevice() { return m_pD3DDevice; }

    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};