#include "stdafx.h"
#include "cNode.h"


cNode::cNode(D3DXVECTOR3 loc, int index)
    : m_vLocation(loc)
    , m_nIndex(index)
    , m_nVia(index)
{
    F = G = H = 0;
    S = E_NONE;
}


cNode::~cNode()
{
    for each (auto p in m_vecEdgeInfos)
        SAFE_DELETE(p);
}

void cNode::AddEdge(cNode* pNode)
{
    ST_EdgeInfo* pEdge = new ST_EdgeInfo;
    pEdge->nIndex = pNode->m_nIndex;

    // ���� �ڽ�Ʈ : ���� ���� ���� ����� �Ÿ�(���)
    Vector3 dist = this->m_vLocation - pNode->m_vLocation;
    pEdge->fEdgeCost = D3DXVec3Length(&dist);

    m_vecEdgeInfos.push_back(pEdge);

    // ���� ���� ���� ��带 �̾��ִ� ���� �׸��� ���� ����
    m_vecLines.push_back(this->m_vLocation);
    m_vecLines.push_back(pNode->m_vLocation);
}
