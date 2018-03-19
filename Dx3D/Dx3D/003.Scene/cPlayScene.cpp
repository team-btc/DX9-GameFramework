#include "stdafx.h"
#include "cPlayScene.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIProgressBar.h"

cPlayScene::cPlayScene()
    : m_pPlayerStatUILayer(NULL)
    , m_pHPUILayer(NULL)
    , m_fPlayerMaxHP(100)
    , m_fPlayerCurrHP(50)
    , m_fPlayerMaxMP(100)
    , m_fPlayerCurrMP(70)
{
}


cPlayScene::~cPlayScene()
{
}

HRESULT cPlayScene::Start()
{
    //  LOAD MAP

    SetUI();

    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Setup();
    }

    return E_NOTIMPL;
}

HRESULT cPlayScene::Update()
{
    //  UPDATE MAP
    //  UPDATE PLAYER
    //  UPDATE ENEMY

    if (m_pPlayerStatUILayer)
    {
        UpdateUI();
        m_pPlayerStatUILayer->Update();
    }

    return E_NOTIMPL;
}

HRESULT cPlayScene::Render()
{
    //  RENDER MAP
    //  RENDER PLAYER
    //  RENDER ENEMY
    //  RENDER OBJECT
    //  RENDER UI

    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Render();
    }

    // 몬스터를 따라다니는 UI!!
    cUIProgressBar* pProgress = NULL;
    if (m_pHPUILayer)
    {
        // HP
        cUIObject* pObject;
        m_pHPUILayer->FindUIObject(&pObject, "default-hp");
        if (pObject)
        {
            pProgress = (cUIProgressBar*)pObject;
        }
    }
    if (pProgress)
    {
        Vector3 screenPos(0, 0, 100);

        Vector3 vLength = g_vCameraPos - screenPos;
        float fLength = fabsf(D3DXVec3Length(&vLength));
        if (fLength > 1000.0f)
        {
            fLength = 1000.0f;
        }
        fLength = (1000.0f - fLength) / 1000.0f;

        Matrix4 matView, matProj, matVP;

        g_pDevice->GetTransform(D3DTS_VIEW, &matView);
        g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

        D3DVIEWPORT9 vp;
        g_pDevice->GetViewport(&vp);
        D3DXMatrixIdentity(&matVP);
        matVP._11 = vp.Width / 2.0f;
        matVP._22 = -(vp.Height / 2.0f);
        matVP._33 = vp.MaxZ - vp.MinZ;
        matVP._41 = vp.X + vp.Width / 2.0f;
        matVP._42 = vp.Y + vp.Height / 2.0f;
        matVP._43 = vp.MinZ;

        D3DXVec3TransformCoord(&screenPos, &screenPos, &(matView * matProj * matVP));

        pProgress->SetLocalPos(screenPos);
        pProgress->SetMaxGuage(100);
        pProgress->SetCurrentGuage(20);
        pProgress->SetSize(Vector2(80, 15) * fLength);
        m_pHPUILayer->Update();
        m_pHPUILayer->Render();
    }

    return E_NOTIMPL;
}

ULONG cPlayScene::Release()
{
    SAFE_DELETE(m_pPlayerStatUILayer);
    SAFE_DELETE(m_pHPUILayer);

    cObject::Release();

    return 0;
}

void cPlayScene::SetUI()
{
    ST_SIZE stLayerSize = ST_SIZE(W_WIDTH / 4.0f, W_HEIGHT / 10.0f);
    Vector2 vHP_MPSize = Vector2(stLayerSize.w * 0.8f, stLayerSize.h * 0.3f);

    ST_SIZE stMonLayerSize = ST_SIZE(80, 15);
    Vector2 vMonHPSize = Vector2(stMonLayerSize.w, stMonLayerSize.h);

    string szRedPath = INTERFACE_PATH + (string)"buttons/redgrad64.png";
    string szGreenPath = INTERFACE_PATH + (string)"buttons/greengrad64.png";
    string szBluePath = INTERFACE_PATH + (string)"buttons/bluegrad64.png";
    string szGreyPath = INTERFACE_PATH + (string)"buttons/greyscaleramp64.png";

    LPFONTDX pFont = g_pFontManager->GetFont(g_pFontManager->E_QUEST);

    // 캐릭터 스탯 레이어 초기화
    m_pPlayerStatUILayer = new cUILayer;
    m_pPlayerStatUILayer->SetLayer("player-stat", Vector3(0, 0, 0), stLayerSize, false);
    m_pPlayerStatUILayer->SetActive(true);

    // HP바
    cUIProgressBar* pUIProgressHP = new cUIProgressBar;
    pUIProgressHP->SetSize(vHP_MPSize);
    pUIProgressHP->AddGuageTexture(szGreenPath, 0);
    pUIProgressHP->AddGuageTexture(szRedPath, 1);
    pUIProgressHP->SetMaxGuage(m_fPlayerMaxHP);
    pUIProgressHP->SetCurrentGuage(m_fPlayerCurrHP);
    pUIProgressHP->SetLocalPos(Vector3(80, 15, 0));
    pUIProgressHP->SetName("player-hp");
    pUIProgressHP->AddText(pFont, 0);
    m_pPlayerStatUILayer->AddUIObject(pUIProgressHP);

    // MP바
    cUIProgressBar* pUIProgressMP = new cUIProgressBar;
    pUIProgressMP->SetSize(vHP_MPSize);
    pUIProgressMP->AddGuageTexture(szBluePath, 0);
    pUIProgressMP->AddGuageTexture(szRedPath, 1);
    pUIProgressMP->SetMaxGuage(m_fPlayerMaxHP);
    pUIProgressMP->SetCurrentGuage(m_fPlayerCurrHP);
    pUIProgressMP->SetLocalPos(Vector3(80, 50, 0));
    pUIProgressMP->SetName("player-mp");
    pUIProgressMP->AddText(pFont, 0);
    m_pPlayerStatUILayer->AddUIObject(pUIProgressMP);

    // 기본 HP 레이어 셋팅 (몬스터용)
    m_pHPUILayer = new cUILayer;
    m_pHPUILayer->SetLayer("default-hp", Vector3(0, 0, 0), stMonLayerSize, false);
    m_pHPUILayer->SetActive(true);

    // HP바
    pUIProgressHP = new cUIProgressBar;
    pUIProgressHP->SetSize(vMonHPSize);
    pUIProgressHP->AddGuageTexture(szRedPath, 0);
    pUIProgressHP->AddGuageTexture(szGreyPath, 1);
    pUIProgressHP->SetMaxGuage(100);
    pUIProgressHP->SetCurrentGuage(30);
    pUIProgressHP->SetName("default-hp");
    m_pHPUILayer->AddUIObject(pUIProgressHP);
}

void cPlayScene::UpdateUI()
{
    cUIObject* pObject;

    // HP
    m_pPlayerStatUILayer->FindUIObject(&pObject, "player-hp");
    if (pObject)
    {
        cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
        pProgress->SetCurrentGuage(m_fPlayerCurrHP);
    }
    // MP
    m_pPlayerStatUILayer->FindUIObject(&pObject, "player-mp");
    if (pObject)
    {
        cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
        pProgress->SetCurrentGuage(m_fPlayerCurrMP);
    }

}
