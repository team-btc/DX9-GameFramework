#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
    : m_vLocalPos(0, 0, 0)
    , m_pParent(NULL)
    , m_stSize(0, 0)
    , m_isDebugRender(false)
    , m_nTag(-1)
{
    D3DXMatrixIdentity(&m_matWorld);
}

cUIObject::~cUIObject()
{
    cout << m_szName << endl;
    cout << this << endl;

    if (!m_vecChild.empty())
    {
        for (int i = 0; i < m_vecChild.size(); i++)
        {
            SAFE_RELEASE(m_vecChild[i]);
        }
    }
}

void cUIObject::Update()
{
    UpdateWorldTM();
    UpdateChildren();
}

void cUIObject::UpdateWorldTM()
{
    m_matWorld._41 = m_vLocalPos.x;
    m_matWorld._42 = m_vLocalPos.y;
    m_matWorld._43 = m_vLocalPos.z;

    if (m_pParent)	// 트리 구조에서이 위치는 부모를 따라간다.
    {
        m_matWorld._41 += m_pParent->m_matWorld._41;
        m_matWorld._42 += m_pParent->m_matWorld._42;
        m_matWorld._43 += m_pParent->m_matWorld._43;
    }

    D3DXMatrixInverse(&m_matWorldInv, NULL, &m_matWorld);
    m_matWorldInv._11 = 1;
    m_matWorldInv._22 = 1;

    m_rtBody.left = (int)m_matWorld._41;
    m_rtBody.top = (int)m_matWorld._42;
    m_rtBody.right = m_rtBody.left + (int)(m_stSize.x * m_matWorld._11);
    m_rtBody.bottom = m_rtBody.top + (int)(m_stSize.y * m_matWorld._22);

    if (m_vecVertex.size() != 8)
    {
        m_vecVertex.resize(8);
    }

    D3DCOLOR c = D3DCOLOR_XRGB(255, 0, 0);
    m_vecVertex[0] = VertexRHWC(Vector4((float)m_rtBody.left, (float)m_rtBody.top, 0, 1), c);
    m_vecVertex[1] = VertexRHWC(Vector4((float)m_rtBody.right, (float)m_rtBody.top, 0, 1), c);
    m_vecVertex[2] = VertexRHWC(Vector4((float)m_rtBody.right, (float)m_rtBody.top, 0, 1), c);
    m_vecVertex[3] = VertexRHWC(Vector4((float)m_rtBody.right, (float)m_rtBody.bottom, 0, 1), c);
    m_vecVertex[4] = VertexRHWC(Vector4((float)m_rtBody.right, (float)m_rtBody.bottom, 0, 1), c);
    m_vecVertex[5] = VertexRHWC(Vector4((float)m_rtBody.left, (float)m_rtBody.bottom, 0, 1), c);
    m_vecVertex[6] = VertexRHWC(Vector4((float)m_rtBody.left, (float)m_rtBody.bottom, 0, 1), c);
    m_vecVertex[7] = VertexRHWC(Vector4((float)m_rtBody.left, (float)m_rtBody.top, 0, 1), c);
}

void cUIObject::UpdateChildren()
{
    for each (auto vPos in m_vecChild)
        vPos->Update();
}

void cUIObject::Render(LPSPRITE pSprite)
{
    if (m_isDebugRender)
    {
        g_pDevice->SetFVF(VertexRHWC::FVF);
        g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &m_vecVertex[0], sizeof(VertexRHWC));
    }

    for each (auto vPos in m_vecChild)
        vPos->Render(pSprite);
}

void cUIObject::AddChild(cUIObject* pChild)
{
    pChild->m_pParent = this;		// 자식으로 추가 될 UI의 부모를 현재 UI로 설정
    m_vecChild.push_back(pChild);	// 자식 벡터에 추가
}

void cUIObject::SetSize(Vector2 size)
{
    m_stSize = size;
}

Vector2 cUIObject::GetSize() const
{
    return m_stSize;
}

cUIObject* cUIObject::GetChildByTag(int tag)
{
    if (m_nTag == tag)	// 찾고 있는 tag 값이 현재 UI 인 경우
        return this;

    for each(auto vPos in m_vecChild)	// 자식들 중에 찾는 tag 값이 있는지 검색
    {
        cUIObject* pChild = vPos->GetChildByTag(tag);
        if (pChild)
            return pChild;
    }

    return NULL;
}

cUIObject * cUIObject::GetChildByName(string strChildName)
{
    if (strChildName.empty() || strChildName[0] == NULL)
    {
        return NULL;
    }

    if (m_szName == strChildName)
    {
        return this;
    }
    else
    {
        if (!m_vecChild.empty())
        {
            for each(auto vPos in m_vecChild)
            {
                if (vPos->m_szName == strChildName)
                {
                    return vPos;
                }
            }
        }
    }

    return NULL;
}

cUIObject* cUIObject::IsClicked(POINT point, bool isTransform)
{
    if (!isTransform)
    {
        /*
        D3DXVECTOR3 pt((float)point.x, (float)point.y, 0);
        D3DXVec3TransformCoord(&pt, &pt, &m_matWorldInv);
        point.x = (int)pt.x;
        point.y = (int)pt.y;
        isTransform = true;
        */
    }

    if (PtInRect(&m_rtBody, point))
    {
        return this;
    }

    for each (auto iter in m_vecChild)
    {
        cUIObject* pChild = iter->IsClicked(point, isTransform);
        if (pChild)
            return pChild;
    }

    return NULL;
}
