#include "stdafx.h"
#include "cInventory.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cInventory::cInventory()
    : m_pInvenLayer(NULL)
    , m_pPlusStatLayer(NULL)
    , m_nPlayerMoney(g_pGameManager->GetCurrGold())
    , m_nCurrSelectItem(-1)
    , m_nSlotMaxNum(12)
    , m_isClickInven(false)
    , m_isOpen(false)
{
    m_rtInvenSize.left = W_WIDTH - (LONG)(W_WIDTH * 0.18f) - 300;
    m_rtInvenSize.top = W_HEIGHT - (LONG)(W_HEIGHT * 0.35f) - 150;
    m_rtInvenSize.right = m_rtInvenSize.left + (LONG)(W_WIDTH * 0.18f);
    m_rtInvenSize.bottom = m_rtInvenSize.top + (LONG)(W_HEIGHT * 0.35f);

    m_stItemSize = ST_SIZE(60, 60);

    m_stPlusStatDefSize = ST_SIZE(100, 20);

    g_pSndManager->AddSound("inven-open", "inven", "Assets\\Sound\\Effect\\Common\\pickupbag.ogg");
    g_pSndManager->AddSound("inven-close", "inven", "Assets\\Sound\\Effect\\Common\\putdownbag.ogg");
    g_pSndManager->AddSound("inven-use", "inven", "Assets\\Sound\\Effect\\Common\\pickupmetallarge.ogg");
    g_pSndManager->AddSound("inven-click", "inven", "Assets\\Sound\\Effect\\Interface\\CharacterSheet\\click.ogg");
}


cInventory::~cInventory()
{
}

