#pragma once
#include "iSingletonManager.h"

#define g_pDeviceManager cDeviceManager::GetInstance()
#define g_pDevice cDeviceManager::GetInstance()->GetDevice()

class cDeviceManager : public iSingletonManager
{
    SINGLETON(cDeviceManager);

private:
    LPDIRECT3D9         m_pD3D;         // D3D9 �������̽����� �����͸� ��´�.
    LPDEVICE9           m_pD3DDevice;   // ��ġ�� ���� D3D9 �������̽��� ����µ� ��� �ȴ�.
    D3DPRESENT_PARAMETERS m_stPParam;
    POINT               m_ptWindowSize;
    float               m_fWindowRatio;
    bool                m_isWindowed;
public:

    LPDEVICE9 GetDevice() { return m_pD3DDevice; }

    // iSingletonManager��(��) ���� ��ӵ�
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    void Reset();

    float GetWindowRatio() const { return m_fWindowRatio; }
    POINT GetWindowSize() const { return m_ptWindowSize; }
    POINT GetDisplaySize();
};