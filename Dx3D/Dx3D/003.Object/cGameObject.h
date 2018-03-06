#pragma once
#include "iGameObject.h"

class cGameObject : public cObject, public iGameObject
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

    virtual Vector3 GetScale() override;
    virtual void SetScale(const Vector3 scale) override;

    virtual Vector3 GetRotation() override;
    virtual void SetRotation(const Vector3 rotation) override;

    virtual Vector3 GetPosition() override;
    virtual void SetPosition(const Vector3 position) override;

    virtual bool IsActive() override;
    virtual void Activate() override;
    virtual void Deactivate() override;

#pragma region EmitMessageTest
private:
    float hp;
public:
    template<typename ... T>
    bool EmitMessage(string szCommand, const T ... _t);

    void Deal(float dmg) { hp -= dmg; }
    void Heal(float heal) { hp += heal; }
#pragma endregion
};


#pragma region EmitMessageTest
template<typename ... T>
inline bool cGameObject::EmitMessage(string szCommand, const T ... _t)
{
    bool result = true;
    if (szCommand == "deal")
    {
        cout << "CMD : " << szCommand << endl;
        cout << "HP : " << to_string(hp) << " -> ";
        Deal((float)_t...);
        cout << to_string(hp) << endl;
    }
    else if (szCommand == "heal")
    {
        cout << "CMD : " << szCommand << endl;
        cout << "HP : " << to_string(hp) << " -> ";
        Heal((float)_t...);
        cout << to_string(hp) << endl;
    }
    else
    {
        result = false;
    }

    return result;
}
#pragma endregion