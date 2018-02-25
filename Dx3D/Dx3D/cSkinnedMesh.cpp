#include "stdafx.h"
#include "cSkinnedMesh.h"


cSkinnedMesh::cSkinnedMesh()
{
    Init();
}

cSkinnedMesh::cSkinnedMesh(string szKey)
{
    HRESULT hr;
    Init();
    D3DXMatrixIdentity(&m_matWorld);
    cSkinnedMesh* pNewMesh = g_pMeshManager->GetMesh(szKey);
    if (pNewMesh)
    {
        m_pRoot = pNewMesh->m_pRoot;
        hr = pNewMesh->m_pAnimationController->CloneAnimationController(pNewMesh->m_pAnimationController->GetMaxNumAnimationOutputs(),
                                                                        pNewMesh->m_pAnimationController->GetMaxNumAnimationSets(),
                                                                        pNewMesh->m_pAnimationController->GetMaxNumTracks(),
                                                                        pNewMesh->m_pAnimationController->GetMaxNumEvents(),
                                                                        &m_pAnimationController);
        SetupAnimation();
    }
    else
    {
        g_pLogManager->WriteLog(EL_ERROR, " SkinnedMesh Load failed. Key : " + szKey);
    }
}

cSkinnedMesh::cSkinnedMesh(string szKey, string szDirectory, string szFilepath)
{
    HRESULT hr;
    Init();
    cSkinnedMesh* pNewMesh = g_pMeshManager->GetMesh(szKey, szDirectory, szFilepath);
    if (pNewMesh)
    {
        m_pRoot = pNewMesh->m_pRoot;
        hr = pNewMesh->m_pAnimationController->CloneAnimationController(pNewMesh->m_pAnimationController->GetMaxNumAnimationOutputs(),
                                                                   pNewMesh->m_pAnimationController->GetMaxNumAnimationSets(),
                                                                   pNewMesh->m_pAnimationController->GetMaxNumTracks(),
                                                                   pNewMesh->m_pAnimationController->GetMaxNumEvents(),
                                                                   &m_pAnimationController);
        SetupAnimation();
    }
    else
    {
        g_pLogManager->WriteLog(EL_ERROR, " SkinnedMesh Generate failed. Path : " + szFilepath);
    }
}


cSkinnedMesh::~cSkinnedMesh()
{
    SAFE_RELEASE(m_pAnimationController);
}

void cSkinnedMesh::Init()
{
    m_pRoot = NULL;
    m_pAnimationController = NULL;
    m_eCurrState = IDLE;
    m_eNextState = IDLE;
    m_isBlending = false;
    m_fWeight = 0;
    m_position = Vector3(0, 0, 0);
    m_nCurrAnimation = 0;
    D3DXMatrixIdentity(&m_matWorld);
}

void cSkinnedMesh::Load(string szDirPath, string szFilepath)
{
    string fullpath = szDirPath + "\\" + szFilepath;
    cAllocateHierarchy alloc;
    alloc.SetDirectory(szDirPath);
    D3DXLoadMeshHierarchyFromXA(fullpath.c_str(),
                               D3DXMESH_MANAGED,
                               g_pDevice,
                               &alloc,
                               NULL,
                               &m_pRoot,
                               &m_pAnimationController);

    SetupBoneMatrixPtrs(m_pRoot);

    g_pLogManager->WriteLog(EL_INFO, "SkinnedMesh Imported : " + fullpath);
}

void cSkinnedMesh::Destroy()
{
    cAllocateHierarchy alloc;
    D3DXFrameDestroy(m_pRoot, &alloc);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPFRAME pFrame)
{
    // 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
    // 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
    if (pBoneMesh)
    {
        LPSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
        // pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
        DWORD numBone = pSkinInfo->GetNumBones();
        for (DWORD i = 0; i < numBone; i++)
        {
            // pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
            LPCSTR name = pSkinInfo->GetBoneName(i);
            // D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
            ST_BONE* pBoneFind = (ST_BONE*)D3DXFrameFind(m_pRoot, name);
            // 찾아서 월드매트릭스를 걸어줘라.
            pBoneMesh->ppBoneMatrixPtrs[i] = &pBoneFind->CombinedTransformMatrix;
        }
    }

    // 재귀적으로 모든 프레임에 대해서 실행.
    if (pFrame->pFrameSibling)
    {
        SetupBoneMatrixPtrs(pFrame->pFrameSibling);
    }

    if (pFrame->pFrameFirstChild)
    {
        SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
    }
}

void cSkinnedMesh::SetupAnimation()
{
    m_nMaxAnimSet = m_pAnimationController->GetMaxNumAnimationSets();
    HRESULT hr;
    for (UINT i = 0; i < m_nMaxAnimSet; i++)
    {
        hr = m_pAnimationController->GetAnimationSet(i, &m_mapAnimSet[i]);
    }

    int randNum = rand() % m_nMaxAnimSet;
    hr = m_pAnimationController->SetTrackAnimationSet(0, m_mapAnimSet[(E_STATE)randNum]);
    hr = m_pAnimationController->SetTrackPosition(0, 0.0f);
    hr = m_pAnimationController->SetTrackEnable(0, true);
}

