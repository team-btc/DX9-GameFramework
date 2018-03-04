#pragma once
#include "cGameObject.h"

#define LOOKAT_POS 1.0f
#define LIMITED_ROT 90.0f

class cCamera : public cGameObject
{
private:
    float           m_fDistance;    // ť�� ���� ī�޶� �Ÿ�
    Vector3         m_vEye;         // ī�޶� ��ġ
    Vector3         m_vLookAt;      // �ٶ󺸴� ��ġ
    Vector3         m_vUp;          // ������ �� ����

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

    void Update(Vector3* pTarget = NULL);

    Matrix4 GetViewProjMatrix() { return m_matView * m_matProj; }
};

