#pragma once
#include "stdafx.h"
#include "iGameObject.h"

interface iCamera : public iGameObject
{
protected:
    float           m_fFov;

public:
    //  ī�޶� �þ߰� ����
    virtual HRESULT SetFov(IN const float fov) PURE;
};