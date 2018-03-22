#pragma once
#include "cSkinnedMesh.h"

class cQuestIndicator
{
private:
    cSkinnedMesh*       m_pMesh;
    bool                m_isVisible;

public:
    cQuestIndicator();
    ~cQuestIndicator();

    void Setup();
    void Update();
    void Render();
    void Release();

    void SetScale(float scale);
    void SetRotation(Vector3 rotation);
    void SetPosition(Vector3 position);

    void SetVisible(bool val) { m_isVisible = val; }
};

class cQuest
{
private:
    ST_QUEST_INFO                   m_stProgress;
    vector<ST_QUEST_INFO>           m_vecQuestInfo;
    cQuestIndicator*                m_pIndicator;

    RECT        m_rtQuestTitle;
    RECT        m_rtQuestList;
    RECT        m_rtQuestTitle2;
    RECT        m_rtQuestList2;

    LPSPRITE    m_pSprite;
    LPFONTDX    m_pTitleFont;
    LPFONTDX    m_pListFont;

    string      m_szTitle;
    string      m_szList;

public:
    cQuest();
    ~cQuest();

    void Setup(string map);
    void Update();
    void Render();
    void Release();

    void SetNextQuest();

    void EmitMessage(string event);

private:
    E_QUEST_TYPE StringToType(string type);
};

