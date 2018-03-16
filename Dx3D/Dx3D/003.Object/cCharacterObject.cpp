#include "stdafx.h"
#include "cCharacterObject.h"
#include "cSkinnedMesh.h"
#include "cMonster.h"


cCharacterObject::cCharacterObject()
{
   m_pTarget = NULL;
   m_pMesh = NULL;
   m_pUILayer = NULL;
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
   m_pPikingMesh = NULL;

   
   m_eTag = END_TAG;
   m_eState = END_STATE;

   isAttack = false;
   isRun = false;
   isIdle = false;
   isHeal = false;
   isStatic = false;
   isActive = false;
   isAlive = true;
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

//void cCharacterObject::Destroy()
//{
//}

bool cCharacterObject::RayCast(iCharacterObject * Charater)
{
    m_pTarget = Charater;
    return false;
}

void cCharacterObject::Action(string Command, float value)
{
    if (Command == "Attack")
    {
        Attack(value);
    }
    else if (Command == "Heal")
    {
        Heal(value);
    }
    else if (Command == "Skil1")
    {

    }
    else if (Command == "Skil2")
    {

    }
}

void cCharacterObject::Attack(int ATK)
{
    if (m_pTarget)
    {
        m_pTarget->GetStatus().m_fCurHP -= ATK;
        cout << m_pTarget->GetStatus().m_fCurHP << endl;
    }
}

void cCharacterObject::Heal(int Value)
{
    m_stStat.m_fCurHP += Value;
}

void cCharacterObject::AttackAnim()
{
    FalseAnim();
    isAttack = true;

    if (m_eTag == PLAYER)
    {
        int RandomNum = rand() % 3;
        if (RandomNum == 0)
            m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Attack")->second.nStateNum);
        else if (RandomNum == 1)
            m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Attack2")->second.nStateNum);
        else if (RandomNum == 2)
            m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Attack3")->second.nStateNum);
    }
    else
        m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Attack")->second.nStateNum);
}

void cCharacterObject::RunAnim()
{
    FalseAnim();
    isRun = true;
    m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Run")->second.nStateNum);
}

void cCharacterObject::IdleAnim()
{
    FalseAnim();
    isIdle = true;
    m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Stand")->second.nStateNum);
}

void cCharacterObject::LeftAnim()
{
    FalseAnim();
    m_pMesh->SetAnimationIndex(m_mapStateInfo.find("TurnLeft")->second.nStateNum);
}

void cCharacterObject::RightAnim()
{
    FalseAnim();
    m_pMesh->SetAnimationIndex(m_mapStateInfo.find("TurnRight")->second.nStateNum);
}

void cCharacterObject::DeadAnim()
{
    FalseAnim();
    m_pMesh->SetAnimationIndex(m_mapStateInfo.find("Death")->second.nStateNum);
}

void cCharacterObject::FalseAnim()
{
   isAttack = false;
   isRun = false;
   isIdle = false;
   isHeal = false;
}

void cCharacterObject::MoveForword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition += m_vDir* Speed;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::MoveBackword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition -= m_vDir* Speed;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::RotateLeft()
{
    m_fRotY -= 0.005f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
    
}

void cCharacterObject::RotateRight()
{
    m_fRotY += 0.005f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
}

void cCharacterObject::NearestSearch(vector<cMonster*> _vec)
{
    float nearDist = 9999;
    for (auto iter = _vec.begin(); iter != _vec.end(); iter++)
    {
        if (Distance((*iter)->GetPosition()) < nearDist)
        {
            nearDist = Distance((*iter)->GetPosition());
            RayCast(*iter);
        }
    }
}

float cCharacterObject::Distance(Vector3 Pos)
{
    Vector3 vDistance = Pos - m_vPosition;
    float fDistance = D3DXVec3Length(&vDistance);
    
    return fDistance;
}
