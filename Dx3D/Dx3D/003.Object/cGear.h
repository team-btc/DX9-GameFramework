#pragma once
#include "cObject.h"

class cUILayer;
class cUIImageView;
class cUITextView;

class cGear : public cObject
{
private:
    cUILayer*                           m_pGearLayer;
    vector<cUIImageView*>               m_vecGearUI;
    map<E_PLAYER_STAT, cUITextView*>    m_mapTextUI;
    cUILayer*                           m_pPlusStatLayer;

    RECT                                m_rtGearSize;
    ST_SIZE                             m_stGearSlotSize;

    vector<RECT>                        m_vecItemRect;
    ST_SIZE                             m_stPlusStatDefSize;

    ST_GEAR                             m_stCurrGear;

    int                                 m_nCurrSelectGear;

    SYNTHESIZE(bool, m_isClickGear, IsClickGear);
    SYNTHESIZE(bool, m_isOpen, IsOpen);

private:
    void SetGearUI();
    void SetStatText(Vector3 vGearPos);
    cUITextView* MakeStatTextUI(string name, Vector3 pos, Vector2 size, DWORD color);
    void SetStatUI(Vector3 vGearPos);
    void UpdateGearUI(ST_STATUS stPlayerStat);
    void SetPlusStatUI();
    void UpdatePlusStatUI(int id, Vector3 vPos);
    bool CheckPtInItem();

public:
    cGear();
    ~cGear();

    void Setup();
    void Update(ST_STATUS stPlayerStat);
    void Render();

    void OpenGear();
    void CloseGear();

    virtual ULONG Release(void) override;
};

