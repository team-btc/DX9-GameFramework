#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
    SINGLETON(cFontManager);

public:
    enum eFontType
    {
        E_NORMAL,
        E_CHAT,
        E_QUEST,
        E_ALERT,
        E_DEBUG
    };

private:
    map<eFontType, LPFONT>	m_mapFont;

public:
    LPFONT GetFont(eFontType e);
    void Destory();
};