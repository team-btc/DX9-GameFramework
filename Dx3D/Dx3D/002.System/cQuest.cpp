#include "stdafx.h"
#include "cQuest.h"
#include "cPlayer.h"

cQuestIndicator::cQuestIndicator()
    : m_pMesh(NULL)
    , m_isVisible(true)
{
}

cQuestIndicator::~cQuestIndicator()
{

}

void cQuestIndicator::Setup()
{
    m_pMesh = m_pMesh == NULL ? g_pMeshManager->GetSkinnedMesh("quest-indicator") : m_pMesh;
    m_pMesh->SetScale(50.0f);
}

void cQuestIndicator::Update()
{
}

void cQuestIndicator::Render()
{
    if (m_pMesh && m_isVisible)
    {
        m_pMesh->UpdateAndRender();
    }
}

void cQuestIndicator::SetScale(float scale)
{
    if (m_pMesh)
    {
        m_pMesh->SetScale(scale);
    }
}

void cQuestIndicator::SetRotation(Vector3 rotation)
{
    if (m_pMesh)
    {
        m_pMesh->SetRotation(rotation);
    }
}

void cQuestIndicator::SetPosition(Vector3 position)
{
    if (m_pMesh)
    {
        m_pMesh->SetPosition(position);
    }
}

void cQuestIndicator::Release()
{
}

cQuest::cQuest()
    : m_pIndicator(NULL)
    , m_pSprite(NULL)
    , m_pTitleFont(NULL)
    , m_pListFont(NULL)
    , m_szTitle("")
    , m_szList("")
    , m_szCount("")
    , m_fTimer(0.0f)
{
}


cQuest::~cQuest()
{
}

void cQuest::Setup(string map)
{
    m_pIndicator = m_pIndicator == NULL ? new cQuestIndicator : m_pIndicator;
    m_pIndicator->Setup();

    int centerx = W_WIDTH / 2;
    int centery = W_HEIGHT / 2;
    m_rtQuestTitle = { centerx - 400, 200, centerx + 400, 300 };
    m_rtQuestList = { W_WIDTH - 650, 300, W_WIDTH - 50, 700 };

    m_rtQuestTitle2 = m_rtQuestTitle;
    m_rtQuestList2 = m_rtQuestList;

    int offset = 3;
    m_rtQuestTitle2.left += offset;
    m_rtQuestTitle2.top += offset;
    m_rtQuestTitle2.right += offset;
    m_rtQuestTitle2.bottom += offset;

    m_rtQuestList2.left += offset;
    m_rtQuestList2.top += offset;
    m_rtQuestList2.right += offset;
    m_rtQuestList2.bottom += offset;

    m_pTitleFont = g_pFontManager->GetFont(cFontManager::E_ALERT);
    m_pListFont = g_pFontManager->GetFont(cFontManager::E_QUEST);

    if (!m_pSprite)
    {
        D3DXCreateSprite(g_pDevice, &m_pSprite);
    }

    m_vecQuestInfo.clear();
    json q = g_pGameManager->GetQuest(map);
    for (int i = 0; i < (int)q.size(); i++)
    {
        string name = q[i]["name"].is_null() ? "" : q[i]["name"];
        string type = q[i]["type"].is_null() ? "" : q[i]["type"];
        string target = q[i]["target"].is_null() ? "" : q[i]["target"];
        string event = q[i]["event"].is_null() ? "" : q[i]["event"];
        string desc = q[i]["desc"].is_null() ? "" : q[i]["desc"];
        int count = q[i]["count"].is_null() ? 0 : q[i]["count"];
        int goal = q[i]["goal"].is_null() ? 1 : q[i]["goal"];
        int exp = q[i]["exp"].is_null() ? 0 : q[i]["exp"];

        ST_QUEST_INFO elem;
        elem.name = name;
        elem.type = StringToType(type);
        elem.target = target;
        elem.event = event;
        elem.count = count;
        elem.goal = goal;
        elem.desc = desc;
        elem.exp = exp;

        m_vecQuestInfo.push_back(elem);
    }

    SetNextQuest();
}

