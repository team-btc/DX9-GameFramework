#pragma once
#include "iSceneObject.h"
#include "cUiLayer.h"
#include "cUIImageView.h"

#define     SCROLL_SPD      0.354f
class cEndingScene : public iSceneObject
{
private:
    cUILayer*           m_pBGLayer;
    float               m_fTexPos;
    float               m_fTexPos2;

    cUIImageView*       m_pUIBG;
    cUIImageView*       m_pUIBG2;

    bool                m_isStart;
public:
    cEndingScene();
    ~cEndingScene();

    // iSceneObject��(��) ���� ��ӵ�
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

