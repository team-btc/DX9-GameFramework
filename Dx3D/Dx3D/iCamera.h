#pragma once
#include "stdafx.h"
#include "iGameObject.h"

interface iCamera : public iGameObject
{
protected:
    iGameObject*    m_pTargetObject;
    float           m_fFov;

public:
    //  카메라 시야각 설정
    virtual HRESULT SetFov(IN const float fov) PURE;
};