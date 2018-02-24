#include "stdafx.h"
#include "cAStar.h"
#include "cNode.h"


cAStar::cAStar()
{
    D3DXCreateSphere(g_pDevice, 1.0f, 10, 10, &m_pSphere, NULL);
}


cAStar::~cAStar()
{
    SAFE_RELEASE(m_pSphere);
}

void cAStar::Setup(iMap* pMap)
{
    // ��� ����(��ġ �� �ε��� ����)
    for (int z = 0; z < NODE_Z; ++z)
    {
        for (int x = 0; x < NODE_X; ++x)
        {
            Vector3 loc((float)x * NODE_INTERVAL, 0, (float)z * NODE_INTERVAL);
            pMap->GetHeight(loc.x, loc.y, loc.z);
            int index = z * NODE_X + x;

            cNode* pNode = new cNode(loc, index);
            g_pAutoReleasePool->AddObject(pNode);

            m_vecNodes.push_back(pNode);
        }
    }

    for (int z = 0; z < NODE_Z; ++z)
    {
        for (int x = 0; x < NODE_X; ++x)
        {
            int index = z * NODE_X + x;

            if (index % NODE_X != 0)
                m_vecNodes[index]->AddEdge(m_vecNodes[index - 1]); // ���� ���

            if (index % NODE_X != NODE_X - 1)
                m_vecNodes[index]->AddEdge(m_vecNodes[index + 1]); // ������ ���

            if (z > 0)
                m_vecNodes[index]->AddEdge(m_vecNodes[index - NODE_X]); // �Ʒ��� ���

            if (z < NODE_Z - 1)
                m_vecNodes[index]->AddEdge(m_vecNodes[index + NODE_X]); // ���� ���
        }
    }
}

void cAStar::Render()
{
    Matrix4 matT;

    for each (auto p in m_vecNodes)
    {
        D3DXMatrixTranslation(&matT, p->m_vLocation.x, p->m_vLocation.y, p->m_vLocation.z);

        g_pDevice->SetTexture(0, NULL);

        switch (p->S)
        {
            case E_NONE:
                g_pDevice->SetMaterial(&WHITE_MTRL);
                break;
            case E_OPEN:
                g_pDevice->SetMaterial(&GREEN_MTRL);
                break;
            case E_CLOSE:
                g_pDevice->SetMaterial(&RED_MTRL);
                break;
            case E_USING:
                g_pDevice->SetMaterial(&YELLOW_MTRL);
                break;
            case E_BLOCK:
                g_pDevice->SetMaterial(&BLUE_MTRL);
                break;
        }

        g_pDevice->SetTransform(D3DTS_WORLD, &matT);
        m_pSphere->DrawSubset(0);

        D3DXMatrixIdentity(&matT);

        g_pDevice->SetMaterial(&RED_MTRL);
        g_pDevice->SetTransform(D3DTS_WORLD, &matT);
        g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST,
            (UINT)p->GetLines().size() / 2,
                                      &p->GetLines()[0],
                                      (UINT)sizeof(Vector3));
    }
}

