#include "stdafx.h"
#include "cGear.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cGear::cGear()
    : m_pGearLayer(NULL)
    , m_nCurrSelectGear(-1)
    , m_isClickGear(false)
    , m_isOpen(false)
{
    m_rtGearSize.left = W_WIDTH - (LONG)(W_WIDTH * 0.18f) - 300;
    m_rtGearSize.top = 100;
    m_rtGearSize.right = m_rtGearSize.left + (LONG)(W_WIDTH * 0.18f);
    m_rtGearSize.bottom = m_rtGearSize.top + (LONG)(W_HEIGHT * 0.35f);

    m_stGearSlotSize = ST_SIZE(60, 60);
}

cGear::~cGear()
{
}

void cGear::Setup()
{
    m_pGearLayer = new cUILayer;
    m_pGearLayer->SetLayer("Inven", Vector3(0, 0, 0), ST_SIZE(W_WIDTH, W_HEIGHT));
    m_pGearLayer->SetActive(true);

    m_stCurrGear = g_pGameManager->GetGearInfo();

    SetGearUI();

    if (m_pGearLayer)
    {
        m_pGearLayer->Setup();
    }
}

void cGear::Update()
{
    m_isClickGear = false;

    if (!m_pGearLayer)
    {
        return;
    }

    // 기본 정보 로드
    m_stCurrGear = g_pGameManager->GetGearInfo();
    // 계속해서 장비와 스탯 체크하고 업데이트
    UpdateGearUI();

    // 장비창 렉트 안에 마우스가 있다면
    if (PtInRect(&m_rtGearSize, g_ptMouse))
    {
        // 버튼 클릭 무효화를 위해 
        g_pKeyManager->isOnceKeyDown(VK_LBUTTON);
        g_pKeyManager->isOnceKeyDown(VK_RBUTTON);
        m_isClickGear = true;
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
}

void cGear::Render()
{
    if (m_pGearLayer)
    {
        m_pGearLayer->Render();
    }
}

void cGear::OpenGear()
{
    m_isClickGear = false;
    m_isOpen = true;
    m_stCurrGear = g_pGameManager->GetGearInfo();
    UpdateGearUI();
}

void cGear::CloseGear()
{
    m_isClickGear = false;
    m_isOpen = false;
}

ULONG cGear::Release(void)
{
    SAFE_DELETE(m_pGearLayer);

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

    SetStatUI(vGearPos);
}

void cGear::SetStatUI(Vector3 vGearPos)
{
    // 아이템 이미지
    ST_SIZE stSlotSize = m_stGearSlotSize;
    Vector3 vItemPos(vGearPos.x + 190, vGearPos.y + 85, 0);
    float fPlusY = stSlotSize.h + 5;

    for (int i = 0; i < 3; ++i)
    {
        if (i != 0)
        {
            vItemPos.y += fPlusY;
        }

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

void cGear::UpdateGearUI()
{
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
