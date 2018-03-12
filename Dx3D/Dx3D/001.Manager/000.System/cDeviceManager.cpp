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
        m_ptWindowSize.y = W_WIDTH / 16 * 9;
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

    D3DCAPS9 stCaps;		// ��ġ�� ���� ������ ����
    int nVertexProcessing;	// ���ؽ��� ó�� �ϴ� ���

    m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);

    if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    
    m_stPParam.Windowed = m_isWindowed;
    m_stPParam.BackBufferWidth = m_ptWindowSize.x;
    m_stPParam.BackBufferHeight = m_ptWindowSize.y;

    m_stPParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_stPParam.hDeviceWindow = g_hWnd;
    m_stPParam.BackBufferFormat = D3DFMT_X8R8G8B8;     // �ȼ� ���� p79
    m_stPParam.EnableAutoDepthStencil = true;          // ���� ���� �ڵ� ����
    m_stPParam.AutoDepthStencilFormat = D3DFMT_D16;    // ���� ���� p82
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
#ifdef _DEBUG
        string msg = "����̽��� �̿��ؼ� ������ ��ü �� �Ҹ���� ���� ��ü�� ����. " + to_string(ul);
        assert(ul == 0 && msg.c_str());
#endif // _DEBUG
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