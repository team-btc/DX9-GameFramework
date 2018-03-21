#pragma once
#include "iSingletonManager.h"
#include "iSceneObject.h"

#define g_pScnManager cSceneManager::GetInstance()

class cSceneManager : public iSingletonManager
{
    SINGLETON(cSceneManager);

private:
    map<string, iSceneObject*>                   m_scnList;

    static      iSceneObject*    m_pCurrScene;
    string      m_szNextScene = "";
    string      m_szPrevScene = "";

public:
    HRESULT AddScene(IN string szName, IN iSceneObject* pScene);
    HRESULT FindScene(IN string szName, OUT iSceneObject* pScene);
    HRESULT NextScene();
    HRESULT ChangeScene(IN string szName);

    string GetNextSceneName();
    void SetNextSceneName(IN const string szName);
    string GetPrevSceneName();
    void SetPrevSceneName(IN const string szName);

    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};