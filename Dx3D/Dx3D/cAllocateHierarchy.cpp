#include "stdafx.h"
#include "cAllocateHierarchy.h"


cAllocateHierarchy::cAllocateHierarchy()
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

HRESULT cAllocateHierarchy::CreateFrame(LPCSTR Name, LPFRAME* ppNewFrame)
{
    HRESULT hr = S_OK;
    ST_BONE* pBone = new ST_BONE;
    *ppNewFrame = NULL;

    hr = AllocateName(Name, &pBone->Name);
    D3DXMatrixIdentity(&pBone->TransformationMatrix);

    pBone->pMeshContainer = NULL;
    pBone->pFrameSibling = NULL;
    pBone->pFrameFirstChild = NULL;

    *ppNewFrame = pBone;

    return hr;
}

HRESULT cAllocateHierarchy::CreateMeshContainer(LPCSTR Name,
                                                CONST MESHDATA* pMeshData,
                                                CONST MATERIAL* pMaterials,
                                                CONST D3DXEFFECTINSTANCE* pEffectInstances,
                                                DWORD NumMaterials,
                                                CONST DWORD* pAdjacency,
                                                LPSKININFO pSkinInfo,
                                                LPMESHCONTAINER* ppNewMeshContainer)
{
    HRESULT hr = S_FALSE;
    ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;

    hr = AllocateName(Name, &pBoneMesh->Name);

    pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
    SAFE_ADDREF(pMeshData->pMesh);

    // ADD1. 메쉬 복사 (원본 메쉬 사용, 워크 메쉬 복사)
    pBoneMesh->pOriginMesh = pMeshData->pMesh;
    SAFE_ADDREF(pMeshData->pMesh);

    hr = pBoneMesh->pOriginMesh->CloneMeshFVF(D3DXMESH_MANAGED, pBoneMesh->pOriginMesh->GetFVF(), g_pDevice, &pBoneMesh->pWorkMesh);

    pBoneMesh->NumMaterials = NumMaterials;
    pBoneMesh->pMaterials = new MATERIAL[max(1, pBoneMesh->NumMaterials)];
    for (DWORD i = 0; i < pBoneMesh->NumMaterials; i++)
    {
        pBoneMesh->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
        hr = AllocateName(pMaterials[i].pTextureFilename, &pBoneMesh->pMaterials[i].pTextureFilename);
        g_pTextureManager->AddTexture(pMaterials->pTextureFilename, m_szDirPath + "\\" + string(pMaterials->pTextureFilename));
    }
    pBoneMesh->pEffects = NULL;
    
    pBoneMesh->pAdjacency = NULL;

    // ADD2. 스킨인포 저장
    pBoneMesh->pSkinInfo = pSkinInfo;
    SAFE_ADDREF(pSkinInfo);
    DWORD numBones = pSkinInfo->GetNumBones();
    // ADD3. 매트릭스들에 대한 할당 : ppBoneMatrixPtrs, pBoneOffsetMatrixces, pCurrentBoneMatrices
    pBoneMesh->pBoneOffsetMatrices = new Matrix4[numBones];
    pBoneMesh->pCurrentBoneMatrices = new Matrix4[numBones];
    pBoneMesh->ppBoneMatrixPtrs = new Matrix4*[numBones];
    for (DWORD i = 0; i < numBones; i++)
    {
        // ADD4. pBoneOffsetMatrixces 값 셋팅
        pBoneMesh->pBoneOffsetMatrices[i] = *pSkinInfo->GetBoneOffsetMatrix(i);
    }
    
    *ppNewMeshContainer = pBoneMesh;

    return hr;
}

HRESULT cAllocateHierarchy::DestroyFrame(LPFRAME pFrameToFree)
{
    HRESULT hr = S_FALSE;
    ST_BONE* pBone = (ST_BONE*)pFrameToFree;
    SAFE_DELETE_ARRAY(pBone->Name);
    SAFE_DELETE(pBone);

    hr = S_OK;
    
    return hr;
}

HRESULT cAllocateHierarchy::DestroyMeshContainer(LPMESHCONTAINER pMeshContainerToFree)
{
    HRESULT hr = S_FALSE;
    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;
    
    for (DWORD i = 0; i < pBoneMesh->NumMaterials; i++)
    {
        SAFE_DELETE_ARRAY(pBoneMesh->pMaterials[i].pTextureFilename);
    }
    SAFE_DELETE_ARRAY(pBoneMesh->pMaterials);
    SAFE_DELETE_ARRAY(pBoneMesh->Name);
    SAFE_RELEASE(pBoneMesh->MeshData.pMesh);

    // ADD5. CreateMeshContainer 에서 바뀐 내용 또는 추가적인 할당에 대한 해제
    SAFE_DELETE(pBoneMesh->pBoneOffsetMatrices);
    SAFE_DELETE(pBoneMesh->pCurrentBoneMatrices);
    SAFE_DELETE(pBoneMesh->ppBoneMatrixPtrs);
    SAFE_RELEASE(pBoneMesh->pOriginMesh);
    SAFE_RELEASE(pBoneMesh->pWorkMesh);

    SAFE_DELETE(pBoneMesh);
    hr = S_OK;

    return hr;
}

HRESULT cAllocateHierarchy::AllocateName(LPCSTR Name, LPSTR* pNewName)
{
    UINT cbLength;

    if (Name != NULL)
    {
        cbLength = (UINT)strlen(Name) + 1;
        *pNewName = new CHAR[cbLength];
        if (*pNewName == NULL)
        {
            return E_OUTOFMEMORY;
        }
        memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
    }
    else
    {
        *pNewName = NULL;
    }

    return S_OK;
}