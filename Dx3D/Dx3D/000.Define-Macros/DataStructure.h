#pragma once
#pragma region VERTEX
typedef struct ST_PC_VERTEX
{
    Vector3     vPos;
    Color       c;

    static const int FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

    ST_PC_VERTEX() {}
    ST_PC_VERTEX(Vector3 _p, Color _c) : vPos(_p), c(_c) {}
} ST_PARTICLE;

struct ST_PT_VERTEX
{
    Vector3     vPos;
    Vector2     t;

    static const int FVF = D3DFVF_XYZ | D3DFVF_TEX1;

    ST_PT_VERTEX() {}
    ST_PT_VERTEX(Vector3 _p, Vector2 _t) : vPos(_p), t(_t) {}
};

struct ST_PN_VERTEX
{
    Vector3		vPos;
    Vector3		n;

    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

    ST_PN_VERTEX() {}
    ST_PN_VERTEX(Vector3 _p, Vector3 _n) : vPos(_p), n(_n) {}
};

struct ST_PNT_VERTEX
{
    Vector3     vPos;
    Vector3     n;
    Vector2     t;

    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

    ST_PNT_VERTEX() {}
    ST_PNT_VERTEX(Vector3 _p, Vector3 _n, Vector2 _t) : vPos(_p), n(_n), t(_t) {}
};

struct ST_RHWC_VERTEX
{
    Vector4     vPos;
    Color       c;

    static const int FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

    ST_RHWC_VERTEX() {}
    ST_RHWC_VERTEX(Vector4 _p, Color _c) : vPos(_p), c(_c) {}
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
    Vector3     vPos;
    float       d;

    ST_BOUNDING_SPHERE() : vPos(0, 0, 0), d(0.0f) {}
    ST_BOUNDING_SPHERE(Vector3 _p, float _d) : vPos(_p), d(_d) {}
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

class cSkinnedMesh;
struct ST_OBJECT_INFO
{
    cSkinnedMesh*       pMesh;
    Matrix4             matWorld;

    ST_OBJECT_INFO() : pMesh(NULL) {}
};

struct ST_MAP_INFO
{
    // 플레이어 시작 위치
    Vector3                 vStartPos;

    //  맵 사이즈
    float                   fMapSize;

    // 지형 매쉬
    LPMESH                  pTerrainMesh;

    // 텍스쳐 밀도 PNG
    LPTEXTURE9              pTextureMap;

    // 텍스쳐
    ST_TEXTURE_INFO         arrTextureInfo[4];

    // 물
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
    // 하늘
    LPCUBETEXTURE9          pSkyTexture;

    // 장애물
    LPMESH                  pObstacleMesh;

    // 이벤트 트랩
    vector<ST_EVENT_INFO>   vecEventInfo;

    // 오브젝트 정보
    vector<ST_OBJECT_INFO>  vecObjectInfo;

    ST_MAP_INFO() : pTerrainMesh(NULL), pWaterTexture(NULL), pWaterMesh(NULL), pSkyTexture(NULL), pObstacleMesh(NULL) {};
};

// 파티클의 속성 구조체
struct ST_PARTICLE_ATTR
{
    Vector3         vPos;               //  파티클의 현재 위치
    float           fPosOffset;         //  파티클 생성 영역 범위
    Vector3         vSpeed;             //  파티클의 방향 벡터
    float           fSpeed;             //  파티클의 속력(스칼라 값)
    Vector3         deltaAccelMin;      //  파티클 변속 최소값
    Vector3         deltaAccelMax;      //  파티클 변속 최대값
    Vector3         vAccel;             //  파티클의 가속
    float           fGravity;           //  중력
    float           fMaxLife;           //  최대 생명
    float           fMinLife;           //  최소 생명
    float           life;               //  파티클이 소멸 할때까지 유지되는 시간
    float           age;                //  파티클의 현재 나이
    XColor          color;              //  파티클의 컬러
    XColor          originColor;
    XColor          fadeColor;          //  파티클의 컬러가 시간이 흐름에 따라 퇴색하는 방법
    bool            isFade;
    bool            isAlive;            //  파티클이 생존 true, 소멸 false
    bool            isVariableSpeed;    //  변속 가능 여부
    bool            isLoop;             //  파티클 재활용 여부

    // 파티클 속성 구조체의 초기값 세팅
    ST_PARTICLE_ATTR()
    {
        vPos = Vector3(0, 0, 0);
        vSpeed = Vector3(0, 0, 0);
        vAccel = Vector3(0, 0, 0);
        deltaAccelMax = Vector3(0, 0, 0);
        deltaAccelMin = Vector3(0, 0, 0);
        life = 0.0f;
        fMaxLife = 0.0f;
        fMinLife = 0.0f;
        age = 0.0f;
        fGravity = 0.0f;
        fSpeed = 0.0f;
        isFade = true;
        isAlive = true;
        isVariableSpeed = true;
        isLoop = true;
        color = XColor(1.0f, 1.0f, 1.0f, 1.0f);
        originColor = XColor(1.0f, 1.0f, 1.0f, 1.0f);
        fadeColor = XColor(0.0f, 0.0f, 0.0f, 0.0f);
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
    string          szName;
    float           fATK;
    float           fDEF;
    float           fSTR;
    float           fDEX;
    float           fINT;
    float           fMaxHP;
    float           fMaxMP;
    float           fSpeed;
    int             Level;

    float           fCurHP;
    float           fCurMP;
    int             nCurEXP;
    int             nMaxEXP;
    float           fHPGen;
    float           fMPGen;
    float           fCritical;
    int             nCoolTime;
};

struct ST_ITEM_INFO
{
    enum E_ITEM_TYPE
    {
        E_HELM,
        E_CHEST_PLATE,
        E_SWORD,
        E_HEAL,
        E_MANA
    };

    string          szName;
    string          szPath;
    int             nPrice;
    ST_STATUS       stStat;
    float           fPlusValue;
    bool            isWear;
    E_ITEM_TYPE     eItemType;
};