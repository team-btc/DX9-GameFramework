#pragma once
#include "stdafx.h"

class cSkinnedMesh;
class cMonster;
class cUILayer;

interface iCharacterObject
{
public:
    enum E_TAG { PLAYER = 1, MONSTER, NPC, END_TAG };
    enum E_STATE {END_STATE};

protected:
    iCharacterObject* m_pTarget;

    cSkinnedMesh*   m_pMesh; // ��Ų
    cUILayer*       m_pUILayer;
    Matrix4         m_MatScale; // ũ��
    Matrix4         m_MatRotate; // ȸ�� ��Ʈ����
    Matrix4         m_MatTrans; // ��ġ ��Ʈ����
    Vector3         m_vDir; // ����
    Vector3         m_vPosition; // ��ġ ����
    float           m_fRotY; // ����
    LPMESH          m_pPikingMesh; // ������ ���� ������
    ST_SPHERE       m_stSphere; // ������ �� ���������� ��ŷ�ǰԲ���
    
    // ����
    float           m_fATK;
    float           m_fDEF;
    float           m_fSTR;
    float           m_fDEX;
    float           m_fINT;
    float           m_fHP;
    float           m_fMP;
    float           m_fSpeed;
    E_TAG           m_eTag;
    int             m_Level;
    E_STATE         m_eState;

    bool            isAttack; //�������̳�
    bool            isRun;
    bool            isIdle;
    bool            isHeal;
    bool            isStatic; //
    bool            isActive; // 
    bool            isAlive; // ����ִ� ����üũ
    bool            isMoveToTarget; // Ÿ������ �̵��ϴ� �Ұ�

public:
    virtual void Setup() PURE;
    virtual void Update() PURE;
    virtual void Render() PURE;
    virtual void Destroy() PURE;

    virtual void SetTarget(iCharacterObject* target) PURE;
    virtual void SetSkinnedMesh(cSkinnedMesh* Mesh) PURE;
    virtual void SetUILayer(cUILayer* UILayer) PURE;
    virtual void SetScale(Matrix4 Scale) PURE;
    virtual void SetRotate(Matrix4 Rotate) PURE;
    virtual void SetSphere(ST_SPHERE Sphere) PURE;
    virtual void SetTrans(Matrix4 Trans) PURE;
    virtual void SetDir(Vector3 dir) PURE;
    virtual void SetPosition(Vector3 Pos) PURE;
    virtual void SetPikingMesh(LPMESH Mesh) PURE;
    virtual void SetRotY(float RotY) PURE;
    virtual void SetATK(float ATK) PURE;
    virtual void SetDEF(float DEF) PURE;
    virtual void SetSTR(float STR) PURE;
    virtual void SetDEX(float DEX) PURE;
    virtual void SetINT(float INT) PURE;
    virtual void SetHP(float HP) PURE;
    virtual void SetMP(float MP) PURE;
    virtual void SetSpeed(float Speed) PURE;
    virtual void SetTag(E_TAG Tag) PURE;
    virtual void SetLevel(int Level) PURE;
    virtual void SetState(E_STATE State) PURE;
    virtual void SetAttack(bool Attack) PURE;
    virtual void SetRun(bool Run) PURE;
    virtual void SetIdle(bool Idle) PURE;
    virtual void SetHeal(bool Heal) PURE;
    virtual void SetStatic(bool Static) PURE;
    virtual void SetActive(bool Active) PURE;
    virtual void SetAlive(bool Alive) PURE;
    virtual void SetMoveToTarget(bool MoveToTarget) PURE;

    virtual iCharacterObject* GetTarget() PURE;
    virtual cSkinnedMesh* GetSkinnedMesh() PURE;
    virtual cUILayer* GetUILayer() PURE;
    virtual Matrix4 GetScale() PURE;
    virtual Matrix4 GetRotate() PURE;
    virtual Matrix4 GetTrans() PURE;
    virtual Vector3 GetDir() PURE;
    virtual Vector3 GetPosition() PURE;
    virtual ST_SPHERE GetSphere() PURE;
    virtual LPMESH GetPikingMesh() PURE;
    virtual float GetRotY() PURE;
    virtual float GetATK() PURE;
    virtual float GetDEF() PURE;
    virtual float GetSTR() PURE;
    virtual float GetDEX() PURE;
    virtual float GetINT() PURE;
    virtual float GetHP() PURE;
    virtual float GetMP() PURE;
    virtual float GetSpeed() PURE;
    virtual E_TAG GetTag() PURE;
    virtual int GetLevel() PURE;
    virtual E_STATE GetState() PURE;
    virtual bool GetAttak() PURE;
    virtual bool GetRun() PURE;
    virtual bool GetIdle() PURE;
    virtual bool GetHeal() PURE;
    virtual bool GetStatic() PURE;
    virtual bool GetActive() PURE;
    virtual bool GetAlive() PURE;
    virtual bool GetMoveToTarget() PURE;

    //���� ����
    virtual bool RayCast(iCharacterObject* Charater) PURE;

    //�׼� �� �Լ�
    virtual void Action(string Command, string value) PURE;
    virtual void Attack(int ATK) PURE;
    virtual void Heal(int Value) PURE;

    //�ִϸ��̼� ����
    virtual void AttackAnim() PURE;
    virtual void RunAnim() PURE;
    virtual void IdleAnim() PURE;
    virtual void FalseAnim() PURE;

    //��Ʈ�ѷ� �Լ�
    virtual void MoveForword() PURE;
    virtual void MoveBackword() PURE;
    virtual void RotateLeft() PURE;
    virtual void RotateRight() PURE;

    // �Ÿ�üũ �Լ�
    virtual void NearestSearch(vector<cMonster*> _vec) PURE;
    virtual float Distance(Vector3 Pos) PURE;
};