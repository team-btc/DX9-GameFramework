#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
    string fontpath = FONT_PATH;
    m_listFont.push_back(fontpath + "Abaddon.ttf");
    m_listFont.push_back(fontpath + "Folkard.ttf");
    m_listFont.push_back(fontpath + "FrizQt.ttf");
    m_listFont.push_back(fontpath + "Warcraft.ttf");
}

cFontManager::~cFontManager()
{
}

HRESULT cFontManager::Setup()
{
    for (auto iter = m_listFont.begin(); iter != m_listFont.end(); iter++)
    {
        AddFontResourceA(iter->c_str());
    }

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
    for (auto iter = m_listFont.begin(); iter != m_listFont.end(); iter++)
    {
        RemoveFontResourceA(iter->c_str());
    }

    for each (auto p in m_mapFont)
    {
        SAFE_RELEASE(p.second);
    }

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
                    _TEXT("FrizQt"),
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
            {
                break;
            }
            case cFontManager::E_CHAT:
            {
                break;
            }
            case cFontManager::E_QUEST:
            {
                D3DXCreateFont(g_pDevice,
                    45,
                    0,
                    FW_DONTCARE,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    _TEXT("Warcraft"),
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
        }
    }

    return m_mapFont[e];
}