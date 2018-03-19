#pragma once
#include "iSceneObject.h"

class cUILayer;

class cPlayScene : public iSceneObject
{
private:
    cUILayer*                       m_pPlayerStatUILayer;
    cUILayer*                       m_pHPUILayer;

    float                           m_fPlayerMaxHP;
    float                           m_fPlayerCurrHP;
    float                           m_fPlayerMaxMP;
    float                           m_fPlayerCurrMP;

private:
    void SetUI();
    void UpdateUI();

public:
    cPlayScene();
    ~cPlayScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
};

