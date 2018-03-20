#include "stdafx.h"
#include "cPlayScene.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIProgressBar.h"
#include "cShop.h"

cPlayScene::cPlayScene()
    : m_pPlayerStatUILayer(NULL)
    , m_pHPUILayer(NULL)
	, m_stMapInfo(NULL)
    , m_pCamera(NULL)
    , m_pGameMap(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_szMapKey("start")
    , m_pShop(NULL)
{
}


cPlayScene::~cPlayScene()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pGameMap);
    SAFE_RELEASE(m_pCamera);
    SAFE_RELEASE(m_pShop);
}

HRESULT cPlayScene::Start()
{
    //  CAMERA SETUP
    if (!m_pCamera)
    {
        m_pCamera = new cCamera;
    }
    m_pCamera->Setup();

    //  LOAD MAP
    if (m_stMapInfo == NULL)
    {
        cMapLoader mapLoader;
        mapLoader.SetKey(m_szMapKey);
        mapLoader.LoadMap();
        for (int i = 0; i < mapLoader.GetObjectMaxCnt(); ++i)
        {
            mapLoader.LoadObject(i);
        }

        m_stMapInfo = g_pMapManager->GetCurrMapInfo();

        // ���� �� ����
        if (!m_pGameMap)
        {
            m_pGameMap = new cGameMap;
        }

        m_pGameMap->SetCurrMapInfo(m_stMapInfo);

        // �ؽ��� ���̴� ����
        if (!m_pTextureShader)
        {
            m_pTextureShader = new cTextureShader;
        }

        m_pTextureShader->SetMesh(m_stMapInfo->pTerrainMesh);
        m_pTextureShader->SetBGTexture(m_stMapInfo->arrTextureInfo[0].pTexture, m_stMapInfo->arrTextureInfo[0].fDensity);
        m_pTextureShader->SetTexture1(m_stMapInfo->arrTextureInfo[1].pTexture, m_stMapInfo->arrTextureInfo[1].fDensity);
        m_pTextureShader->SetTexture2(m_stMapInfo->arrTextureInfo[2].pTexture, m_stMapInfo->arrTextureInfo[2].fDensity);
        m_pTextureShader->SetTexture3(m_stMapInfo->arrTextureInfo[3].pTexture, m_stMapInfo->arrTextureInfo[3].fDensity);
        m_pTextureShader->SetAlphaDraw(m_stMapInfo->pTextureMap);

        // ��ī�̹ڽ� ���̴� ����
        if (!m_pSkyBoxShader)
        {
            m_pSkyBoxShader = new cSkyBoxShader;
        }

        m_pSkyBoxShader->SetBox(m_stMapInfo->pSkyTexture);

        // ���� �����ϸ� ���̺� ���̴� ����
        if (m_stMapInfo->isEnableWater)
        {
            if (!m_pWaveShader)
            {
                m_pWaveShader = new cWaveShader;
            }

            m_pWaveShader->SetMesh(m_stMapInfo->pWaterMesh);
            m_pWaveShader->SetWaveTexture(m_stMapInfo->pWaterTexture);
            m_pWaveShader->SetShader(m_stMapInfo->fWaterHeight, m_stMapInfo->fWaterWaveHeight, m_stMapInfo->fWaterHeightSpeed,
                m_stMapInfo->fWaterUVSpeed, m_stMapInfo->fWaterfrequency, m_stMapInfo->fWaterTransparent);
        }
    }

    if (!m_pFrustum)
    {
        m_pFrustum = new cFrustum;
        g_pAutoReleasePool->AddObject(m_pFrustum);
    }

    m_pFrustum->Setup();

    if (!m_pPlayer)
    {
        m_pPlayer = g_pCharacterManager->GetPlayer();
    }

    //  ���� ���� ����
    string szPrevMap = "from-" + g_pMapManager->GetPrevMap();
    auto iter = m_stMapInfo->vecEventInfo.begin();
    for (; iter != m_stMapInfo->vecEventInfo.end(); iter++)
    {
        if (iter->szName == szPrevMap)
        {
            m_stMapInfo->vStartPos = iter->vPos;
            break;
        }

        if (szPrevMap == "from-" && iter->szName == "startpos")
        {
            m_stMapInfo->vStartPos = iter->vPos;
            break;
        }
    }
    m_pPlayer->SetPosition(m_stMapInfo->vStartPos);

    if (!m_vecMonster)
    {
        m_vecMonster = new vector<cMonster*>;
    }
    m_vecMonster->clear();

    for (int i = 0; i < 1; i++)
    {
        cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
        m_pEnermy->SetPosition(m_stMapInfo->vecEventInfo[0].vPos);
        m_pEnermy->SetActive(true);
        (*m_vecMonster).push_back(m_pEnermy);
    }

    m_pPlayer->SetVecMonster(m_vecMonster);
    m_pPlayer->SetTerrain(m_stMapInfo->pTerrainMesh);

    // UI ����
    SetUI();

    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Setup();
    }

    // ���� ����
    m_pShop = new cShop;
    
    if (m_pShop)
    {
        m_pShop->Setup();
    }

    return S_OK;
}

