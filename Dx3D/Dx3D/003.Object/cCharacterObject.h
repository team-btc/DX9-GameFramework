#pragma once
#include "iCharacterObject.h"

#define Speed 0.3f

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

#pragma region Set
    virtual void SetTarget(iCharacterObject* target) override { m_pTarget = target; }
    virtual void SetSkinnedMesh(cSkinnedMesh* Mesh) override { m_pMesh = Mesh; }
    virtual void SetUILayer(cUILayer* UILayer)override { m_pUILayer = UILayer; }
    virtual void SetScale(Matrix4 Scale) override { m_MatScale = Scale; }
    virtual void SetRotate(Matrix4 Rotate) override { m_MatRotate = Rotate; }
    virtual void SetTrans(Matrix4 Trans) override { m_MatTrans = Trans; }
    virtual void SetDir(Vector3 dir) override { m_vDir = dir; }
    virtual void SetPosition(Vector3 Pos) override { m_vPosition = Pos; m_stSphere.vCenter = Pos; }
    virtual void SetSphere(ST_SPHERE Sphere) override { m_stSphere = Sphere; };
    virtual void SetPikingMesh(LPMESH Mesh) override { m_pPikingMesh = Mesh; }
    virtual void SetMoveSpeed(float speed) override { m_fMoveSpeed = speed; }

    virtual void SetStatus(ST_STATUS stat) override { m_stStat = stat; }
   
    virtual void SetTag(E_TAG Tag) override { m_eTag = Tag; }
    virtual void SetState(E_STATE State) override { m_eState = State; }

    virtual void SetAttack(bool Attack) { isAttack = Attack; }
    virtual void SetRun(bool Run) { isRun = Run; }
    virtual void SetWalk(bool Walk) { isWalk = Walk; }
    virtual void SetIdle(bool Idle) override { isIdle = Idle; }
    virtual void SetHeal(bool Heal) override { isHeal = Heal; }
    virtual void SetStatic(bool Static) override { isStatic = Static; }
    virtual void SetActive(bool Active) override { isActive = Active; }
    virtual void SetAlive(bool Alive) override { isAlive = Alive; }
    virtual void SetMoveToTarget(bool MoveToTarget) { isMoveToTarget = MoveToTarget; }
#pragma endregion

#pragma region Get
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
    virtual float GetMoveSpeed()  override { return m_fMoveSpeed; }

    virtual ST_STATUS& GetStatus() override { return m_stStat; }
    
    virtual E_TAG GetTag() override { return m_eTag; }
    virtual E_STATE GetState() override { return m_eState; }

    virtual bool GetAttak() override { return isAttack; }
    virtual bool GetRun() override { return isRun; }
    virtual bool GetWalk() override { return isWalk; }
    virtual bool GetIdle() override { return isIdle; }
    virtual bool GetHeal() override { return isHeal; }
    virtual bool GetStatic() override { return isStatic; }
    virtual bool GetActive() override { return isActive; }
    virtual bool GetAlive() override { return isAlive; }
    virtual bool GetMoveToTarget() override { return isMoveToTarget; }
#pragma endregion

    virtual bool RayCast(iCharacterObject* Charater) override;

    virtual void Action(string Command, float value) override;
    virtual void Attack(float ATK) override;
    virtual void Heal(float Value) override;

    virtual void AttackAnim() override;
    virtual void RunAnim() override;
    virtual void WalkAnim() override;
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

