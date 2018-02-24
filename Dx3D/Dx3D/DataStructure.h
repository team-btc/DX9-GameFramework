#pragma once

#pragma region VERTEX
/* 버텍스 FVF 정보 셋팅 옵션 */

// Flexible Vertex Format(유연한 버텍스 포맷) - 사용 할 정점의 다양한 정보를 사용자가 직접 필요한 부분만 골라서 정의하여 쓸 수 있는 방법
//	1. Position 정점의 좌표 x,y,z(float)				: D3DFVF_XYZ - 정점 포맷이 변환되지 않은 정점의 위치 좌표
// 	2. RHW (float)                                  : D3DFVF_XYZRHW - 정점 포맷이 변환 된 정점의 위치 좌표 (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 	3. Blending Weight Data 결합 가중치 (float)		: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 	4. Vertex Normal 정점의 법선 벡터 x,y,z(float)	: D3DFVF_NORMAL
// 	5. Vertex Point Size 정점의 점 크기 (float)		: D3DFVF_PSIZE
// 	6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE			// 확산광		// Ambient 환경 반사광
// 	7. Specular Color (DWORD)						: D3DFVF_SPECULAR			// 정반사광
// 	8. Texture Coordinate Set 1 (float)             : D3DFVF_TEX0 - D3DFVF_TEX8

/* 정점에 대한 정보를 구조체로 만들어서 관리 */
struct ST_PC_VERTEX
{
    Vector3     p;
    Color       c;

    static const int FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

    ST_PC_VERTEX() {}
    ST_PC_VERTEX(Vector3 _p, Color _c) : p(_p), c(_c) {}
};

struct ST_PT_VERTEX
{
    Vector3         p;
    Vector2         t;

    static const int FVF = D3DFVF_XYZ | D3DFVF_TEX1;

    ST_PT_VERTEX() {}
    ST_PT_VERTEX(Vector3 _p, Vector2 _t) : p(_p), t(_t) {}
};

struct ST_PN_VERTEX
{
    Vector3		p;
    Vector3		n;

    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

    ST_PN_VERTEX() {}
    ST_PN_VERTEX(Vector3 _p, Vector3 _n) : p(_p), n(_n) {}
};

struct ST_PNT_VERTEX
{
    Vector3     p;
    Vector3     n;
    Vector2     t;

    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

    ST_PNT_VERTEX() {}
    ST_PNT_VERTEX(Vector3 _p, Vector3 _n, Vector2 _t) : p(_p), n(_n), t(_t) {}
};

struct ST_RHWC_VERTEX
{
    Vector4     p;
    Color       c;

    static const int FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

    ST_RHWC_VERTEX() {}
    ST_RHWC_VERTEX(Vector4 _p, Color _c) : p(_p), c(_c) {}
};

using VertexPC = ST_PC_VERTEX;
using VertexPN = ST_PN_VERTEX;
using VertexPT = ST_PT_VERTEX;
using VertexPNT = ST_PNT_VERTEX;
using VertexRHWC = ST_RHWC_VERTEX;
#pragma endregion


struct ST_BOUNDING_SPHERE
{
    Vector3     p;
    float       d;

    ST_BOUNDING_SPHERE() { p = Vector3(0, 0, 0); d = 0.0f; }
    ST_BOUNDING_SPHERE(Vector3 _p, float _d) { p = _p, d = _d; }
};