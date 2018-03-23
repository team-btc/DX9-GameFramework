#pragma once
#include "iSceneObject.h"
#include "cUILayer.h"

class cSplashScene : public iSceneObject
{
private:
    float       m_fTransitionTime;
    cUILayer*   m_pImg;

public:
    cSplashScene();
    ~cSplashScene();

    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

