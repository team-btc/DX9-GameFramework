#pragma once

#define g_pScnManager cSceneManager::GetInstance()

class cGameNode;
class cSceneManager
{
    SINGLETON(cSceneManager);
private:
    map<string, cGameNode*>                   m_scnList;
    map<string, cGameNode*>::iterator         m_scnIter;

    static cGameNode*        m_pCurrScene;
    string                  m_szNextScene = "";
    string                  m_szPrevScene = "";

public:
#pragma region CRUD
    //  CREATE
    cGameNode* AddScene(string ScnName, cGameNode* Scene);
    //  READ
    cGameNode* FindScene(string ScnName);
    //  UPDATE
    void NextScene();
    void ChangeScene(string ScnName);
    void Update(float dt);
    void Render();
    //  DELETE
    void ReleaseAll();
#pragma endregion

    string  GetNextScene() { return m_szNextScene; }
    void    SetNextScene(string Scene) { m_szNextScene = Scene; }
    string  GetPrevScene() { return m_szPrevScene; }
    void    SetPrevScene(string Scene) { m_szPrevScene = Scene; }
};