#include "stdafx.h"
#include "cHeightMap.h"
#include "cMtlTex.h"

cHeightMap::cHeightMap()
    : m_pMesh(NULL)
    , m_pMtlTex(NULL)
    , m_fSizeX(1.0f)
    , m_fSizeZ(1.0f)
{
}


cHeightMap::~cHeightMap()
{
    SAFE_RELEASE(m_pMesh);
    SAFE_RELEASE(m_pMtlTex);
}

void cHeightMap::Load(IN char* szFilePath, IN Matrix4* pMat/*= NULL*/)
{
    vector<Vector3> vecTest;
    // resize
    vecTest.resize(10);
    vecTest.push_back(Vector3(1, 1, 1));
    // vecTest의 크기는 11이 된다.
    // reserve
    vecTest.reserve(10);
    vecTest.push_back(Vector3(1, 1, 1));
    // vecTest의 크기는 1이다.


    vector<VertexPNT> vecPNTVertex;
    vecPNTVertex.resize(VERTEX_DIM * VERTEX_DIM);
    m_vecVertex.resize(VERTEX_DIM * VERTEX_DIM);

    FILE* fp = NULL;
    fopen_s(&fp, szFilePath, "rb"); // rb : 한 바이트씩 읽는다. (1byte == 8bit)

    for (int z = 0; z < VERTEX_DIM; ++z)
    {
        for (int x = 0; x < VERTEX_DIM; ++x)
        {
            int nIndex = z * VERTEX_DIM + x;
            float y = (unsigned char)fgetc(fp) / 5.0f;  // 8bit == 2^8 == 0 ~  255
            vecPNTVertex[nIndex].p = Vector3((float)x, (float)y, (float)z);
            vecPNTVertex[nIndex].n = Vector3(0, 1, 0);
            vecPNTVertex[nIndex].t = Vector2((float)x / (float)TILE_N, z / (float)TILE_N);

            if (pMat)
                D3DXVec3TransformCoord(&vecPNTVertex[nIndex].p, &vecPNTVertex[nIndex].p, pMat);

            m_vecVertex[nIndex] = vecPNTVertex[nIndex].p;
        }
    }

    if (pMat)
    {
        m_fSizeX = pMat->_11;
        m_fSizeZ = pMat->_33;
    }

    fclose(fp);

    // 인덱스 셋팅
    m_vecIndex.reserve(TILE_N * TILE_N * 3 * 2); // 가로 타일 * 세로 타일 * 삼각형 정점수 * 삼각형 개수

    for (int z = 0; z < TILE_N; ++z)
    {
        for (int x = 0; x < TILE_N; ++x)
        {
            //1--3
            //|\ |
            //| \|
            //0--2
            int _0 = ((z + 0) * VERTEX_DIM) + (x + 0);
            int _1 = ((z + 1) * VERTEX_DIM) + (x + 0);
            int _2 = ((z + 0) * VERTEX_DIM) + (x + 1);
            int _3 = ((z + 1) * VERTEX_DIM) + (x + 1);

            m_vecIndex.push_back(_0);
            m_vecIndex.push_back(_1);
            m_vecIndex.push_back(_2);
            m_vecIndex.push_back(_2);
            m_vecIndex.push_back(_1);
            m_vecIndex.push_back(_3);
        }
    }

    // 노말값 계산 및 셋팅
    for (int z = 1; z < VERTEX_DIM - 1; ++z)
    {
        for (int x = 1; x < VERTEX_DIM - 1; ++x)
        {
            // ---u---
            // |\ |\ |
            // | \| \|
            // L--n--r
            // |\ |\ |
            // | \| \|
            // ---d---
            int nIndex = z * VERTEX_DIM + x;    // 현재 인덱스

            int l = nIndex - 1;
            int r = nIndex + 1;
            int u = nIndex + VERTEX_DIM;
            int d = nIndex - VERTEX_DIM;

            Vector3 du = m_vecVertex[u] - m_vecVertex[d];
            Vector3 lr = m_vecVertex[r] - m_vecVertex[l];
            Vector3 n;
            D3DXVec3Cross(&n, &du, &lr);
            D3DXVec3Normalize(&n, &n);

            vecPNTVertex[nIndex].n = n;
        }
    }

    // 메쉬 생성 및 셋팅
    D3DXCreateMeshFVF((DWORD)(m_vecIndex.size() / 3), (DWORD)vecPNTVertex.size(),
                      D3DXMESH_MANAGED | D3DXMESH_32BIT, VertexPNT::FVF, g_pDevice, &m_pMesh);

    VertexPNT* pV = NULL;
    m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
    memcpy(pV, &vecPNTVertex[0], vecPNTVertex.size() * sizeof(VertexPNT));
    m_pMesh->UnlockVertexBuffer();

    DWORD* pI = NULL;
    m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
    memcpy(pI, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
    m_pMesh->UnlockIndexBuffer();

    DWORD* pA = NULL;
    m_pMesh->LockAttributeBuffer(0, &pA);
    ZeroMemory(pA, m_pMesh->GetNumFaces());
    m_pMesh->UnlockAttributeBuffer();

    // 메쉬 최적화
    vector<DWORD> vecAdjBuf(m_pMesh->GetNumFaces() * 3);
    m_pMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf[0]);
    m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                             &vecAdjBuf[0], 0, 0, 0);

    // 속성 설정(메터리얼, 텍스쳐)
    m_pMtlTex = new cMtlTex;
    MATERIAL9 stMtl = WHITE_MTRL;
    m_pMtlTex->SetMtl(stMtl);
    g_pTextureManager->AddTexture("Terrain", "HeightMapData/terrain.jpg");
    m_pMtlTex->SetTexture(*g_pTextureManager->GetTexture("Terrain"));
}

