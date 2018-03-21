#pragma once
#include "cObject.h"

class cUILayer;
class cUIImageView;

class cInventory : public cObject
{
    cUILayer*                   m_pInvenLayer;
    vector<cUIImageView*>       m_vecItemUI;

    RECT                        m_rtInvenSize;
    ST_SIZE                     m_stItemSize;

    vector<ST_ITEM_INFO>        m_vecPlayerItemInfo;

    int                         m_nPlayerMoney;

    int                         m_nCurrSelectItem;

    int                         m_nSlotMaxNum;

    bool                        m_isClickInven;

private:
    void SetInvenUI();
    void SetItemUI(Vector3 vInvenPos);
    void UpdateItemUI();

public:
    cInventory();
    ~cInventory();

    void Setup();
    void Update(int nPlayerMoney);
    void Render();

    void OpenInventory();
    void CloseInventory();

    virtual ULONG Release(void) override;
};

