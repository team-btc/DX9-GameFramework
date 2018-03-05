#pragma once
#include "iSingletonManager.h"

#define g_pMaterialManager  cMaterialManager::GetInstance()

class cMaterialManager : public iSingletonManager
{
    SINGLETON(cMaterialManager);

private:
    map<string, MATERIAL9>  m_mapMaterial;

public:
    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;

public:
    MATERIAL9 InitMtrl(XColor a, XColor d, XColor s, XColor e, float p);
    MATERIAL9* GetMaterial(string szKey);
};