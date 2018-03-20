#include "stdafx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager()
    : m_pD3D(NULL)
    , m_pD3DDevice(NULL)
    , m_fWindowRatio(16.0f / 9.0f)
#ifdef _DEBUG
    , m_isWindowed(true)
#else
    , m_isWindowed(false)
#endif // _DEBUG
{
    if (m_isWindowed)
    {
        m_ptWindowSize.x = W_WIDTH;
        m_ptWindowSize.y = W_HEIGHT;
    }
    else
    {
        m_ptWindowSize = GetDisplaySize();
    }

    ZeroMemory(&m_stPParam, sizeof(D3DPRESENT_PARAMETERS));
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

    m_stPParam.Windowed = m_isWindowed;
    m_stPParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_stPParam.hDeviceWindow = g_hWnd;
    m_stPParam.BackBufferFormat = D3DFMT_X8R8G8B8;     // 픽셀 포맷 p79
    m_stPParam.EnableAutoDepthStencil = true;          // 깊이 버퍼 자동 관리
    m_stPParam.AutoDepthStencilFormat = D3DFMT_D16;    // 깊이 버퍼 p82
    m_stPParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        g_hWnd,
        nVertexProcessing,
        &m_stPParam,
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
        string msg = "디바이스를 이용해서 생성한 객체 중 소멸되지 않은 객체가 있음. " + to_string(ul);
        assert(ul == 0 && msg.c_str());
    }

    return hr;
}

void cDeviceManager::Reset()
{
    g_pDevice->Reset(&m_stPParam);
}

POINT cDeviceManager::GetDisplaySize()
{
    RECT rtDisplay;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &rtDisplay);

    return POINT{ rtDisplay.right, rtDisplay.bottom };
}