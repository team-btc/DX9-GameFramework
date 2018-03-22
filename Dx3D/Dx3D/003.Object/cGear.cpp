#include "stdafx.h"
#include "cGear.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cGear::cGear()
    : m_pGearLayer(NULL)
    , m_pPlusStatLayer(NULL)
    , m_nCurrSelectGear(-1)
    , m_isClickGear(false)
    , m_isOpen(false)
{
    m_rtGearSize.left = W_WIDTH - (LONG)(W_WIDTH * 0.18f) - 300;
    m_rtGearSize.top = 100;
    m_rtGearSize.right = m_rtGearSize.left + (LONG)(W_WIDTH * 0.18f);
    m_rtGearSize.bottom = m_rtGearSize.top + (LONG)(W_HEIGHT * 0.35f);

    m_stGearSlotSize = ST_SIZE(60, 60);

    m_stPlusStatDefSize = ST_SIZE(100, 20);

    g_pSndManager->AddSound("gear-open", "gear", "Assets\\Sound\\Effect\\Interface\\CharacterSheet\\open.ogg");
    g_pSndManager->AddSound("gear-close", "gear", "Assets\\Sound\\Effect\\Interface\\CharacterSheet\\close.ogg");
    g_pSndManager->AddSound("gear-click", "gear", "Assets\\Sound\\Effect\\Interface\\CharacterSheet\\click.ogg");
    g_pSndManager->AddSound("gear-pull", "gear", "Assets\\Sound\\Effect\\Common\\pickuplargechain.ogg");
}

cGear::~cGear()
{
}

void cGear::Setup()
{
    m_pGearLayer = new cUILayer;
    m_pGearLayer->SetLayer("Inven", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pGearLayer->SetActive(true);

    m_pPlusStatLayer = new cUILayer;
    m_pPlusStatLayer->SetLayer("plus-stat", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pPlusStatLayer->SetActive(false);

    m_stCurrGear = g_pGameManager->GetGearInfo();

    SetGearUI();
    SetPlusStatUI();

    if (m_pGearLayer)
    {
        m_pGearLayer->Setup();
    }
    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Setup();
    }
}

void cGear::Update(ST_STATUS stPlayerStat)
{
    m_isClickGear = false;

    if (!m_pGearLayer)
    {
        return;
    }

    // 기본 정보 로드
    m_stCurrGear = g_pGameManager->GetGearInfo();
    // 계속해서 장비와 스탯 체크하고 업데이트
    UpdateGearUI(stPlayerStat);

    // 장비창 렉트 안에 마우스가 있다면
    if (PtInRect(&m_rtGearSize, g_ptMouse))
    {
        // 버튼 클릭 무효화를 위해 
        g_pKeyManager->isOnceKeyDown(VK_LBUTTON);
        g_pKeyManager->isOnceKeyDown(VK_RBUTTON);
        m_isClickGear = true;

        // 아이템에 마우스가 올려져있는지 체크하고 레이어 셋팅
        if (!CheckPtInItem())
        {
            m_pPlusStatLayer->SetActive(false);
        }
    }
    else
    {
        m_pPlusStatLayer->SetActive(false);
    }

    // 버튼 클릭을 했다면
    if (m_pGearLayer->IsClickButton())
    {
        string szClickBtnName = m_pGearLayer->GetClickButtonName();

        // X버튼 클릭 시
        if (!strcmp(szClickBtnName.c_str(), "gear-x-btn"))
        {
            CloseGear();
        }
        // 해지 버튼 클릭 시
        else if (!strcmp(szClickBtnName.c_str(), "gear-pull-btn"))
        {
            // 아이템을 선택한 상태라면
            if (m_nCurrSelectGear != -1)
            {
                g_pSndManager->Play("gear-pull");
                // 아이템 해지 하기!!!!!!!!!!!!!!!!!11 -> 효과 해지!!!
                if (m_nCurrSelectGear == ST_ITEM_INFO::E_HELM && m_stCurrGear.helmId != -1)
                {
                    g_pGameManager->PullGear(m_stCurrGear.helmId);
                }
                else if (m_nCurrSelectGear == ST_ITEM_INFO::E_CHEST_PLATE && m_stCurrGear.chestPlateId != -1)
                {
                    g_pGameManager->PullGear(m_stCurrGear.chestPlateId);
                }
                else if (m_nCurrSelectGear == ST_ITEM_INFO::E_SWORD && m_stCurrGear.swordId != -1)
                {
                    g_pGameManager->PullGear(m_stCurrGear.swordId);
                }
                // 선택 이미지 해지
                cUIObject* pUIObject;
                char Buf[20];
                sprintf_s(Buf, sizeof(Buf), "gear-select-%d", m_nCurrSelectGear);
                pUIObject = m_vecGearUI[m_nCurrSelectGear]->GetChildByName(Buf);
                if (pUIObject)
                {
                    pUIObject->SetAxtive(false);
                }
                m_nCurrSelectGear = -1;
            }
        }
        // 선택 버튼 클릭 시
        for (int i = 0; i < m_vecGearUI.size(); ++i)
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), "gear-btn-%d", i);
            if (!strcmp(szClickBtnName.c_str(), Buf))
            {
                // 현재 선택된 아이템이 아닌 다른 아이템을 클릭 했으면
                if (m_nCurrSelectGear != i)
                {
                    g_pSndManager->Play("gear-click");

                    m_nCurrSelectGear = i;

                    for (int j = 0; j < m_vecGearUI.size(); ++j)
                    {
                        // 선택 이미지 활성화, 다른 이미지 비활성화
                        cUIObject* pUIObject;
                        sprintf_s(Buf, sizeof(Buf), "gear-select-%d", j);
                        m_pGearLayer->FindUIObject(&pUIObject, Buf);
                        if (pUIObject)
                        {
                            if (i == j)
                            {
                                pUIObject->SetAxtive(true);
                            }
                            else
                            {
                                pUIObject->SetAxtive(false);
                            }
                        }
                    }
                }
                break;
            }
        }
    }

    m_pGearLayer->Update();

    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Update();
    }
}

