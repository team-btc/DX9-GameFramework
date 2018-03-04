#include "stdafx.h"
#include "cFrustum.h"


cFrustum::cFrustum()
{
}


cFrustum::~cFrustum()
{
}

HRESULT cFrustum::Setup()
{
    m_vecOrigProjVertex.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    m_vecOrigProjVertex.push_back(Vector3(-1.0f,  1.0f, -1.0f));
    m_vecOrigProjVertex.push_back(Vector3( 1.0f,  1.0f, -1.0f));
    m_vecOrigProjVertex.push_back(Vector3( 1.0f, -1.0f, -1.0f));

    m_vecOrigProjVertex.push_back(Vector3(-1.0f, -1.0f,  1.0f));
    m_vecOrigProjVertex.push_back(Vector3(-1.0f,  1.0f,  1.0f));
    m_vecOrigProjVertex.push_back(Vector3( 1.0f,  1.0f,  1.0f));
    m_vecOrigProjVertex.push_back(Vector3( 1.0f, -1.0f,  1.0f));
    
    m_vecPlane.resize(6);

    return S_OK;
}

HRESULT cFrustum::Update()
{
    vector<Vector3> vecWorldVertex(8);
    Matrix4 matProj, matView;
    g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
    g_pDevice->GetTransform(D3DTS_VIEW, &matView);

    for (int i = 0; i < 8; i++)
    {
        D3DXVec3Unproject(&vecWorldVertex[i], &m_vecOrigProjVertex[i],
            NULL, &matProj, &matView, NULL);
    }

    D3DXPlaneFromPoints(&m_vecPlane[0],
        &vecWorldVertex[0], &vecWorldVertex[1], &vecWorldVertex[2]);

    D3DXPlaneFromPoints(&m_vecPlane[1],
        &vecWorldVertex[4], &vecWorldVertex[6], &vecWorldVertex[5]);

    D3DXPlaneFromPoints(&m_vecPlane[2],
        &vecWorldVertex[0], &vecWorldVertex[5], &vecWorldVertex[1]);

    D3DXPlaneFromPoints(&m_vecPlane[3],
        &vecWorldVertex[3], &vecWorldVertex[2], &vecWorldVertex[6]);

    D3DXPlaneFromPoints(&m_vecPlane[4],
        &vecWorldVertex[1], &vecWorldVertex[5], &vecWorldVertex[2]);

    D3DXPlaneFromPoints(&m_vecPlane[5],
        &vecWorldVertex[0], &vecWorldVertex[3], &vecWorldVertex[4]);

    return S_OK;
}

HRESULT cFrustum::IsInFrustum(OUT bool& result, ST_SPHERE* pSphere)
{
    result = true;

    for each (Plane var in m_vecPlane)
    {
        Vector3 vCenter = pSphere->vCenter;
        if (D3DXPlaneDotCoord(&var, &pSphere->vCenter) > 0.0f)
        {
            result = false;
            break;
        }
    }

    pSphere->isRender = result;

    return S_OK;
}
