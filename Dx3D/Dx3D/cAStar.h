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
    void Reset(); // �� ���� �ʱ�ȭ
    int FindClosestNode(Vector3 loc);   // ���� ��ġ�� ���� ����� ��带 ã���ִ� �Լ�
    void Extend(int nCenter, int nDest);    // Ȯ��(�ֺ� ��� ����) �Լ�
    float GetManhattanDistance(int nCurr, int nDest); // H���� ����ź ���Ͻ� ������ ��� ���ִ� �Լ�
    int GetMinFNode(); // ���� ��� �߿� F���� ���� ���� ��带 ã���ִ� �Լ�

    bool IsBlocked(int nCenter, int i);

    void MakeDirectPath(IN Vector3 vStart, IN Vector3 vDest,
                        OUT vector<Vector3>& vecPath, bool isCutFromBack);
};

/*
A*(A Star) �˰���
F : G + H
G : ��� ���� ���� ���� ������ ���� ���� �� ���
H : ���� ��� ���� ���������� ���� ���� ���
S : ���� ��� ���� (�⺻, ����, Ŭ����)

1. ��� ������ Ŭ���� �ϰ� �ֺ� ��� ���� �����Ѵ�.
* �ֺ� ��� ����
- �⺻ ���� : ���� ��尡 �ǰ� G, H, F ���� ����
- ���� ���� : ������ ������ �ִ� G���� ���� �Ϸ��� G�� ����
ū ��� G���� F���� �����Ѵ�. (H, S�� ���� �ǹ� ����)
2. ���� �� ���� �߿��� F ���� ���� ���� ��带 ã�´�.
3. ã�� ��带 Ŭ���� �ϰ� '�ֺ� ���'�� ���� �Ѵ�.
4. ������ ��尡 Ŭ���� �� ������ 2, 3���� �ݺ� �Ѵ�.
5. ������ ��尡 Ŭ��� �Ǹ� ������ ��� ���� via�� Ÿ�� �ö󰡸鼭
��θ� �����Ѵ�.
*/

