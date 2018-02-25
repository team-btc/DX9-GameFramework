#include "stdafx.h"
#include "cHeightMap.h"
#include "cMtlTex.h"

cHeightMap::cHeightMap()
    : m_pMesh(NULL)
    , m_pMtlTex(NULL)
    , m_fSizeX(1.0f)
    , m_fSizeZ(1.0f)
{
}


cHeightMap::~cHeightMap()
{
    SAFE_RELEASE(m_pMesh);
    SAFE_RELEASE(m_pMtlTex);
}

void cHeightMap::Load(IN char* szFilePath, IN Matrix4* pMat/*= NULL*/)
{
}

vector<Vector3>& cHeightMap::GetVertex()
{
    return m_vecVertex;
}

vector<DWORD>& cHeightMap::GetIndex()
{
    return m_vecIndex;
}

void cHeightMap::Render()
{
    Matrix4 matW;
    D3DXMatrixIdentity(&matW);
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    g_pDevice->SetMaterial(m_pMtlTex->GetMtl());
    g_pDevice->SetTexture(0, m_pMtlTex->GetTexture());
    m_pMesh->DrawSubset(0);
}
