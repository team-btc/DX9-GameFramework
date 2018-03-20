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

    Vector3         m_vRotation;
    float           m_fRotX;
    float           m_fRotY;
    POINT           m_ptPrevMouse;
    bool            m_isRButtonDown;
    bool            m_isFocus;
    bool            m_isControl;
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
    Vector3 GetLookat() { return m_vLookAt; }

    void TrackingToggle() { m_isFocus = !m_isFocus; }
    void TrackingEnable() { m_isFocus = true; }
    void TrackingDisable() { m_isFocus = false; }

    float GetLookatOffset() { return m_fLookatOffset; }
    void SetLookatOffset(float val) { m_fLookatOffset = val; }
    void SetFovy(float fov) { m_fFov = fov; }

    void SetDist(float val) { m_fDistance = val; }
    void SetMinDist(float val) { m_fMinDist = val; }
    void SetMaxDist(float val) { m_fMaxDist = val; }

    void SetControl(bool b) { m_isControl = b; }

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

