#include "stdafx.h"
#include "cTitleScene.h"


cTitleScene::cTitleScene()
{
}


cTitleScene::~cTitleScene()
{
}

HRESULT cTitleScene::Start()
{
    //  NO LOADING SCENE
    //  LOAD TITLE SCENE RESOURCES

    return E_NOTIMPL;
}

HRESULT cTitleScene::Update()
{
    return E_NOTIMPL;
}

HRESULT cTitleScene::Render()
{
    //  RENDER BACKGROUND
    //  

    return E_NOTIMPL;
}

ULONG cTitleScene::Release()
{
    cObject::Release();
    return 0;
}

void cTitleScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