bool cHeightMap::GetHeight(IN const float& x, OUT float& y, IN const float& z)
{
    if (x < 0 || x / m_fSizeX > TILE_N || z < 0 || z / m_fSizeZ > TILE_N)
    {
        return false;
    }

    //1--3
    //|\ |
    //| \|
    //0--2

    // 0번 x, z 값
    int nX = (int)(x / m_fSizeX);
    int nZ = (int)(z / m_fSizeZ);
    // 캐릭터 위치값 (해당 면(0번 좌표) 기준)
    float fDeltaX = x / m_fSizeX - nX;
    float fDeltaZ = z / m_fSizeZ - nZ;

    // 인덱스 번호
    int _0 = ((nZ + 0) * VERTEX_DIM) + (nX + 0);
    int _1 = ((nZ + 1) * VERTEX_DIM) + (nX + 0);
    int _2 = ((nZ + 0) * VERTEX_DIM) + (nX + 1);
    int _3 = ((nZ + 1) * VERTEX_DIM) + (nX + 1);

    if (fDeltaX + fDeltaZ < 1.0f) // 아랫쪽 삼각형
    {
        // z축 높이 차이
        float zY = m_vecVertex[_1].y - m_vecVertex[_0].y;
        // x축 높이 차이
        float xY = m_vecVertex[_2].y - m_vecVertex[_0].y;

        y = m_vecVertex[_0].y + zY * fDeltaZ + xY * fDeltaX;
    }
    else // 윗쪽 삼각형
    {
        // z축 높이 차이
        float zY = m_vecVertex[_2].y - m_vecVertex[_3].y;
        // x축 높이 차이
        float xY = m_vecVertex[_1].y - m_vecVertex[_3].y;

        y = m_vecVertex[_3].y + zY * (1.0f - fDeltaZ) + xY * (1.0f - fDeltaX);
    }

    return true;
}

vector<Vector3>& cHeightMap::GetVertex()
{
    return m_vecVertex;
}

vector<DWORD>& cHeightMap::GetIndex()
{
    return m_vecIndex;
}

void cHeightMap::Render()
{
    Matrix4 matW;
    D3DXMatrixIdentity(&matW);
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);

    g_pDevice->SetMaterial(m_pMtlTex->GetMtl());
    g_pDevice->SetTexture(0, m_pMtlTex->GetTexture());
    m_pMesh->DrawSubset(0);
}
