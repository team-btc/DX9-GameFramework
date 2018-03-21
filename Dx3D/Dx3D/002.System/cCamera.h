#pragma once
#include "cGameObject.h"

#define LOOKAT_POS 1.0f
#define LIMIT_ROTX (90.0f - D3DX_16F_EPSILON)

class cCamera : public cGameObject
{
private:
    float           m_fDistance;    // ť�� ���� ī�޶� �Ÿ�
    Vector3         m_vEye;         // ī�޶� ��ġ
    Vector3         m_vTargetPos;
    Vector3         m_vLookAt;      // �ٶ󺸴� ��ġ
    Vector3         m_vUp;          // ������ �� ����

    Matrix4         m_matView;
    Matrix4         m_matProj;

    Vector3         m_vRotation;
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

    void TrackingToggle() { m_isFocus = !   m_isFocus; }
    void TrackingEnable() { m_isFocus = true; }
    void TrackingDisable() { m_isFocus = false; }
    void EnableControl() { m_isControl = true; }
    void DisableControl() { m_isControl = false; }
    void ToggleControl() { m_isControl = !m_isControl; }
    void ToggleFocus() { m_isFocus = !m_isFocus; }
    void EnableFocus() { m_isFocus = true; }
    void DisableFocus() { m_isFocus = false; }er

    float GetLookatOffset() { return m_fLookatOffset; }
    void SetLookatOffset(float val) { m_fLookatOffset = val; }
    void SetFovy(float fov);

    void SetDist(float val) { m_fDistance = val; }
    void SetMinDist(float val) { m_fMinDist = val; }
    void SetMaxDist(float val) { m_fMaxDist = val; }

    void SetControl(bool b) { m_isControl = b; }
};