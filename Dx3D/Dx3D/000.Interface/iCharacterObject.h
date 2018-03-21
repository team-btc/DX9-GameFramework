#pragma once
#include "stdafx.h"

class cSkinnedMesh;
class cMonster;
class cUILayer;

interface iCharacterObject :public cObject
{
public:
    enum E_TAG { PLAYER = 1, MONSTER , BOSS, NPC, END_TAG };
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
    float           m_fMoveSpeed;
    LPMESH          m_pPikingMesh; // ������ ���� ������
    ST_SPHERE       m_stSphere; // ������ �� ���������� ��ŷ�ǰԲ���

    // ����
    ST_STATUS       m_stStat;
    ST_STATUS       m_stGrowthStat;
   
    E_TAG           m_eTag;
    E_STATE         m_eState;


    bool            isAttack; //�������̳�
    bool            isRun;
    bool            isWalk;
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
    //virtual void Destroy() PURE;
#pragma region Set
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
    virtual void SetMoveSpeed(float speed)PURE;

    virtual void SetStatus(ST_STATUS stat)PURE;

    virtual void SetTag(E_TAG Tag) PURE;
    virtual void SetState(E_STATE State) PURE;

    virtual void SetAttack(bool Attack) PURE;
    virtual void SetRun(bool Run) PURE;
    virtual void SetWalk(bool Walk)PURE;
    virtual void SetIdle(bool Idle) PURE;
    virtual void SetHeal(bool Heal) PURE;
    virtual void SetStatic(bool Static) PURE;
    virtual void SetActive(bool Active) PURE;
    virtual void SetAlive(bool Alive) PURE;
    virtual void SetMoveToTarget(bool MoveToTarget) PURE;
#pragma endregion

#pragma region Get
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
    virtual float GetMoveSpeed() PURE;

    virtual ST_STATUS& GetStatus() PURE;

    virtual E_TAG GetTag() PURE;
    virtual E_STATE GetState() PURE;

    virtual bool GetAttak() PURE;
    virtual bool GetRun() PURE;
    virtual bool GetWalk() PURE;
    virtual bool GetIdle() PURE;
    virtual bool GetHeal() PURE;
    virtual bool GetStatic() PURE;
    virtual bool GetActive() PURE;
    virtual bool GetAlive() PURE;
    virtual bool GetMoveToTarget() PURE;
#pragma endregion

    //���� ����
    virtual bool RayCast(iCharacterObject* Charater) PURE;

    //�׼� �� �Լ�
    virtual void Action(string Command, float value) PURE;
    virtual void Attack(float ATK) PURE;
    virtual void Heal(float Value) PURE;

    //�ִϸ��̼� ����
    virtual void AttackAnim() PURE;
    virtual void RunAnim() PURE;
    virtual void WalkAnim() PURE;
    virtual void IdleAnim() PURE;
    virtual void LeftAnim() PURE;
    virtual void RightAnim() PURE;
    virtual void DeadAnim() PURE;
    virtual void FalseAnim() PURE;

    //��Ʈ�ѷ� �Լ�
    virtual void MoveForword() PURE;
    virtual void MoveBackword() PURE;
    virtual void RotateLeft() PURE;
    virtual void RotateRight() PURE;

    // �Ÿ�üũ �Լ�
    virtual void NearestSearch(vector<cMonster*> _vec) PURE;
    virtual float Distance(Vector3 Pos) PURE;

    //  ������
    virtual void SumExp(int exp) PURE;

    //  ���� ����
    virtual void UpdateStatus() PURE;
};