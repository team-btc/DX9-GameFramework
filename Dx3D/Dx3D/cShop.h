#pragma once
#include "cObject.h"

class cUILayer;

class cShop : public cObject
{
private:
    vector<ST_ITEM_INFO*>       m_vecItemInfo;

    cUILayer*                   m_pShopLayer;

    vector<string>              m_vecSzHumanSound;

    int                         m_nCurrSelectItem;

    RECT                        m_rtShopSize;

    int                         m_nPlayerMoney;

    SYNTHESIZE(bool, m_isOpen, IsOpen);
    SYNTHESIZE(bool, m_isClickShop, ClickShop);
    SYNTHESIZE(bool, m_isBuyItem, IsBuyItem);

private:
    void SetShopUI();
    void SetShopItemUI(Vector3 vShopPos);

public:
    cShop();
    ~cShop();

    void Setup();
    void Update();
    void Render();

    void OpenShop();
    void CloseShop();

    virtual ULONG Release(void) override;
};

