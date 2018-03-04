#include "stdafx.h"
#include "cSceneManager.h"

iSceneObject* cSceneManager::m_pCurrScene = NULL;

cSceneManager::cSceneManager()
{

}

cSceneManager::~cSceneManager()
{

}

HRESULT cSceneManager::Setup()
{
    return E_NOTIMPL;
}

HRESULT cSceneManager::Update()
{
    if (m_pCurrScene != NULL)
    {
        return m_pCurrScene->Update();
    }
    else
    {
        return E_FAIL;
    }
}

HRESULT cSceneManager::Render()
{
    if (m_pCurrScene != NULL)
    {
        return m_pCurrScene->Render();
    }
    else
    {
        return E_FAIL;
    }
}

HRESULT cSceneManager::Destroy()
{
    HRESULT hr = S_OK;
    auto iter = m_scnList.begin();
    for (; iter != m_scnList.end();)
    {
        // 삭제
        if (iter->second != NULL)
        {
            iter->second->Release();
            iter = m_scnList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    m_scnList.clear();

    return hr;
}

HRESULT cSceneManager::AddScene(IN string szName, IN iSceneObject* pScene)
{
    HRESULT hr = S_OK;
    if (pScene)
    {
        pScene->SetName(szName);
        m_scnList.insert(make_pair(szName, pScene));
        hr = S_OK;
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}

HRESULT cSceneManager::FindScene(IN string szName, OUT iSceneObject* pScene)
{
    HRESULT hr = S_OK;
    auto iter = m_scnList.find(szName);
    if (iter != m_scnList.end())
    {
        pScene = iter->second;
        hr = S_OK;
    }
    else
    {
        pScene = NULL;
        hr = E_FAIL;
    }

    return hr;
}

HRESULT cSceneManager::NextScene()
{
    HRESULT hr = S_OK;
    if (m_szNextScene != "")
    {
        ChangeScene(m_szNextScene);
        m_szNextScene = "";
        hr = S_OK;
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}

HRESULT cSceneManager::ChangeScene(IN string szName)
{
    HRESULT hr = S_OK;
    if (szName != "")
    {
        m_szPrevScene = szName;
        auto iter = m_scnList.find(szName);

        if (iter == m_scnList.end())
        {
            g_pLogManager->WriteLog(EL_WARNING, "Scene change failed : Not found");
            hr = E_FAIL;
        }

        if (iter->second == m_pCurrScene)
        {
            g_pLogManager->WriteLog(EL_WARNING, "Scene change failed : Target is current scene");
            hr = E_ABORT;
        }

        if (m_pCurrScene != NULL)
        {
            m_pCurrScene->GetName(m_szPrevScene);
            g_pLogManager->WriteLog(EL_INFO, "Scene release : " + m_szPrevScene);
            m_pCurrScene->Release();
            hr = S_OK;
        }

        m_pCurrScene = iter->second;
        hr = m_pCurrScene->Start();
    }
    else
    {
        hr = E_INVALIDARG;
    }

    return hr;
}
