#include "stdafx.h"
#include "cRay.h"


cRay::cRay()
    : m_vOrg(0, 0, 0)
    , m_vDir(0, 0, 1)
    , m_eRaySpace(E_NONE)
{
}


cRay::~cRay()
{
}

cRay cRay::RayAtViewSpace(int x, int y)
{
    VIEWPORT9 vp;
    g_pDevice->GetViewport(&vp);

    Matrix4 matProj;
    g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

    /* �� ��Ʈ�� �������� ��Ʈ������ ����Ͽ� ��ũ�� ��ǥ(x, y)�� ����
    �� �����̽������� ���̸� ���� �Ѵ�. */

    cRay ray;
    ray.m_vDir.x = ((2.0f * x) / vp.Width - 1.0f) / matProj._11;
    ray.m_vDir.y = ((-2.0f * y) / vp.Height + 1.0f) / matProj._22;
    ray.m_vDir.z = 1.0f;
    D3DXVec3Normalize(&ray.m_vDir, &ray.m_vDir);

    ray.m_eRaySpace = E_VIEW;

    return ray;
}

cRay cRay::RayAtWorldSpace(int x, int y)
{
    cRay ray = cRay::RayAtViewSpace(x, y);

    Matrix4 matView, matInvView;
    g_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matInvView, NULL, &matView);

    /* �� �����̽����� ���� �����̽��� ���̸� �ѱ��. */

    D3DXVec3TransformCoord(&ray.m_vOrg, &ray.m_vOrg, &matInvView);
    D3DXVec3TransformNormal(&ray.m_vDir, &ray.m_vDir, &matInvView);

    ray.m_eRaySpace = E_WORLD;

    return ray;
}

bool cRay::IsPicked(ST_SPHERE* pSphere)
{
    assert(m_eRaySpace == E_WORLD && "���̰� ���� �������� ��ȯ�� ���� �ʾҽ��ϴ�.");

    // QV ^ 2 - (VV)(QQ - rr) : ��� ���� picking ��� ����
    // Q : ������ �����
    // V : ������ ����
    // r : ���� ������

    // ���� ������ �������� �̵� �Ͽ� ����ϸ� ����� ����ȭ �Ǳ� ������
    // ���� �Բ� ���̸� �̵��ϴ� ���·� ����Ѵ�.
    Vector3 vLocalOrg = m_vOrg - pSphere->vCenter;

    float qv = D3DXVec3Dot(&vLocalOrg, &m_vDir);
    float vv = D3DXVec3Dot(&m_vDir, &m_vDir);
    float qq = D3DXVec3Dot(&vLocalOrg, &vLocalOrg);
    float rr = pSphere->fRadius * pSphere->fRadius;

    return qv * qv - vv * (qq - rr) >= 0;
}
