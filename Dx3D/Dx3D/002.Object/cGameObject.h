#pragma once
#include "iGameObject.h"

class cGameObject : public iGameObject
{
private:
    Matrix4     m_matScale;
    Matrix4     m_matRot;
    Matrix4     m_matTrans;
    bool        m_bIsValidate;

public:
    cGameObject();
    ~cGameObject();

private:
    void Validate();

public:
    virtual HRESULT Init() override;
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Detroy() override;

    virtual HRESULT GetScale(Vector3& scale) override;
    virtual HRESULT SetScale(const Vector3 scale) override;
    virtual HRESULT GetRotation(Vector3& rotation) override;
    virtual HRESULT SetRotation(const Vector3 rotation) override;
    virtual HRESULT GetPosition(Vector3& position) override;
    virtual HRESULT SetPosition(const Vector3 position) override;
    virtual HRESULT IsActive(bool& active) override;
    virtual HRESULT Activate() override;
    virtual HRESULT Deactivate() override;
};