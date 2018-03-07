#include "stdafx.h"
#include "cUnitObject.h"


cUnitObject::cUnitObject()
{
    Init();
}


cUnitObject::~cUnitObject()
{
}

HRESULT cUnitObject::Init()
{
    m_pMesh = NULL;

    return S_OK;
}

HRESULT cUnitObject::Setup()
{
    return E_NOTIMPL;
}

HRESULT cUnitObject::Update()
{
    return E_NOTIMPL;
}

HRESULT cUnitObject::Render()
{
    if (m_pMesh)
    {
        m_pMesh->UpdateAndRender();
    }

    return S_OK;
}

HRESULT cUnitObject::Detroy()
{
    m_pMesh->Release();

    return S_OK;
}

ULONG cUnitObject::Release()
{
    SAFE_RELEASE(m_pMesh);

    return cObject::Release();
}

void cUnitObject::Load(string szKey)
{
    m_pMesh = new cSkinnedMesh(szKey);
}

