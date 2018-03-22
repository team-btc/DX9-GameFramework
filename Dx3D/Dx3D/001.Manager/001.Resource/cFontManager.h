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
        E_SHOP_BIG,
        E_SHOP_DEFAULT,
        E_SHOP_SMALL,
        E_DAMAGE
    };

private:
    map<eFontType, LPFONTDX>	m_mapFont;

public:
    // iSingletonManager��(��) ���� ��ӵ�
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    LPFONTDX GetFont(eFontType e);
};