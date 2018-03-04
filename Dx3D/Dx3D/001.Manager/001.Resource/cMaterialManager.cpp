#include "stdafx.h"
#include "cMaterialManager.h"


cMaterialManager::cMaterialManager()
{
}


cMaterialManager::~cMaterialManager()
{
}

HRESULT cMaterialManager::Setup()
{
    MATERIAL9 mWhite = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);

    m_mapMaterial.insert(make_pair("white", mWhite));

    return S_OK;
}

HRESULT cMaterialManager::Update()
{
    return E_NOTIMPL;
}

HRESULT cMaterialManager::Render()
{
    return E_NOTIMPL;
}

HRESULT cMaterialManager::Destroy()
{
    return E_NOTIMPL;
}

MATERIAL9 cMaterialManager::InitMtrl(XColor a, XColor d, XColor s, XColor e, float p)
{
    MATERIAL9 mtrl;
    mtrl.Ambient = a;
    mtrl.Diffuse = d;
    mtrl.Specular = s;
    mtrl.Emissive = e;
    mtrl.Power = p;

    return mtrl;
}
