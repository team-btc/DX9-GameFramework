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

    eRaySpace   m_eRaySpace;    // 현재 레이의 위치 정보(렌더링 파이프 라인)

public:
    Vector3 m_vOrg;
    Vector3 m_vDir;

public:
    cRay();
    ~cRay();

    static cRay RayAtViewSpace(int x, int y);   // 좌표를 받아서 뷰 스페이스에서의 레이를 생성
    static cRay RayAtWorldSpace(int x, int y);  // 좌표를 받아서 월드 스페이스에서의 레이를 생성

    bool IsPicked(tagSphere* pSphere);          // 구 정보를 받아서 현재 레이와 충돌 여부 확인
};

