#include "stdafx.h"
#include "cNonPlayer.h"


cNonPlayer::cNonPlayer(string szKey)
{
    m_pMesh = new cSkinnedMesh(szKey);
    g_pAutoReleasePool->AddObject(m_pMesh);

    m_eTag = NPC;
    
}

cNonPlayer::cNonPlayer()
{
}


cNonPlayer::~cNonPlayer()
{
}

void cNonPlayer::Setup()
{
}

void cNonPlayer::Update()
{
    Matrix4 matR, matW;
    D3DXMatrixScaling(&m_MatScale, 2, 2, 2);
    D3DXMatrixRotationY(&matR, -D3DX_PI / 2);
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    m_stSphere.vCenter = m_vPosition;
    matW = m_MatScale * m_MatRotate* matR * m_MatTrans;
    m_pMesh->SetWorldMatrix(matW);
}

void cNonPlayer::Render()
{
    m_pMesh->UpdateAndRender();

    Matrix4 matS, matW;
    D3DXMatrixScaling(&matS, m_stSphere.fRadius, m_stSphere.fRadius, m_stSphere.fRadius);
    D3DXMatrixTranslation(&m_MatTrans, m_stSphere.vCenter.x, m_stSphere.vCenter.y + 5.0f, m_stSphere.vCenter.z);
    matW = matS * m_MatRotate * m_MatTrans;
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pPikingMesh->DrawSubset(0);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
