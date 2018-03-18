#pragma once
#pragma region VERTEX
typedef struct ST_PC_VERTEX
{
    Vector3     p;
    Color       c;

    static const int FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

    ST_PC_VERTEX() {}
    ST_PC_VERTEX(Vector3 _p, Color _c) : p(_p), c(_c) {}
} Particle;

struct ST_PT_VERTEX
{
    Vector3     p;
    Vector2     t;

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

using VertexPC      = ST_PC_VERTEX;
using VertexPN      = ST_PN_VERTEX;
using VertexPT      = ST_PT_VERTEX;
using VertexPNT     = ST_PNT_VERTEX;
using VertexRHWC    = ST_RHWC_VERTEX;
#pragma endregion

struct ST_SIZE
{
    float       w;
    float       h;

    ST_SIZE() : w(0.0f), h(0.0f) {}
    ST_SIZE(float _w, float _h) : w(_w), h(_h) {}
};

struct ST_BONE : public D3DXFRAME
{
    Matrix4 CombinedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
    vector<LPTEXTURE9>          vecTexture;
    vector<D3DMATERIAL9>        vecMaterial;

    LPMESH              pWorkingMesh;
    LPMESH              pOrigMesh;
    Matrix4**           ppBoneMatrixPtrs;
    Matrix4*            pBoneOffsetMatrices;

    DWORD               dwNumPaletteEntries;
    DWORD               dwMaxNumFaceInfls;
    DWORD               dwNumAttrGroups;
    LPBUFFER            pBufBoneCombos;
};

struct ST_BOUNDING_SPHERE
{
    Vector3     p;
    float       d;

    ST_BOUNDING_SPHERE() : p(0, 0, 0), d(0.0f) {}
    ST_BOUNDING_SPHERE(Vector3 _p, float _d) : p(_p), d(_d) {}
};


struct ST_SPHERE
{
    bool        isPicked;
    float       fRadius;
    Vector3     vCenter;
    bool        isRender;
};

struct ST_TEXTURE_INFO
{
    LPTEXTURE9          pTexture;
    float               fDensity;

    ST_TEXTURE_INFO() : pTexture(NULL) {}
};

struct ST_EVENT_INFO
{
    string              szName;
    Vector3             vPos;
    float               fRadius;
};

struct ST_OBJECT_INFO
{
    LPMESH              pMesh;
    Matrix4             matWorld;

    ST_OBJECT_INFO() : pMesh(NULL) {}
};

struct ST_MAP_INFO
{
    // �÷��̾� ���� ��ġ
    Vector3                 vStartPos;

    // ���� �Ž�
    LPMESH                  pTerrainMesh;

    // �ؽ��� �е� PNG
    LPTEXTURE9              pTextureMap;

    // �ؽ���
    ST_TEXTURE_INFO         arrTextureInfo[4];

    // ��
    LPMESH                  pWaterMesh;
    LPTEXTURE9              pWaterTexture;
    bool                    isEnableWater;
    float                   fWaterDensity;
    float                   fWaterHeight;
    float                   fWaterUVSpeed;
    float                   fWaterWaveHeight;
    float                   fWaterHeightSpeed;
    float                   fWaterfrequency;
    float                   fWaterTransparent;

    // �ϴ�
    LPCUBETEXTURE9          pSkyTexture;

    // ��ֹ�
    LPMESH                  pObstacleMesh;

    // �̺�Ʈ Ʈ��
    vector<ST_EVENT_INFO>   vecEventInfo;

    // ������Ʈ ����
    vector<ST_OBJECT_INFO>  vecObjectInfo;

    ST_MAP_INFO() : pTerrainMesh(NULL), pWaterTexture(NULL), pWaterMesh(NULL), pSkyTexture(NULL), pObstacleMesh(NULL) {}
};

// ��ƼŬ�� �Ӽ� ����ü
struct ST_PARTICLE_ATTR
{
    Vector3         p;          // ���彺���̽� ���� ��ƼŬ ��ġ
    Vector3         v;          // ��ƼŬ�� �ӵ�, ������ �ʴ� �̵� ������ ���
    Vector3         a;          // ��ƼŬ�� ����,
    float           g;          // �߷�
    float           life;       // ��ƼŬ�� �Ҹ� �Ҷ����� �����Ǵ� �ð�
    float           age;        // ��ƼŬ�� ���� ����
    XColor          c;          // ��ƼŬ�� �÷�
    XColor          fade;       // ��ƼŬ�� �÷��� �ð��� �帧�� ���� ����ϴ� ���
    bool            isAlive;    // ��ƼŬ�� ���� true, �Ҹ� false

    // ��ƼŬ �Ӽ� ����ü�� �ʱⰪ ����
    ST_PARTICLE_ATTR()
    {
        life = 0.0f;
        age = 0.0f;
        g = 0.0f;
        isAlive = true;
    }
};

struct ST_STATE 
{
    int nStateNum;
    string szStateName;
    map<string, float> mapPosition;
};

struct ST_STATUS
{
public:
    string          m_szName;
    float           m_fATK;
    float           m_fDEF;
    float           m_fSTR;
    float           m_fDEX;
    float           m_fINT;
    float           m_fMaxHP;
    float           m_fMaxMP;
    float           m_fSpeed;
    int             m_Level;

    float           m_fCurHP;
    float           m_fCurMP;
    int             m_nCurEXP;
    int             m_nMaxEXP;
    float           m_fHPGen;
    float           m_fMPGen;
    float           m_fCritical;
    int             m_nCoolTime;
};