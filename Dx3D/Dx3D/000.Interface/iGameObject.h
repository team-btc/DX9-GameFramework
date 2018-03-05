#pragma once
#include "stdafx.h"

interface iGameObject : public cObject
{
protected:
    Vector3     m_vScale;
    Vector3     m_vRotation;
    Vector3     m_vPosition;
    bool        m_bActive;
    bool        m_bVisible;

public:
    virtual HRESULT Init() PURE;
    virtual HRESULT Setup() PURE;
    virtual HRESULT Update() PURE;
    virtual HRESULT Render() PURE;
    virtual HRESULT Detroy() PURE;

    virtual Vector3 GetScale() PURE;
    virtual void SetScale(const Vector3 scale) PURE;
    
    virtual Vector3 GetRotation() PURE;
    virtual void SetRotation(const Vector3 rotation) PURE;
    
    virtual Vector3 GetPosition() PURE;
    virtual void SetPosition(const Vector3 position) PURE;

    virtual bool IsActive() PURE;
    virtual void Activate() PURE;
    virtual void Deactivate() PURE;
};