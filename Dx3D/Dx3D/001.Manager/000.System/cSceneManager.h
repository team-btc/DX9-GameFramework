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

    HRESULT GetNextScene(OUT string& szName, OUT iSceneObject* pScene = NULL);
    HRESULT SetNextScene(IN const string szName);
    HRESULT GetPrevScene(OUT string& szName, OUT iSceneObject* pScene = NULL);
    HRESULT SetPrevScene(IN const string szName);

    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};