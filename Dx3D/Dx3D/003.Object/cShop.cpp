#include "stdafx.h"
#include "cShop.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cShop::cShop()
    : m_pShopLayer(NULL)
    , m_pPlusStatLayer(NULL)
    , m_nCurrSelectItem(0)
    , m_nPlayerMoney(g_pGameManager->GetCurrGold())
    , m_isOpen(false)
    , m_isClickShop(false)
{
    m_rtShopSize.left = 50;
    m_rtShopSize.top = 150;
    m_rtShopSize.right = m_rtShopSize.left + (LONG)(W_WIDTH * 0.4f);
    m_rtShopSize.bottom = m_rtShopSize.top + (LONG)(W_HEIGHT * 0.6f);

    m_stPlusStatDefSize = ST_SIZE(100, 20);

    g_pSndManager->AddSound("store-buy", "store", "Assets\\Sound\\Effect\\Common\\uilootpickupitem.ogg");
    g_pSndManager->AddSound("store-click", "store", "Assets\\Sound\\Effect\\Interface\\CharacterSheet\\click.ogg");
    g_pSndManager->AddSound("store-open", "store", STORE_SOUND_PATH + (string)"auctionwindowopen.ogg");
    g_pSndManager->AddSound("store-close", "store", STORE_SOUND_PATH + (string)"auctionwindowclose.ogg");
    g_pSndManager->AddSound("store-human0", "store", STORE_SOUND_PATH + (string)"humanmalemerchantcstartshopping01.ogg");
    g_pSndManager->AddSound("store-human1", "store", STORE_SOUND_PATH + (string)"humanmalemerchantcstartshopping04.ogg");
    g_pSndManager->AddSound("store-human2", "store", STORE_SOUND_PATH + (string)"humanmalemerchantcstartshopping09.ogg");
    g_pSndManager->AddSound("store-human3", "store", STORE_SOUND_PATH + (string)"humanmalemerchantcstartshopping13.ogg");
    g_pSndManager->AddSound("store-human4", "store", STORE_SOUND_PATH + (string)"humanmalemerchantcstartshopping18.ogg");
    g_pSndManager->AddSound("store-human5", "store", STORE_SOUND_PATH + (string)"humanmalemerchantcstartshopping19.ogg");
    m_vecSzHumanSound.push_back("store-human0");
    m_vecSzHumanSound.push_back("store-human1");
    m_vecSzHumanSound.push_back("store-human2");
    m_vecSzHumanSound.push_back("store-human3");
    m_vecSzHumanSound.push_back("store-human4");
    m_vecSzHumanSound.push_back("store-human5");
}


cShop::~cShop()
{
}

