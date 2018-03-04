#include "stdafx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager()
{
}


cDeviceManager::~cDeviceManager()
{
}

HRESULT cDeviceManager::Setup()
{
    HRESULT hr = S_OK;
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    D3DCAPS9 stCaps;		// 장치에 대한 정보를 얻어옴
    int nVertexProcessing;	// 버텍스를 처리 하는 방식

    m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);

    if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    D3DPRESENT_PARAMETERS stD3DPP;
    ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
    stD3DPP.Windowed = true;
    stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    stD3DPP.hDeviceWindow = g_hWnd;
    stD3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;     // 픽셀 포맷 p79
    stD3DPP.BackBufferWidth = W_WIDTH;
    stD3DPP.BackBufferHeight = W_HEIGHT;
    stD3DPP.EnableAutoDepthStencil = true;          // 깊이 버퍼 자동 관리
    stD3DPP.AutoDepthStencilFormat = D3DFMT_D16;    // 깊이 버퍼 p82

    hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        g_hWnd,
        nVertexProcessing,
        &stD3DPP,
        &m_pD3DDevice);

    return hr;
}

HRESULT cDeviceManager::Update()
{
    HRESULT hr = E_NOTIMPL;
    return hr;
}

HRESULT cDeviceManager::Render()
{
    HRESULT hr = E_NOTIMPL;
    return hr;
}

HRESULT cDeviceManager::Destroy()
{
    HRESULT hr = S_OK;
    SAFE_RELEASE(m_pD3D);

    if (m_pD3DDevice)
    {
        ULONG ul = m_pD3DDevice->Release();
        assert(ul == 0 && "디바이스를 이용해서 생성한 객체 중 소멸되지 않은 객체가 있음!");
    }

    return hr;
}
