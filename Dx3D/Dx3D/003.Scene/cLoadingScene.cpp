#include "stdafx.h"
#include "cLoadingScene.h"


cLoadingScene::cLoadingScene()
{
}


cLoadingScene::~cLoadingScene()
{
}

HRESULT cLoadingScene::Start()
{
    //  MAKE RESOURCE LIST
    return E_NOTIMPL;
}

HRESULT cLoadingScene::Update()
{
    //  LOAD RESOURCES

    return E_NOTIMPL;
}

HRESULT cLoadingScene::Render()
{
    //  BACKGROUND IMAGE
    //  PROGRESS BAR

    return E_NOTIMPL;
}

ULONG cLoadingScene::Release()
{
    cObject::Release();
    return 0;
}

void cLoadingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
