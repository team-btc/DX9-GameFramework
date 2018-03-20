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

struct ST_ITEM_INFO
{
    E_ITEM_TYPE     eType;
    string          szName;
    string          szPath;
    int             nCount;
    int             nPrice;
    int             nPlusStat;  //증가 효과(체력. 민첩 등등) enum문으로 만들기
    float           fPlusValue; //증가 효과 값
};

class cShop : public cObject
{
private:
    vector<ST_ITEM_INFO>        m_vecItemInfo;

    cUILayer*                   m_pShopLayer;

    vector<string>              m_vecSzHumanSound;

    E_ITEM_TYPE                 m_eCurrSelectItem;

    int                         m_nPlayerMoney;

private:
    void SetShopUI();
    void SetShopItemUI(Vector3 vShopPos);

public:
    cShop();
    ~cShop();

    void Setup();
    void Update();
    void Render();

    virtual ULONG Release(void) override;
};