HRESULT cPlayScene::Update()
{
    //  UPDATE CAMERA
    if (m_pCamera)
    {
        if (m_pPlayer)
        {
            m_pCamera->Update(&m_pPlayer->GetPosition());
        }
        else
        {
            m_pCamera->Update();
        }
    }

    //  FRUSTUM CULL UPDATE
    m_pFrustum->Update();

    //  PLAYER UPDATE
    m_pPlayer->Update();

    // ��ġ üũ
    Vector3 Pos = m_pPlayer->GetPosition();
    m_pGameMap->GetHeight(Pos);
    m_pPlayer->SetPosition(Pos);

    if (m_vecMonster->size() == 0)
    {
        cMonster* m_pEnermy = g_pCharacterManager->GetMonster();
        m_pEnermy->SetStartPoint(m_stMapInfo->vecEventInfo[1].vPos);
        m_pEnermy->SetActive(true);
        (*m_vecMonster).push_back(m_pEnermy);
    }

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        (*iter)->Update();

        Vector3 Pos = (*iter)->GetPosition();
        m_pGameMap->GetHeight(Pos);
        (*iter)->SetPosition(Pos);
    }

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end();)
    {
        if ((*iter)->GetAlive())
        {
            iter++;
        }
        else
        {
            iter = (*m_vecMonster).erase(iter);
        }
    }

    if (m_pPlayer->GetMove())
    {
        cRay ray;
        ray.m_vOrg = m_pPlayer->GetPosition();
        ray.m_vDir = m_pPlayer->GetDir();
        // ���鿡 ��ֹ��� ���ų�, �̵� ���� �Ÿ����� �հ��� ��ֹ��� ������
        float fDist = FLT_MAX;
        if (m_pGameMap->CheckObstacle(fDist, ray) == true
            && fDist < 3.0f)
        {
            // ������ �ִ�.
            m_pPlayer->SetMoveToPoint(false);
            m_pPlayer->SetMoveToTarget(false);
        }
    }

    // UI ������Ʈ (�÷��̾� ����)
    if (m_pPlayerStatUILayer)
    {
        UpdateUI();
        m_pPlayerStatUILayer->Update();
    }

    if (m_pShop)
    {
        m_pShop->Update();
    }

    // �̺�Ʈ üũ
    string szEventName = "";
    if (m_pGameMap->CheckEvent(szEventName, m_pPlayer->GetPosition()))
    {
        // �̺�Ʈ �ߵ�
        ParseEvent(szEventName);
    }

    return S_OK;
}

