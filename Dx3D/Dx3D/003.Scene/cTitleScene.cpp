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
    , m_vArthasPos(243.59f, 97.0f, 450.0f)
    , m_vSindraPos(229.0f, 199.0f, 242.0f)
    , m_fWorldTime(-1.0f)
    , m_isPressSpace(false)
    , m_isPopup(false)
    , m_nSoundIndex(0)
{
    g_pSndManager->AddSound("title-bgm", "title", "Assets\\Sound\\BGM\\title\\ir_frozenthrone_06.mp3", true, true);
    g_pSndManager->AddSound("title-start", "title", "Assets\\Sound\\Effect\\Arthaslichking\\ic_lich king_frostmourne_is_hunger.ogg");
    g_pSndManager->AddSound("title-walk", "title", "Assets\\Sound\\Effect\\Footstep\\dirt\\mon_footstep_bipedal_foot_medium_dirt_011.ogg", false, false);
    g_pSndManager->AddSound("title-fly", "title", "Assets\\Sound\\Effect\\Sindragosa\\hugewingflap3.ogg", false, true);
    g_pSndManager->AddSound("title-roar", "title", "Assets\\Sound\\Effect\\Sindragosa\\spell_dk_artifact_sindragosasfury_dragonroar.ogg");
    g_pSndManager->AddSound("title-breath", "title", "Assets\\Sound\\Effect\\Deathwing\\mon_deathwing_death_02.ogg");

}


cTitleScene::~cTitleScene()
{
    SAFE_DELETE(m_pSCLayer);
    SAFE_DELETE(m_pBGLayer);
}