void cAStar::FindAStarPath(IN Vector3 vStart, IN Vector3 vDest, OUT vector<Vector3>& vecPath)
{
    Reset();

    int nStart = FindClosestNode(vStart);
    int nDest = FindClosestNode(vDest);

    // 0. ��ŸƮ ��� ���� ����
    float G = 0;
    float H = GetManhattanDistance(nStart, nDest);

    m_vecNodes[nStart]->G = G;
    m_vecNodes[nStart]->H = H;
    m_vecNodes[nStart]->F = G + H;
    m_vecNodes[nStart]->m_nVia = nStart;

    // 1. ��� ������ Ŭ���� �ϰ� �ֺ� ��� ���� �����Ѵ�.
    m_vecNodes[nStart]->S = E_CLOSE;
    Extend(nStart, nDest);

    // 2. ���� �� ���� �߿��� F ���� ���� ���� ��带 ã�´�.
    // 3. ã�� ��带 Ŭ���� �ϰ� '�ֺ� ���'�� ���� �Ѵ�.
    // 4. ������ ��尡 Ŭ���� �� ������ 2, 3���� �ݺ� �Ѵ�.
    while (true)
    {
        int nMinFNode = GetMinFNode();
        if (nMinFNode == -1)
        {
            Reset();
            break;
        }
        m_vecNodes[nMinFNode]->S = E_CLOSE;
        if (nMinFNode == nDest)
            break;
        Extend(nMinFNode, nDest);
    }

    // 5. ������ ��� ���� via�� Ÿ�� �ö󰡸鼭 ��θ� �����Ѵ�.
    vecPath.push_back(vDest);
    int nVia = nDest;	// ��� �̿��� ���� ������
    while (true)
    {
        vecPath.push_back(m_vecNodes[nVia]->m_vLocation);
        m_vecNodes[nVia]->S = E_USING;
        nVia = m_vecNodes[nVia]->m_nVia;
        if (nVia == nStart)
            break;
    }

    // 5.5 ��� ����
    MakeDirectPath(vStart, vDest, vecPath, false);
    MakeDirectPath(vStart, vDest, vecPath, true);
}

void cAStar::Reset()
{
    for each (auto p in m_vecNodes)
    {
        p->S = E_NONE;
    }

    m_vecOpenNodes.clear();
}

int cAStar::FindClosestNode(Vector3 loc)
{
    float fMinDist = FLT_MAX;
    int nIndex = -1;
    Vector3 vDist;

    for (int i = 0; i < (int)m_vecNodes.size(); ++i)
    {
        vDist = m_vecNodes[i]->m_vLocation - loc;
        float dist = D3DXVec3Length(&vDist);

        if (fMinDist > dist)
        {
            fMinDist = dist;
            nIndex = i;
        }
    }

    return nIndex;
}

void cAStar::Extend(int nCenter, int nDest)
{
    vector<ST_EdgeInfo*> vecEdges = m_vecNodes[nCenter]->GetEdgeInfos();

    for (int i = 0; i < (int)vecEdges.size(); ++i)
    {
        cNode* pEdgeNode = m_vecNodes[vecEdges[i]->nIndex];
        float fNewG = m_vecNodes[nCenter]->G + vecEdges[i]->fEdgeCost;

        if (pEdgeNode->S == E_CLOSE || IsBlocked(nCenter, vecEdges[i]->nIndex)) continue;

        if (pEdgeNode->S == E_OPEN)
        {
            float fOldG = pEdgeNode->G;

            if (fNewG < fOldG)
            {
                pEdgeNode->G = fNewG;
                pEdgeNode->F = pEdgeNode->H + pEdgeNode->G;
                pEdgeNode->m_nVia = nCenter;
            }
        }
        else
        {
            pEdgeNode->G = fNewG;
            pEdgeNode->H = GetManhattanDistance(pEdgeNode->m_nIndex, nDest);
            pEdgeNode->F = pEdgeNode->G + pEdgeNode->H;
            pEdgeNode->S = E_OPEN;
            pEdgeNode->m_nVia = nCenter;
            m_vecOpenNodes.push_back(pEdgeNode->m_nIndex);
        }
    }
}

float cAStar::GetManhattanDistance(int nCurr, int nDest)
{
    Vector3 vCurr = m_vecNodes[nCurr]->m_vLocation;
    Vector3 vDest = m_vecNodes[nDest]->m_vLocation;

    return fabs(vCurr.x - vDest.x) + fabs(vCurr.y - vDest.y) + fabs(vCurr.z - vDest.z);
}

int cAStar::GetMinFNode()
{
    if (m_vecOpenNodes.empty())
    {
        return -1;
    }
    int nMinNode = m_vecOpenNodes[0];
    float fMinF = m_vecNodes[nMinNode]->F;
    int nCloseIndex = 0;

    for (int i = 1; i < (int)m_vecOpenNodes.size(); ++i)
    {
        float F = m_vecNodes[m_vecOpenNodes[i]]->F;
        if (fMinF > F)
        {
            nMinNode = m_vecOpenNodes[i];
            fMinF = F;
            nCloseIndex = i;
        }
    }

    m_vecOpenNodes.erase(m_vecOpenNodes.begin() + nCloseIndex);

    return nMinNode;
}

