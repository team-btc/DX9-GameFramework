#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
    string fontpath = FONT_PATH;
    m_listFont.push_back("Assets\\Font\\Abaddon.ttf");
    m_listFont.push_back("Assets\\Font\\Folkard.ttf");
    m_listFont.push_back("Assets\\Font\\FrizQt.ttf");
    m_listFont.push_back("Assets\\Font\\Warcraft.ttf");
}

cFontManager::~cFontManager()
{
}

HRESULT cFontManager::Setup()
{
    for (auto iter = m_listFont.begin(); iter != m_listFont.end(); iter++)
    {
        int i = AddFontResourceA(iter->c_str());
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
    // ã�� ��Ʈ�� ���� ��� ����
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
                    _TEXT("Abaddon��"),
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
                HRESULT hr = D3DXCreateFontA(g_pDevice,
                    40,
                    0,
                    FW_DONTCARE,
                    1,
                    false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    false,
                    "WarCraftFont",
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
                    _TEXT("WarCraftFont"),
                    &m_mapFont[e]);
                break;
            }
        }
    }

    return m_mapFont[e];
}