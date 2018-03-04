#pragma once
#include "cObject.h"

struct ST_BONE;

class cSkinnedMesh : public cObject
{
    friend class cMeshManager;

private:
    //하나만 생성
    ST_BONE*                    m_pRootFrame;
    DWORD                       m_dwWorkingPaletteSize;
    Matrix4*                    m_pmWorkingPalette;
    LPEFFECT                    m_pEffect;
    ST_SPHERE                   m_stBoundingSphere;

    // 객체마다 생성
    LPANIMCONTROLLER            m_pAnimController;
    Vector3                     m_vPosition;

private:
    cSkinnedMesh();

    void Load(string szFolder, string szFilename);
    LPEFFECT LoadEffect(string szFilename);
    void Update(ST_BONE* pCurrent, Matrix4* pmatParent);
    void Render(ST_BONE* pBone = NULL);
    void SetupBoneMatrixPtrs(ST_BONE* pBone);
    void SetAnimationIndex(int nIndex);
    virtual HRESULT Destroy() override;

public:
    cSkinnedMesh(string szKey, string szFolder, string szFilename);
    ~cSkinnedMesh(void);

    void UpdateAndRender();

    void SetRandomTrackPosition();  // 테스트용
    void SetPosition(Vector3 v)
    {
        m_vPosition = v;
        m_stBoundingSphere.vCenter = v;
    }
    ST_SPHERE* GetBoundingSphere()
    {
        return &m_stBoundingSphere;
    }
};
