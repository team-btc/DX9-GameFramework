#include "stdafx.h"
#include "cShop.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cShop::cShop()
    : m_pShopLayer(NULL)
    , m_eCurrSelectItem(E_ITEM_SWORD1)
    , m_nPlayerMoney(0)
    , m_isOpen(false)
    , m_isClickShop(false)
{
    m_rtShopSize.left = 50;
    m_rtShopSize.top = 150;
    m_rtShopSize.right = m_rtShopSize.left + (LONG)(W_WIDTH * 0.4f);
    m_rtShopSize.bottom = m_rtShopSize.top + (LONG)(W_HEIGHT * 0.6f);

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
    // �̹� ������ �ε� ������ ����
    if (!m_vecItemInfo.empty())
    {
        return;
    }

    // ���� �ε��ϱ�
    json jLoad;
    ifstream iFile;
    iFile.open(SHOP_PATH + (string)"shop.json");
    iFile >> jLoad;
    iFile.close();

    json jItem = jLoad["item"];
    for (int i = 0; i < jItem.size(); ++i)
    {
        ST_ITEM_INFO stItem;
        string szName = jItem[i]["item-name"];
        stItem.szName = szName;
        string szPath = jItem[i]["item-path"];
        stItem.szPath = szPath;
        stItem.nCount = jItem[i]["item-count"];
        stItem.nPrice = jItem[i]["item-price"];
        int nStatNum = jItem[i]["item-plus-stat"];
        char szBuf[10];
        sprintf_s(szBuf, sizeof(szBuf), "%d", nStatNum);
        stItem.stStat.szName = szBuf;
        ZeroMemory(&stItem.stStat, sizeof(ST_STATUS));
        stItem.fPlusValue = jItem[i]["item-plus-value"];

        m_vecItemInfo.push_back(stItem);
    }

    // ���̾� ����
    m_pShopLayer = new cUILayer;
    m_pShopLayer->SetLayer("shop", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pShopLayer->SetActive(true);

    SetShopUI();

    if (m_pShopLayer)
    {
        m_pShopLayer->Setup();
    }
}

void cShop::Update(int nPlayerMoney)
{
    m_isClickShop = false;

    if (!m_pShopLayer)
    {
        return;
    }

    // ���� ��Ʈ �ȿ��� ���콺�� �ִٸ�
    if (PtInRect(&m_rtShopSize, g_ptMouse))
    {
        // ��ư Ŭ�� ��ȿȭ�� ���� 
        g_pKeyManager->isOnceKeyDown(VK_LBUTTON);
        g_pKeyManager->isOnceKeyDown(VK_RBUTTON);
        m_isClickShop = true;
    }

    m_nPlayerMoney = nPlayerMoney;

    // �÷��̾� ������ �ؽ��� ����
    cUIObject* pUIObject;
    m_pShopLayer->FindUIObject(&pUIObject, "shop-player-money-tex");
    if (pUIObject)
    {
        cUITextView* pUIText = (cUITextView*)pUIObject;
        char szMoney[20];
        sprintf_s(szMoney, sizeof(szMoney), "%d", m_nPlayerMoney);
        pUIText->SetText(szMoney);
    }

    // ��ư Ŭ���� �ߴٸ�
    if (m_pShopLayer->IsClickButton())
    {
        string szClickBtnName = m_pShopLayer->GetClickButtonName();

        // X��ư Ŭ�� ��
        if (!strcmp(szClickBtnName.c_str(), "shop-x-btn"))
        {
            CloseShop();
        }
        // ���� ��ư Ŭ�� ��
        else if (!strcmp(szClickBtnName.c_str(), "shop-buy-btn"))
        {

        }
        // ���� ��ư Ŭ�� ��
        for (int i = 0; i < m_vecItemInfo.size(); ++i)
        {
            if (!strcmp(szClickBtnName.c_str(), m_vecItemInfo[i].szName.c_str()))
            {
                // ���� ���õ� �������� �ƴ� �ٸ� �������� Ŭ�� ������
                if (m_eCurrSelectItem != (E_ITEM_TYPE)i)
                {
                    m_eCurrSelectItem = (E_ITEM_TYPE)i;

                    for (int j = 0; j < m_vecItemInfo.size(); ++j)
                    {
                        // ���� �̹��� Ȱ��ȭ, �ٸ� �̹��� ��Ȱ��ȭ
                        cUIObject* pUIObject;
                        m_pShopLayer->FindUIObject(&pUIObject, m_vecItemInfo[j].szName + "select-img");
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
}

void cShop::Render()
{
    if (m_pShopLayer)
    {
        m_pShopLayer->Render();
    }
}

void cShop::OpenShop()
{
    g_pSndManager->Play("store-open");
    g_pSndManager->Play(m_vecSzHumanSound[rand() % 6]);
    m_isOpen = true;
    m_isClickShop = false;
}

void cShop::CloseShop()
{
    g_pSndManager->Play("store-close");

    m_isClickShop = false;
    m_isOpen = false;
    m_eCurrSelectItem = E_ITEM_SWORD1;

    // ù��° �̹��� ���� Ȱ��ȭ, �ٸ� �̹��� ��Ȱ��ȭ
    cUIObject* pUIObject;
    for (int i = 0; i < m_vecItemInfo.size(); ++i)
    {
        m_pShopLayer->FindUIObject(&pUIObject, m_vecItemInfo[i].szName + "select-img");
        if (pUIObject)
        {
            if (i == m_eCurrSelectItem)
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

    cObject::Release();

    return 0;
}

void cShop::SetShopUI()
{
    Vector3 vShopPos((float)m_rtShopSize.left, (float)m_rtShopSize.top, 0);
    ST_SIZE stShopSize((float)m_rtShopSize.right - (float)m_rtShopSize.left, (float)m_rtShopSize.bottom - (float)m_rtShopSize.top);
    string szPath;
    IMAGE_INFO imageInfo;

    // ���� ���� �ʻ�ȭ
    cUIImageView* pHumanImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"shop-human.png";
    g_pTextureManager->AddTexture("shop-human-img", szPath, true);
    pHumanImage->SetName("shop-human-img");
    pHumanImage->SetLocalPos(Vector3(vShopPos.x + 10, vShopPos.y + 8, 0));
    pHumanImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-human-img", &imageInfo));
    pHumanImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pHumanImage->SetScale(70, 70);
    m_pShopLayer->AddUIObject(pHumanImage);

    // ���� ���
    cUIImageView* pBGImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"shop-bg.png";
    g_pTextureManager->AddTexture("shop-bg-img", szPath, true);
    pBGImage->SetName("shop-bg-img");
    pBGImage->SetLocalPos(vShopPos);
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stShopSize.w, stShopSize.h);
    m_pShopLayer->AddUIObject(pBGImage);

    // ���� �̸�
    cUITextView* pShopNameText = new cUITextView;
    pShopNameText->SetName("shop-name-tex");
    pShopNameText->SetLocalPos(Vector3(vShopPos.x + 240, vShopPos.y + 20, 0));
    pShopNameText->SetSize(Vector2(200, 15));
    pShopNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_TITLE));
    pShopNameText->SetText("STORE");
    pShopNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pShopLayer->AddUIObject(pShopNameText);

    // X��ư
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

    // ���Թ�ư
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

    // ���� �̸�
    cUITextView* pBuyText = new cUITextView;
    pBuyText->SetName("shop-buy-tex");
    pBuyText->SetLocalPos(Vector3(vShopPos.x + stShopSize.w - 110, vShopPos.y + stShopSize.h - 27, 0));
    pBuyText->SetSize(Vector2(50, 15));
    pBuyText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
    pBuyText->SetText("BUY");
    pBuyText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
    m_pShopLayer->AddUIObject(pBuyText);

    // ���� ������ �ؽ�Ʈ
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

    // ���� �̹���
    cUIImageView* pCoinImage = new cUIImageView;
    szPath = INTERFACE_PATH + (string)"coin.png";
    g_pTextureManager->AddTexture("shop-coin-img", szPath, true);
    pCoinImage->SetName("shop-coin-img");
    pCoinImage->SetLocalPos(Vector3(vShopPos.x + 200, vShopPos.y + stShopSize.h - 27, 0));
    pCoinImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-coin-img", &imageInfo));
    pCoinImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pCoinImage->SetScale(18, 18);
    m_pShopLayer->AddUIObject(pCoinImage);

    SetShopItemUI(vShopPos);
}

void cShop::SetShopItemUI(Vector3 vShopPos)
{
    // ������ �̹���
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
        string szPath = m_vecItemInfo[i].szPath;
        string szTexKey = m_vecItemInfo[i].szName;

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

        // �̹���
        cUIImageView* pItemImage = new cUIImageView;
        g_pTextureManager->AddTexture(szTexKey, szPath, true);
        pItemImage->SetName(szTexKey);
        pItemImage->SetLocalPos(vItemPos);
        IMAGE_INFO imageInfo;
        pItemImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szTexKey, &imageInfo));
        pItemImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemImage->SetScale(stItemSize.w, stItemSize.h);
        m_pShopLayer->AddUIObject(pItemImage);

        // ���� ��ư
        cUIButton* pSelectButton = new cUIButton;
        pSelectButton->SetName(m_vecItemInfo[i].szName);
        pSelectButton->SetLocalPos(Vector3(0, 0, 0));
        pSelectButton->SetButtonState();
        pSelectButton->SetTexture("", "", "");
        pSelectButton->SetSize(Vector2(stItemSize.w + stItemContentsSize.w, stItemSize.h));
        pSelectButton->SetUIButton(m_pShopLayer);
        pItemImage->AddChild(pSelectButton);

        // ���� �̹���
        cUIImageView* pItemSelectImage = new cUIImageView;
        g_pTextureManager->AddTexture("item-select", INTERFACE_PATH + (string)"item/item-select.png", true);
        pItemSelectImage->SetName(m_vecItemInfo[i].szName + "select-img");
        pItemSelectImage->SetLocalPos(Vector3(vItemPos.x - 3, vItemPos.y - 3, 0));
        pItemSelectImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("item-select", &imageInfo));
        pItemSelectImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemSelectImage->SetScale(stItemSize.w + 6, stItemSize.h + 6);
        pItemSelectImage->SetAxtive(false);
        if (i == m_eCurrSelectItem)
        {
            pItemSelectImage->SetAxtive(true);
        }
        m_pShopLayer->AddUIObject(pItemSelectImage);

        // ������ ���� �̹���
        cUIImageView* pItemContentsImage = new cUIImageView;
        pItemContentsImage->SetName(szItemContentsName);
        pItemContentsImage->SetLocalPos(Vector3(80, 0, 0));
        pItemContentsImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture(szItemContentsName, &imageInfo));
        pItemContentsImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pItemContentsImage->SetScale(stItemContentsSize.w, stItemContentsSize.h);
        pItemImage->AddChild(pItemContentsImage);

        // �����۸� �ؽ�Ʈ
        cUITextView* pNameText = new cUITextView;
        pNameText->SetName(m_vecItemInfo[i].szName);
        pNameText->SetLocalPos(Vector3(95, 10, 0));
        pNameText->SetSize(Vector2(150, 15));
        pNameText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        pNameText->SetText(m_vecItemInfo[i].szName);
        pNameText->SetColor(D3DCOLOR_XRGB(255, 255, 0));
        pNameText->SetDrawTextFormat(DT_LEFT);
        pItemImage->AddChild(pNameText);

        // ����ġ �ؽ�Ʈ
        cUITextView* pPlusValueText = new cUITextView;
        pPlusValueText->SetName("plus-value");
        pPlusValueText->SetLocalPos(Vector3(95, 30, 0));
        pPlusValueText->SetSize(Vector2(150, 15));
        pPlusValueText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        char buf[20];
        sprintf_s(buf, sizeof(buf), "%s +%d", m_vecItemInfo[i].stStat.szName.c_str(), (int)m_vecItemInfo[i].fPlusValue);
        pPlusValueText->SetText(buf);
        pPlusValueText->SetColor(D3DCOLOR_XRGB(0, 255, 0));
        pPlusValueText->SetDrawTextFormat(DT_LEFT);
        pItemImage->AddChild(pPlusValueText);

        // ���� �̹���
        cUIImageView* pCoinImage = new cUIImageView;
        szPath = INTERFACE_PATH + (string)"coin.png";
        g_pTextureManager->AddTexture("shop-coin-img", szPath, true);
        pCoinImage->SetName("coin-img");
        pCoinImage->SetLocalPos(Vector3(95, 50, 0));
        pCoinImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-coin-img", &imageInfo));
        pCoinImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
        pCoinImage->SetScale(18, 18);
        pItemImage->AddChild(pCoinImage);

        // ���� �ؽ�Ʈ
        cUITextView* pPriceText = new cUITextView;
        pPriceText->SetName("price");
        pPriceText->SetLocalPos(Vector3(115, 50, 0));
        pPriceText->SetSize(Vector2(100, 15));
        pPriceText->SetFont(g_pFontManager->GetFont(g_pFontManager->E_SHOP_DEFAULT));
        sprintf_s(buf, sizeof(buf), "%d", m_vecItemInfo[i].nPrice);
        pPriceText->SetText(buf);
        pPriceText->SetDrawTextFormat(DT_LEFT);
        pItemImage->AddChild(pPriceText);
    }
}