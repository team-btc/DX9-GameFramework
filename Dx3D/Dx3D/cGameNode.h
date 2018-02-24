#pragma once

class cGameNode
{
protected:
    string          m_szTagName;

public:
    void            SetTagName(string TagName) { m_szTagName = TagName; }
    const string    GetTagName() { return m_szTagName; }

    cGameNode();
    virtual ~cGameNode();

    virtual bool Start() PURE;
    virtual void Update(float dt) PURE;
    virtual void Render() PURE;
    virtual void Destroy() PURE;
};