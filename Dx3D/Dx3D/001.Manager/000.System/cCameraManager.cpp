#include "stdafx.h"
#include "cCameraManager.h"


cCameraManager::cCameraManager()
    : m_isCollidable(false)
    , m_pCollisionMesh(NULL)
    , m_pCurrCamera(NULL)
    , m_pRay(NULL)
{
}


cCameraManager::~cCameraManager()
{
}

HRESULT cCameraManager::Setup()
{
    cCamera* pDefaultCamera = new cCamera;
    pDefaultCamera->Setup();
    AddCamera("default", pDefaultCamera);
    m_pCurrCamera = pDefaultCamera;

    return S_OK;
}

HRESULT cCameraManager::Update()
{
    if (m_pCurrCamera)
    {
        if (m_isCollidable)
        {
            if (m_pCollisionMesh)
            {
                if (!m_pRay)
                {
                    m_pRay = new cRay;
                }
                m_pRay->m_vOrg = m_pCurrCamera->GetLookat();
                m_pRay->m_vDir = m_pCurrCamera->GetEye() - m_pRay->m_vOrg;
                D3DXVec3Normalize(&m_pRay->m_vDir, &m_pRay->m_vDir);
                BOOL isHit = FALSE;
                float fDist = FLT_MAX;
                D3DXIntersectSubset(m_pCollisionMesh,
                    0,
                    &m_pRay->m_vOrg,
                    &m_pRay->m_vDir,
                    &isHit,
                    NULL, NULL, NULL,
                    &fDist,
                    NULL, NULL);

                if (isHit)
                {
                    m_pCurrCamera->SetDist(fDist);
                }
            }
        }

        m_pCurrCamera->Update();
    }
#ifdef _DEBUG
    else
    {
        cout << "NO CAMERA" << endl;
    }
#endif // _DEBUG

    return S_OK;
}

HRESULT cCameraManager::Render()
{
    RECT rt = { W_WIDTH - 200, 0, W_WIDTH, W_HEIGHT };

    string szCamPos;
    string szLookatPos;
    string szPickPos;
    
    sprintf_s(m_szBuffer, -1, "CAM POS : %.2f, %.2f, %.2f", g_vCameraPos.x, g_vCameraPos.y, g_vCameraPos.z);
    szCamPos = m_szBuffer;
    sprintf_s(m_szBuffer, -1, "LOOKAT POS : %.2f, %.2f, %.2f",
        m_pCurrCamera->GetLookat().x, m_pCurrCamera->GetLookat().y, m_pCurrCamera->GetLookat().z);
    szLookatPos = m_szBuffer;
    g_pFontManager->GetFont(
        cFontManager::E_DEBUG)->DrawTextA(
            NULL, szCamPos.c_str(), -1, &rt, DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
    rt.top = 15;
    g_pFontManager->GetFont(
        cFontManager::E_DEBUG)->DrawTextA(
            NULL, szLookatPos.c_str(), -1, &rt, DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

    return S_OK;
}

HRESULT cCameraManager::Destroy()
{
    auto iter = m_mapCamera.begin();
    for (; iter != m_mapCamera.end();)
    {
        SAFE_RELEASE(iter->second);
        iter = m_mapCamera.erase(iter);
    }

    return S_OK;
}

void cCameraManager::AddCamera(string szKey, cCamera* pCam)
{
    m_mapCamera.insert(make_pair(szKey, pCam));
}

void cCameraManager::RemoveCamera(string szKey)
{
    m_mapCamera.erase(szKey);
}

void cCameraManager::SetCurrCamera(string szKey)
{
    m_pCurrCamera = m_mapCamera[szKey] == NULL ? m_pCurrCamera : m_mapCamera[szKey];
}
