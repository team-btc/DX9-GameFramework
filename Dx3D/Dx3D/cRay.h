#pragma once

typedef struct ST_SPHERE
{
    bool        isPicked;
    float       fRadius;
    D3DXVECTOR3 vCenter;
} tagSphere;

class cRay
{
private:
    enum eRaySpace
    {
        E_NONE,
        E_VIEW,
        E_WORLD
    };

    eRaySpace   m_eRaySpace;    // ���� ������ ��ġ ����(������ ������ ����)

public:
    Vector3 m_vOrg;
    Vector3 m_vDir;

public:
    cRay();
    ~cRay();

    static cRay RayAtViewSpace(int x, int y);   // ��ǥ�� �޾Ƽ� �� �����̽������� ���̸� ����
    static cRay RayAtWorldSpace(int x, int y);  // ��ǥ�� �޾Ƽ� ���� �����̽������� ���̸� ����

    bool IsPicked(tagSphere* pSphere);          // �� ������ �޾Ƽ� ���� ���̿� �浹 ���� Ȯ��
};

