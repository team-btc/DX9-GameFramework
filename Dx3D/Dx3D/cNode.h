#pragma once
#include "cObject.h"

class cNode;

struct ST_EdgeInfo
{
    int     nIndex;
    float   fEdgeCost;
};

enum eNodeState { E_NONE, E_OPEN, E_CLOSE, E_USING, E_BLOCK };

class cNode : public cObject
{
private:
    // ���� ��忡 ���� �Ǿ� �ִ� ������� ����
    SYNTHESIZE_PASS_BY_REF(vector<ST_EdgeInfo*>, m_vecEdgeInfos, EdgeInfos);
    // ���� �� ������ ���� �߱� ���� ��ġ ����(������ġ, ���� �����ġ �ݺ�)
    SYNTHESIZE_PASS_BY_REF(vector<Vector3>, m_vecLines, Lines);

public:
    Vector3     m_vLocation;    // ����� ��ġ ��
    int         m_nIndex;       // ���� ����� �ε��� ��ȣ
    int         m_nVia;         // ��� ������ ������������ �ִܰŸ� ���� ���

    float       F;              // �� ��� (G + H)
    float       G;              // ���� ��� ������ ���� ���
    float       H;              // ���� ��忡�� ������������ ���� ��� (����ź ���Ͻ�)
    eNodeState  S;              // ���� ����� ����

public:
    cNode(Vector3 loc, int index);
    ~cNode();

    void AddEdge(cNode* pNode);
};