void cGear::Render()
{
    if (m_pGearLayer)
    {
        m_pGearLayer->Render();
    }
    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Render();
    }
}

void cGear::OpenGear()
{
    g_pSndManager->Play("gear-open");

    m_isClickGear = false;
    m_isOpen = true;
    m_stCurrGear = g_pGameManager->GetGearInfo();
}

void cGear::CloseGear()
{
    g_pSndManager->Play("gear-close");

    m_isClickGear = false;
    m_isOpen = false;
}

ULONG cGear::Release(void)
{
    SAFE_DELETE(m_pGearLayer);
    SAFE_DELETE(m_pPlusStatLayer);

    return cObject::Release();
}

void cGear::SetGearUI()
{
    Vector3 vGearPos((float)m_rtGearSize.left, (float)m_rtGearSize.top, 0);
    ST_SIZE stGearSize((float)m_rtGearSize.right - (float)m_rtGearSize.left, (float)m_rtGearSize.bottom - (float)m_rtGearSize.top);
    string szPath;
    IMAGE_INFO imageInfo;

    // 캐릭터 초상화
    cUIImageView* pCharacterImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"gear-character.png";
    g_pTextureManager->AddTexture("gear-character-img", szPath, true);
    pCharacterImage->SetName("gear-character-img");
    pCharacterImage->SetLocalPos(Vector3(vGearPos.x + 10, vGearPos.y + 5, 0));
    pCharacterImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("gear-character-img", &imageInfo));
    pCharacterImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pCharacterImage->SetScale(70 / (float)imageInfo.Width, 70 / (float)imageInfo.Height);
    m_pGearLayer->AddUIObject(pCharacterImage);

    // 장비창 배경
    cUIImageView* pBGImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"gear-bg.png";
    g_pTextureManager->AddTexture("gear-bg-img", szPath, true);
    pBGImage->SetName("gear-bg-img");
    pBGImage->SetLocalPos(vGearPos);
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("gear-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stGearSize.w / (float)imageInfo.Width, stGearSize.h / (float)imageInfo.Height);
    m_pGearLayer->AddUIObject(pBGImage);

    // 장비창 이름
    cUITextView* pGearNameText = new cUITextView;
    pGearNameText->SetName("gear-name-tex");
    pGearNameText->SetLocalPos(Vector3(vGearPos.x + 120, vGearPos.y + 15, 0));
    pGearNameText->SetSize(Vector2(100, 15));
    pGearNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_TITLE));
    pGearNameText->SetText("GEAR");
    pGearNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pGearLayer->AddUIObject(pGearNameText);

    // X버튼
    cUIButton* pXButton = new cUIButton;
    pXButton->SetName("gear-x-btn");
    pXButton->SetLocalPos(Vector3(vGearPos.x + stGearSize.w - 33, vGearPos.y + 5, 0));
    pXButton->SetButtonState();
    szPath = INTERFACE_PATH + (string)"button/x-button-mouse-over.png";
    g_pTextureManager->AddTexture("x-mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/x-button-select.png";
    g_pTextureManager->AddTexture("x-select-btn", szPath, true);
    pXButton->SetTexture("x-mouse-over-btn", "x-mouse-over-btn", "x-select-btn");
    pXButton->SetSize(Vector2(38, 36));
    pXButton->SetUIButton(m_pGearLayer);
    m_pGearLayer->AddUIObject(pXButton);

    SetStatText(vGearPos);
    SetStatUI(vGearPos);
}

