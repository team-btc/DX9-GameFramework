#pragma once
#include "stdafx.h"
#include "cObject.h"

class iSceneObject : public cObject
{
protected:

public:
    virtual HRESULT Start() PURE;
    virtual HRESULT Update() PURE;
    virtual HRESULT Render() PURE;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) PURE;
};