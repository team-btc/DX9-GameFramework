#pragma once
#include "cObject.h"

struct ST_BONE;

class cSkinnedMesh : public cObject
{
    friend class cMeshManager;

private:
    //�ϳ��� ����
    ST_BONE*                    m_pRootFrame;
    DWORD                       m_dwWorkingPaletteSize;
    Matrix4*                    m_pmWorkingPalette;
    LPEFFECT                    m_pEffect;
    ST_SPHERE                   m_stBoundingSphere;

    // ��ü���� ����
    LPANIMCONTROLLER            m_pAnimController;
    Vector3                     m_vScale;
    Vector3                     m_vRotation;
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
    cSkinnedMesh(string szKey);
    cSkinnedMesh(string szKey, string szFolder, string szFilename);
    ~cSkinnedMesh(void);

    void UpdateAndRender();

    void SetRandomTrackPosition();  // �׽�Ʈ��
    void SetScale(Vector3 v) { m_vScale = v; }
    void SetRotation(Vector3 v) { m_vRotation = v; }
    void SetPosition(Vector3 v) { m_vPosition = v; }

    Vector3 GetScale() { return m_vScale; }
    Vector3 GetRotation() { return m_vRotation; }
    Vector3 GetPosition() { return m_vPosition; }
};
