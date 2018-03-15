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

void cCharacterObject::Action(string Command, string value)
{
    if (Command == "Attack")
    {
        int ATK = atoi(value.c_str());
        Attack(ATK);
    }
    else if (Command == "Heal")
    {
        int Value = atoi(value.c_str());
        Heal(Value);
    }
}

void cCharacterObject::Attack(int ATK)
{
    if (m_pTarget)
    {
        m_pTarget->SetHP(m_pTarget->GetHP() - ATK);
        cout << m_pTarget->GetHP() << endl;
    }
}

void cCharacterObject::Heal(int Value)
{
    m_fHP += Value;
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
    isIdle = true;
    m_pMesh->SetAnimationIndex(m_mapStateInfo.find("TurnLeft")->second.nStateNum);
}

void cCharacterObject::RightAnim()
{
    FalseAnim();
    isIdle = true;
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
