#pragma once

#pragma region VERTEX
/* ���ؽ� FVF ���� ���� �ɼ� */

// Flexible Vertex Format(������ ���ؽ� ����) - ��� �� ������ �پ��� ������ ����ڰ� ���� �ʿ��� �κи� ��� �����Ͽ� �� �� �ִ� ���
//	1. Position ������ ��ǥ x,y,z(float)				: D3DFVF_XYZ - ���� ������ ��ȯ���� ���� ������ ��ġ ��ǥ
// 	2. RHW (float)                                  : D3DFVF_XYZRHW - ���� ������ ��ȯ �� ������ ��ġ ��ǥ (D3DFVF_XYZ �Ǵ� D3DFVF_NORMAL�� ���� ���Ұ�)
// 	3. Blending Weight Data ���� ����ġ (float)		: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 	4. Vertex Normal ������ ���� ���� x,y,z(float)	: D3DFVF_NORMAL
// 	5. Vertex Point Size ������ �� ũ�� (float)		: D3DFVF_PSIZE
// 	6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE			// Ȯ�걤		// Ambient ȯ�� �ݻ籤
// 	7. Specular Color (DWORD)						: D3DFVF_SPECULAR			// ���ݻ籤
// 	8. Texture Coordinate Set 1 (float)             : D3DFVF_TEX0 - D3DFVF_TEX8

/* ������ ���� ������ ����ü�� ���� ���� */
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


struct ST_BONE : public D3DXFRAME
{
    D3DXMATRIXA16 CombinedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
    vector<LPDIRECT3DTEXTURE9> vecTexture;
    vector<D3DMATERIAL9>		vecMaterial;

    LPD3DXMESH				pWorkingMesh;			// �۾��޽�
    LPD3DXMESH				pOrigMesh;				// ���� �޽� CloneMeshFVF�� ����
    D3DXMATRIXA16**			ppBoneMatrixPtrs;		// �� �޽��� ������ �ִ� ������'��'�� �����Ʈ���� ������ �迭
    D3DXMATRIXA16*			pBoneOffsetMatrices;	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������.

    DWORD					dwNumPaletteEntries;
    DWORD					dwMaxNumFaceInfls;
    DWORD					dwNumAttrGroups;
    LPD3DXBUFFER			pBufBoneCombos;
};

struct ST_BOUNDING_SPHERE
{
    Vector3     p;
    float       d;

    ST_BOUNDING_SPHERE() { p = Vector3(0, 0, 0); d = 0.0f; }
    ST_BOUNDING_SPHERE(Vector3 _p, float _d) { p = _p, d = _d; }
};


typedef struct ST_SPHERE
{
    bool        isPicked;
    float       fRadius;
    D3DXVECTOR3 vCenter;
    bool        isRender;
} tagSphere;