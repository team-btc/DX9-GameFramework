#pragma once
#include "cObject.h"
#include "cAllocateHierarchy.h"

enum E_STATE
{
    IDLE = 0, RUN, ATK_1, ATK_2, ATK_3
};

class cSkinnedMesh : public cObject
{
    friend class cMeshManager;
private:
    LPFRAME         m_pRoot;
    ANIMCONTROLLER  m_pAnimationController;
    UINT            m_nMaxAnimSet;
    map<int, LPANIMATIONSET> m_mapAnimSet;
    E_STATE         m_eCurrState;
    E_STATE         m_eNextState;
    float           m_fWeight;
    bool            m_isBlending;
    Matrix4         m_matWorld;
    Vector3         m_position;
    UINT            m_nCurrAnimation;

private:
    cSkinnedMesh();

    void Init();
    void Load(string szDirPath, string szFilepath);
    void SetupBoneMatrixPtrs(LPFRAME pFrame);
    void SetupAnimation();
    void Update(LPFRAME pFrame, LPFRAME pParent);
    void UpdateSkinnedMesh(LPFRAME pFrame);

public:
    cSkinnedMesh(string szKey);
    cSkinnedMesh(string szKey, string szDirectory, string szFilepath);
    ~cSkinnedMesh();

    void Destroy();

    void Update();
    void Render(LPFRAME pFrame = NULL);
    void UpdateRender();

    Vector3 GetPosition() { return m_position; }

    void SetAnimation(UINT animation);
    void SetWorld(Matrix4& m) { m_matWorld = m; }
};