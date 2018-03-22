#include "stdafx.h"
#include "cQuest.h"


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
    m_rtQuestList = { W_WIDTH - 450, 300, W_WIDTH - 50, 700 };

    m_rtQuestTitle2 = m_rtQuestTitle;
    m_rtQuestList2 = m_rtQuestList;

    m_rtQuestTitle2.left += 10;
    m_rtQuestTitle2.top += 10;
    m_rtQuestTitle2.right += 10;
    m_rtQuestTitle2.bottom += 10;

    m_rtQuestList2.left += 10;
    m_rtQuestList2.top += 10;
    m_rtQuestList2.right += 10;
    m_rtQuestList2.bottom += 10;

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

        ST_QUEST_INFO elem;
        elem.name = name;
        elem.type = StringToType(type);
        elem.target = target;
        elem.event = event;
        elem.count = count;
        elem.goal = goal;
        elem.desc = desc;

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
}

void cQuest::Render()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    if (m_pIndicator)
    {
        m_pIndicator->Render();
    }

    //m_pSprite->Begin(D3DXSPRITE_BILLBOARD | D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    DWORD option = DT_CENTER | DT_NOCLIP;
    m_pTitleFont->DrawTextA(NULL, m_szTitle.c_str(), -1, &m_rtQuestTitle2, option, D3DCOLOR_XRGB(255, 255, 255));
    m_pTitleFont->DrawTextA(NULL, m_szTitle.c_str(), -1, &m_rtQuestTitle, option, D3DCOLOR_XRGB(0, 0, 0));
    option = DT_LEFT | DT_NOCLIP;
    m_pListFont->DrawTextA(NULL, m_szList.c_str(), -1, &m_rtQuestList2, option, D3DCOLOR_XRGB(255, 255, 255));
    m_pListFont->DrawTextA(NULL, m_szList.c_str(), -1, &m_rtQuestList, option, D3DCOLOR_XRGB(0, 0, 0));
    //m_pSprite->End();

    g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
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
        m_szTitle = m_stProgress.name;
        m_szList = m_stProgress.desc;

        switch (iter->type)
        {
        case Q_MOVE:
        {
            m_pIndicator->SetVisible(true);
            ST_EVENT_INFO eventInfo = g_pMapManager->GetEventByName(iter->event);
            m_pIndicator->SetPosition(eventInfo.vPos);
            m_pIndicator->SetScale(10.0f);
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
}

void cQuest::EmitMessage(string event)
{
    if (m_stProgress.event.compare(event) == 0)
    {
        m_stProgress.count++;
        m_szTitle = "MOVED !!!";
    }

    if (m_stProgress.goal <= m_stProgress.count)
    {
        SetNextQuest();
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