void cGear::SetStatText(Vector3 vGearPos)
{
    float fBaseX = vGearPos.x + 40;
    float fPlusX = 50;
    float fPlusY = 25;
    Vector3 vTextPos(fBaseX, vGearPos.y + 90, 0);
    Vector2 vTitleSize(45, 20);
    Vector2 vValueSize(110, 20);
    DWORD dTitleFontColor = D3DCOLOR_XRGB(255, 255, 0);
    DWORD dValueFontColor = D3DCOLOR_XRGB(255, 255, 255);

    // 레벨 :
    m_pGearLayer->AddUIObject(MakeStatTextUI("Level", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    cUITextView* UIText = MakeStatTextUI(": 1", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_LEVEL, UIText));

    // 이름 :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Name", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": name", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_NAME, UIText));

    // 공격력 :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Att", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 12", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_ATT, UIText));

    // 방어력 :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Def", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 34", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_DEF, UIText));

    // 힘 :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Str", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 56", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_STR, UIText));

    // 민첩 :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Dex", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 78", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_DEX, UIText));

    // 지능 :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Int", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 90", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_INT, UIText));
}

cUITextView* cGear::MakeStatTextUI(string name, Vector3 pos, Vector2 size, DWORD color)
{
    cUITextView* pTextUI = new cUITextView;
    pTextUI->SetName(name);
    pTextUI->SetLocalPos(pos);
    pTextUI->SetSize(size);
    pTextUI->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    pTextUI->SetText(name);
    pTextUI->SetDrawTextFormat(DT_LEFT);
    pTextUI->SetColor(color);

    return pTextUI;
}

void cGear::SetStatUI(Vector3 vGearPos)
{
    // 아이템 이미지
    ST_SIZE stSlotSize = m_stGearSlotSize;
    Vector3 vItemPos(vGearPos.x + 200, vGearPos.y + 85, 0);
    float fPlusY = stSlotSize.h + 5;

    for (int i = 0; i < 3; ++i)
    {
        if (i != 0)
        {
            vItemPos.y += fPlusY;
        }

        // 렉트 만들기
        RECT rtItem;
        rtItem.left = (LONG)vItemPos.x;
        rtItem.top = (LONG)vItemPos.y;
        rtItem.right = rtItem.left + (LONG)stSlotSize.w;
        rtItem.bottom = rtItem.top + (LONG)stSlotSize.h;
        m_vecItemRect.push_back(rtItem);

        // 슬롯 이미지
        string szSlotPath;
        string szSlotName;
        if (i == ST_ITEM_INFO::E_HELM)
        {
            szSlotPath = INTERFACE_PATH + (string)"item/helm-slot.png";
            szSlotName = "helm-slot-img";
        }
        else if (i == ST_ITEM_INFO::E_CHEST_PLATE)
        {
            szSlotPath = INTERFACE_PATH + (string)"item/chest-plate-slot.png";
            szSlotName = "chest-plate-slot-img";
        }
        else//E_SWORD
        {
            szSlotPath = INTERFACE_PATH + (string)"item/sword-slot.png";
            szSlotName = "sword-slot-img";
        }
        g_pTextureManager->AddTexture(szSlotName, szSlotPath, true);

        cUIImageView* pSlotImage = new cUIImageView;
        pSlotImage->SetName(szSlotName);
        pSlotImage->SetLocalPos(vItemPos);
        IMAGE_INFO imageInfo;
        pSlotImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szSlotName, &imageInfo));
        pSlotImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pSlotImage->SetScale(stSlotSize.w / (float)imageInfo.Width, stSlotSize.h / (float)imageInfo.Height);
        m_pGearLayer->AddUIObject(pSlotImage);

        // 장비 이미지
        cUIImageView* pGearImage = new cUIImageView;
        pGearImage->SetLocalPos(vItemPos);
        char Buf[20];
        sprintf_s(Buf, sizeof(Buf), "gear-img-%d", i);
        pGearImage->SetName(Buf);
        pGearImage->SetAxtive(false);
        m_pGearLayer->AddUIObject(pGearImage);

        // 이미지 변경을 위해
        m_vecGearUI.push_back(pGearImage);

        // 선택 버튼
        cUIButton* pSelectButton = new cUIButton;
        sprintf_s(Buf, sizeof(Buf), "gear-btn-%d", i);
        pSelectButton->SetName(Buf);
        pSelectButton->SetLocalPos(Vector3(0, 0, 0));
        pSelectButton->SetButtonState();
        pSelectButton->SetTexture("", "", "");
        pSelectButton->SetSize(Vector2(stSlotSize.w, stSlotSize.h));
        pSelectButton->SetUIButton(m_pGearLayer);
        pSelectButton->SetAxtive(false);
        pGearImage->AddChild(pSelectButton);

        // 선택 이미지
        cUIImageView* pItemSelectImage = new cUIImageView;
        g_pTextureManager->AddTexture("gear-select", INTERFACE_PATH + (string)"item/item-select.png", true);
        sprintf_s(Buf, sizeof(Buf), "gear-select-%d", i);
        pItemSelectImage->SetName(Buf);
        pItemSelectImage->SetLocalPos(Vector3(-2, -2, 0));
        pItemSelectImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("item-select", &imageInfo));
        pItemSelectImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemSelectImage->SetScale((stSlotSize.w + 4) / (float)imageInfo.Width, (stSlotSize.h + 4) / (float)imageInfo.Height);
        pItemSelectImage->SetAxtive(false);
        pGearImage->AddChild(pItemSelectImage);

        // 해당 칸에 보여질 아이템이 있으면
        if (i == ST_ITEM_INFO::E_HELM && m_stCurrGear.helmId != -1)
        {
            ST_ITEM_INFO* stItemInfo = g_pGameManager->GetItemInfoById(m_stCurrGear.helmId);
            string szPath = stItemInfo->szPath;
            string szTexKey = stItemInfo->szName;
            g_pTextureManager->AddTexture(szTexKey, szPath, true);
            IMAGE_INFO imageInfo;
            pGearImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
            pGearImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
            pGearImage->SetScale(stSlotSize.w / (float)imageInfo.Width, stSlotSize.h / (float)imageInfo.Height);
            pGearImage->SetAxtive(true);
            pSelectButton->SetAxtive(true);
        }
        else if (i == ST_ITEM_INFO::E_CHEST_PLATE && m_stCurrGear.chestPlateId != -1)
        {
            ST_ITEM_INFO* stItemInfo = g_pGameManager->GetItemInfoById(m_stCurrGear.chestPlateId);
            string szPath = stItemInfo->szPath;
            string szTexKey = stItemInfo->szName;
            g_pTextureManager->AddTexture(szTexKey, szPath, true);
            IMAGE_INFO imageInfo;
            pGearImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
            pGearImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
            pGearImage->SetScale(stSlotSize.w / (float)imageInfo.Width, stSlotSize.h / (float)imageInfo.Height);
            pGearImage->SetAxtive(true);
            pSelectButton->SetAxtive(true);
        }
        else if (i == ST_ITEM_INFO::E_SWORD && m_stCurrGear.swordId != -1)
        {
            ST_ITEM_INFO* stItemInfo = g_pGameManager->GetItemInfoById(m_stCurrGear.swordId);
            string szPath = stItemInfo->szPath;
            string szTexKey = stItemInfo->szName;
            g_pTextureManager->AddTexture(szTexKey, szPath, true);
            IMAGE_INFO imageInfo;
            pGearImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
            pGearImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
            pGearImage->SetScale(stSlotSize.w / (float)imageInfo.Width, stSlotSize.h / (float)imageInfo.Height);
            pGearImage->SetAxtive(true);
            pSelectButton->SetAxtive(true);
        }
    }

    // 해지 버튼
    vItemPos.y += fPlusY;
    cUIButton* pPullButton = new cUIButton;
    pPullButton->SetName("gear-pull-btn");
    pPullButton->SetLocalPos(vItemPos);
    pPullButton->SetButtonState();
    string szPath = INTERFACE_PATH + (string)"button/button-normal.png";
    g_pTextureManager->AddTexture("normal-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/button-mouse-over.png";
    g_pTextureManager->AddTexture("mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/button-select.png";
    g_pTextureManager->AddTexture("select-btn", szPath, true);
    pPullButton->SetTexture("normal-btn", "mouse-over-btn", "select-btn");
    pPullButton->SetSize(Vector2(stSlotSize.w, stSlotSize.h * 0.3f));
    pPullButton->SetUIButton(m_pGearLayer);
    m_pGearLayer->AddUIObject(pPullButton);

    // 해지 이름
    cUITextView* pPullText = new cUITextView;
    pPullText->SetName("gear-pull-tex");
    pPullText->SetLocalPos(vItemPos);
    pPullText->SetSize(Vector2(stSlotSize.w, stSlotSize.h * 0.3f));
    pPullText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    pPullText->SetText("PULL");
    pPullText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pGearLayer->AddUIObject(pPullText);
}

void cGear::UpdateGearUI(ST_STATUS stPlayerStat)
{
    // 스탯 셋팅
    for (auto iter = m_mapTextUI.begin(); iter != m_mapTextUI.end(); ++iter)
    {
        switch (iter->first)
        {
        case E_PLAYER_LEVEL:
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), ": %d", stPlayerStat.Level);
            iter->second->SetText(Buf);
        }
            break;
        case E_PLAYER_NAME:
        {
            string szName = ": " + stPlayerStat.szName;
            iter->second->SetText(szName);
        }
            break;
        case E_PLAYER_ATT:
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), ": %d", (int)stPlayerStat.fATK);
            iter->second->SetText(Buf);
        }
            break;
        case E_PLAYER_DEF:
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), ": %d", (int)stPlayerStat.fDEF);
            iter->second->SetText(Buf);
        }
            break;
        case E_PLAYER_STR:
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), ": %d", (int)stPlayerStat.fSTR);
            iter->second->SetText(Buf);
        }
            break;
        case E_PLAYER_DEX:
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), ": %d", (int)stPlayerStat.fDEX);
            iter->second->SetText(Buf);
        }
            break;
        case E_PLAYER_INT:
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), ": %d", (int)stPlayerStat.fINT);
            iter->second->SetText(Buf);
        }
            break;
        }
    }

    // 아이템 체크
    for (int i = 0; i < m_vecGearUI.size(); ++i)
    {
        int nId;
        if (i == ST_ITEM_INFO::E_HELM)
        {
            nId = m_stCurrGear.helmId;
        }
        else if (i == ST_ITEM_INFO::E_CHEST_PLATE)
        {
            nId = m_stCurrGear.chestPlateId;
        }
        else
        {
            nId = m_stCurrGear.swordId;
        }

        if (nId != -1)
        {
            ST_ITEM_INFO* stItemInfo = g_pGameManager->GetItemInfoById(nId);
            string szPath = stItemInfo->szPath;
            string szTexKey = stItemInfo->szName;
            g_pTextureManager->AddTexture(szTexKey, szPath, true);
            IMAGE_INFO imageInfo;
            m_vecGearUI[i]->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
            m_vecGearUI[i]->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
            m_vecGearUI[i]->SetScale(m_stGearSlotSize.w / (float)imageInfo.Width, m_stGearSlotSize.h / (float)imageInfo.Height);
            m_vecGearUI[i]->SetAxtive(true);
            // 선택 버튼 활성화
            cUIObject* pUIObject;
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), "gear-btn-%d", i);
            pUIObject = m_vecGearUI[i]->GetChildByName(Buf);
            if (pUIObject)
            {
                pUIObject->SetAxtive(true);
            }
        }
        else
        {
            m_vecGearUI[i]->SetAxtive(false);
            // 선택 버튼 비활성화
            cUIObject* pUIObject;
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), "gear-btn-%d", i);
            pUIObject = m_vecGearUI[i]->GetChildByName(Buf);
            if (pUIObject)
            {
                pUIObject->SetAxtive(false);
            }
        }
    }
}