void cInventory::Setup()
{
    m_pInvenLayer = new cUILayer;
    m_pInvenLayer->SetLayer("Inven", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pInvenLayer->SetActive(true);

    m_pPlusStatLayer = new cUILayer;
    m_pPlusStatLayer->SetLayer("plus-stat", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pPlusStatLayer->SetActive(false);

    m_vecInvenItem = g_pGameManager->GetInvetoryInfo();

    SetInvenUI();
    SetPlusStatUI();

    if (m_pInvenLayer)
    {
        m_pInvenLayer->Setup();
    }
    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Setup();
    }
}

void cInventory::Update()
{
    m_isClickInven = false;

    if (!m_pInvenLayer)
    {
        return;
    }

    // 기본 정보 로드
    m_vecInvenItem = g_pGameManager->GetInvetoryInfo();
    m_nPlayerMoney = g_pGameManager->GetCurrGold();

    // 인벤 렉트 안에 마우스가 있다면
    if (PtInRect(&m_rtInvenSize, g_ptMouse))
    {
        // 버튼 클릭 무효화를 위해 
        g_pKeyManager->isOnceKeyDown(VK_LBUTTON);
        g_pKeyManager->isOnceKeyDown(VK_RBUTTON);
        m_isClickInven = true;

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

    // 플레이어 소지금 텍스쳐 변경
    cUIObject* pUIObject;
    m_pInvenLayer->FindUIObject(&pUIObject, "Inven-player-money-tex");
    if (pUIObject)
    {
        cUITextView* pUIText = (cUITextView*)pUIObject;
        char szMoney[20];
        sprintf_s(szMoney, sizeof(szMoney), "%d", m_nPlayerMoney);
        pUIText->SetText(szMoney);
    }

    // 계속해서 아이템을 체크하고 업데이트
    UpdateItemUI();

    // 버튼 클릭을 했다면
    if (m_pInvenLayer->IsClickButton())
    {
        string szClickBtnName = m_pInvenLayer->GetClickButtonName();

        // X버튼 클릭 시
        if (!strcmp(szClickBtnName.c_str(), "inven-x-btn"))
        {
            CloseInventory();
        }
        // 사용 버튼 클릭 시
        else if (!strcmp(szClickBtnName.c_str(), "Inven-use-btn"))
        {
            // 현재 인벤토리 정보
            vector<ST_ITEM> stInventory = g_pGameManager->GetInvetoryInfo();
            
            // 아이템을 선택한 상태라면
            if (m_nCurrSelectItem != -1 && m_nCurrSelectItem < stInventory.size())
            {
                g_pSndManager->Play("inven-use");
                // 아이템사용하기!!!!!!!!!!!!!!!!!11 -> 효과 발동!!! 또는 장착!!!
                g_pGameManager->PullItem(m_vecInvenItem[m_nCurrSelectItem].id);
                // 아이템이 0개가 되는 상황이면
                if (m_vecInvenItem[m_nCurrSelectItem].count == 1)
                {
                    m_nCurrSelectItem = -1;
                }
            }
        }
        // 선택 버튼 클릭 시
        for (int i = 0; i < m_nSlotMaxNum; ++i)
        {
            char Buf[20];
            sprintf_s(Buf, sizeof(Buf), "item-btn-%d", i);
            if (!strcmp(szClickBtnName.c_str(), Buf))
            {
                // 현재 선택된 아이템이 아닌 다른 아이템을 클릭 했으면
                if (m_nCurrSelectItem != i)
                {
                    g_pSndManager->Play("inven-click");

                    if (i < m_vecInvenItem.size())
                    {
                        m_nCurrSelectItem = i;

                        for (int j = 0; j < m_nSlotMaxNum; ++j)
                        {
                            // 선택 이미지 활성화, 다른 이미지 비활성화
                            cUIObject* pUIObject;
                            sprintf_s(Buf, sizeof(Buf), "item-select-%d", j);
                            m_pInvenLayer->FindUIObject(&pUIObject, Buf);
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
                }
                break;
            }
        }
    }

    m_pInvenLayer->Update();

    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Update();
    }
}

void cInventory::Render()
{
    if (m_pInvenLayer)
    {
        m_pInvenLayer->Render();
    }
    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Render();
    }
}

void cInventory::OpenInventory()
{
    g_pSndManager->Play("inven-open");

    m_isOpen = true;
    m_isClickInven = false;
    m_nCurrSelectItem = -1;

    m_vecInvenItem = g_pGameManager->GetInvetoryInfo();
    m_nPlayerMoney = g_pGameManager->GetCurrGold();

    UpdateItemUI();
}

void cInventory::CloseInventory()
{
    g_pSndManager->Play("inven-close");

    m_isOpen = false;
    m_isClickInven = false;
    m_nCurrSelectItem = -1;
}

ULONG cInventory::Release(void)
{
    SAFE_DELETE(m_pInvenLayer);
    SAFE_DELETE(m_pPlusStatLayer);

    return cObject::Release();
}

void cInventory::SetInvenUI()
{
    Vector3 vInvenPos((float)m_rtInvenSize.left, (float)m_rtInvenSize.top, 0);
    ST_SIZE stInvenSize((float)m_rtInvenSize.right - (float)m_rtInvenSize.left, (float)m_rtInvenSize.bottom - (float)m_rtInvenSize.top);
    string szPath;
    IMAGE_INFO imageInfo;

    // 주머니 이미지
    cUIImageView* pBackPackImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"Inventory-backpack.png";
    g_pTextureManager->AddTexture("Inven-backpack-img", szPath, true);
    pBackPackImage->SetName("Inven-backpack-img");
    pBackPackImage->SetLocalPos(Vector3(vInvenPos.x + 10, vInvenPos.y + 5, 0));
    pBackPackImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("Inven-backpack-img", &imageInfo));
    pBackPackImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBackPackImage->SetScale(70 / (float)imageInfo.Width, 70 / (float)imageInfo.Height);
    m_pInvenLayer->AddUIObject(pBackPackImage);

    // 인벤 배경
    cUIImageView* pBGImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"inventory-bg.png";
    g_pTextureManager->AddTexture("Inven-bg-img", szPath, true);
    pBGImage->SetName("Inven-bg-img");
    pBGImage->SetLocalPos(vInvenPos);
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("Inven-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stInvenSize.w / (float)imageInfo.Width, stInvenSize.h / (float)imageInfo.Height);
    m_pInvenLayer->AddUIObject(pBGImage);

    // 인벤 이름
    cUITextView* pInvenpNameText = new cUITextView;
    pInvenpNameText->SetName("Inven-name-tex");
    pInvenpNameText->SetLocalPos(Vector3(vInvenPos.x + 120, vInvenPos.y + 15, 0));
    pInvenpNameText->SetSize(Vector2(100, 15));
    pInvenpNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_TITLE));
    pInvenpNameText->SetText("INVENTORY");
    pInvenpNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pInvenLayer->AddUIObject(pInvenpNameText);

    // X버튼
    cUIButton* pXButton = new cUIButton;
    pXButton->SetName("inven-x-btn");
    pXButton->SetLocalPos(Vector3(vInvenPos.x + stInvenSize.w - 33, vInvenPos.y + 5, 0));
    pXButton->SetButtonState();
    szPath = INTERFACE_PATH + (string)"button/x-button-mouse-over.png";
    g_pTextureManager->AddTexture("x-mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/x-button-select.png";
    g_pTextureManager->AddTexture("x-select-btn", szPath, true);
    pXButton->SetTexture("x-mouse-over-btn", "x-mouse-over-btn", "x-select-btn");
    pXButton->SetSize(Vector2(38, 36));
    pXButton->SetUIButton(m_pInvenLayer);
    m_pInvenLayer->AddUIObject(pXButton);

    // 사용버튼
    cUIButton* pUseButton = new cUIButton;
    pUseButton->SetName("Inven-use-btn");
    pUseButton->SetLocalPos(Vector3(vInvenPos.x + stInvenSize.w - 120, vInvenPos.y + stInvenSize.h - 27, 0));
    pUseButton->SetButtonState();
    szPath = INTERFACE_PATH + (string)"button/button-normal.png";
    g_pTextureManager->AddTexture("normal-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/button-mouse-over.png";
    g_pTextureManager->AddTexture("mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/button-select.png";
    g_pTextureManager->AddTexture("select-btn", szPath, true);
    pUseButton->SetTexture("normal-btn", "mouse-over-btn", "select-btn");
    pUseButton->SetSize(Vector2(110, 30));
    pUseButton->SetUIButton(m_pInvenLayer);
    m_pInvenLayer->AddUIObject(pUseButton);

    // 사용 이름
    cUITextView* pUseText = new cUITextView;
    pUseText->SetName("Inven-use-tex");
    pUseText->SetLocalPos(Vector3(vInvenPos.x + stInvenSize.w - 90, vInvenPos.y + stInvenSize.h - 24, 0));
    pUseText->SetSize(Vector2(50, 15));
    pUseText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    pUseText->SetText("USE");
    pUseText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pInvenLayer->AddUIObject(pUseText);

    // 현재 소지금 텍스트
    cUITextView* pMoneyText = new cUITextView;
    pMoneyText->SetName("Inven-player-money-tex");
    pMoneyText->SetLocalPos(Vector3(vInvenPos.x + 40, vInvenPos.y + stInvenSize.h - 24, 0));
    pMoneyText->SetSize(Vector2(100, 15));
    pMoneyText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    char szMoney[20];
    sprintf_s(szMoney, sizeof(szMoney), "%d", m_nPlayerMoney);
    pMoneyText->SetText(szMoney);
    pMoneyText->SetDrawTextFormat(DT_RIGHT);
    m_pInvenLayer->AddUIObject(pMoneyText);

    // 코인 이미지
    cUIImageView* pCoinImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"coin.png";
    g_pTextureManager->AddTexture("Inven-coin-img", szPath, true);
    pCoinImage->SetName("Inven-coin-img");
    pCoinImage->SetLocalPos(Vector3(vInvenPos.x + 140, vInvenPos.y + stInvenSize.h - 24, 0));
    pCoinImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("Inven-coin-img", &imageInfo));
    pCoinImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pCoinImage->SetScale(18 / (float)imageInfo.Width, 18 / (float)imageInfo.Height);
    m_pInvenLayer->AddUIObject(pCoinImage);

    SetItemUI(vInvenPos);
}

void cInventory::SetItemUI(Vector3 vInvenPos)
{
    // 아이템 이미지
    ST_SIZE stItemSize = m_stItemSize;
    float fBaseX = vInvenPos.x + 24;
    float fBaseY = vInvenPos.y + 80;
    Vector3 vItemPos(fBaseX, fBaseY, 0);
    float fPlusX = stItemSize.w + 5;
    float fPlusY = stItemSize.h + 5;

    string szItemSlotPath = INTERFACE_PATH + (string)"item/inventory-slot.png";
    string szItemSlotName = "inven-slot-img";
    g_pTextureManager->AddTexture(szItemSlotName, szItemSlotPath, true);

    for (int i = 0; i < m_nSlotMaxNum; ++i)
    {
        if (i != 0)
        {
            if (i % 4 == 0)
            {
                vItemPos.x = fBaseX;
                vItemPos.y += fPlusY;
            }
            else
            {
                vItemPos.x += fPlusX;
            }
        }

        // 렉트 만들기
        RECT rtItem;
        rtItem.left = (LONG)vItemPos.x;
        rtItem.top = (LONG)vItemPos.y;
        rtItem.right = rtItem.left + (LONG)stItemSize.w;
        rtItem.bottom = rtItem.top + (LONG)stItemSize.h;
        m_vecItemRect.push_back(rtItem);

        // 슬롯 이미지
        cUIImageView* pSlotImage = new cUIImageView;
        pSlotImage->SetName(szItemSlotName);
        pSlotImage->SetLocalPos(vItemPos);
        IMAGE_INFO imageInfo;
        pSlotImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szItemSlotName, &imageInfo));
        pSlotImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pSlotImage->SetScale(stItemSize.w / (float)imageInfo.Width, stItemSize.h / (float)imageInfo.Height);
        m_pInvenLayer->AddUIObject(pSlotImage);

        // 아이템 이미지
        cUIImageView* pItemImage = new cUIImageView;
        pItemImage->SetLocalPos(vItemPos);
        char Buf[20];
        sprintf_s(Buf, sizeof(Buf), "item-img-%d", i);
        pItemImage->SetName(Buf);
        pItemImage->SetAxtive(false);
        m_pInvenLayer->AddUIObject(pItemImage);

        // 해당 칸에 보여질 아이템이 있으면
        if (i < m_vecInvenItem.size())
        {
            ST_ITEM_INFO* stItemInfo = g_pGameManager->GetItemInfoById(m_vecInvenItem[i].id);
            string szPath = stItemInfo->szPath;
            string szTexKey = stItemInfo->szName;
            g_pTextureManager->AddTexture(szTexKey, szPath, true);
            IMAGE_INFO imageInfo;
            pItemImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
            pItemImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
            pItemImage->SetScale(stItemSize.w / (float)imageInfo.Width, stItemSize.h / (float)imageInfo.Height);
            pItemImage->SetAxtive(true);
        }

        // 선택 버튼
        cUIButton* pSelectButton = new cUIButton;
        sprintf_s(Buf, sizeof(Buf), "item-btn-%d", i);
        pSelectButton->SetName(Buf);
        pSelectButton->SetLocalPos(Vector3(0, 0, 0));
        pSelectButton->SetButtonState();
        pSelectButton->SetTexture("", "", "");
        pSelectButton->SetSize(Vector2(stItemSize.w, stItemSize.h));
        pSelectButton->SetUIButton(m_pInvenLayer);
        // 해당 칸에 보여질 아이템이 없으면
        if (i >= m_vecInvenItem.size())
        {
            pSelectButton->SetAxtive(false);
        }
        pItemImage->AddChild(pSelectButton);

        // 선택 이미지
        cUIImageView* pItemSelectImage = new cUIImageView;
        g_pTextureManager->AddTexture("item-select", INTERFACE_PATH + (string)"item/item-select.png", true);
        sprintf_s(Buf, sizeof(Buf), "item-select-%d", i);
        pItemSelectImage->SetName(Buf);
        pItemSelectImage->SetLocalPos(Vector3(-2, -2, 0));
        pItemSelectImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("item-select", &imageInfo));
        pItemSelectImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemSelectImage->SetScale((stItemSize.w + 4) / (float)imageInfo.Width, (stItemSize.h + 4) / (float)imageInfo.Height);
        pItemSelectImage->SetAxtive(false);
        if (i == m_nCurrSelectItem)
        {
            pItemSelectImage->SetAxtive(true);
        }
        pItemImage->AddChild(pItemSelectImage);

        // 갯수 텍스트
        cUITextView* pCountText = new cUITextView;
        sprintf_s(Buf, sizeof(Buf), "item-cnt-%d",i);
        pCountText->SetName(Buf);
        pCountText->SetLocalPos(Vector3(stItemSize.w - 34, stItemSize.h - 21, 0));
        pCountText->SetSize(Vector2(25, 15));
        pCountText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_TITLE));
        pCountText->SetDrawTextFormat(DT_RIGHT);
        pCountText->SetAxtive(false);
        // 해당 칸에 보여질 아이템이 있으면
        if (i < m_vecInvenItem.size())
        {
            sprintf_s(Buf, sizeof(Buf), "%d", m_vecInvenItem[i].count);
            pCountText->SetText(Buf);
            pCountText->SetAxtive(true);
        }
        pItemImage->AddChild(pCountText);

        // 아이템 UI를 담아둔다.
        m_vecItemUI.push_back(pItemImage);
    }
}

