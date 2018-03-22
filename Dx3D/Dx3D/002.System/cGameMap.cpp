#include "stdafx.h"
#include "cGameMap.h"


cGameMap::cGameMap()
    : m_stCurrMapInfo(NULL)
{
}


cGameMap::~cGameMap()
{
}

void cGameMap::GetHeight(OUT Vector3& vPos)
{
    if (m_stCurrMapInfo == NULL)
    {
        return;
    }

    cRay ray;
    ray.m_vOrg = vPos;
    ray.m_vOrg.y = 255.0f;
    ray.m_vDir = Vector3(0, -1, 0);
    BOOL isHit = false;
    float fDist;
    D3DXIntersectSubset(m_stCurrMapInfo->pTerrainMesh, NULL, &ray.m_vOrg, &ray.m_vDir, &isHit,
        NULL, NULL, NULL, &fDist, NULL, NULL);

    if (isHit)
    {
        vPos.y = ray.m_vOrg.y - fDist;
    }
}

bool cGameMap::CheckObstacle(OUT float& fDist, IN cRay ray)
{
    BOOL isHit = false;

    D3DXIntersectSubset(m_stCurrMapInfo->pObstacleMesh, NULL, &ray.m_vOrg, &ray.m_vDir, &isHit,
        NULL, NULL, NULL, &fDist, NULL, NULL);

    return isHit;
}

bool cGameMap::CheckEvent(OUT string& szEventName, IN Vector3 vPos)
{
    bool isEvent = false;

    for (int i = 0; i < m_stCurrMapInfo->vecEventInfo.size(); ++i)
    {
        Vector2 v1(vPos.x, vPos.z);
        Vector2 v2(m_stCurrMapInfo->vecEventInfo[i].vPos.x, m_stCurrMapInfo->vecEventInfo[i].vPos.z);
        Vector2 v3 = v1 - v2;
        float fLength = fabsf(D3DXVec2Length(&v3));

        if (fLength < m_stCurrMapInfo->vecEventInfo[i].fRadius)
        {
            isEvent = true;
            szEventName = m_stCurrMapInfo->vecEventInfo[i].szName;
            break;
        }
    }

    return isEvent;
}

void cGameMap::RendObstacle()
{
    Matrix4 matW;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetTexture(0, NULL);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_stCurrMapInfo->pObstacleMesh->DrawSubset(0);

    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void cGameMap::RendEventTrap()
{
    g_pDevice->SetTexture(0, NULL);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    for (int i = 0; i < m_stCurrMapInfo->vecEventInfo.size(); ++i)
    {
        LPMESH mesh = g_pMeshManager->GetBasicMesh("sphere");

        Matrix4 matS, matT, matW;

        float fRadius = m_stCurrMapInfo->vecEventInfo[i].fRadius;
        D3DXMatrixScaling(&matS, fRadius, fRadius, fRadius);

        Vector3 vPos = m_stCurrMapInfo->vecEventInfo[i].vPos;
        D3DXMatrixTranslation(&matT, vPos.x, vPos.y, vPos.z);

        matW = matS * matT;

        g_pDevice->SetTransform(D3DTS_WORLD, &matW);

        mesh->DrawSubset(0);
    }
    
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
