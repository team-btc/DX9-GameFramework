#pragma once

struct ST_BONE : public FRAME
{
    Matrix4     CombinedTransformMatrix;

    ST_BONE()
    {
        D3DXMatrixIdentity(&CombinedTransformMatrix);
    }
};

struct ST_BONE_MESH : public MESHCONTAINER
{
    LPMESH      pOriginMesh;
    LPMESH      pWorkMesh;
    Matrix4**   ppBoneMatrixPtrs;
    Matrix4*    pBoneOffsetMatrices;
    Matrix4*    pCurrentBoneMatrices;

    ST_BONE_MESH()
    {
        pOriginMesh = NULL;
        pWorkMesh = NULL;
        ppBoneMatrixPtrs = NULL;
        pBoneOffsetMatrices = NULL;
        pCurrentBoneMatrices = NULL;
    }
};

class cAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
    SYNTHESIZE_PASS_BY_REF(string, m_szDirPath, Directory);

public:
    cAllocateHierarchy();
    ~cAllocateHierarchy();

    // ID3DXAllocateHierarchy을(를) 통해 상속됨
    virtual HRESULT CreateFrame(LPCSTR Name, LPFRAME* ppNewFrame) override;
    virtual HRESULT CreateMeshContainer(LPCSTR Name,
                                        CONST MESHDATA* pMeshData,
                                        CONST MATERIAL* pMaterials,
                                        CONST D3DXEFFECTINSTANCE* pEffectInstances,
                                        DWORD NumMaterials,
                                        CONST DWORD* pAdjacency,
                                        LPSKININFO pSkinInfo,
                                        LPMESHCONTAINER* ppNewMeshContainer) override;
    virtual HRESULT DestroyFrame(LPFRAME pFrameToFree) override;
    virtual HRESULT DestroyMeshContainer(LPMESHCONTAINER pMeshContainerToFree) override;

    HRESULT AllocateName(LPCSTR Name, LPSTR * pNewName);
};

