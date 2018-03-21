#pragma once
#include "cGameObject.h"

#define LOOKAT_POS 1.0f
#define LIMIT_ROTX (90.0f - D3DX_16F_EPSILON)

class cCamera : public cGameObject
{
private:
    float           m_fDistance;    // 큐브 기준 카메라 거리
    Vector3         m_vEye;         // 카메라 위치
    Vector3         m_vTargetPos;
    Vector3         m_vLookAt;      // 바라보는 위치
    Vector3         m_vUp;          // 임의의 업 벡터

    Matrix4         m_matView;
    Matrix4         m_matProj;

    POINT           m_ptPrevMouse;
    bool            m_isRButtonDown;
    bool            m_isFocus;
    bool            m_isControl;
    bool            m_isMovable;
    float           m_fFov;
    float           m_fMaxDist;
    float           m_fMinDist;
    float           m_fLookatOffset;

public:
    cCamera();
    ~cCamera();

    HRESULT Setup();
    virtual ULONG Release() override;

    HRESULT Update() override;

    Matrix4 GetViewProjMatrix() { return m_matView * m_matProj; }
    void SetTargetPos(Vector3 vPos) { m_vTargetPos = vPos; }
    void SetLength(float l) { m_fDistance = l; }
    float GetLength() { return m_fDistance; }
    Vector3 GetEye() { return m_vEye; }
    void SetEye(Vector3 pos) { m_vEye = pos; }
    Vector3 GetLookAt() { return m_vLookAt; }
    void SetLookAt(Vector3 pos) { m_vLookAt = pos; }

    void ToggleControl() { m_isControl = !m_isControl; }
    void EnableControl() { m_isControl = true; }
    void DisableControl() { m_isControl = false; }

    void ToggleFocus() { m_isFocus = !m_isFocus; }
    void EnableFocus() { m_isFocus = true; m_isMovable = false; }
    void DisableFocus() { m_isFocus = false; }

    void ToggleMove() { m_isMovable = !m_isMovable; }
    void EnableMove() { m_isMovable = true; m_isFocus = false; }
    void DisableMove() { m_isMovable = false; }

    float GetLookatOffset() { return m_fLookatOffset; }
    void SetLookatOffset(float val) { m_fLookatOffset = val; }
    void SetFov(float fov);
    float GetFov() { return m_fFov; }

    void SetDist(float val) { m_fDistance = val; }
    void SetMinDist(float val) { m_fMinDist = val; }
    void SetMaxDist(float val) { m_fMaxDist = val; }

    void SetControl(bool b) { m_isControl = b; }
};