#pragma once
#include "cObject.h"
#include "iMap.h"

#define NODE_X 16
#define NODE_Z 16
#define NODE_INTERVAL 10

class cNode;

class cAStar : public cObject
{
private:
    vector<cNode*>      m_vecNodes;
    LPMESH          m_pSphere;

    vector<int>         m_vecOpenNodes;
    vector<Vector3> m_vecObstacle;

public:
    cAStar();
    ~cAStar();

    void Setup(iMap *pMap);
    void Render();

    void FindAStarPath(IN Vector3 vStart, IN Vector3 vDest,
                       OUT vector<Vector3>& vecPath);
    void SetObstacle(vector<Vector3>& vec) { m_vecObstacle = vec; }

private:
    void Reset(); // 전 노드들 초기화
    int FindClosestNode(Vector3 loc);   // 받은 위치에 가장 가까운 노드를 찾아주는 함수
    void Extend(int nCenter, int nDest);    // 확장(주변 노드 갱신) 함수
    float GetManhattanDistance(int nCurr, int nDest); // H값을 멘하탄 디스턴스 값으로 계산 해주는 함수
    int GetMinFNode(); // 오픈 노드 중에 F값이 가장 작은 노드를 찾아주는 함수

    bool IsBlocked(int nCenter, int i);

    void MakeDirectPath(IN Vector3 vStart, IN Vector3 vDest,
                        OUT vector<Vector3>& vecPath, bool isCutFromBack);
};

/*
A*(A Star) 알고리즘
F : G + H
G : 출발 지점 부터 현재 노드까지 오는 들어가는 총 비용
H : 현재 노드 부터 목적지까지 들어가는 예상 비용
S : 현재 노드 상태 (기본, 오픈, 클로즈)

1. 출발 지점을 클로즈 하고 주변 노드 들을 갱신한다.
* 주변 노드 갱신
- 기본 상태 : 오픈 노드가 되고 G, H, F 값을 갱신
- 오픈 상태 : 기존에 가지고 있는 G값이 갱신 하려는 G값 보다
큰 경우 G값과 F값을 갱신한다. (H, S는 갱신 의미 없음)
2. 오픈 된 노드들 중에서 F 값이 가장 작은 노드를 찾는다.
3. 찾은 노드를 클로즈 하고 '주변 노드'를 갱신 한다.
4. 목적지 노드가 클로즈 될 때까지 2, 3번을 반복 한다.
5. 목적지 노드가 클로즈가 되면 목적지 노드 부터 via를 타고 올라가면서
경로를 설정한다.
*/

