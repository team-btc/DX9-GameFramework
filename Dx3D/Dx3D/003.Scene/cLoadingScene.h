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

    bool                        m_isMapDefLoad;                 // �⺻ �ε� �Ϸ� ����
    int                         m_nObjectMaxCnt;
    int                         m_nObjectCurrCnt;
    float                       m_fMaxCount;
    float                       m_fCurrCount;

public:
    cLoadingScene();
    ~cLoadingScene();

    // iSceneObject��(��) ���� ��ӵ�
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