HRESULT cPlayScene::Render()
{
    if (m_stMapInfo == NULL ||
        m_stMapInfo->pTerrainMesh == NULL)
    {
        return E_FAIL;
    }

    Matrix4 matW, matV, matP;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    MATERIAL9 mtrl;
    mtrl.Ambient = WHITE;
    mtrl.Diffuse = WHITE;
    mtrl.Specular = WHITE;
    mtrl.Emissive = BLACK;
    mtrl.Power = 8.0f;
    g_pDevice->SetMaterial(&mtrl);
    g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->LightEnable(0, true);

    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    Vector4 vP(g_vCameraPos, 1.0f);

    if (m_pSkyBoxShader)
    {
        m_pSkyBoxShader->Render(vP);
    }

    m_pPlayer->Render();

    for (auto iter = (*m_vecMonster).begin(); iter != (*m_vecMonster).end(); iter++)
    {
        bool result = false;
        m_pFrustum->IsInFrustum(result, &(*iter)->GetSphere());
        if (result)
        {
            (*iter)->Render();
        }
    }

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    for (int i = 0; i < m_stMapInfo->vecObjectInfo.size(); ++i)
    {
        m_stMapInfo->vecObjectInfo[i].pMesh->UpdateAndRender();
    }
    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

    if (m_stMapInfo->isEnableWater && m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    // UI ����
    if (m_pPlayerStatUILayer)
    {
        m_pPlayerStatUILayer->Render();
    }

    // ���͸� ����ٴϴ� UI!!
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
    // ���� �������� ����ϸ��!! 
    // ===============================================
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
    if (pProgress)
    {
        Vector3 vMonsterPos = m_pPlayer->GetPosition();///////////////////���ͷμ���!
        vMonsterPos.y += 10.0f; //������ Ű(����)

        Vector3 vScreenPos;
        D3DXVec3TransformCoord(&vScreenPos, &vMonsterPos, &(matView * matProj * matVP));

        pProgress->SetMaxGuage(m_pPlayer->GetStatus().fMaxHP);///////////////////���ͷμ���!
        pProgress->SetCurrentGuage(m_pPlayer->GetStatus().fCurHP);///////////////////���ͷμ���!
        vScreenPos.x -= pProgress->GetSize().x * 0.5f;
        m_pHPUILayer->SetPosition(vScreenPos);
        pProgress->SetLocalPos(vScreenPos);
        m_pHPUILayer->Update();
        m_pHPUILayer->Render();
    }
    // ===============================================

    //g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    //g_pDevice->SetRenderState(D3DRS_ZENABLE, true);

    if (m_pShop)
    {
        m_pShop->Render();
    }

#ifdef _DEBUG

    // ��ֹ�, �̺�Ʈ Ʈ�� ���
    m_pGameMap->RendObstacle();
    m_pGameMap->RendEventTrap();

#endif // _DEBUG

    return S_OK;
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

    // ĳ���� ���� ���̾� �ʱ�ȭ
    m_pPlayerStatUILayer = new cUILayer;
    m_pPlayerStatUILayer->SetLayer("player-stat", Vector3(0, 0, 0), stLayerSize);
    m_pPlayerStatUILayer->SetActive(true);

    // HP��
    cUIProgressBar* pUIProgressHP = new cUIProgressBar;
    pUIProgressHP->SetSize(vHP_MPSize);
    pUIProgressHP->AddGuageTexture(szGreenPath, 0);
    pUIProgressHP->AddGuageTexture(szRedPath, 1);
    pUIProgressHP->SetMaxGuage(m_pPlayer->GetStatus().fMaxHP);
    pUIProgressHP->SetCurrentGuage(m_pPlayer->GetStatus().fCurHP);
    pUIProgressHP->SetLocalPos(Vector3(80, 15, 0));
    pUIProgressHP->SetName("player-hp");
    pUIProgressHP->AddText(pFont, 0);
    m_pPlayerStatUILayer->AddUIObject(pUIProgressHP);

    // MP��
    cUIProgressBar* pUIProgressMP = new cUIProgressBar;
    pUIProgressMP->SetSize(vHP_MPSize);
    pUIProgressMP->AddGuageTexture(szBluePath, 0);
    pUIProgressMP->AddGuageTexture(szRedPath, 1);
    pUIProgressMP->SetMaxGuage(m_pPlayer->GetStatus().fMaxMP);
    pUIProgressMP->SetCurrentGuage(m_pPlayer->GetStatus().fCurMP);
    pUIProgressMP->SetLocalPos(Vector3(80, 50, 0));
    pUIProgressMP->SetName("player-mp");
    pUIProgressMP->AddText(pFont, 0);
    m_pPlayerStatUILayer->AddUIObject(pUIProgressMP);

    // �⺻ HP ���̾� ���� (���Ϳ�)
    m_pHPUILayer = new cUILayer;
    m_pHPUILayer->SetLayer("default-hp", Vector3(0, 0, 0), stMonLayerSize);
    m_pHPUILayer->SetActive(true);

    // HP��
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
        pProgress->SetCurrentGuage(m_pPlayer->GetStatus().fCurHP);
    }
    // MP
    m_pPlayerStatUILayer->FindUIObject(&pObject, "player-mp");
    if (pObject)
    {
        cUIProgressBar* pProgress = (cUIProgressBar*)pObject;
        pProgress->SetCurrentGuage(m_pPlayer->GetStatus().fCurMP);
    }
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_pCamera)
    {
        m_pCamera->WndProc(hWnd, message, wParam, lParam);
    }
}

void cPlayScene::ParseEvent(string szCommand)
{
    string szPrefix = szCommand.substr(0, szCommand.find('-'));
    if (szPrefix == "to")
    {
        string szPostfix = szCommand.substr(szCommand.find('-') + 1);
        TransportMap(szPostfix);
    }
}

void cPlayScene::TransportMap(string szMap)
{
    m_szMapKey = szMap;
    m_stMapInfo = NULL;
    Start();
    Update();
}
