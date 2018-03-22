#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}

cFontManager::~cFontManager()
{
}

HRESULT cFontManager::Setup()
{
    AddFontResource(_TEXT("UI/umberto.ttf"));
    AddFontResource(_TEXT("Assets\\Font\\Warcraft.ttf"));

    return S_OK;
}

HRESULT cFontManager::Update()
{
    return E_NOTIMPL;
}

HRESULT cFontManager::Render()
{
    return E_NOTIMPL;
}

HRESULT cFontManager::Destroy()
{
    RemoveFontResource(_TEXT("umberto"));
    RemoveFontResource(_TEXT("Assets\\Font\\Warcraft.ttf"));

    for each (auto p in m_mapFont)
        SAFE_RELEASE(p.second);

    m_mapFont.clear();

    return S_OK;
}

LPFONTDX cFontManager::GetFont(eFontType e)
{
    // 찾는 폰트가 없는 경우 생성
    if (m_mapFont.find(e) == m_mapFont.end())
    {
        switch (e)
        {
            case cFontManager::E_ALERT:
            {
                D3DXCreateFont(g_pDevice,
                    75,
                    0,
                    FW_DONTCARE,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("umberto"),
                    &m_mapFont[e]);
                break;
            }
            case cFontManager::E_DEBUG:
            {
                D3DXCreateFont(g_pDevice,
                    15,
                    0,
                    FW_DONTCARE,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("Consolas"),
                    &m_mapFont[e]);
                break;
            }
            case cFontManager::E_NORMAL:
                break;
            case cFontManager::E_CHAT:
                break;
            case cFontManager::E_QUEST:
            {
                D3DXCreateFont(g_pDevice,
                    25,
                    0,
                    FW_DONTCARE,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("umberto"),
                    &m_mapFont[e]);
                break;
            }
            case cFontManager::E_SHOP_TITLE:
            {
                D3DXCreateFont(g_pDevice,
                    20,
                    0,
                    FW_BOLD,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("Calibri"),
                    &m_mapFont[e]);
                break;
            }
            case cFontManager::E_SHOP_DEFAULT:
            {
                D3DXCreateFont(g_pDevice,
                    15,
                    0,
                    FW_BOLD,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("Calibri"),
                    &m_mapFont[e]);
                break;
            }
            case cFontManager::E_DAMAGE:
            {
                D3DXCreateFont(g_pDevice,
                    30,
                    0,
                    FW_DONTCARE,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("Rimbo"),
                    &m_mapFont[e]);
                break;
            }
        }
    }

    return m_mapFont[e];
}