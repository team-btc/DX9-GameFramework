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
    // �÷��̾� ���� ��ġ
    Vector3                 vStartPos;

    //  �� ������
    float                   fMapSize;

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

    ST_MAP_INFO() : pTerrainMesh(NULL), pWaterTexture(NULL), pWaterMesh(NULL), pSkyTexture(NULL), pObstacleMesh(NULL) {};
};

// ��ƼŬ�� �Ӽ� ����ü
struct ST_PARTICLE_ATTR
{
    Vector3         vPos;               //  ��ƼŬ�� ���� ��ġ
    float           fPosOffset;         //  ��ƼŬ ���� ���� ����
    Vector3         vSpeed;             //  ��ƼŬ�� ���� ����
    float           fSpeed;             //  ��ƼŬ�� �ӷ�(��Į�� ��)
    Vector3         deltaAccelMin;      //  ��ƼŬ ���� �ּҰ�
    Vector3         deltaAccelMax;      //  ��ƼŬ ���� �ִ밪
    Vector3         vAccel;             //  ��ƼŬ�� ����
    float           fGravity;           //  �߷�
    float           fMaxLife;           //  �ִ� ����
    float           fMinLife;           //  �ּ� ����
    float           life;               //  ��ƼŬ�� �Ҹ� �Ҷ����� �����Ǵ� �ð�
    float           age;                //  ��ƼŬ�� ���� ����
    XColor          color;              //  ��ƼŬ�� �÷�
    XColor          originColor;
    XColor          fadeColor;          //  ��ƼŬ�� �÷��� �ð��� �帧�� ���� ����ϴ� ���
    bool            isFade;
    bool            isAlive;            //  ��ƼŬ�� ���� true, �Ҹ� false
    bool            isVariableSpeed;    //  ���� ���� ����
    bool            isLoop;             //  ��ƼŬ ��Ȱ�� ����

    // ��ƼŬ �Ӽ� ����ü�� �ʱⰪ ����
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
    float           fSTR;//��
    float           fDEX;//��
    float           fINT;//��
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

struct ST_PLUS_STAT_INFO
{
    int     nType;
    float   fPlusValue;
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
    float           fPlusValue;
    bool            isWear;
    E_ITEM_TYPE     eItemType;

    vector<ST_PLUS_STAT_INFO> vecPlusStat;
}; 

class cUILayer;
class cUITextView;
struct ST_DAMAGE_TEXT
{
    cUILayer*       pUILayer;
    cUITextView*    pTextUI;
    string          szDamage;
    Vector3         vCurrPos;
    float           fMaxY;
    float           fPlusY;
    bool            isAxtive;

    ST_DAMAGE_TEXT()
    {
        pUILayer = NULL;
        pTextUI = NULL;
        szDamage = "";
        vCurrPos = Vector3(0, 0, 0);
        fMaxY = 0;
        fPlusY = 0;
        isAxtive = false;
    }
};