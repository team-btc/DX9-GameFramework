#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

void cTextureManager::LoadParticleTexture()
{
    AddTexture("snow", "Assets\\Texture\\Particle\\particle_snow.png");
}

void cTextureManager::Destroy()
{
    auto iter = m_mapTexture.begin();
    for (; iter != m_mapTexture.end(); ++iter)
    {
        SAFE_RELEASE(iter->second);
    }

    m_mapTexture.clear();
}

void cTextureManager::AddTexture(string szKey, string szFilepath, bool saveImageInfo)
{
    HRESULT hr;
    if (m_mapTexture[szKey] == NULL)
    {
        LPTEXTURE9 t;
        if (saveImageInfo)
        {
            hr = D3DXCreateTextureFromFileExA(g_pDevice,
                szFilepath.c_str(),
                D3DX_DEFAULT_NONPOW2,
                D3DX_DEFAULT_NONPOW2,
                D3DX_DEFAULT,
                0,
                D3DFMT_UNKNOWN,
                D3DPOOL_MANAGED,
                D3DX_FILTER_NONE,
                D3DX_DEFAULT,
                0,
                &m_mapImageInfo[szKey],
                NULL,
                &t);

        }
        else
        {
            hr = D3DXCreateTextureFromFileA(g_pDevice,
                szFilepath.c_str(),
                &t);
        }

        if (hr == S_OK)
        {
            m_mapTexture[szKey] = t;
        }
#ifdef _DEBUG
        else
        {
            cout << "texture load error" << endl;
        }
#endif // _DEBUG
    }
}

void cTextureManager::AddCubeTexture(string szKey, string szFilepath)
{
    HRESULT hr;
    if (m_mapTexture[szKey] == NULL)
    {
        LPCUBETEXTURE9 t;
        hr = D3DXCreateCubeTextureFromFileA(g_pDevice,
            szFilepath.c_str(),
            &t);

        if (hr == S_OK)
        {
            m_mapTexture[szKey] = t;
        }
#ifdef _DEBUG
        else
        {
            cout << "cube texture load error" << endl;
        }
#endif // _DEBUG
    }
}

BASETEXTURE9* cTextureManager::GetTexture(string key, OUT IMAGE_INFO* pImageInfo)
{
    auto iter = m_mapTexture.find(key);
    if (iter != m_mapTexture.end())
    {
        if (pImageInfo)
        {
            auto iter2 = m_mapImageInfo.find(key);
            if (iter2 != m_mapImageInfo.end())
            {
                *pImageInfo = iter2->second;
            }
        }

        return iter->second;
    }
    else
    {
        return NULL;
    }
}