#include "stdafx.h"
#include "cGameObject.h"


cGameObject::cGameObject()
{
    cGameObject::Init();
}


cGameObject::~cGameObject()
{
}

void cGameObject::Validate()
{
    if (m_bIsValidate)
    {
        m_vScale.x = max(0.0f, m_vScale.x);
        m_vScale.y = max(0.0f, m_vScale.y);
        m_vScale.z = max(0.0f, m_vScale.z);

        m_vPosition.x = max(0.0f, m_vPosition.x);
        m_vPosition.y = max(0.0f, m_vPosition.y);
        m_vPosition.z = max(0.0f, m_vPosition.z);
    }
}

HRESULT cGameObject::Init()
{
    m_bActive = false;
    m_bVisible = false;
    m_szName = "";
    m_vPosition = Vector3(0, 0, 0);
    m_vRotation = Vector3(0, 0, 0);
    m_vScale = Vector3(1, 1, 1);

    m_bIsValidate = false;
    D3DXMatrixIdentity(&m_matScale);
    D3DXMatrixIdentity(&m_matRot);
    D3DXMatrixIdentity(&m_matTrans);

    return S_OK;
}

HRESULT cGameObject::Setup()
{
    m_bActive = true;
    m_bVisible = true;

    return S_OK;
}

HRESULT cGameObject::Update()
{
    Validate();

    return S_OK;
}

HRESULT cGameObject::Render()
{
    return S_OK;
}

HRESULT cGameObject::Detroy()
{
    return S_OK;
}

HRESULT cGameObject::GetScale(Vector3& scale)
{
    scale = m_vScale;

    return S_OK;
}

HRESULT cGameObject::SetScale(const Vector3 scale)
{
    m_vScale = scale;

    return S_OK;
}

HRESULT cGameObject::GetRotation(Vector3& rotation)
{
    rotation = m_vRotation;

    return S_OK;
}

HRESULT cGameObject::SetRotation(const Vector3 rotation)
{
    m_vRotation = rotation;

    return S_OK;
}

HRESULT cGameObject::GetPosition(Vector3& position)
{
    position = m_vPosition;

    return S_OK;
}

HRESULT cGameObject::SetPosition(const Vector3 position)
{
    if (position.x > 0 &&
        position.y > 0 &&
        position.z > 0)
    {
        m_vPosition = position;

        return S_OK;
    }
    else
    {
        return E_INVALIDARG;
    }
}

HRESULT cGameObject::IsActive(bool& active)
{
    active = m_bActive;

    return S_OK;
}

HRESULT cGameObject::Activate()
{
    HRESULT hr = S_OK;
    if (m_bActive)
    {
        hr = E_ABORT;
    }
    m_bActive = true;

    return hr;
}

HRESULT cGameObject::Deactivate()
{
    HRESULT hr = S_OK;
    if (!m_bActive)
    {
        hr = E_ABORT;
    }
    m_bActive = false;

    return hr;
}