void cQuest::Update()
{
    if (m_pIndicator)
    {
        m_pIndicator->Update();
    }

    if (m_fTimer < g_pTimerManager->GetWorldTime())
    {
        m_szTitle = "";
    }

    MakeCountMessage();
}

void cQuest::Render()
{
    if (m_pIndicator)
    {
        m_pIndicator->Render();
    }

    DWORD option = DT_CENTER | DT_NOCLIP;
    m_pTitleFont->DrawTextA(NULL, m_szTitle.c_str(), -1, &m_rtQuestTitle2, option, D3DCOLOR_XRGB(0, 0, 0));
    m_pTitleFont->DrawTextA(NULL, m_szTitle.c_str(), -1, &m_rtQuestTitle, option, D3DCOLOR_XRGB(255, 255, 255));
    option = DT_RIGHT | DT_NOCLIP;
    m_pListFont->DrawTextA(NULL, (m_szList + m_szCount).c_str(), -1, &m_rtQuestList2, option, D3DCOLOR_XRGB(0, 0, 0));
    m_pListFont->DrawTextA(NULL, (m_szList + m_szCount).c_str(), -1, &m_rtQuestList, option, D3DCOLOR_XRGB(255, 255, 255));
}

void cQuest::Release()
{
    m_pIndicator->Release();
    m_pSprite->Release();
}

void cQuest::SetNextQuest()
{
    auto iter = m_vecQuestInfo.begin();
    if (iter != m_vecQuestInfo.end())
    {
        switch (iter->type)
        {
        case Q_MOVE:
        {
            m_pIndicator->SetVisible(true);
            ST_EVENT_INFO eventInfo = g_pMapManager->GetEventByName(iter->event);
            m_pIndicator->SetPosition(eventInfo.vPos);
            break;
        }
        case Q_HUNT:
        {
            m_pIndicator->SetVisible(false);
            break;
        }
        default:
        {
            m_stProgress = ST_QUEST_INFO();
            m_pIndicator->SetVisible(false);
            break;
        }
        }

        m_stProgress = *iter;
        m_vecQuestInfo.erase(iter);
    }
    else
    {
        m_stProgress = ST_QUEST_INFO();
        m_pIndicator->SetVisible(false);
    }

    m_fTimer = g_pTimerManager->GetWorldTime() + 5.0f;
    m_szTitle = m_stProgress.name;
    m_szList = m_stProgress.desc;
}

void cQuest::EmitMessage(string event)
{
    transform(event.begin(), event.end(), event.begin(), tolower);

    switch (m_stProgress.type)
    {
    case Q_MOVE:
    {
        if (m_stProgress.event.compare(event) == 0)
        {
            m_stProgress.count++;
        }
        break;
    }
    case Q_HUNT:
    {
        if (m_stProgress.target.compare(event) == 0)
        {
            m_stProgress.count++;
        }
        else
        {
            m_szCount = "";
        }
        break;
    }
    }

    if (m_stProgress.goal <= m_stProgress.count)
    {
        cPlayer* p = g_pCharacterManager->GetPlayer();
        ST_STATUS& status = p->GetStatus();
        status.nCurEXP += m_stProgress.exp;
        SetNextQuest();
    }
}

void cQuest::MakeCountMessage()
{
    if (m_stProgress.type != Q_END)
    {
        m_szCount = "\n" + to_string(m_stProgress.count) + " / " + to_string(m_stProgress.goal);
    }
    else
    {
        m_szCount = "";
    }
}

E_QUEST_TYPE cQuest::StringToType(string type)
{
    if (type == "MOVE")
    {
        return Q_MOVE;
    }
    else if (type == "HUNT")
    {
        return Q_HUNT;
    }
    else
    {
        return Q_END;
    }
}
