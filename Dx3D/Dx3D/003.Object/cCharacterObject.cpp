#include "stdafx.h"
#include "cCharacterObject.h"
#include "cSkinnedMesh.h"
#include "cMonster.h"
#include "cPlayer.h"


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
   isMove = false;
   isRun = false;
   isWalk = false;
   isIdle = false;
   isHeal = false;
   isRoar = false;
   isFire = false;

   isRecovery = false;
   isAction = false;
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
    else if (Command == "Death")
    {
        m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Death")->second.nStateNum);
    }
}

void cCharacterObject::Attack(float ATK)
{
    if (m_pTarget)
    {
        //데미지 계산식을 넣어야함
        m_pTarget->GetStatus().fCurHP -= ATK;
        if (m_pTarget->GetTag() == PLAYER)
        {
            cPlayer* Player = (cPlayer*)m_pTarget;
            Player->SetRecoveryCount(0.0f);
        }
    }
}

void cCharacterObject::Heal(float Value)
{
    m_stStat.fCurHP+Value >m_stStat.fMaxHP ? m_stStat.fCurHP=m_stStat.fMaxHP : m_stStat.fCurHP += Value;
}

void cCharacterObject::AttackAnim()
{
    FalseAnim();
    isAttack = true;

    if (m_eTag == PLAYER)
    {
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
    else
    {
        m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Attack0")->second.nStateNum);
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

void cCharacterObject::HealAnim()
{
    FalseAnim();
    isHeal = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("CastReady")->second.nStateNum);
}

void cCharacterObject::RoarAnim()
{
    FalseAnim();
    isRoar = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("Roar")->second.nStateNum);
}

void cCharacterObject::CastingAnim()
{
    FalseAnim();
    isCasting = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("CastChanneling")->second.nStateNum);
}

void cCharacterObject::FireAnim()
{
    FalseAnim();
    isFire = true;
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("CastSpell")->second.nStateNum);
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

void cCharacterObject::FlyWalkAnim()
{
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("FlyWalk")->second.nStateNum);
}

void cCharacterObject::FlySitUpAnim()
{
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("FlySitGroundUp")->second.nStateNum);
}

void cCharacterObject::FlySitAnim()
{
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("FlySitGround")->second.nStateNum);
}

void cCharacterObject::FlySitDownAnim()
{
    m_pMesh->SetAnimationIndex(m_pMesh->GetStateInfo().find("FlySitGroundDown")->second.nStateNum);
}

void cCharacterObject::FalseAnim()
{
   isAttack = false;
   isRun = false;
   isWalk = false;
   isIdle = false;
   isHeal = false;
   isRoar = false;
   isFire = false;
   isCasting = false;
   isAction = false;
}

void cCharacterObject::MoveForword()
{
    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);

    m_vPosition += m_vDir* m_fMoveSpeed * g_pTimerManager->GetDeltaTime();
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
    m_fRotY -= 0.005f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);

    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void cCharacterObject::RotateRight()
{
    m_fRotY += 0.005f;
    D3DXMatrixRotationY(&m_MatRotate, m_fRotY);

    m_vDir.x = sin(m_fRotY);
    m_vDir.z = cos(m_fRotY);
    D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void cCharacterObject::NearestSearch(vector<iCharacterObject*> _vec)
{
    float nearDist = FLT_MAX;
    for (auto iter = _vec.begin(); iter != _vec.end(); iter++)
    {
        Vector3 _Dir = (*iter)->GetPosition() - m_vPosition;
        D3DXVec3Normalize(&_Dir, &_Dir);
        if (D3DXVec3Dot(&_Dir, &m_vDir) < 0)
            continue;

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