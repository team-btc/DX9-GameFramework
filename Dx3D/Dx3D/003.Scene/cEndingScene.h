#pragma once
#include "iSceneObject.h"

class cEndingScene : public iSceneObject
{
public:
    cEndingScene();
    ~cEndingScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

