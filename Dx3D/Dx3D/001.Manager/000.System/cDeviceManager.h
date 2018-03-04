#pragma once

#define g_pDeviceManager cDeviceManager::GetInstance()
#define g_pDevice cDeviceManager::GetInstance()->GetDevice()

class cDeviceManager
{
    SINGLETON(cDeviceManager);

private:
    LPDIRECT3D9         m_pD3D;         // D3D9 �������̽����� �����͸� ��´�.
    LPDEVICE9           m_pD3DDevice;   // ��ġ�� ���� D3D9 �������̽��� ����µ� ��� �ȴ�.

public:
    void Destroy();

    LPDEVICE9 GetDevice() { return m_pD3DDevice; }
};