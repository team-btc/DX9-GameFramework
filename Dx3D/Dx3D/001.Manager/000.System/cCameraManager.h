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
    Vector3     m_vPickingPos;
    char        m_szBuffer[256] = { 0 };
    bool        m_isCollidable;

public:
    // iSingletonManager��(��) ���� ��ӵ�
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
    void EnableCollider() { m_isCollidable = true; }
    void DisableCollider() { m_isCollidable = false; }
    void ToggleCollider() { m_isCollidable = !m_isCollidable; }
};

