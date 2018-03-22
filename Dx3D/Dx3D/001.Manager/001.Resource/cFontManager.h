#pragma once
#include "iSingletonManager.h"

#define g_pFontManager cFontManager::GetInstance()

class cFontManager : public iSingletonManager
{
    SINGLETON(cFontManager);

public:
    enum eFontType
    {
        E_NORMAL,
        E_CHAT,
        E_QUEST,
        E_ALERT,
        E_DEBUG,
        E_SHOP_TITLE,
        E_SHOP_DEFAULT,
        E_DAMAGE,
        E_STAT
    };

private:
    map<eFontType, LPFONTDX>	m_mapFont;

public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    LPFONTDX GetFont(eFontType e);
};