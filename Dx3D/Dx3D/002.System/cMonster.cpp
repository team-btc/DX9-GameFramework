#include "stdafx.h"
#include "002.System\cMonster.h"


cMonster::cMonster(string szKey, string szFolder, string szFilename)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);
    m_stSphere.fRadius = 1.0f;
}

cMonster::cMonster()
{
}


cMonster::~cMonster()
{
}

void cMonster::Setup()
{
}

void cMonster::Update()
{
    Matrix4 matW;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    matW = m_MatScale * m_MatRotate * m_MatTrans;
    m_pMesh->SetWorldMatrix(matW);
}

void cMonster::Render()
{
    m_pMesh->UpdateAndRender();
}

void cMonster::Destroy()
{
}
