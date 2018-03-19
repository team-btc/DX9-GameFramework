#pragma once
#include "iSingletonManager.h"
#include "cCamera.h"
#include "cRay.h"

#define g_pCameraManager    cCameraManager::GetInstance()

class cCameraManager : public iSingletonManager
{
    SINGLETON(cCameraManager);

private:
    map<string, cCamera*>   m_mapCamera;
    cCamera*    m_pCurrCamera;
    LPMESH      m_pCollisionMesh;
    cRay*       m_pRay;
    char        m_szBuffer[256] = { 0 };
    bool        m_isCollidable;

public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

    void AddCamera(string szKey, cCamera* pCam);
    void RemoveCamera(string szKey);
    void SetCurrCamera(string szKey);
    cCamera* GetCurrCamera() { return m_pCurrCamera; }

    Vector3 GetCameraPos() { return m_pCurrCamera->GetPosition(); }

    void SetCollisionMesh(LPMESH pMesh) { m_pCollisionMesh = pMesh; }
    void ColliderEnable() { m_isCollidable = true; }
    void ColliderDisable() { m_isCollidable = false; }
    void ColliderToggle() { m_isCollidable = !m_isCollidable; }
};

