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

    cSkinnedMesh*   m_pMesh; // 스킨
    cUILayer*       m_pUILayer;
    Matrix4         m_MatScale; // 크기
    Matrix4         m_MatRotate; // 회전 매트릭스
    Matrix4         m_MatTrans; // 위치 매트릭스
    Vector3         m_vDir; // 방향
    Vector3         m_vPosition; // 위치 벡터
    float           m_fRotY; // 각도
    LPMESH          m_pPikingMesh; // 가상의 구를 보여줌
    ST_SPHERE       m_stSphere; // 가상의 구 실질적으로 피킹되게끔함

    // 스탯
    ST_STATUS       m_stStat;
   
    E_TAG           m_eTag;
    E_STATE         m_eState;


    bool            isAttack; //공격중이냐
    bool            isRun;
    bool            isIdle;
    bool            isHeal;
    bool            isStatic; //
    bool            isActive; // 
    bool            isAlive; // 살아있는 유무체크
    bool            isMoveToTarget; // 타겟으로 이동하는 불값

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

    virtual void SetStatus(ST_STATUS stat)PURE;

    virtual void SetTag(E_TAG Tag) PURE;
    virtual void SetState(E_STATE State) PURE;

    virtual void SetAttack(bool Attack) PURE;
    virtual void SetRun(bool Run) PURE;
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

    virtual ST_STATUS& GetStatus() PURE;

    virtual E_TAG GetTag() PURE;
    virtual E_STATE GetState() PURE;

    virtual bool GetAttak() PURE;
    virtual bool GetRun() PURE;
    virtual bool GetIdle() PURE;
    virtual bool GetHeal() PURE;
    virtual bool GetStatic() PURE;
    virtual bool GetActive() PURE;
    virtual bool GetAlive() PURE;
    virtual bool GetMoveToTarget() PURE;
#pragma endregion

    //상대방 설정
    virtual bool RayCast(iCharacterObject* Charater) PURE;

    //액션 폼 함수
    virtual void Action(string Command, float value) PURE;
    virtual void Attack(float ATK) PURE;
    virtual void Heal(float Value) PURE;

    //애니메이션 변경
    virtual void AttackAnim() PURE;
    virtual void RunAnim() PURE;
    virtual void IdleAnim() PURE;
    virtual void LeftAnim() PURE;
    virtual void RightAnim() PURE;
    virtual void DeadAnim() PURE;
    virtual void FalseAnim() PURE;

    //컨트롤러 함수
    virtual void MoveForword() PURE;
    virtual void MoveBackword() PURE;
    virtual void RotateLeft() PURE;
    virtual void RotateRight() PURE;

    // 거리체크 함수
    virtual void NearestSearch(vector<cMonster*> _vec) PURE;
    virtual float Distance(Vector3 Pos) PURE;
};