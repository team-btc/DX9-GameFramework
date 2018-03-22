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

    // �⺻ ���� �ε�
    m_stCurrGear = g_pGameManager->GetGearInfo();
    // ����ؼ� ���� ���� üũ�ϰ� ������Ʈ
    UpdateGearUI(stPlayerStat);

    // ���â ��Ʈ �ȿ� ���콺�� �ִٸ�
    if (PtInRect(&m_rtGearSize, g_ptMouse))
    {
        // ��ư Ŭ�� ��ȿȭ�� ���� 
        g_pKeyManager->isOnceKeyDown(VK_LBUTTON);
        g_pKeyManager->isOnceKeyDown(VK_RBUTTON);
        m_isClickGear = true;

        // �����ۿ� ���콺�� �÷����ִ��� üũ�ϰ� ���̾� ����
        if (!CheckPtInItem())
        {
            m_pPlusStatLayer->SetActive(false);
        }
    }
    else
    {
        m_pPlusStatLayer->SetActive(false);
    }

    // ��ư Ŭ���� �ߴٸ�
    if (m_pGearLayer->IsClickButton())
    {
        string szClickBtnName = m_pGearLayer->GetClickButtonName();

        // X��ư Ŭ�� ��
        if (!strcmp(szClickBtnName.c_str(), "gear-x-btn"))
        {
            CloseGear();
        }
        // ���� ��ư Ŭ�� ��
        else if (!strcmp(szClickBtnName.c_str(), "gear-pull-btn"))
        {
            // �������� ������ ���¶��
            if (m_nCurrSelectGear != -1)
            {
                g_pSndManager->Play("gear-pull");
                // ������ ���� �ϱ�!!!!!!!!!!!!!!!!!11 -> ȿ�� ����!!!
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
                // ���� �̹��� ����
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
        // ���� ��ư Ŭ�� ��
        for (int i = 0; i < m_vecGearUI.size(); ++i)
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), "gear-btn-%d", i);
            if (!strcmp(szClickBtnName.c_str(), Buf))
            {
                // ���� ���õ� �������� �ƴ� �ٸ� �������� Ŭ�� ������
                if (m_nCurrSelectGear != i)
                {
                    g_pSndManager->Play("gear-click");

                    m_nCurrSelectGear = i;

                    for (int j = 0; j < m_vecGearUI.size(); ++j)
                    {
                        // ���� �̹��� Ȱ��ȭ, �ٸ� �̹��� ��Ȱ��ȭ
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

    // ĳ���� �ʻ�ȭ
    cUIImageView* pCharacterImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"gear-character.png";
    g_pTextureManager->AddTexture("gear-character-img", szPath, true);
    pCharacterImage->SetName("gear-character-img");
    pCharacterImage->SetLocalPos(Vector3(vGearPos.x + 10, vGearPos.y + 5, 0));
    pCharacterImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("gear-character-img", &imageInfo));
    pCharacterImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pCharacterImage->SetScale(70 / (float)imageInfo.Width, 70 / (float)imageInfo.Height);
    m_pGearLayer->AddUIObject(pCharacterImage);

    // ���â ���
    cUIImageView* pBGImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"gear-bg.png";
    g_pTextureManager->AddTexture("gear-bg-img", szPath, true);
    pBGImage->SetName("gear-bg-img");
    pBGImage->SetLocalPos(vGearPos);
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("gear-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stGearSize.w / (float)imageInfo.Width, stGearSize.h / (float)imageInfo.Height);
    m_pGearLayer->AddUIObject(pBGImage);

    // ���â �̸�
    cUITextView* pGearNameText = new cUITextView;
    pGearNameText->SetName("gear-name-tex");
    pGearNameText->SetLocalPos(Vector3(vGearPos.x + 120, vGearPos.y + 15, 0));
    pGearNameText->SetSize(Vector2(100, 15));
    pGearNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_TITLE));
    pGearNameText->SetText("GEAR");
    pGearNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pGearLayer->AddUIObject(pGearNameText);

    // X��ư
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
    Vector2 vValueSize(100, 20);
    DWORD dTitleFontColor = D3DCOLOR_XRGB(255, 255, 0);
    DWORD dValueFontColor = D3DCOLOR_XRGB(255, 255, 255);

    // ���� :
    m_pGearLayer->AddUIObject(MakeStatTextUI("Level", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    cUITextView* UIText = MakeStatTextUI(": 1", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_LEVEL, UIText));

    // �̸� :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Name", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": name", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_NAME, UIText));

    // ���ݷ� :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Att", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 12", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_ATT, UIText));

    // ���� :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Def", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 34", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_DEF, UIText));

    // �� :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Str", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 56", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_STR, UIText));

    // ��ø :
    vTextPos.x = fBaseX;
    vTextPos.y += fPlusY;
    m_pGearLayer->AddUIObject(MakeStatTextUI("Dex", vTextPos, vTitleSize, dTitleFontColor));
    vTextPos.x += fPlusX;
    UIText = MakeStatTextUI(": 78", vTextPos, vValueSize, dValueFontColor);
    m_pGearLayer->AddUIObject(UIText);
    m_mapTextUI.insert(make_pair(E_PLAYER_DEX, UIText));

    // ���� :
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
    pTextUI->SetFont(g_pFontManager->GetFont(g_pFontManager->E_STAT));
    pTextUI->SetText(name);
    pTextUI->SetDrawTextFormat(DT_LEFT);
    pTextUI->SetColor(color);

    return pTextUI;
}

void cGear::SetStatUI(Vector3 vGearPos)
{
    // ������ �̹���
    ST_SIZE stSlotSize = m_stGearSlotSize;
    Vector3 vItemPos(vGearPos.x + 200, vGearPos.y + 85, 0);
    float fPlusY = stSlotSize.h + 5;

    for (int i = 0; i < 3; ++i)
    {
        if (i != 0)
        {
            vItemPos.y += fPlusY;
        }

        // ��Ʈ �����
        RECT rtItem;
        rtItem.left = (LONG)vItemPos.x;
        rtItem.top = (LONG)vItemPos.y;
        rtItem.right = rtItem.left + (LONG)stSlotSize.w;
        rtItem.bottom = rtItem.top + (LONG)stSlotSize.h;
        m_vecItemRect.push_back(rtItem);

        // ���� �̹���
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

        // ��� �̹���
        cUIImageView* pGearImage = new cUIImageView;
        pGearImage->SetLocalPos(vItemPos);
        char Buf[20];
        sprintf_s(Buf, sizeof(Buf), "gear-img-%d", i);
        pGearImage->SetName(Buf);
        pGearImage->SetAxtive(false);
        m_pGearLayer->AddUIObject(pGearImage);

        // �̹��� ������ ����
        m_vecGearUI.push_back(pGearImage);

        // ���� ��ư
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

        // ���� �̹���
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

        // �ش� ĭ�� ������ �������� ������
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

    // ���� ��ư
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

    // ���� �̸�
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
    // ���� ����
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

    // ������ üũ
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
            // ���� ��ư Ȱ��ȭ
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
            // ���� ��ư ��Ȱ��ȭ
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
    // ���
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

    // ���� ���� �ؽ�Ʈ
    for (int i = 0; i < 4; ++i)
    {
        cUITextView* pText = new cUITextView;
        char buf[20];
        sprintf_s(buf, sizeof(buf), "stat-%d", i);
        pText->SetName(buf);
        pText->SetLocalPos(vPos);
        pText->SetSize(Vector2(100, 15));
        pText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        pText->SetDrawTextFormat(DT_LEFT);
        pText->SetColor(D3DCOLOR_XRGB(0, 200, 0));
        pText->SetAxtive(false);
        pBGImage->AddChild(pText);

        vPos.y += fPlusY;
    }
}

void cGear::UpdatePlusStatUI(int id, Vector3 vPos)  // ��Ʈ �ȿ� ���콺�� ���� ���� �ߵ�
{
    ST_ITEM_INFO stItem = *g_pGameManager->GetItemInfoById(id);

    // ���� ���� ������Ʈ
    for (int i = 0; i < 4; ++i)
    {
        cUIObject* pObject;
        char buf[20];
        sprintf_s(buf, sizeof(buf), "stat-%d", i);
        m_pPlusStatLayer->FindUIObject(&pObject, buf);

        if (pObject)
        {
            cUITextView* pText = (cUITextView*)pObject;

            // �����ؾ� �ϴ� �÷��� ������ ������
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
    // ��� ������ �ٲٱ�
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

// ������ ��Ʈ �ȿ� ���콺�� �ִٸ�
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
