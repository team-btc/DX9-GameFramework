#pragma once
#include "cObject.h"

class cUILayer;

class cShop : public cObject
{
private:
    vector<ST_ITEM_INFO*>       m_vecItemInfo;

    cUILayer*                   m_pShopLayer;
    cUILayer*                   m_pPlusStatLayer;

    vector<string>              m_vecSzHumanSound;

    int                         m_nCurrSelectItem;

    RECT                        m_rtShopSize;

    vector<RECT>                m_vecItemRect;
    ST_SIZE                     m_stPlusStatDefSize;

    int                         m_nPlayerMoney;

    SYNTHESIZE(bool, m_isOpen, IsOpen);
    SYNTHESIZE(bool, m_isClickShop, ClickShop);

private:
    void SetShopUI();
    void SetShopItemUI(Vector3 vShopPos);
    void SetPlusStatUI();
    void UpdatePlusStatUI(int id, Vector3 vPos);
    bool CheckPtInItem();

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

