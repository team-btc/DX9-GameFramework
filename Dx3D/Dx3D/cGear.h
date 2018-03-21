#pragma once
#include "cObject.h"

class cUILayer;
class cUIImageView;

class cGear : public cObject
{
private:
    cUILayer*                   m_pGearLayer;
    vector<cUIImageView*>       m_vecGearUI;

    RECT                        m_rtGearSize;
    ST_SIZE                     m_stGearSize;

    //vector<ST_ITEM>             m_vecInvenItem;

    SYNTHESIZE(bool, m_isClickGear, IsClickGear);
    SYNTHESIZE(bool, m_isOpen, IsOpen);

public:
    cGear();
    ~cGear();

    void Setup();
    void Update();
    void Render();

    void OpenGear();
    void CloseGear();

    virtual ULONG Release(void) override;
};

