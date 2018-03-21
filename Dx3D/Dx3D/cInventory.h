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

    vector<ST_ITEM>             m_vecInvenItem;

    int                         m_nPlayerMoney;

    int                         m_nCurrSelectItem;
    int                         m_nSlotMaxNum;

    SYNTHESIZE(bool, m_isClickInven, IsClickInven);
    SYNTHESIZE(bool, m_isOpen, IsOpen);

private:
    void SetInvenUI();
    void SetItemUI(Vector3 vInvenPos);
    void UpdateItemUI();

public:
    cInventory();
    ~cInventory();

    void Setup();
    void Update();
    void Render();

    void OpenInventory();
    void CloseInventory();

    virtual ULONG Release(void) override;
};

