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

    virtual HRESULT GetScale(Vector3& scale) PURE;
    virtual HRESULT SetScale(const Vector3 scale) PURE;
    
    virtual HRESULT GetRotation(Vector3& rotation) PURE;
    virtual HRESULT SetRotation(const Vector3 rotation) PURE;
    
    virtual HRESULT GetPosition(Vector3& position) PURE;
    virtual HRESULT SetPosition(const Vector3 position) PURE;

    virtual HRESULT IsActive(bool& active) PURE;
    virtual HRESULT Activate() PURE;
    virtual HRESULT Deactivate() PURE;
};