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
   m_stSphere.fRadius = 0;
   m_stSphere.isPicked = false;
   m_stSphere.isRender = false;
   m_stSphere.vCenter = Vector3(0, 0, 0);

   m_fSTR =0;
   m_fDEX =0;
   m_fINT =0;
   m_fHP =0;
   m_fMP =0;
   m_fSpeed =0.01f;
   m_eTag = END_TAG;
   m_Level = 1;
   m_eState = END_STATE;

   isAttack = false;
   isStatic = false;
   isActive = false;
   isAlive = false;
   isMoveToTarget = false;
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
    m_pTarget = Charater;
    return false;
}

void cCharacterObject::Attack()
{
    m_pMesh->SetAnimationIndex(2);
}

void cCharacterObject::Idle()
{
    m_pMesh->SetAnimationIndex(0);
}

void cCharacterObject::MoveForword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition += m_vDir* m_fSpeed;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::MoveBackword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition -= m_vDir* m_fSpeed;
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

float cCharacterObject::Distance(Vector3 Pos)
{
    Vector3 vDistance = Pos - m_vPosition;
    float fDistance = D3DXVec3Length(&vDistance);
    
    return fDistance;
}
