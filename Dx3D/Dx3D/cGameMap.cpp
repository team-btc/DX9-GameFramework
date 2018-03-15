#include "stdafx.h"
#include "cGameMap.h"


cGameMap::cGameMap()
{
}


cGameMap::~cGameMap()
{
}

// ±Ì¿∫ ∫πªÁ
void cGameMap::SetCurrObstacleMesh(vector<LPMESH> vecMesh)
{
    for each (auto mesh in vecMesh)
    {
        m_vecObstacleMesh.push_back(mesh);
    }
}

HRESULT cGameMap::GetHeight(OUT Vector3& vPos)
{
    cRay ray;
    ray.m_vOrg = vPos;
    ray.m_vOrg.y += 255.0f;
    ray.m_vDir = Vector3(0, -1, 0);
    BOOL isHit = false;
    float fDist;
    D3DXIntersectSubset(m_pMapMesh, NULL, &ray.m_vOrg, &ray.m_vDir, &isHit, NULL, NULL, NULL, &fDist, NULL, NULL);

    if (isHit)
    {
        vPos.y = ray.m_vOrg.y - fDist;
    }

    return S_OK;
}

bool cGameMap::CheckObstacle(IN cRay ray)
{
    BOOL isHit = false;

    for (int i = 0; i < m_vecObstacleMesh.size(); ++i)
    {
        float fDist;
        D3DXIntersectSubset(m_vecObstacleMesh[i], NULL, &ray.m_vOrg, &ray.m_vDir, &isHit, NULL, NULL, NULL, &fDist, NULL, NULL);
        
        if (isHit && fDist < 3.0f)
        {
            break;
        }
        else if (isHit && fDist >= 3.0f)
        {
            isHit = false;
        }
    }

    return isHit;
}

void cGameMap::RendObstacle()
{
    Matrix4 matW, matV, matP;
    D3DXMatrixIdentity(&matW);

    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);

    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    for (int i = 0; i < m_vecObstacleMesh.size(); ++i)
    {
        m_vecObstacleMesh[i]->DrawSubset(0);
    }

    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
