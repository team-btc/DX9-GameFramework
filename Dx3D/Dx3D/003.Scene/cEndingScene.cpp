#include "stdafx.h"
#include "cEndingScene.h"


cEndingScene::cEndingScene()
{
}


cEndingScene::~cEndingScene()
{
}

HRESULT cEndingScene::Start()
{
    //  LOAD ENDING CREDIT IMAGE

    return E_NOTIMPL;
}

HRESULT cEndingScene::Update()
{
    //  SCROLL IMAGE

    return E_NOTIMPL;
}

HRESULT cEndingScene::Render()
{
    //  ENDING CREDIT IMAGE RENDER

    return E_NOTIMPL;
}

ULONG cEndingScene::Release()
{
    cObject::Release();
    return 0;
}

void cEndingScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
