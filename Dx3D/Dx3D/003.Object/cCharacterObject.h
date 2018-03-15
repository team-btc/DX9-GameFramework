#pragma once
#include "iCharacterObject.h"

class cMonster;

class cCharacterObject : public iCharacterObject
{
private:


public:
    cCharacterObject();
    ~cCharacterObject();

    virtual void Setup() override;
    virtual void Update() override;
    virtual void Render() override;
  //  virtual void Destroy() override;

    virtual void SetTarget(iCharacterObject* target) override { m_pTarget = target; }
    virtual void SetSkinnedMesh(cSkinnedMesh* Mesh) override { m_pMesh = Mesh; }
    virtual void SetUILayer(cUILayer* UILayer)override { m_pUILayer = UILayer; }
    virtual void SetScale(Matrix4 Scale) override { m_MatScale = Scale; }
    virtual void SetRotate(Matrix4 Rotate) override { m_MatRotate = Rotate; }
    virtual void SetTrans(Matrix4 Trans) override { m_MatTrans = Trans; }
    virtual void SetDir(Vector3 dir) override { m_vDir = dir; }
    virtual void SetPosition(Vector3 Pos) override { m_vPosition = Pos; }
    virtual void SetSphere(ST_SPHERE Sphere) override { m_stSphere = Sphere; };
    virtual void SetPikingMesh(LPMESH Mesh) override { m_pPikingMesh = Mesh; }
    virtual void SetRotY(float RotY) override { m_fRotY = RotY; }
    virtual void SetATK(float ATK) override { m_fATK = ATK; }
    virtual void SetDEF(float DEF) override { m_fDEF = DEF; }
    virtual void SetSTR(float STR) override { m_fSTR = STR; }
    virtual void SetDEX(float DEX) override { m_fDEX = DEX; }
    virtual void SetINT(float INT) override { m_fINT = INT; }
    virtual void SetHP(float HP) override { m_fHP = HP; }
    virtual void SetMP(float MP) override { m_fMP = MP; }
    virtual void SetSpeed(float Speed) override { m_fSpeed = Speed; }
    virtual void SetTag(E_TAG Tag) override { m_eTag = Tag; }
    virtual void SetLevel(int Level) override { m_Level = Level; }
    virtual void SetState(E_STATE State) override { m_eState = State; }
    virtual void SetAttack(bool Attack) { isAttack = Attack; }
    virtual void SetRun(bool Run) { isRun = Run; }
    virtual void SetIdle(bool Idle) override { isIdle = Idle; }
    virtual void SetHeal(bool Heal) override { isHeal = Heal; }
    virtual void SetStatic(bool Static) override { isStatic = Static; }
    virtual void SetActive(bool Active) override { isActive = Active; }
    virtual void SetAlive(bool Alive) override { isAlive = Alive; }
    virtual void SetMoveToTarget(bool MoveToTarget) { isMoveToTarget = MoveToTarget; }

    virtual iCharacterObject* GetTarget() override { return m_pTarget; }
    virtual cSkinnedMesh* GetSkinnedMesh() override { return m_pMesh; }
    virtual cUILayer* GetUILayer() override { return m_pUILayer; }
    virtual Matrix4 GetScale() override { return m_MatScale; }
    virtual Matrix4 GetRotate() override { return m_MatRotate; }
    virtual Matrix4 GetTrans() override { return m_MatTrans; }
    virtual Vector3 GetDir() override { return m_vDir; }
    virtual Vector3 GetPosition() override { return m_vPosition; }
    virtual ST_SPHERE GetSphere() override { return m_stSphere; }
    virtual LPMESH GetPikingMesh() override { return m_pPikingMesh; }
    virtual float GetRotY() override { return m_fRotY; }
    virtual float GetATK() override { return m_fATK; }
    virtual float GetDEF() override { return m_fDEF; }
    virtual float GetSTR() override { return m_fSTR; }
    virtual float GetDEX() override { return m_fDEX; }
    virtual float GetINT() override { return m_fINT; }
    virtual float GetHP() override { return m_fHP; }
    virtual float GetMP() override { return m_fMP; }
    virtual float GetSpeed() override { return m_fSpeed; }
    virtual E_TAG GetTag() override { return m_eTag; }
    virtual int GetLevel() override { return m_Level; }
    virtual E_STATE GetState() override { return m_eState; }
    virtual bool GetAttak() override { return isAttack; }
    virtual bool GetRun() override { return isRun; }
    virtual bool GetIdle() override { return isIdle; }
    virtual bool GetHeal() override { return isHeal; }
    virtual bool GetStatic() override { return isStatic; }
    virtual bool GetActive() override { return isActive; }
    virtual bool GetAlive() override { return isAlive; }
    virtual bool GetMoveToTarget() override { return isMoveToTarget; }

    virtual bool RayCast(iCharacterObject* Charater) override;


    virtual void Action(string Command, string value) override;
    virtual void Attack(int ATK) override;
    virtual void Heal(int Value) override;


    virtual void AttackAnim() override;
    virtual void RunAnim() override;
    virtual void IdleAnim() override;
    virtual void LeftAnim() override;
    virtual void RightAnim() override;
    virtual void DeadAnim() override;
    virtual void FalseAnim() override;
    
    virtual void MoveForword() override;
    virtual void MoveBackword() override;
    virtual void RotateLeft() override;
    virtual void RotateRight() override;

    virtual void NearestSearch(vector<cMonster*> _vec) override;
    virtual float Distance(Vector3 Pos) override;
};

