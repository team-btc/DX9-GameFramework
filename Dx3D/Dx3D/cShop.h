#pragma once
#include "cObject.h"

class cUILayer;

enum E_ITEM_TYPE
{
    E_ITEM_SWORD1,
    E_ITEM_SWORD2,
    E_ITEM_HELM1,
    E_ITEM_HELM2,
    E_ITEM_CHEST_PLATE1,
    E_ITEM_CHEST_PLATE_2,
    E_ITEM_POTION_HEAL,
    E_ITEM_POTION_MANA
};

class cShop : public cObject
{
private:
    vector<ST_ITEM_INFO>        m_vecItemInfo;

    cUILayer*                   m_pShopLayer;

    vector<string>              m_vecSzHumanSound;

    E_ITEM_TYPE                 m_eCurrSelectItem;

    RECT                        m_rtShopSize;

    int                         m_nPlayerMoney;

    SYNTHESIZE(bool, m_isOpen, IsOpen);

private:
    void SetShopUI();
    void SetShopItemUI(Vector3 vShopPos);

public:
    cShop();
    ~cShop();

    void Setup();
    void Update(int nPlayerMoney);
    void Render();

    void OpenShop();
    void CloseShop();

    virtual ULONG Release(void) override;
};

