#pragma once
#include "iSceneObject.h"
#include "cUiLayer.h"
#include "cUIImageView.h"

#define     SCROLL_SPD      0.455f
#define     ENDING_SOUND_PATH    "Assets\\Sound\\BGM\\ending\\"
class cEndingScene : public iSceneObject
{
private:
    cUILayer*           m_pBGLayer;
    float               m_fTexPos;
    float               m_fTexPos2;

    cUIImageView*       m_pUIBG;
    cUIImageView*       m_pUIBG2;

    float               m_fWorldTime;
    bool                m_isStart;
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

