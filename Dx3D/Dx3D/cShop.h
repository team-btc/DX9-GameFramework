#pragma once
#include "cObject.h"

class cUILayer;

enum E_ITEM_TYPE
{
    E_ITEM_WEAPON,
    E_ITEM_HEADPIECE,
    E_ITEM_ARMOR
};

struct ST_ITEM_INFO
{
    E_ITEM_TYPE     eType;
    string          szName;
    int             nCount;
    int             nPrice;
    int             nPlusStat;  //���� ȿ��(ü��. ��ø ���) enum������ �����
    float           fPlusValue; //���� ȿ�� ��
};

class cShop : public cObject
{
private:
    vector<ST_ITEM_INFO>        m_vecItemInfo;

    cUILayer*                   m_pShopLayer;

public:
    cShop();
    ~cShop();

    void Setup();
    void Update();
    void Render();

    virtual ULONG Release(void) override;
};

