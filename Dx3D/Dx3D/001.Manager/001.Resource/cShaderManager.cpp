#include "stdafx.h"
#include "cShaderManager.h"


cShaderManager::cShaderManager()
{
}


cShaderManager::~cShaderManager()
{
}

HRESULT cShaderManager::Destroy()
{
    HRESULT hr = S_OK;
    for (auto iter = m_mapEffect.begin(); iter != m_mapEffect.end();)
    {
        ULONG ul = iter->second->Release();
        iter = m_mapEffect.erase(iter);
    }

    if (!m_mapEffect.empty())
    {
        hr = E_FAIL;
    }

    return hr;
}

HRESULT cShaderManager::AddEffect(string key, string filepath)
{
    HRESULT hr = S_OK;
    auto iter = m_mapEffect.find(key);
    if (iter == m_mapEffect.end())
    {
        LPEFFECT ret = NULL;
        LPBUFFER pError = NULL;
        DWORD dwShaderFlags = 0;

#if _DEBUG
        dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

        hr = D3DXCreateEffectFromFileA(g_pDevice,
                                           filepath.c_str(),
                                           NULL, NULL,
                                           dwShaderFlags,
                                           NULL,
                                           &ret,
                                           &pError);

        if (!ret && pError)
        {
            int size = pError->GetBufferSize();
            void *ack = pError->GetBufferPointer();

            if (ack)
            {
                char* str = new char[size];
                sprintf_s(str, size, (const char*)ack);
                OutputDebugStringA(str);
                delete[] str;
            }
        }

        m_mapEffect.insert(make_pair(key, ret));
    }
    else
    {
        hr = E_UNEXPECTED;
    }
    
    return hr;
}

HRESULT cShaderManager::GetEffect(LPEFFECT* effect, string key)
{
    HRESULT hr;
    auto iter = m_mapEffect.find(key);
    if (iter != m_mapEffect.end())
    {
        effect = &iter->second;
        hr = S_OK;
    }
    else
    {
        if (effect)
        {
            hr = E_INVALIDARG;
        }
        else
        {
            hr = E_ABORT;
        }
    }

    return hr;
}

HRESULT cShaderManager::DelEffect(string key)
{
    HRESULT hr = S_OK;
    auto iter = m_mapEffect.find(key);
    if (iter != m_mapEffect.end())
    {
        iter->second->Release();
        m_mapEffect.erase(iter);
        hr = S_OK;
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}
