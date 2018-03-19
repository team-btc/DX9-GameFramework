#pragma once
#include "iSceneObject.h"

class cUILayer;
class cMapLoader;

class cLoadingScene : public iSceneObject
{
private:
    cMapLoader*                 m_pMapLoader;

    cUILayer*                   m_pBGLayer;
    cUILayer*                   m_pProgressBarLayer;

    bool                        m_isMapDefLoad;                 // 기본 로딩 완료 여부
    int                         m_nObjectMaxCnt;
    int                         m_nObjectCurrCnt;
    float                       m_fMaxCount;
    float                       m_fCurrCount;

public:
    cLoadingScene();
    ~cLoadingScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

