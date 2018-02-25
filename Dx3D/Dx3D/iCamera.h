#pragma once
#include "stdafx.h"
#include "iGameObject.h"

interface iCamera : public iGameObject
{
protected:
    iGameObject*    m_pTargetObject;
    float           m_fFov;

public:
    //  ī�޶� �þ߰� ����
    virtual HRESULT SetFov(IN const float fov) PURE;
};