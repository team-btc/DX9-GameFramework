#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh(string szKey, string szFolder, string szFilename)
    : m_pRootFrame(NULL)
    , m_pAnimController(NULL)
    , m_dwWorkingPaletteSize(0)
    , m_pmWorkingPalette(NULL)
    , m_pEffect(NULL)
    , m_vPosition(0, 0, 0)
    , m_fBlendDuration(0.5f)
    , m_fPassedBlendTime(0.0f)
{
    D3DXMatrixIdentity(&m_matWorld);

    cSkinnedMesh* pSkinnedMesh = g_pMeshManager->GetSkinnedMesh(szKey, szFolder, szFilename);
   
    m_pRootFrame = pSkinnedMesh->m_pRootFrame;
    m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
    m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
    m_pEffect = pSkinnedMesh->m_pEffect;
    m_stBoundingSphere = pSkinnedMesh->m_stBoundingSphere;
    m_vScale = pSkinnedMesh->GetScale();
    m_vRotation = pSkinnedMesh->GetRotation();
    m_mapStateInfo = pSkinnedMesh->m_mapStateInfo;

    if (pSkinnedMesh->m_pAnimController)
    {
        pSkinnedMesh->m_pAnimController->CloneAnimationController(
            pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
            pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
            pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
            pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
            &m_pAnimController);
    }
}

cSkinnedMesh::cSkinnedMesh(string szKey)
    : m_pRootFrame(NULL)
    , m_pAnimController(NULL)
    , m_dwWorkingPaletteSize(0)
    , m_pmWorkingPalette(NULL)
    , m_pEffect(NULL)
    , m_vPosition(0, 0, 0)
    , m_fBlendDuration(0.5f)
    , m_fPassedBlendTime(0.0f)
{
    D3DXMatrixIdentity(&m_matWorld);

    cSkinnedMesh* pSkinnedMesh = g_pMeshManager->GetSkinnedMesh(szKey);

    m_pRootFrame = pSkinnedMesh->m_pRootFrame;
    m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
    m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
    m_pEffect = pSkinnedMesh->m_pEffect;
    m_vScale = pSkinnedMesh->GetScale();
    m_vRotation = pSkinnedMesh->GetRotation();
    m_stBoundingSphere = pSkinnedMesh->m_stBoundingSphere;
    m_mapStateInfo = pSkinnedMesh->m_mapStateInfo;

    if (pSkinnedMesh->m_pAnimController)
        pSkinnedMesh->m_pAnimController->CloneAnimationController(
            pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
            pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
            pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
            pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
            &m_pAnimController);

}
cSkinnedMesh::cSkinnedMesh()
    : m_pRootFrame(NULL)
    , m_pAnimController(NULL)
    , m_dwWorkingPaletteSize(0)
    , m_pmWorkingPalette(NULL)
    , m_pEffect(NULL)
{
}