void cGear::SetPlusStatUI()
{
    // 배경
    cUIImageView* pBGImage = new cUIImageView;
    string szPath = INTERFACE_PATH + (string)"stat-bg.png";
    g_pTextureManager->AddTexture("stat-bg-img", szPath, true);
    pBGImage->SetName("stat-bg-img");
    pBGImage->SetLocalPos(Vector3(0, 0, 0));
    IMAGE_INFO imageInfo;
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("stat-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(m_stPlusStatDefSize.w / (float)imageInfo.Width, m_stPlusStatDefSize.h / (float)imageInfo.Height);
    m_pPlusStatLayer->AddUIObject(pBGImage);

    Vector3 vPos(5, 5, 0);
    float fPlusY = 17;

    // 증가 스탯 텍스트
    for (int i = 0; i < 4; ++i)
    {
        cUITextView* pText = new cUITextView;
        char buf[20];
        sprintf_s(buf, sizeof(buf), "stat-%d", i);
        pText->SetName(buf);
        pText->SetLocalPos(vPos);
        pText->SetSize(Vector2(100, 15));
        pText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_SMALL));
        pText->SetDrawTextFormat(DT_LEFT);
        pText->SetColor(D3DCOLOR_XRGB(0, 200, 0));
        pText->SetAxtive(false);
        pBGImage->AddChild(pText);

        vPos.y += fPlusY;
    }
}

