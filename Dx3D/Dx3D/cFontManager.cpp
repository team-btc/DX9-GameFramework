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
    // 찾는 폰트가 없는 경우 생성
    if (m_mapFont.find(e) == m_mapFont.end())
    {
        switch (e)
        {
            case cFontManager::E_ALERT:
            {
                AddFontResource(L"UI/umberto.ttf");
                D3DXCreateFont(g_pDevice, 75, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET,
                               OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, false, L"umberto", &m_mapFont[e]);
                break;
            }
            case cFontManager::E_NORMAL:
                break;
            case cFontManager::E_CHAT:
                break;
            case cFontManager::E_QUEST:
            {
                AddFontResource(L"UI/umberto.ttf");
                D3DXCreateFont(g_pDevice, 25, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET,
                               OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, false, L"umberto", &m_mapFont[e]);
                break;
            }
        }
    }

    return m_mapFont[e];
}

void cFontManager::Destory()
{
    RemoveFontResource(L"umberto");

    for each (auto p in m_mapFont)
        SAFE_RELEASE(p.second);

    m_mapFont.clear();
}
