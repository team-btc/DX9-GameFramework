#include "stdafx.h"
#include "cShop.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cShop::cShop()
    : m_pShopLayer(NULL)
{
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

    // 정보 로드하기
    json jLoad;
    ifstream iFile;
    iFile.open(SHOP_PATH + (string)"shop.json");
    iFile >> jLoad;
    iFile.close();

    json jItem = jLoad["item"];
    for (int i = 0; i < jItem.size(); ++i)
    {
        ST_ITEM_INFO stItem;
        stItem.eType = jItem[i]["item-type"];
        string szName = jItem[i]["item-name"];
        stItem.szName = szName;
        stItem.nCount = jItem[i]["item-count"];
        stItem.nPrice = jItem[i]["item-price"];
        stItem.nPlusStat = jItem[i]["item-plus-stat"];
        stItem.fPlusValue = jItem[i]["item-plus-value"];

        m_vecItemInfo.push_back(stItem);
    }

    // 레이어 셋팅
    m_pShopLayer = new cUILayer;
    m_pShopLayer->SetLayer("shop", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pShopLayer->SetActive(true);

    // 상점 배경
    cUIImageView* pBGImage = new cUIImageView;
    Vector3 vShopPos(50, 150, 0);
    ST_SIZE stShopSize(W_WIDTH * 0.4f, W_HEIGHT * 0.6f);
    string szPath = INTERFACE_PATH + (string)"shop-bg3.png";
    g_pTextureManager->AddTexture("shop-bg-img", szPath, true);
    pBGImage->SetName("shop-bg-img");
    pBGImage->SetLocalPos(vShopPos);
    IMAGE_INFO imageInfo;
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-bg-img", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stShopSize.w, stShopSize.h);
    m_pShopLayer->AddUIObject(pBGImage);

    // 상점 이름
    cUITextView* pTextView = new cUITextView;
    pTextView->SetName("shop-name-tex");
    pTextView->SetLocalPos(Vector3(240, 15, 0));
    pTextView->SetSize(Vector2(200, 15));
    pTextView->SetFont(g_pFontManager->GetFont(g_pFontManager->E_QUEST));
    pTextView->SetText("Shop");
    m_pShopLayer->AddUIObject(pTextView);

    // 구입버튼
    cUIButton* pBuyButton = new cUIButton;
    pBuyButton->SetName("shop-buy-btn");
    pBuyButton->SetLocalPos(Vector3(vShopPos.x + stShopSize.w - 160, vShopPos.y + stShopSize.h - 32, 0));
    pBuyButton->SetButtonState();
    szPath = INTERFACE_PATH + (string)"buttons/button-normal.png";
    g_pTextureManager->AddTexture("normal-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"buttons/button-mouse-over.png";
    g_pTextureManager->AddTexture("mouse-over-btn", szPath, true);
    szPath = INTERFACE_PATH + (string)"buttons/button-select.png";
    g_pTextureManager->AddTexture("select-btn", szPath, true);
    pBuyButton->SetTexture("normal-btn", "mouse-over-btn", "select-btn");
    pBuyButton->SetSize(Vector2(150, 35));
    m_pShopLayer->AddUIObject(pBuyButton);

    if (m_pShopLayer)
    {
        m_pShopLayer->Setup();
    }
}

void cShop::Update()
{
    if (m_pShopLayer)
    {
        m_pShopLayer->Update();
    }
}

void cShop::Render()
{
    if (m_pShopLayer)
    {
        m_pShopLayer->Render();
    }
}

ULONG cShop::Release(void)
{
    SAFE_DELETE(m_pShopLayer);

    cObject::Release();

    return 0;
}
