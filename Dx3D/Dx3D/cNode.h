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
    // 현재 노드에 연결 되어 있는 엣지노드 정보
    SYNTHESIZE_PASS_BY_REF(vector<ST_EdgeInfo*>, m_vecEdgeInfos, EdgeInfos);
    // 연결 된 노드까지 선을 긋기 위한 위치 정보(현재위치, 연결 노드위치 반복)
    SYNTHESIZE_PASS_BY_REF(vector<Vector3>, m_vecLines, Lines);

public:
    Vector3     m_vLocation;    // 노드의 위치 값
    int         m_nIndex;       // 현재 노드의 인덱스 번호
    int         m_nVia;         // 출발 점에서 목적지까지의 최단거리 이전 노드

    float       F;              // 총 비용 (G + H)
    float       G;              // 현재 노드 까지의 누적 비용
    float       H;              // 현재 노드에서 목적지까지의 예측 비용 (멘하탄 디스턴스)
    eNodeState  S;              // 현재 노드의 상태

public:
    cNode(Vector3 loc, int index);
    ~cNode();

    void AddEdge(cNode* pNode);
};

