#include "stdafx.h"
#include "cTitleScene.h"

cTitleScene::cTitleScene()
    : m_pCamera(NULL)
    , m_pTerrain(NULL)
    , m_pWater(NULL)
    , m_pTextureShader(NULL)
    , m_pSkyBoxShader(NULL)
    , m_pWaveShader(NULL)
    , m_nCurrIndex(0)
    , m_vStartPos(0.0f, 0.0f, 0.0f)
    , m_vCameraPos(259.47f, 171.95f, 279.68f)
    , m_vArthasPos(261.0f, 88.0f, 482.0f)
    , m_vSindraPos(229.0f, 199.0f, 242.0f)
    , m_fWorldTime(-1.0f)
{
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 199.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 199.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(230.0f, 400.0f, 242.0f));
    m_vecSindraJumpTarget.push_back(Vector3(280.0f, -88.0f, 630.0f));
   
    for (int i = 0; i < m_vecSindraJumpTarget.size(); ++i)
    {
        m_vecIsArriveSindra.push_back(false);
    }
}


cTitleScene::~cTitleScene()
{
}

HRESULT cTitleScene::Start()
{
    //  LOAD TERRAIN MESH
    D3DXLoadMeshFromXA("Assets\\Map\\icecrown\\icecrown.x", D3DXMESH_32BIT | D3DXMESH_MANAGED,
        g_pDevice, NULL, NULL, NULL, NULL, &m_pTerrain);

    D3DXLoadMeshFromXA("Assets\\Map\\icecrown\\icecrown-water.x", D3DXMESH_32BIT | D3DXMESH_MANAGED,
        g_pDevice, NULL, NULL, NULL, NULL, &m_pWater);

    //  SET SKYBOX SHADER
    g_pTextureManager->AddCubeTexture("icecrown-sky", "Assets\\Texture\\SkyBox\\MidNight.dds");
    m_pSkyBoxShader = m_pSkyBoxShader == NULL ? new cSkyBoxShader : m_pSkyBoxShader;
    m_pSkyBoxShader->SetBox((LPCUBETEXTURE9)g_pTextureManager->GetTexture("icecrown-sky"));

    //  SET TEXTURE SHADER
    g_pTextureManager->AddTexture("icecrown-bg", "Assets\\Texture\\Ice4.jpg");
    g_pTextureManager->AddTexture("icecrown-tex1", "Assets\\Texture\\Ice.jpg");
    g_pTextureManager->AddTexture("icecrown-tex2", "Assets\\Texture\\Ice3.jpg");
    g_pTextureManager->AddTexture("icecrown-tex3", "Assets\\Texture\\Snow2.jpg");

    float fBgDensity, fTex1Density, fTex2Density, fTex3Density;
    fBgDensity = 130.0f;
    fTex1Density = 224.0f;
    fTex2Density = 60.0f;
    fTex3Density = 58.0f;
    m_pTextureShader = m_pTextureShader == NULL ? new cTextureShader : m_pTextureShader;
    m_pTextureShader->SetMesh(m_pTerrain);
    m_pTextureShader->SetBGTexture((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-bg"), fBgDensity);
    m_pTextureShader->SetTexture1((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-tex1"), fTex1Density);
    m_pTextureShader->SetTexture2((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-tex2"), fTex2Density);
    m_pTextureShader->SetTexture3((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-tex3"), fTex3Density);

    //  SET WATER MESH
    g_pTextureManager->AddTexture("icecrown-water", "Assets\\Texture\\DarkSky.jpg");
    m_pWaveShader = m_pWaveShader == NULL ? new cWaveShader : m_pWaveShader;
    m_pWaveShader->SetMesh(m_pWater);
    m_pWaveShader->SetWaveTexture((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-water"));

    //  Arthas ¼¼ÆÃ
    m_pArthas = m_pArthas == NULL ? new cSkinnedMesh("arthaslichking") : m_pArthas;
    m_pArthas->SetPosition(m_vArthasPos);
    m_pArthas->SetScale(8.0f);
    m_pArthas->SetRotation(Vector3(0.0f, -90.0f, 0.0f));
    //m_pArthas->SetAnimationIndex(1);
    m_pArthas->SetAnimationByName("Stand");
    g_pAutoReleasePool->AddObject(m_pArthas);

    m_pSindragosa = m_pSindragosa == NULL ? new cSkinnedMesh("sindragosa") : m_pSindragosa;
    m_pSindragosa->SetScale(8.0f);
    m_pSindragosa->SetPosition(m_vSindraPos);
    m_pSindragosa->SetRotation(Vector3(0.0f, -90.0f, 0.0f));
    m_pSindragosa->SetAnimationByName("Stand");

    //  CAMERA SETUP
    m_pCamera = m_pCamera == NULL ? new cCamera : m_pCamera;
    m_pCamera->EnableFocus();
    m_pCamera->SetMaxDist(400.0f);
    m_pCamera->SetMinDist(5.0f);
    m_pCamera->Setup();
    m_pCamera->SetPosition(m_vCameraPos);
    m_pCamera->SetTargetPos(m_vSindraPos);
    g_pCameraManager->AddCamera("title", m_pCamera);
    g_pCameraManager->DisableCollider();
    g_pCameraManager->SetCurrCamera("title");

    //  INFO PHRASE SETUP
    m_pBGLayer = new cUILayer;
    m_pBGLayer->SetLayer("press", Vector3(0.0f, 0.0f, 0.0f), ST_SIZE(1300.0f, 150.0f));
    string sz = "Assets\\Interface\\Button\\press-space-to-start.png";
    g_pTextureManager->AddTexture("press-tex", sz, true);
    cUIImageView* pUIBG = new cUIImageView;
    pUIBG->SetName("press");
    pUIBG->SetLocalPos(Vector3(150.0f, 650.0f, 0.0f));
    IMAGE_INFO imageInfo;
    pUIBG->SetTexture((LPTEXTURE9)g_pTextureManager->GetTexture("press-tex", &imageInfo));
    pUIBG->SetSize(Vector2((float)imageInfo.Width, (float)imageInfo.Height));
    pUIBG->SetScale(1.0f, 1.0f);
    m_pBGLayer->AddUIObject(pUIBG);
    m_pBGLayer->SetActive(true);
    m_pBGLayer->Setup();
    m_pBGLayer->Update();

    return S_OK;
}

HRESULT cTitleScene::Update()
{
    if (m_nCurrIndex == -1 &&
        g_pKeyManager->isOnceKeyDown(VK_SPACE))
    {
        g_pScnManager->SetNextSceneName("play");
        g_pScnManager->ChangeScene("loading");
    }

    return S_OK;
}

HRESULT cTitleScene::Render()
{
    Matrix4 matW;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    Vector4 vP(g_vCameraPos, 1.0f);
    if (m_pSkyBoxShader)
    {
        m_pSkyBoxShader->Render(vP);
    }

    if (m_pArthas)
    {
        m_pArthas->UpdateAndRender();
    }

    if (m_pSindragosa)
    {
        m_pSindragosa->UpdateAndRender();
    }
  
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    if (m_pTextureShader)
    {
        m_pTextureShader->Render();
    }

    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

    if (m_pWaveShader)
    {
        m_pWaveShader->Render(vP);
    }

    if (m_pBGLayer)
    {
        m_pBGLayer->Render();
    }

    return S_OK;
}

ULONG cTitleScene::Release()
{
    SAFE_DELETE(m_pTextureShader);
    SAFE_DELETE(m_pSkyBoxShader);
    SAFE_DELETE(m_pWaveShader);
    SAFE_DELETE(m_pBGLayer);

    return cObject::Release();
}

void cTitleScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cTitleScene::SetAnimation(string szAnimName, cSkinnedMesh* pMesh)
{
    pMesh->SetAnimationByName(szAnimName);
}