cSkinnedMesh::~cSkinnedMesh(void)
{
    SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load(string szDirectory, string szFilename)
{
    m_pEffect = LoadEffect("Assets\\MultiAnimation.hpp");

    int nPaletteSize = 0;
    m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

    cAllocateHierarchy ah;
    ah.SetDirectory(szDirectory);
    ah.SetDefaultPaletteSize(nPaletteSize);

    m_stBoundingSphere.vCenter = (ah.GetMin() + ah.GetMax()) / 2.0f;
    m_stBoundingSphere.fRadius = D3DXVec3Length(&(ah.GetMin() - ah.GetMax()));

    string sFullPath(szDirectory);
    sFullPath += "\\" + std::string(szFilename);

    HRESULT hr = D3DXLoadMeshHierarchyFromXA(sFullPath.c_str(),
                               D3DXMESH_MANAGED | D3DXMESH_32BIT,
                               g_pDevice,
                               &ah,
                               NULL,
                               (LPFRAME*)&m_pRootFrame,
                               &m_pAnimController);

    if (hr == D3DERR_INVALIDCALL)
    {
        cout << "invalid" << endl;
    }
    else if (hr == E_OUTOFMEMORY)
    {
        cout << "out of mem" << endl;
    }
    if (m_pmWorkingPalette)
        delete[] m_pmWorkingPalette;

    m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
    m_pmWorkingPalette = new Matrix4[m_dwWorkingPaletteSize];
    if (m_pmWorkingPalette == NULL)
    {
        m_dwWorkingPaletteSize = 0;
    }

    if (m_pRootFrame)
    {
        SetupBoneMatrixPtrs(m_pRootFrame);
    }
}

void cSkinnedMesh::UpdateAndRender()
{
    if (m_pAnimController)
    {
        m_pAnimController->AdvanceTime(g_pTimerManager->GetDeltaTime(), NULL);

        if (m_fPassedBlendTime < m_fBlendDuration) // ���� ���� ��
        {
            m_fPassedBlendTime += g_pTimerManager->GetDeltaTime();

            if (m_fPassedBlendTime < m_fBlendDuration) // ������ ���� ���� ���� ����
            {
                float fWeight = m_fPassedBlendTime / m_fBlendDuration;

                m_pAnimController->SetTrackWeight(0, fWeight);
                m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
            }
            else // ������ ���� ����
            {
                m_pAnimController->SetTrackWeight(0, 1);
                m_pAnimController->SetTrackEnable(1, false);
            }
        }
    }

    if (m_pRootFrame)
    {
        Matrix4 matS, matR, matT, matW;
        D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
        D3DXMatrixRotationYawPitchRoll(&matR,
            D3DXToRadian(m_vRotation.y),
            D3DXToRadian(m_vRotation.x),
            D3DXToRadian(m_vRotation.z));
        D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        matW = (matS * matR * matT);

        Update(m_pRootFrame, &matW);
        Render(m_pRootFrame);
    }
}

void cSkinnedMesh::Render(ST_BONE* pBone /*= NULL*/)
{
    assert(pBone);

    // �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
    // �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
    if (pBone->pMeshContainer)
    {
        ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

        // get bone combinations
        LPBONECOMBINATION pBoneCombos =
            (LPBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

        Matrix4 matViewProj, matView, matProj;
        g_pDevice->GetTransform(D3DTS_VIEW, &matView);
        g_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
        matViewProj = matView * matProj;

        Matrix4 mView, mInvView;
        g_pDevice->GetTransform(D3DTS_VIEW, &mView);
        D3DXMatrixInverse(&mInvView, 0, &mView);
        Vector3 vEye = Vector3(0, 0, 0);
        D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

        // for each palette
        for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
        {
            // set each transform into the palette
            for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
            {
                DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
                if (dwMatrixIndex != UINT_MAX)
                {
                    m_pmWorkingPalette[dwPalEntry] =
                        pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
                        (*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
                }
            }

            // set the matrix palette into the effect
            m_pEffect->SetMatrixArray("amPalette",
                                      m_pmWorkingPalette,
                                      pBoneMesh->dwNumPaletteEntries);

            m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
            m_pEffect->SetVector("vLightDiffuse", &Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            m_pEffect->SetVector("vWorldLightPos", &Vector4(500.0f, 500.0f, 500.0f, 1.0f));
            m_pEffect->SetVector("vWorldCameraPos", &Vector4(vEye, 1.0f));
            m_pEffect->SetVector("vMaterialAmbient", &Vector4(0.53f, 0.53f, 0.53f, 0.53f));
            m_pEffect->SetVector("vMaterialDiffuse", &Vector4(1.0f, 1.0f, 1.0f, 1.0f));

            // we're pretty much ignoring the materials we got from the x-file; just set
            // the texture here
            m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

            // set the current number of bones; this tells the effect which shader to use
            m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

            // set the technique we use to draw
            m_pEffect->SetTechnique("Skinning20");

            UINT uiPasses, uiPass;

            // run through each pass and draw
            m_pEffect->Begin(&uiPasses, 0);
            for (uiPass = 0; uiPass < uiPasses; ++uiPass)
            {
                m_pEffect->BeginPass(uiPass);
                pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
                m_pEffect->EndPass();
            }
            m_pEffect->End();
        }
    }

    //��������� ��� �����ӿ� ���ؼ� ����.
    if (pBone->pFrameSibling)
    {
        Render((ST_BONE*)pBone->pFrameSibling);
    }

    if (pBone->pFrameFirstChild)
    {
        Render((ST_BONE*)pBone->pFrameFirstChild);
    }
}

LPEFFECT cSkinnedMesh::LoadEffect(string szFilename)
{
    LPEFFECT pEffect = NULL;

    D3DXMACRO mac[2] =
    {
        { "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
        {
            NULL,
            NULL
        }
    };

    D3DCAPS9 caps;
    D3DXMACRO* pMacro = NULL;
    g_pDevice->GetDeviceCaps(&caps);
    if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
        pMacro = mac;

    DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
    dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
    dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

    ID3DXBuffer* pBuffer = NULL;
    if (FAILED(D3DXCreateEffectFromFileA(g_pDevice,
                                         szFilename.c_str(),
                                         pMacro,
                                         NULL,
                                         dwShaderFlags,
                                         NULL,
                                         &pEffect,
                                         &pBuffer)))
    {
        // if creation fails, and debug information has been returned, output debug info
        if (pBuffer)
        {
            OutputDebugStringA((char*)pBuffer->GetBufferPointer());
            MessageBoxA(g_hWnd, "Load Effect error", "ERROR", MB_OK);
            SAFE_RELEASE(pBuffer);
        }

        return NULL;
    }

    return pEffect;
}

void cSkinnedMesh::Update(ST_BONE* pCurrent, Matrix4* pmatParent)
{
    pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
    if (pmatParent)
    {
        pCurrent->CombinedTransformationMatrix =
            pCurrent->CombinedTransformationMatrix * (*pmatParent);
    }

    if (pCurrent->pFrameSibling)
    {
        Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
    }

    if (pCurrent->pFrameFirstChild)
    {
        Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
    }
}

void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
    assert(pBone);

    // �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
    // �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
    if (pBone->pMeshContainer)
    {
        ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
        if (pBoneMesh->pSkinInfo)
        {
            LPSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
            // pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
            // pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
            // D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
            // ã�Ƽ� �����Ʈ������ �ɾ����.
            DWORD dwNumBones = pSkinInfo->GetNumBones();
            for (DWORD i = 0; i < dwNumBones; ++i)
            {
                LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
                if (szBoneName == NULL || strlen(szBoneName) == 0)
                    continue;
                ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
                pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
            }
        }
    }

    //��������� ��� �����ӿ� ���ؼ� ����.
    if (pBone->pFrameSibling)
    {
        SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
    }

    if (pBone->pFrameFirstChild)
    {
        SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
    }
}

void cSkinnedMesh::SetAnimationIndex(int nIndex, bool isBlend)
{
    if (!m_pAnimController)
        return;
    LPD3DXANIMATIONSET pNextAnimSet = NULL;
    m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);

    if (isBlend)
    {
        LPD3DXANIMATIONSET	pPrevAnimSet = NULL;
        D3DXTRACK_DESC	stTrackDesc;
        // ���� ������̴� �ִϸ��̼� ���� ��������
        m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
        m_pAnimController->GetTrackDesc(0, &stTrackDesc);
        // 0�� Ʈ�� -> 1�� Ʈ��
        m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
        m_pAnimController->SetTrackDesc(1, &stTrackDesc);

        m_pAnimController->SetTrackWeight(0, 0.0f);

        SAFE_RELEASE(pPrevAnimSet);

        m_fPassedBlendTime = 0.0f;
    }

    m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
    m_pAnimController->SetTrackPosition(0, 0.0f);
    SAFE_RELEASE(pNextAnimSet);
    
}

void cSkinnedMesh::SetDescZeroPos()
{
    D3DXTRACK_DESC desc;
    m_pAnimController->GetTrackDesc(0, &desc);

    m_pAnimController->SetTrackPosition(0, 0);
}

float cSkinnedMesh::GetdescPos()
{
    D3DXTRACK_DESC desc;
    m_pAnimController->GetTrackDesc(0, &desc);

    return (float)desc.Position;
}

int cSkinnedMesh::GetCurPos()
{
    int nResult = 0;

    if (m_pAnimController)
    {
        LPANIMATIONSET pAnimSet = NULL;
        m_pAnimController->GetTrackAnimationSet(0, &pAnimSet);

        D3DXTRACK_DESC desc;
        m_pAnimController->GetTrackDesc(0, &desc);

        int CurPos = (int)desc.Position / (int)pAnimSet->GetPeriod();

        nResult =  CurPos;
    }
    return nResult;
}

string cSkinnedMesh::GetAnimName()
{
    LPANIMATIONSET pAnimSet = NULL;
    m_pAnimController->GetTrackAnimationSet(0, &pAnimSet);

    return (string)pAnimSet->GetName();
}

HRESULT cSkinnedMesh::Destroy()
{
    cAllocateHierarchy ah;
    D3DXFrameDestroy((LPFRAME)m_pRootFrame, &ah);
    SAFE_DELETE_ARRAY(m_pmWorkingPalette);
    SAFE_RELEASE(m_pEffect);

    return S_OK;
}

void cSkinnedMesh::SetRandomTrackPosition()
{
    m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}