void cShop::Setup()
{
    // 이미 정보를 로드 했으면 리턴
    if (!m_vecItemInfo.empty())
    {
        return;
    }

    // 정보 가져오기
    m_vecItemInfo = g_pGameManager->GetItemInfo();

    // 레이어 셋팅
    m_pShopLayer = new cUILayer;
    m_pShopLayer->SetLayer("shop", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pShopLayer->SetActive(true);

    m_pPlusStatLayer = new cUILayer;
    m_pPlusStatLayer->SetLayer("plus-stat", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pPlusStatLayer->SetActive(false);

    SetShopUI();
    SetPlusStatUI();

    if (m_pShopLayer)
    {
        m_pShopLayer->Setup();
    }
    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Setup();
    }
}

void cShop::Update()
{
    m_isClickShop = false;

    if (!m_pShopLayer)
    {
        return;
    }

    // 현재 소지금 셋팅
    m_nPlayerMoney = g_pGameManager->GetCurrGold();

    // 상점 렉트 안에서 마우스가 있다면
    if (PtInRect(&m_rtShopSize, g_ptMouse))
    {
        // 버튼 클릭 무효화를 위해 
        g_pKeyManager->isOnceKeyDown(VK_LBUTTON);
        g_pKeyManager->isOnceKeyDown(VK_RBUTTON);
        m_isClickShop = true;

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
    m_pShopLayer->FindUIObject(&pUIObject, "shop-player-money-tex");
    if (pUIObject)
    {
        cUITextView* pUIText = (cUITextView*)pUIObject;
        char szMoney[20];
        sprintf_s(szMoney, sizeof(szMoney), "%d", m_nPlayerMoney);
        pUIText->SetText(szMoney);
    }

    // 버튼 클릭을 했다면
    if (m_pShopLayer->IsClickButton())
    {
        string szClickBtnName = m_pShopLayer->GetClickButtonName();

        // X버튼 클릭 시
        if (!strcmp(szClickBtnName.c_str(), "shop-x-btn"))
        {
            CloseShop();
        }
        // 구입 버튼 클릭 시
        else if (!strcmp(szClickBtnName.c_str(), "shop-buy-btn"))
        {
            // 플레이어에게 돈이 있으면 구입
            if (m_vecItemInfo[m_nCurrSelectItem]->nPrice <= m_nPlayerMoney)
            {
                g_pSndManager->Play("store-buy");
                m_nPlayerMoney -= m_vecItemInfo[m_nCurrSelectItem]->nPrice;
                g_pGameManager->Pay(-m_vecItemInfo[m_nCurrSelectItem]->nPrice);
                g_pGameManager->PushItem(m_nCurrSelectItem);
            }
        }
        // 선택 버튼 클릭 시
        for (int i = 0; i < m_vecItemInfo.size(); ++i)
        {
            if (!strcmp(szClickBtnName.c_str(), m_vecItemInfo[i]->szName.c_str()))
            {
                // 현재 선택된 아이템이 아닌 다른 아이템을 클릭 했으면
                if (m_nCurrSelectItem != i)
                {
                    g_pSndManager->Play("store-click");

                    m_nCurrSelectItem = i;

                    for (int j = 0; j < m_vecItemInfo.size(); ++j)
                    {
                        // 선택 이미지 활성화, 다른 이미지 비활성화
                        cUIObject* pUIObject;
                        m_pShopLayer->FindUIObject(&pUIObject, m_vecItemInfo[j]->szName + "select-img");
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

    m_pShopLayer->Update();

    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Update();
    }
}

void cShop::Render()
{
    if (m_pShopLayer)
    {
        m_pShopLayer->Render();
    }
    if (m_pPlusStatLayer)
    {
        m_pPlusStatLayer->Render();
    }
}

void cShop::OpenShop()
{
    g_pSndManager->Play("store-open");
    g_pSndManager->Play(m_vecSzHumanSound[rand() % 6]);
    m_isOpen = true;
    m_isClickShop = false;
    m_nPlayerMoney = g_pGameManager->GetCurrGold();
}

void cShop::CloseShop()
{
    g_pSndManager->Play("store-close");

    m_isClickShop = false;
    m_isOpen = false;
    m_nCurrSelectItem = 0;

    // 첫번째 이미지 선택 활성화, 다른 이미지 비활성화
    cUIObject* pUIObject;
    for (int i = 0; i < m_vecItemInfo.size(); ++i)
    {
        m_pShopLayer->FindUIObject(&pUIObject, m_vecItemInfo[i]->szName + "select-img");
        if (pUIObject)
        {
            if (i == m_nCurrSelectItem)
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

ULONG cShop::Release(void)
{
    SAFE_DELETE(m_pShopLayer);
    SAFE_DELETE(m_pPlusStatLayer);

    cObject::Release();

    return 0;
}

void cShop::SetShopUI()
{
    Vector3 vShopPos((float)m_rtShopSize.left, (float)m_rtShopSize.top, 0);
    ST_SIZE stShopSize((float)m_rtShopSize.right - (float)m_rtShopSize.left, (float)m_rtShopSize.bottom - (float)m_rtShopSize.top);
    string szPath;
    IMAGE_INFO imageInfo;

    // 상점 주인 초상화
    cUIImageView* pHumanImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"shop-human.png";
    g_pTextureManager->AddTexture("shop-human-img", szPath, true);
    pHumanImage->SetName("shop-human-img");
    pHumanImage->SetLocalPos(Vector3(vShopPos.x + 10, vShopPos.y + 8, 0));
    pHumanImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-human-img", &imageInfo));
    pHumanImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pHumanImage->SetScale(70.0f / (float)imageInfo.Width, 70.0f / (float)imageInfo.Height);
    m_pShopLayer->AddUIObject(pHumanImage);

    // 상점 배경
    cUIImageView* pBGImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"shop-bg.png";
    g_pTextureManager->AddTexture("shop-bg-img", szPath, true);
    pBGImage->SetName("shop-bg-img");
    pBGImage->SetLocalPos(vShopPos);
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stShopSize.w / (float)imageInfo.Width, stShopSize.h / (float)imageInfo.Height);
    m_pShopLayer->AddUIObject(pBGImage);

    // 상점 이름
    cUITextView* pShopNameText = new cUITextView;
    pShopNameText->SetName("shop-name-tex");
    pShopNameText->SetLocalPos(Vector3(vShopPos.x + 240, vShopPos.y + 20, 0));
    pShopNameText->SetSize(Vector2(200, 15));
    pShopNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_TITLE));
    pShopNameText->SetText("STORE");
    pShopNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pShopLayer->AddUIObject(pShopNameText);

    // X버튼
    cUIButton* pXButton = new cUIButton;
    pXButton->SetName("shop-x-btn");
    pXButton->SetLocalPos(Vector3(vShopPos.x + stShopSize.w - 38, vShopPos.y + 7, 0));
    pXButton->SetButtonState();
    szPath = INTERFACE_PATH + (string)"button/x-button-mouse-over.png";
    g_pTextureManager->AddTexture("x-mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/x-button-select.png";
    g_pTextureManager->AddTexture("x-select-btn", szPath, true);
    pXButton->SetTexture("x-mouse-over-btn", "x-mouse-over-btn", "x-select-btn");
    pXButton->SetSize(Vector2(43, 41));
    pXButton->SetUIButton(m_pShopLayer);
    m_pShopLayer->AddUIObject(pXButton);

    // 구입버튼
    cUIButton* pBuyButton = new cUIButton;
    pBuyButton->SetName("shop-buy-btn");
    pBuyButton->SetLocalPos(Vector3(vShopPos.x + stShopSize.w - 160, vShopPos.y + stShopSize.h - 32, 0));
    pBuyButton->SetButtonState();
    szPath = INTERFACE_PATH + (string)"button/button-normal.png";
    g_pTextureManager->AddTexture("normal-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/button-mouse-over.png";
    g_pTextureManager->AddTexture("mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"button/button-select.png";
    g_pTextureManager->AddTexture("select-btn", szPath, true);
    pBuyButton->SetTexture("normal-btn", "mouse-over-btn", "select-btn");
    pBuyButton->SetSize(Vector2(150, 35));
    pBuyButton->SetUIButton(m_pShopLayer);
    m_pShopLayer->AddUIObject(pBuyButton);

    // 구입 이름
    cUITextView* pBuyText = new cUITextView;
    pBuyText->SetName("shop-buy-tex");
    pBuyText->SetLocalPos(Vector3(vShopPos.x + stShopSize.w - 110, vShopPos.y + stShopSize.h - 27, 0));
    pBuyText->SetSize(Vector2(50, 15));
    pBuyText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    pBuyText->SetText("BUY");
    pBuyText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pShopLayer->AddUIObject(pBuyText);

    // 현재 소지금 텍스트
    cUITextView* pMoneyText = new cUITextView;
    pMoneyText->SetName("shop-player-money-tex");
    pMoneyText->SetLocalPos(Vector3(vShopPos.x + 50, vShopPos.y + stShopSize.h - 27, 0));
    pMoneyText->SetSize(Vector2(150, 15));
    pMoneyText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    char szMoney[20];
    sprintf_s(szMoney, sizeof(szMoney), "%d", m_nPlayerMoney);
    pMoneyText->SetText(szMoney);
    pMoneyText->SetDrawTextFormat(DT_RIGHT);
    m_pShopLayer->AddUIObject(pMoneyText);

    // 코인 이미지
    cUIImageView* pCoinImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"coin.png";
    g_pTextureManager->AddTexture("shop-coin-img", szPath, true);
    pCoinImage->SetName("shop-coin-img");
    pCoinImage->SetLocalPos(Vector3(vShopPos.x + 200, vShopPos.y + stShopSize.h - 27, 0));
    pCoinImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-coin-img", &imageInfo));
    pCoinImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pCoinImage->SetScale(18 / (float)imageInfo.Width, 18 / (float)imageInfo.Height);
    m_pShopLayer->AddUIObject(pCoinImage);

    SetShopItemUI(vShopPos);
}

void cShop::SetShopItemUI(Vector3 vShopPos)
{
    // 아이템 이미지
    ST_SIZE stItemSize(60, 60);
    ST_SIZE stItemContentsSize(190, 70);
    float fBaseX = vShopPos.x + 40;
    float fBaseY = vShopPos.y + 100;
    Vector3 vItemPos(fBaseX, fBaseY, 0);
    float fPlusX = stItemSize.w + 230;
    float fPlusY = stItemSize.h + 30;

    string szItemContentsPath = INTERFACE_PATH + (string)"item/item-contents.png";
    string szItemContentsName = "shop-item-contents-img";
    g_pTextureManager->AddTexture(szItemContentsName, szItemContentsPath, true);

    for (int i = 0; i < m_vecItemInfo.size(); ++i)
    {
        string szPath = m_vecItemInfo[i]->szPath;
        string szTexKey = m_vecItemInfo[i]->szName;

        if (i != 0)
        {
            if (i % 2 == 0)
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
        rtItem.right = rtItem.left + (LONG)stItemSize.w + (LONG)stItemContentsSize.w;
        rtItem.bottom = rtItem.top + (LONG)stItemSize.h;
        m_vecItemRect.push_back(rtItem);

        // 이미지
        cUIImageView* pItemImage = new cUIImageView;
        g_pTextureManager->AddTexture(szTexKey, szPath, true);
        pItemImage->SetName(szTexKey);
        pItemImage->SetLocalPos(vItemPos);
        IMAGE_INFO imageInfo;
        pItemImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
        pItemImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemImage->SetScale(stItemSize.w / (float)imageInfo.Width, stItemSize.h / (float)imageInfo.Height);
        m_pShopLayer->AddUIObject(pItemImage);

        // 선택 버튼
        cUIButton* pSelectButton = new cUIButton;
        pSelectButton->SetName(m_vecItemInfo[i]->szName);
        pSelectButton->SetLocalPos(Vector3(0, 0, 0));
        pSelectButton->SetButtonState();
        pSelectButton->SetTexture("", "", "");
        pSelectButton->SetSize(Vector2(stItemSize.w + stItemContentsSize.w, stItemSize.h));
        pSelectButton->SetUIButton(m_pShopLayer);
        pItemImage->AddChild(pSelectButton);

        // 선택 이미지
        cUIImageView* pItemSelectImage = new cUIImageView;
        g_pTextureManager->AddTexture("item-select", INTERFACE_PATH + (string)"item/item-select.png", true);
        pItemSelectImage->SetName(m_vecItemInfo[i]->szName + "select-img");
        pItemSelectImage->SetLocalPos(Vector3(-3, -3, 0));
        pItemSelectImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("item-select", &imageInfo));
        pItemSelectImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemSelectImage->SetScale((stItemSize.w + 6) / (float)imageInfo.Width, (stItemSize.h + 6) / (float)imageInfo.Height);
        pItemSelectImage->SetAxtive(false);
        if (i == m_nCurrSelectItem)
        {
            pItemSelectImage->SetAxtive(true);
        }
        pItemImage->AddChild(pItemSelectImage);

        // 아이템 내용 이미지
        cUIImageView* pItemContentsImage = new cUIImageView;
        pItemContentsImage->SetName(szItemContentsName);
        pItemContentsImage->SetLocalPos(Vector3(80, 0, 0));
        pItemContentsImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szItemContentsName, &imageInfo));
        pItemContentsImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemContentsImage->SetScale(stItemContentsSize.w / (float)imageInfo.Width, stItemContentsSize.h / (float)imageInfo.Height);
        pItemImage->AddChild(pItemContentsImage);

        // 아이템명 텍스트
        cUITextView* pNameText = new cUITextView;
        pNameText->SetName(m_vecItemInfo[i]->szName);
        pNameText->SetLocalPos(Vector3(95, 10, 0));
        pNameText->SetSize(Vector2(150, 15));
        pNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        pNameText->SetText(m_vecItemInfo[i]->szName);
        pNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
        pNameText->SetDrawTextFormat(DT_LEFT);
        pItemImage->AddChild(pNameText);
        
        // 코인 이미지
        cUIImageView* pCoinImage = new cUIImageView;
        szPath = INTERFACE_PATH + (string)"coin.png";
        g_pTextureManager->AddTexture("shop-coin-img", szPath, true);
        pCoinImage->SetName("coin-img");
        pCoinImage->SetLocalPos(Vector3(95, 50, 0));
        pCoinImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-coin-img", &imageInfo));
        pCoinImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pCoinImage->SetScale(18 / (float)imageInfo.Width, 18 / (float)imageInfo.Height);
        pItemImage->AddChild(pCoinImage);

        // 가격 텍스트
        cUITextView* pPriceText = new cUITextView;
        pPriceText->SetName("price");
        pPriceText->SetLocalPos(Vector3(115, 50, 0));
        pPriceText->SetSize(Vector2(100, 15));
        pPriceText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        char buf[20];
        sprintf_s(buf, sizeof(buf), "%d", m_vecItemInfo[i]->nPrice);
        pPriceText->SetText(buf);
        pPriceText->SetDrawTextFormat(DT_LEFT);
        pItemImage->AddChild(pPriceText);
    }
}

void cShop::SetPlusStatUI()
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

void cShop::UpdatePlusStatUI(int id, Vector3 vPos)  // 렉트 안에 마우스가 있을 때만 발동
{
    ST_ITEM_INFO stItem =  *g_pGameManager->GetItemInfoById(id);

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
bool cShop::CheckPtInItem()
{
    bool isIn = false;

    for (int i = 0; i < m_vecItemRect.size(); ++i)
    {
        if (PtInRect(&m_vecItemRect[i], g_ptMouse))
        {
            UpdatePlusStatUI(i, Vector3((float)g_ptMouse.x + 10, (float)g_ptMouse.y, 0));
            isIn = true;
            break;
        }
    }

    return isIn;
}