HRESULT cTitleScene::Start()
{
    m_fSpeed = 60.0f;

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
    g_pTextureManager->AddTexture("icecrown-alphamap", "Assets\\Map\\IceCrown\\IceCrown.png");

    float fBgDensity, fTex1Density, fTex2Density, fTex3Density;
    fBgDensity = 130.0f;
    fTex1Density = 224.0f;
    fTex2Density = 60.0f;
    fTex3Density = 58.0f;
    m_pTextureShader = m_pTextureShader == NULL ? new cTextureShader : m_pTextureShader;
    m_pTextureShader->SetMesh(m_pTerrain);
    m_pTextureShader->SetAlphaDraw((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-alphamap"));
    m_pTextureShader->SetBGTexture((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-bg"), fBgDensity);
    m_pTextureShader->SetTexture1((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-tex1"), fTex1Density);
    m_pTextureShader->SetTexture2((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-tex2"), fTex2Density);
    m_pTextureShader->SetTexture3((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-tex3"), fTex3Density);

    //  SET WATER MESH
    g_pTextureManager->AddTexture("icecrown-water", "Assets\\Texture\\DarkSky.jpg");
    m_pWaveShader = m_pWaveShader == NULL ? new cWaveShader : m_pWaveShader;
    m_pWaveShader->SetMesh(m_pWater);
    m_pWaveShader->SetWaveTexture((LPTEXTURE9)g_pTextureManager->GetTexture("icecrown-water"));

    //  Arthas ����
    m_pArthas = m_pArthas == NULL ? new cSkinnedMesh("arthaslichking") : m_pArthas;
    m_pArthas->SetPosition(m_vArthasPos);
    m_pArthas->SetScale(22.0f);
    m_pArthas->SetRotation(Vector3(0.0f, -70.0f, -15.0f));
    //m_pArthas->SetAnimationIndex(1);
    m_pArthas->SetAnimationByName("CastChanneling");
    m_pArthas->SetAnimationSpeed(m_pArthas->GetAnimationSpeed() * 0.5f);
    g_pAutoReleasePool->AddObject(m_pArthas);

    m_pSindragosa = m_pSindragosa == NULL ? new cSkinnedMesh("sindragosa") : m_pSindragosa;
    m_pSindragosa->SetScale(8.0f);
    m_pSindragosa->SetPosition(m_vSindraPos);
    m_pSindragosa->SetRotation(Vector3(0.0f, -90.0f, 0.0f));
    m_pSindragosa->SetAnimationByName("Stand");
    g_pAutoReleasePool->AddObject(m_pSindragosa);

    //  CAMERA SETUP
    m_pCamera = m_pCamera == NULL ? new cCamera : m_pCamera;
    m_pCamera->Setup();
    m_pCamera->SetLookAt(Vector3(256.0f, 104.0f, 481.0f));
    m_pCamera->SetEye(Vector3(255.77f, 101.21f, 490.6f));
    m_pCamera->SetRotation(Vector3(14.0f, -0.4f, 0.0f));
    m_pCamera->SetDist(10.0f);
    m_pCamera->EnableMove();
    g_pCameraManager->SetCollisionMesh(m_pTerrain);
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

    sz = "Assets\\Texture\\blue.png";
    g_pTextureManager->AddTexture("blue", sz, true);
    m_pSCLayer = new cUILayer;
    m_pSCLayer->SetLayer("press", Vector3(0.0f, 0.0f, 0.0f), ST_SIZE(W_WIDTH, W_HEIGHT), true, D3DCOLOR_RGBA(255,255,255,255), "blue");
    m_pSCLayer->SetTransparent(true);
    m_pSCLayer->SetAlphaInterval(2);
    m_pSCLayer->SetDeltaInterval(0.01f);
    m_pSCLayer->SetActive(true);
    g_pSndManager->Play("title-bgm", 0.5f);
    return S_OK;
}

HRESULT cTitleScene::Update()
{

    //m_pSCLayer->Update();
    if (m_fWorldTime < 0.0f)
    {
        m_pSindragosa->SetAnimationByName("Stand");
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.0f;
        m_fRoarTime = g_pTimerManager->GetWorldTime() + 5.75f;
    }
    if (m_nSoundIndex == 0)
    {
        if (m_fRoarTime < g_pTimerManager->GetWorldTime())
        {
            g_pSndManager->Play("title-roar");
            m_nSoundIndex++;
        }
    }
    if (m_nCurrIndex == 0)
    {
      
        if (m_fWorldTime < g_pTimerManager->GetWorldTime())
        {
            m_nCurrIndex++;
            m_pSindragosa->SetAnimationByName("Roar");
            m_fWorldTime = g_pTimerManager->GetWorldTime() + 5.8f;

        }
    }
    else if (m_nCurrIndex == 1)
    {
        if (m_fWorldTime < g_pTimerManager->GetWorldTime())
        {
            m_nCurrIndex++;
            m_pSindragosa->SetAnimationByName("FlySitGroundUp");
            m_fWorldTime = g_pTimerManager->GetWorldTime() + 3.0f;
            if (m_nSoundIndex == 1)
            {
                g_pSndManager->Play("title-fly");
                m_nSoundIndex++;
            }
        }
    }
    else if (m_nCurrIndex == 2)
    {
        Vector3 rot = m_pSindragosa->GetRotation();
        rot.z -= 5.0f * g_pTimerManager->GetDeltaTime();
        m_pSindragosa->SetRotation(rot);
        Vector3 pos = m_pSindragosa->GetPosition();
        pos.y += m_fSpeed * g_pTimerManager->GetDeltaTime();
        m_fSpeed += 1.0f;
        m_pSindragosa->SetPosition(pos);
        if (m_fWorldTime < g_pTimerManager->GetWorldTime())
        {
            m_nCurrIndex++;
            Vector3 pos(255.77f, 400.0f, 320.00f);
            m_pSindragosa->SetPosition(pos);
            m_pSindragosa->SetRotation(Vector3(0.0f, -90.0f, -60.0f));
            m_pSindragosa->SetScale(20.0f);
            m_fSpeed = 100.0f;
            m_pSindragosa->SetAnimationByName("FlyWalk");
            m_fWorldTime = g_pTimerManager->GetWorldTime() + 15.0f;
            g_pSndManager->Pause("title-fly");
        }
    }
    else if (m_nCurrIndex == 3)
    {
        Vector3 pos = m_pSindragosa->GetPosition();
        pos.y -= m_fSpeed * g_pTimerManager->GetDeltaTime();
        pos.z += m_fSpeed * 0.4f * g_pTimerManager->GetDeltaTime();
        m_pSindragosa->SetPosition(pos);
        if (m_fWorldTime < g_pTimerManager->GetWorldTime())
        {
            m_nCurrIndex++;
        }
        if (g_pTimerManager->GetWorldTime() + 12.0f > m_fWorldTime)
        {
            m_isPopup = true;
        }
    }
    else if (m_nCurrIndex == 4)
    {
        m_fWalkTime = g_pTimerManager->GetWorldTime() + 1.0f;
    }
    else if (m_nCurrIndex == 5)
    {

        Vector3 pos = m_pArthas->GetPosition();
        pos.x += 10.0f * g_pTimerManager->GetDeltaTime();
        pos.z += 30.0f * g_pTimerManager->GetDeltaTime();
        pos.y += 2.0f * g_pTimerManager->GetDeltaTime();
        m_pArthas->SetPosition(pos);
        if (m_fWorldTime < g_pTimerManager->GetWorldTime())
        {
            //  SET BLACK SCREEN
            m_nCurrIndex = 6;
        }
        if (m_fWalkTime < g_pTimerManager->GetWorldTime())
        {
            g_pSndManager->Play("title-walk", 0.7f);
            m_fWalkTime = g_pTimerManager->GetWorldTime() + 1.5f;
        }
    }
    else if (m_nCurrIndex == 6)
    {
        m_pSCLayer->Update();
        if (m_pSCLayer->GetTwinkleCount() == 1)
        {
            m_pSCLayer->SetTransparent(false);
            m_pSCLayer->SetBackGroundColor(D3DCOLOR_RGBA(255, 255, 255, 255));
            m_nCurrIndex = 10;
        }
        if (m_fWalkTime < g_pTimerManager->GetWorldTime())
        {
            g_pSndManager->Play("title-walk", 0.7f);
            m_fWalkTime = g_pTimerManager->GetWorldTime() + 1.5f;
        }

    }
    else if (m_nCurrIndex == 10)
    {
        g_pSndManager->Pause("title-bgm");
        //g_pScnManager->SetNextSceneName("play");
        g_pScnManager->ChangeScene("play");
    }

    if (m_isPopup && g_pKeyManager->isOnceKeyDown(VK_SPACE))
    {
        if (m_nSoundIndex == 2)
        {
            g_pSndManager->Play("title-start");
        }
        m_pArthas->SetAnimationByName("Walk");
        m_pArthas->SetAnimationSpeed(1.0f);
        m_pArthas->SetRotation(Vector3(0.0f, -80.0f, 0.0f));
        m_nCurrIndex = 5;
        m_isPressSpace = true;
        m_fWorldTime = g_pTimerManager->GetWorldTime() + 1.7f;
    }

    if (g_pKeyManager->isOnceKeyDown(VK_RETURN))
    {
        m_nCurrIndex = 10;
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

    if (m_isPopup && m_pArthas)
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

    if (m_isPopup && m_pBGLayer)
    {
        m_pBGLayer->Render();
    }
    m_pSCLayer->Render();
    return S_OK;
}

ULONG cTitleScene::Release()
{
    SAFE_RELEASE(m_pTerrain);
    SAFE_RELEASE(m_pWater);
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