void cGear::UpdatePlusStatUI(int id, Vector3 vPos)  // 렉트 안에 마우스가 있을 때만 발동
{
    ST_ITEM_INFO stItem = *g_pGameManager->GetItemInfoById(id);

    // 증가 스탯 업데이트
    for (int i = 0; i < 4; ++i)
    {
        cUIObject* pObject;
        char buf[20];
        sprintf_s(buf, sizeof(buf), "stat-%d", i);
        m_pPlusStatLayer->FindUIObject(&pObject, buf);

        if (pObject)
        {
            cUITextView* pText = (cUITextView*)pObject;

            // 셋팅해야 하는 플러스 스탯이 있으면
            if (i < stItem.vecPlusStat.size())
            {
                string szPlusName = g_pGameManager->GetStatName((E_PLAYER_STAT)stItem.vecPlusStat[i].nType);
                sprintf_s(buf, sizeof(buf), " +%d", (int)stItem.vecPlusStat[i].fPlusValue);
                string sztotal = szPlusName + buf;
                pText->SetText(sztotal);
                pText->SetAxtive(true);
            }
            else
            {
                pText->SetAxtive(false);
            }
        }
    }
    // 배경 사이즈 바꾸기
    cUIObject* pObject;
    m_pPlusStatLayer->FindUIObject(&pObject, "stat-bg-img");

    if (pObject)
    {
        pObject->SetLocalPos(vPos);
        cUIImageView* pImage = (cUIImageView*)pObject;
        ST_SIZE stSize;
        stSize.h = m_stPlusStatDefSize.h * stItem.vecPlusStat.size();
        stSize.w = m_stPlusStatDefSize.w;
        pImage->SetScale((float)stSize.w / pImage->GetSize().x, (float)stSize.h / pImage->GetSize().y);
    }

    m_pPlusStatLayer->SetActive(true);
}

