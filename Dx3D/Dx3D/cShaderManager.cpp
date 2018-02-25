#include "stdafx.h"
#include "cShaderManager.h"


cShaderManager::cShaderManager()
{
}


cShaderManager::~cShaderManager()
{
}

void cShaderManager::Destroy()
{
    for (auto iter = m_mapEffect.begin(); iter != m_mapEffect.end();)
    {
        iter->second->Release();
        iter = m_mapEffect.erase(iter);
    }
}

HRESULT cShaderManager::AddEffect(string key, string filepath)
{
    HRESULT result = S_OK;
    auto iter = m_mapEffect.find(key);
    if (iter == m_mapEffect.end())
    {
        LPD3DXEFFECT ret = NULL;
        LPD3DXBUFFER pError = NULL;
        DWORD dwShaderFlags = 0;

#if _DEBUG
        dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

        result = D3DXCreateEffectFromFileA(g_pDevice, filepath.c_str(),
                                  NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

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
        result = E_UNEXPECTED;
    }
    
    return result;
}

LPEFFECT cShaderManager::GetEffect(string key)
{
    auto iter = m_mapEffect.find(key);
    if (iter != m_mapEffect.end())
    {
        return iter->second;
    }
    else
    {
        return NULL;
    }
}
