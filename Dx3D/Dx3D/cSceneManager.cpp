#include "stdafx.h"
#include "cSceneManager.h"
#include "cGameNode.h"

cGameNode* cSceneManager::m_pCurrScene = NULL;

cSceneManager::cSceneManager()
{

}

cSceneManager::~cSceneManager()
{

}

cGameNode* cSceneManager::AddScene(string ScnName, cGameNode * Scene)
{
    if (Scene) 
        m_scnList.insert(make_pair(ScnName, Scene));
    return Scene;
}

cGameNode * cSceneManager::FindScene(string ScnName)
{
    return m_scnList.find(ScnName)->second;
}

void cSceneManager::NextScene()
{
    if (m_szNextScene != "")
    {
        ChangeScene(m_szNextScene);
        m_szNextScene = "";
    }
}

void cSceneManager::ChangeScene(string ScnName)
{
    if (m_szPrevScene == "")
    {
        m_szPrevScene = ScnName;
    }
    m_scnIter = m_scnList.find(ScnName);

    if (m_scnIter == m_scnList.end())
        g_pLogManager->WriteLog(EL_WARNING, "Scene change failed : Not found");

    if (m_scnIter->second == m_pCurrScene)
        g_pLogManager->WriteLog(EL_WARNING, "Scene change failed : Target is current scene");

    if (m_pCurrScene != NULL)
    {
        m_szPrevScene = m_pCurrScene->GetTagName();
        g_pLogManager->WriteLog(EL_INFO, "Scene release : " + m_szPrevScene);
        m_pCurrScene->Destroy();
    }

    m_pCurrScene = m_scnIter->second;
    m_pCurrScene->Start();
}

void cSceneManager::Update(float dt)
{
    if (m_pCurrScene != NULL)
    {
        m_pCurrScene->Update(dt);
    }
}

void cSceneManager::Render()
{
    if (m_pCurrScene != NULL)
    {
        m_pCurrScene->Render();
    }
}

void cSceneManager::ReleaseAll()
{
    m_scnIter = m_scnList.begin();
    for (; m_scnIter != m_scnList.end();)
    {
        // 삭제
        if (m_scnIter->second != NULL)
        {
            m_scnIter->second->Destroy();
            SAFE_DELETE(m_scnIter->second);
            m_scnIter = m_scnList.erase(m_scnIter);
        }
        else
        {
            ++m_scnIter;
        }
    }

    m_scnList.clear();
}
