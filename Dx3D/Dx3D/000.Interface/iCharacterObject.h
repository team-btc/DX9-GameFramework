#pragma once
#include "stdafx.h"

class cSkinnedMesh;

interface iCharacterObject
{
public:
    enum E_TAG { PLAYER = 1, MONSTER, NPC, END_TAG };
    enum E_STATE {END_STATE};

protected:
    iCharacterObject* m_pTarget;

    cSkinnedMesh*   m_pMesh;
    Matrix4         m_MatScale;
    Matrix4         m_MatRotate;
    Matrix4         m_MatTrans;
    Vector3         m_vDir;
    Vector3         m_vPosition;
    float           m_fRotY;
    ST_SPHERE       m_stSphere;

    float           m_fSTR;
    float           m_fDEX;
    float           m_fINT;
    float           m_fHP;
    float           m_fMP;
    float           m_fSpeed;
    E_TAG           m_eTag;
    int             m_Level;
    E_STATE         m_eState;

    bool            isAttack;
    bool            isStatic;
    bool            isActive;
    bool            isAlive;
    bool            isMoveToTarget;

public:
    virtual void Setup() PURE;
    virtual void Update() PURE;
    virtual void Render() PURE;
    virtual void Destroy() PURE;

    virtual void SetTarget(iCharacterObject* target) PURE;
    virtual void SetSkinnedMesh(cSkinnedMesh* Mesh) PURE;
    virtual void SetScale(Matrix4 Scale) PURE;
    virtual void SetRotate(Matrix4 Rotate) PURE;
    virtual void SetSphere(ST_SPHERE Sphere) PURE;
    virtual void SetTrans(Matrix4 Trans) PURE;
    virtual void SetDir(Vector3 dir) PURE;
    virtual void SetPosition(Vector3 Pos) PURE;
    virtual void SetRotY(float RotY) PURE;
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
    virtual void SetStatic(bool Static) PURE;
    virtual void SetActive(bool Active) PURE;
    virtual void SetAlive(bool Alive) PURE;
    virtual void SetMoveToTarget(bool MoveToTarget) PURE;

    virtual iCharacterObject* GetTarget() PURE;
    virtual cSkinnedMesh* GetSkinnedMesh() PURE;
    virtual Matrix4 GetScale() PURE;
    virtual Matrix4 GetRotate() PURE;
    virtual Matrix4 GetTrans() PURE;
    virtual Vector3 GetDir() PURE;
    virtual Vector3 GetPosition() PURE;
    virtual ST_SPHERE GetSphere() PURE;
    virtual float GetRotY() PURE;
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
    virtual bool GetStatic() PURE;
    virtual bool GetActive() PURE;
    virtual bool GetAlive() PURE;
    virtual bool GetMoveToTarget() PURE;

    virtual bool RayCast(iCharacterObject* Charater) PURE;
    virtual void Attack() PURE;
    virtual void Idle() PURE;
    virtual void MoveForword() PURE;
    virtual void MoveBackword() PURE;
    virtual void RotateLeft() PURE;
    virtual void RotateRight() PURE;
    virtual float Distance(Vector3 Pos) PURE;
};