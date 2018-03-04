#pragma once

#pragma region VERTEX
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
    Matrix4 CombinedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
    vector<LPDIRECT3DTEXTURE9> vecTexture;
    vector<D3DMATERIAL9>		vecMaterial;

    LPMESH              pWorkingMesh;           // 작업메쉬
    LPMESH              pOrigMesh;              // 원본 메쉬 CloneMeshFVF로 복사
    Matrix4**           ppBoneMatrixPtrs;       // 이 메쉬에 영향을 주는 프레임'들'의 월드매트릭스 포인터 배열
    Matrix4*            pBoneOffsetMatrices;    // 원본 메쉬를 로컬스페이스로 보내는 매트릭스들.

    DWORD               dwNumPaletteEntries;
    DWORD               dwMaxNumFaceInfls;
    DWORD               dwNumAttrGroups;
    LPBUFFER            pBufBoneCombos;
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
    Vector3     vCenter;
    bool        isRender;
} tagSphere;