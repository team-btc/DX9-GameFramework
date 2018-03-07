#include "stdafx.h"
#include "cCharacterObject.h"
#include "cSkinnedMesh.h"


cCharacterObject::cCharacterObject()
{
   D3DXMatrixIdentity(&m_MatScale);
   D3DXMatrixIdentity(&m_MatRotate);
   D3DXMatrixIdentity(&m_MatTrans);
   m_vDir = Vector3(0, 0, 0);
   m_vPosition = Vector3(0, 0, 0);
   m_fRotY = 0;
   m_fSTR =0;
   m_fDEX =0;
   m_fINT =0;
   m_fHP =0;
   m_fMP =0;
   m_fSpeed =5;
   m_eTag = END_TAG;
   m_Level = 1;
   m_eState = END_STATE;

   isStatic = false;
   isActive = false;
   isAlive = false;
}


cCharacterObject::~cCharacterObject()
{
}

void cCharacterObject::Setup()
{
}

void cCharacterObject::Update()
{
}

void cCharacterObject::Render()
{
}

void cCharacterObject::Destroy()
{
}

bool cCharacterObject::RayCast(iCharacterObject * Charater)
{
    return false;
}

void cCharacterObject::Attack()
{
    m_pMesh->SetAnimationIndex(2, true);
}

void cCharacterObject::Idle()
{
    m_pMesh->SetAnimationIndex(0, false);
}

void cCharacterObject::MoveForword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition += m_vDir*0.01f ;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::MoveBackword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition -= m_vDir* 0.01f;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::RotateLeft()
{
    m_fRotY -= 0.01f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
}

void cCharacterObject::RotateRight()
{
    m_fRotY += 0.01f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
}
