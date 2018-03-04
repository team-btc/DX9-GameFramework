#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}

cFontManager::~cFontManager()
{
}

LPFONT cFontManager::GetFont(eFontType e)
{
    // ã�� ��Ʈ�� ���� ��� ����
    if (m_mapFont.find(e) == m_mapFont.end())
    {
        switch (e)
        {
            case cFontManager::E_ALERT:
            {
                AddFontResource(_TEXT("UI/umberto.ttf"));
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
                    75,
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
                AddFontResource(_TEXT("UI/umberto.ttf"));
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
        }
    }

    return m_mapFont[e];
}

void cFontManager::Destory()
{
    RemoveFontResource(_TEXT("umberto"));

    for each (auto p in m_mapFont)
        SAFE_RELEASE(p.second);

    m_mapFont.clear();
}
