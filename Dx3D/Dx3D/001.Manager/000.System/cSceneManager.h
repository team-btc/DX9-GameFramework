#pragma once
#include "iSceneObject.h"
#define g_pScnManager cSceneManager::GetInstance()

class cSceneManager
{
    SINGLETON(cSceneManager);

private:
    map<string, iSceneObject*>                   m_scnList;

    static      iSceneObject*    m_pCurrScene;
    string      m_szNextScene = "";
    string      m_szPrevScene = "";

public:
#pragma region CRUD
    //  CREATE
    HRESULT AddScene(IN string szName, IN iSceneObject* pScene);
    //  READ
    HRESULT FindScene(IN string szName, OUT iSceneObject* pScene);
    //  UPDATE
    HRESULT NextScene();
    HRESULT ChangeScene(IN string szName);
    HRESULT Update();
    HRESULT Render();
    //  DELETE
    HRESULT Destroy();
#pragma endregion

    HRESULT GetNextScene(OUT string& szName, OUT iSceneObject* pScene = NULL);
    HRESULT SetNextScene(IN const string szName);
    HRESULT GetPrevScene(OUT string& szName, OUT iSceneObject* pScene = NULL);
    HRESULT SetPrevScene(IN const string szName);
};