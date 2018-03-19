#include "stdafx.h"
#include "cShop.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"

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
    Vector3 vLayerPos(50, 150, 0);
    ST_SIZE stLayerSize(W_WIDTH * 0.4f, W_HEIGHT * 0.5f);
    m_pShopLayer->SetLayer("shop", vLayerPos, stLayerSize);
    m_pShopLayer->SetActive(true);

    // 상점 배경
    cUIImageView* pBGImage = new cUIImageView;
    string szPath = INTERFACE_PATH + (string)"shop-bg1.png";
    g_pTextureManager->AddTexture("shop-bg", szPath, true);
    pBGImage->SetName("shop-bg");
    pBGImage->SetLocalPos(Vector3(0, 0, 0));
    IMAGE_INFO imageInfo;
    pBGImage->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("shop-bg", &imageInfo));
    pBGImage->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pBGImage->SetScale(stLayerSize.w, stLayerSize.h);
    m_pShopLayer->AddUIObject(pBGImage);

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
