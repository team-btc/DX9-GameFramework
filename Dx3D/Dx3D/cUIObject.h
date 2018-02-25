#pragma once
#include "cObject.h"

struct ST_SIZE
{
    float fWidth;
    float fHeight;

    ST_SIZE() : fWidth(0.0f), fHeight(0.0f) {}
    ST_SIZE(float w, float h) : fWidth(w), fHeight(h) {}
};

class cUIObject : public cObject
{
protected:
    Matrix4                 m_matWorld;
    Matrix4                 m_matWorldInv;
    vector<cUIObject*>      m_vecChild;
    vector<ST_RHWC_VERTEX>  m_vecVertex;

    SYNTHESIZE(Vector3, m_vLocalPos, LocalPos);
    SYNTHESIZE(cUIObject*, m_pParent, Parent);
    SYNTHESIZE(Vector2, m_stSize, Size);
    SYNTHESIZE(bool, m_isDebugRender, DebugRender);
    SYNTHESIZE(int, m_nTag, Tag);
    SYNTHESIZE(RECT, m_rtBody, RectBody);

public:
    cUIObject();
    virtual ~cUIObject();

    virtual void Update();
    virtual void UpdateWorldTM();
    virtual void UpdateChildren();
    virtual void Render(LPSPRITE pSprite);
    virtual void AddChild(cUIObject* pChild);
    virtual cUIObject* GetChildByTag(int tag);
    virtual cUIObject* IsClicked(POINT point, bool isTransform);
};

