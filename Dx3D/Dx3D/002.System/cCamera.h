#pragma once
#include "cGameObject.h"

#define LOOKAT_POS 1.0f
#define LIMITED_ROT 90.0f

class cCamera : public cGameObject
{
private:
    float           m_fDistance;    // 큐브 기준 카메라 거리
    Vector3         m_vEye;         // 카메라 위치
    Vector3         m_vLookAt;      // 바라보는 위치
    Vector3         m_vUp;          // 임의의 업 벡터

    Matrix4         m_matView;
    Matrix4         m_matProj;

    float           m_fRotX;
    float           m_fRotY;
    POINT           m_ptPrevMouse;
    bool            m_isRButtonDown;
    bool            m_isFocus;

public:
    cCamera();
    ~cCamera();

    HRESULT Setup();
    virtual ULONG Release() override;

    void Update(Vector3* pTarget = NULL);

    Matrix4 GetViewProjMatrix() { return m_matView * m_matProj; }
    void SetLength(float l) { m_fDistance = l; }
    float GetLength() { return m_fDistance; }
};