// 계속해서 아이템과 소지금을 체크한다.
void cInventory::UpdateItemUI()
{
    if (m_nCurrSelectItem >= m_vecInvenItem.size())
    {
        m_nCurrSelectItem = -1;
    }

    for (int i = 0; i < m_nSlotMaxNum; ++i)
    {
        if (i >= m_vecItemUI.size())
        {
            break;
        }

        cUIObject* pButtonObject;
        char Buf[20];
        sprintf_s(Buf, sizeof(Buf), "item-btn-%d", i);
        pButtonObject = m_vecItemUI[i]->GetChildByName(Buf);

        cUIObject* pSelectObject;
        sprintf_s(Buf, sizeof(Buf), "item-select-%d", i);
        pSelectObject = m_vecItemUI[i]->GetChildByName(Buf);

        cUIObject* pCountObject;
        sprintf_s(Buf, sizeof(Buf), "item-cnt-%d", i);
        pCountObject = m_vecItemUI[i]->GetChildByName(Buf);

        // 아이템이 있으면
        if (i < m_vecInvenItem.size())
        {
            // 아이템 이미지
            ST_ITEM_INFO* stItemInfo = g_pGameManager->GetItemInfoById(m_vecInvenItem[i].id);
            string szPath = stItemInfo->szPath;
            string szTexKey = stItemInfo->szName;
            g_pTextureManager->AddTexture(szTexKey, szPath, true);
            IMAGE_INFO imageInfo;
            m_vecItemUI[i]->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
            m_vecItemUI[i]->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
            m_vecItemUI[i]->SetScale(m_stItemSize.w / (float)imageInfo.Width, m_stItemSize.h / (float)imageInfo.Height);
            m_vecItemUI[i]->SetAxtive(true);

            // 아이템 버튼 
            if (pButtonObject)
            {
                pButtonObject->SetAxtive(true);
            }
            // 아이템 선택 이미지
            if (pSelectObject)
            {
                if (m_nCurrSelectItem == i)
                {
                    pSelectObject->SetAxtive(true);
                }
                else
                {
                    pSelectObject->SetAxtive(false);
                }
            }
            // 아이템 갯수 텍스트
            if (pCountObject)
            {
                cUITextView* pText = (cUITextView*)pCountObject;
                sprintf_s(Buf, sizeof(Buf), "%d", m_vecInvenItem[i].count);
                pText->SetText(Buf);
                pText->SetAxtive(true);
            }
        }
        // 아이템이 없으면
        else
        {
            m_vecItemUI[i]->SetAxtive(false);

            // 아이템 버튼 
            if (pButtonObject)
            {
                pButtonObject->SetAxtive(false);
            }
            // 아이템 선택 이미지
            if (pSelectObject)
            {
                pSelectObject->SetAxtive(false);
            }
            // 아이템 갯수 텍스트
            if (pCountObject)
            {
                pCountObject->SetAxtive(false);
            }
        }
    }
}

void cInventory::SetPlusStatUI()
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
        pText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        pText->SetDrawTextFormat(DT_LEFT);
        pText->SetColor(D3DCOLOR_XRGB(0, 200, 0));
        pText->SetAxtive(false);
        pBGImage->AddChild(pText);

        vPos.y += fPlusY;
    }
}

void cInventory::UpdatePlusStatUI(int id, Vector3 vPos)  // 렉트 안에 마우스가 있을 때만 발동
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
bool cInventory::CheckPtInItem()
{
    bool isIn = false;

    for (int i = 0; i < m_vecInvenItem.size(); ++i)
    {
        if (PtInRect(&m_vecItemRect[i], g_ptMouse))
        {
            UpdatePlusStatUI(m_vecInvenItem[i].id, Vector3((float)g_ptMouse.x + 10, (float)g_ptMouse.y, 0));
            isIn = true;
            break;
        }
    }

    return isIn;
}