void cSkinnedMesh::Update()
{
    float fDeltaTime = g_pTimerManager->GetDeltaTime();
    m_pAnimationController->AdvanceTime(fDeltaTime, NULL);
    
    /*
    if (m_eCurrState == ATK_1 ||
        m_eCurrState == ATK_2 ||
        m_eCurrState == ATK_3)
    {
        TRACK_DESC desc;
        m_pAnimationController->GetTrackDesc(0, &desc);
        DOUBLE period = m_mapAnimSet[m_eCurrState]->GetPeriod();
        DOUBLE position = desc.Position;
        if (position >= period)
        {
            m_eNextState = IDLE;
        }
    }

    if (m_eCurrState != m_eNextState)
    {
        m_fWeight = 0.0f;
        m_isBlending = true;
        TRACK_DESC pTrackDesc;
        m_pAnimationController->GetTrackDesc(0, &pTrackDesc);
        m_pAnimationController->SetTrackAnimationSet(0, m_mapAnimSet[m_eNextState]);
        m_pAnimationController->SetTrackPosition(0, 0);
        m_pAnimationController->SetTrackAnimationSet(1, m_mapAnimSet[m_eCurrState]);
        m_pAnimationController->SetTrackDesc(1, &pTrackDesc);
        m_pAnimationController->SetTrackEnable(1, true);
        m_eCurrState = m_eNextState;
    }
    */
    if (m_eCurrState == 2)
    {
        TRACK_DESC desc;
        m_pAnimationController->GetTrackDesc(0, &desc);
        DOUBLE period = m_mapAnimSet[m_eCurrState]->GetPeriod();
        DOUBLE position = desc.Position;
        if (position >= period)
        {
            m_eCurrState = IDLE;
        }
    }

    if (m_isBlending)
    {
        m_fWeight += 0.05f;
        if (m_fWeight >= 1.0f)
        {
            m_fWeight = 1.0f;
            m_isBlending = false;
            m_pAnimationController->SetTrackEnable(1, m_isBlending);
        }

        m_pAnimationController->SetTrackWeight(0, m_fWeight);
        m_pAnimationController->SetTrackWeight(1, 1.0f - m_fWeight);
    }

    Update(m_pRoot, NULL);
    UpdateSkinnedMesh(m_pRoot);
}

void cSkinnedMesh::Update(LPFRAME pFrame, LPFRAME pParent)
{
    // 스킨드 메쉬의 월드 매트릭스(컴바인드 트랜스폼 매트릭스) 업데이트
    ST_BONE* pBone = (ST_BONE*)pFrame;
    pBone->CombinedTransformMatrix = pBone->TransformationMatrix;

    if (pParent)
    {
        pBone->CombinedTransformMatrix *= ((ST_BONE*)pParent)->CombinedTransformMatrix;
    }

    // 재귀적으로 모든 프레임에 대해서 실행.
    if (pFrame->pFrameSibling)
    {
        Update(pFrame->pFrameSibling, pParent);
    }

    if (pFrame->pFrameFirstChild)
    {
        Update(pFrame->pFrameFirstChild, pFrame);
    }
}

void cSkinnedMesh::UpdateSkinnedMesh(LPFRAME pFrame)
{
    if (pFrame == NULL)
    {
        return;
    }

    ST_BONE* pBone = (ST_BONE*)pFrame;
    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

    if (pBoneMesh)
    {
        if (pBoneMesh->pSkinInfo)
        {
            LPSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
            DWORD dwNumBone = pSkinInfo->GetNumBones();
            for (DWORD i = 0; i < dwNumBone; i++)
            {
                pBoneMesh->pCurrentBoneMatrices[i] =
                    pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
            }

            BYTE* src = NULL;
            BYTE* dest = NULL;
            pBoneMesh->pOriginMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
            pBoneMesh->pWorkMesh->LockVertexBuffer(NULL, (void**)&dest);

            pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrentBoneMatrices,
                                         NULL,
                                         src,
                                         dest);

            pBoneMesh->pOriginMesh->UnlockVertexBuffer();
            pBoneMesh->pWorkMesh->UnlockVertexBuffer();
        }
    }

    UpdateSkinnedMesh(pFrame->pFrameFirstChild);
    UpdateSkinnedMesh(pFrame->pFrameSibling);
}

void cSkinnedMesh::UpdateRender()
{
    Update();
    Render();
}

void cSkinnedMesh::SetAnimation(UINT animation)
{
    UINT max = m_pAnimationController->GetMaxNumAnimationSets();
    animation = animation % max;

    if (m_nCurrAnimation == animation)
    {
        return;
    }

    m_isBlending = true;
    TRACK_DESC pTrackDesc;
    m_pAnimationController->GetTrackDesc(0, &pTrackDesc);
    m_pAnimationController->SetTrackAnimationSet(0, m_mapAnimSet[animation]);
    m_pAnimationController->SetTrackPosition(0, 0);
    m_pAnimationController->SetTrackAnimationSet(1, m_mapAnimSet[m_nCurrAnimation]);
    m_pAnimationController->SetTrackDesc(1, &pTrackDesc);
    m_pAnimationController->SetTrackEnable(1, true);
    m_nCurrAnimation = animation;
}

void cSkinnedMesh::Render(LPFRAME pFrame)
{
    if (pFrame == NULL)
    {
        pFrame = m_pRoot;
    }

    ST_BONE* pBone = (ST_BONE*)pFrame;
    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
    if (pBoneMesh)
    {
        for (DWORD i = 0; i < pBone->pMeshContainer->NumMaterials; i++)
        {
            g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
            string key = pBoneMesh->pMaterials[i].pTextureFilename;
            g_pDevice->SetMaterial(&pBoneMesh->pMaterials[i].MatD3D);
            g_pDevice->SetTexture(0, g_pTextureManager->GetTexture(key));
            g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
            pBoneMesh->pWorkMesh->DrawSubset(i);
            g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
        }
    }

    if (pBone->pFrameFirstChild)
    {
        Render(pBone->pFrameFirstChild);
    }

    if (pBone->pFrameSibling)
    {
        Render(pBone->pFrameSibling);
    }
}