// 아이템 렉트 안에 마우스가 있다면
bool cGear::CheckPtInItem()
{
    bool isIn = false;

    for (int i = 0; i < m_vecGearUI.size(); ++i)
    {
        if (PtInRect(&m_vecItemRect[i], g_ptMouse))
        {
            if (i == ST_ITEM_INFO::E_HELM && m_stCurrGear.helmId != -1)
            {
                UpdatePlusStatUI(m_stCurrGear.helmId, Vector3((float)g_ptMouse.x + 10, (float)g_ptMouse.y, 0));
                isIn = true;
            }
            else if (i == ST_ITEM_INFO::E_CHEST_PLATE && m_stCurrGear.chestPlateId != -1)
            {
                UpdatePlusStatUI(m_stCurrGear.chestPlateId, Vector3((float)g_ptMouse.x + 10, (float)g_ptMouse.y, 0));
                isIn = true;
            }
            else if (i == ST_ITEM_INFO::E_SWORD && m_stCurrGear.swordId != -1)
            {
                UpdatePlusStatUI(m_stCurrGear.swordId, Vector3((float)g_ptMouse.x + 10, (float)g_ptMouse.y, 0));
                isIn = true;
            }
            if (isIn)
            {
                break;
            }
        }
    }

    return isIn;
}
