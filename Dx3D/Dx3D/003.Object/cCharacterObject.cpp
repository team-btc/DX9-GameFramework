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
   m_fRotY = 0.0f;
   m_fMoveSpeed = 0.0f;
   m_stSphere.fRadius = 0.0f;
   m_stSphere.isPicked = false;
   m_stSphere.isRender = false;
   m_stSphere.vCenter = Vector3(0, 0, 0);
   m_pPikingMesh = NULL;

   
   m_eTag = END_TAG;
   m_eState = END_STATE;

   isAttack = false;
   isRun = false;
   isWalk = false;
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

void cCharacterObject::Attack(float ATK)
{
    if (m_pTarget)
    {
        //데미지 계산식을 넣어야함
        m_pTarget->GetStatus().fCurHP -= ATK;
        cout << m_pTarget->GetStatus().fCurHP << endl;
    }
}

void cCharacterObject::Heal(float Value)
{
    m_stStat.fCurHP += Value;
}

void cCharacterObject::AttackAnim()
{
    FalseAnim();
    isAttack = true;
    int RandomNum = rand() % 3;
    if (RandomNum == 0)
    {
        m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Attack0")->second.nStateNum);
    }
    else if (RandomNum == 1)
    {
        m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Attack1")->second.nStateNum);
    }
    else if (RandomNum == 2)
    {
        m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Attack2")->second.nStateNum);
    }

}

void cCharacterObject::RunAnim()
{
    FalseAnim();
    isRun = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Run")->second.nStateNum);
}

void cCharacterObject::WalkAnim()
{
    FalseAnim();
    isWalk = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Walk")->second.nStateNum);
}

void cCharacterObject::IdleAnim()
{
    FalseAnim();
    isIdle = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Stand")->second.nStateNum);
}

void cCharacterObject::LeftAnim()
{
    FalseAnim();
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("TurnLeft")->second.nStateNum);
}

void cCharacterObject::RightAnim()
{
    FalseAnim();
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("TurnRight")->second.nStateNum);
}

void cCharacterObject::DeadAnim()
{
    FalseAnim();
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Death")->second.nStateNum);
}

void cCharacterObject::FalseAnim()
{
   isAttack = false;
   isRun = false;
   isWalk = false;
   isIdle = false;
   isHeal = false;
}

void cCharacterObject::MoveForword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition += m_vDir* m_fMoveSpeed;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::MoveBackword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition -= m_vDir* m_fMoveSpeed;
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacterObject::RotateLeft()
{
    m_fRotY -= 0.05f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
    
}

void cCharacterObject::RotateRight()
{
    m_fRotY += 0.05f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);
}

void cCharacterObject::NearestSearch(vector<cMonster*> _vec)
{
    float nearDist = FLT_MAX;
    for (auto iter = _vec.begin(); iter != _vec.end(); iter++)
    {
        if (D3DXVec3Length(&(*iter)->GetPosition()) < nearDist)
        {
            nearDist = D3DXVec3Length(&(*iter)->GetPosition());
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
