#pragma once
#include "stdafx.h"
#include "iGameObject.h"

interface iProjectileObject : public iGameObject
{
private:
    bool    m_isAutoDestroy;
    float   m_fDestroyTime;

protected:
public:
    virtual HRESULT SetAutoDestroy() PURE;
};