bool cAStar::IsBlocked(int nCenter, int nEdge)
{
    // ���� ��ġ���� ������ ���������� ���� ����
    cRay ray;
    ray.m_vOrg = m_vecNodes[nCenter]->m_vLocation;
    ray.m_vDir = m_vecNodes[nEdge]->m_vLocation - m_vecNodes[nCenter]->m_vLocation;
    float dist = D3DXVec3Length(&ray.m_vDir);   // ������������ ���� �Ÿ�
    D3DXVec3Normalize(&ray.m_vDir, &ray.m_vDir);

    bool isBlocked = false;
    for (int i = 0; i < (int)m_vecObstacle.size(); i += 3)
    {
        float f;
        if (D3DXIntersectTri(&m_vecObstacle[i + 0], &m_vecObstacle[i + 1],
                             &m_vecObstacle[i + 2], &ray.m_vOrg, &ray.m_vDir, NULL, NULL, &f))
        {
            if (f < dist)   // ������ ������ ���� �ȿ� ��ֹ��� �ִ� ���
            {
                m_vecNodes[nEdge]->S = E_BLOCK;
                isBlocked = true;
                break;
            }
        }
    }

    return isBlocked;
}

void cAStar::MakeDirectPath(IN Vector3 vStart, IN Vector3 vDest,
                            OUT vector<Vector3>& vecPath, bool isCutFromBack)
{
    cRay ray;
    if (isCutFromBack)
        ray.m_vOrg = vDest;
    else
        ray.m_vOrg = vStart;

    float dist;         // ������� ���� Ÿ�� ������ �Ÿ�
    bool isBlocked;     // ������� Ÿ�� ���̿� ��ֹ��� �ִ°�?
    int cutNum = 0;     // ���ʿ��� ��� ����

    for (int i = 1 - isCutFromBack; i < (int)vecPath.size() - 2; ++i)
    {
        int index = i;
        if (isCutFromBack) // �ݴ� ���� �˻�� �ε��� ��ȣ�� ����� ó�� �Ѵ�.
            index = (int)vecPath.size() - i - 1;

        ray.m_vDir = vecPath[index] - ray.m_vOrg;   // ��������� Ÿ�������� ���� ����
        dist = D3DXVec3Length(&ray.m_vDir); // �Ÿ� ���
        D3DXVec3Normalize(&ray.m_vDir, &ray.m_vDir); // ���� ���� ����ȭ

        float f;            // ��������� �浹���������� �Ÿ�
        isBlocked = false;  // �⺻������ �浹 ���� ���� ���·� �ʱ�ȭ

        for (int j = 0; j < (int)m_vecObstacle.size(); j += 3)  // ��ü �浹ü���� �˻�
        {
            if (D3DXIntersectTri(&m_vecObstacle[j + 0], &m_vecObstacle[j + 1],
                                 &m_vecObstacle[j + 2], &ray.m_vOrg, &ray.m_vDir, NULL, NULL, &f))
            {
                if (f < dist) // ��������� Ÿ�� ���̿� ��ֹ��� �ִ� ���
                {
                    isBlocked = true;   // ��ֹ��� �ִٴ°� ���
                    break;              // ��ֹ��� 1���� �־ �� �� ����. (�߰� ��ֹ� �˻� �ʿ䰡 ����)
                }
            }
        }

        if (!isBlocked) // ��ֹ��� ���� ��� (���̷�Ʈ �뼱 �߰�)
        {
            cutNum = (int)vecPath.size() - i - 1 - isCutFromBack;   // Ÿ�� ����(�ٷ� �� �� �ִ�) ������ ���� ����
            break;
        }
    }

    for (int i = 0; i < cutNum; ++i) // ���ʿ��� ��� ����
    {
        if (isCutFromBack)
            vecPath.erase(vecPath.begin() + 1);
        else
            vecPath.pop_back();
    }
}