#pragma once
#include "cObject.h"

class cUIObject : public cObject
{
protected:
    Matrix4                 m_matWorldInv;
    vector<cUIObject*>      m_vecChild;
    vector<ST_RHWC_VERTEX>  m_vecVertex;
    Vector2					m_stSize;

    SYNTHESIZE(Matrix4, m_matWorld, MatWorld);
    SYNTHESIZE(bool, m_isAxtive, Axtive);
    SYNTHESIZE(Vector3, m_vLocalPos, LocalPos);
    SYNTHESIZE(cUIObject*, m_pParent, Parent);
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
    virtual void SetSize(Vector2 size);
    virtual Vector2 GetSize() const;
    virtual cUIObject* GetChildByTag(int tag);
    virtual cUIObject* GetChildByName(string strChildName);
    virtual cUIObject* IsClicked(POINT point, bool isTransform);
    virtual ULONG Release(void);
};

