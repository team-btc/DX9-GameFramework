#include "stdafx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"


cObjLoader::cObjLoader()
{
}


cObjLoader::~cObjLoader()
{
    for each (auto p in m_mapMtlTex)
        SAFE_RELEASE(p.second);
}

void cObjLoader::Load(IN char* szFilePath, IN Matrix4* pMat, OUT vector<cGroup*>& vecGroup)
{
    vector<Vector3> vecP;
    vector<Vector3> vecN;
    vector<Vector2> vecT;
    vector<VertexPNT> vecPNTVertex;
    string sMtlName;

    FILE* fp = NULL;
    fopen_s(&fp, szFilePath, "r");

    while (!feof(fp))
    {
        char szTemp[1024];
        fgets(szTemp, 1024, fp);

        if (szTemp[0] == 'm')
        {
            char szMtlPath[1024];
            char szFullPath[1024];
            sscanf_s(szTemp, "%*s ./%s", szMtlPath, (UINT)sizeof(szMtlPath));
            sprintf_s(szFullPath, "obj/%s", szMtlPath);
            LoadMtlLib(szFullPath);
        }
        else if (szTemp[0] == 'v')
        {
            Vector3 v;
            sscanf_s(szTemp, "%*s %f %f %f", &v.x, &v.y, &v.z);

            if (szTemp[1] == 't')
            {
                Vector2 t;
                t.x = v.x;
                t.y = v.y;
                vecT.push_back(t);
            }
            else if (szTemp[1] == 'n')
                vecN.push_back(v);
            else
                vecP.push_back(v);
        }
        else if (szTemp[0] == 'u')
        {
            char szMtlName[1024];
            sscanf_s(szTemp, "%*s %s", szMtlName, (UINT)sizeof(szMtlName));
            sMtlName = szMtlName;
        }
        else if (szTemp[0] == 'f')
        {
            int aIndex[3][3];
            sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
                     &aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
                     &aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
                     &aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

            for (int i = 0; i < 3; ++i)
            {
                VertexPNT pnt;
                pnt.p = vecP[aIndex[i][0] - 1];
                pnt.t = vecT[aIndex[i][1] - 1];
                pnt.n = vecN[aIndex[i][2] - 1];

                if (pMat)
                {
                    D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
                    D3DXVec3TransformNormal(&pnt.n, &pnt.n, pMat);
                }
                vecPNTVertex.push_back(pnt);
            }
        }
        else if (szTemp[0] == 'g')
        {
            if (vecPNTVertex.empty()) continue;

            cGroup* pGroup = new cGroup;
            g_pAutoReleasePool->AddObject(pGroup);
            pGroup->SetPNTVertex(vecPNTVertex);
            pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
            vecGroup.push_back(pGroup);
            vecPNTVertex.clear();
        }
    }

    fclose(fp);
}

LPMESH cObjLoader::LoadMesh(IN char * szFilePath, IN Matrix4* pMat,
                                OUT vector<cMtlTex*>& vecMtlTex)
{
    vector<Vector3> vecP;
    vector<Vector3> vecN;
    vector<Vector2> vecT;
    vector<VertexPNT> vecPNTVertex;
    string sMtlName;

    vector<DWORD> vecAttbuf;        // 메쉬 셋팅을 위한 속성 버퍼 추가

    FILE* fp = NULL;
    fopen_s(&fp, szFilePath, "r");

    while (!feof(fp))
    {
        char szTemp[1024];
        fgets(szTemp, 1024, fp);

        if (szTemp[0] == 'm')
        {
            char szMtlPath[1024];
            char szFullPath[1024];
            sscanf_s(szTemp, "%*s ./%s", szMtlPath, (UINT)sizeof(szMtlPath));
            sprintf_s(szFullPath, "obj/%s", szMtlPath);
            LoadMtlLib(szFullPath);

            // 메쉬 속성 배열 채우기 : 파일에서 읽어드린 순서로 저장한다.
            vecMtlTex.resize(m_mapMtlTex.size());
            for each (auto p in m_mapMtlTex)
            {
                p.second->AddRef();
                g_pAutoReleasePool->AddObject(p.second);
                vecMtlTex[p.second->GetMtlRef()] = p.second;
            }

        }
        else if (szTemp[0] == 'v')
        {
            Vector3 v;
            sscanf_s(szTemp, "%*s %f %f %f", &v.x, &v.y, &v.z);

            if (szTemp[1] == 't')
            {
                Vector2 t;
                t.x = v.x;
                t.y = v.y;
                vecT.push_back(t);
            }
            else if (szTemp[1] == 'n')
                vecN.push_back(v);
            else
                vecP.push_back(v);
        }
        else if (szTemp[0] == 'u')
        {
            char szMtlName[1024];
            sscanf_s(szTemp, "%*s %s", szMtlName, (int)sizeof(szMtlName));
            sMtlName = szMtlName;
        }
        else if (szTemp[0] == 'f')
        {
            int aIndex[3][3];
            sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
                     &aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
                     &aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
                     &aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

            for (int i = 0; i < 3; ++i)
            {
                VertexPNT pnt;
                pnt.p = vecP[aIndex[i][0] - 1];
                pnt.t = vecT[aIndex[i][1] - 1];
                pnt.n = vecN[aIndex[i][2] - 1];

                if (pMat)
                {
                    D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
                    D3DXVec3TransformNormal(&pnt.n, &pnt.n, pMat);
                }
                vecPNTVertex.push_back(pnt);
            }

            // 각 페이스별 속성 버퍼 번호를 순차적으로 저장해 놓는다.
            vecAttbuf.push_back(m_mapMtlTex[sMtlName]->GetMtlRef());
        }
    }

    fclose(fp);

    // 메쉬 생성 및 정보 채우기, 최적화
    LPMESH pMesh = NULL;
    D3DXCreateMeshFVF((DWORD)(vecPNTVertex.size() / 3),
        (DWORD)vecPNTVertex.size(),
                      D3DXMESH_MANAGED,
                      VertexPNT::FVF, g_pDevice, &pMesh);

    // 메쉬 버텍스 버퍼 기록
    VertexPNT* pV = NULL;
    pMesh->LockVertexBuffer(NULL, (LPVOID*)&pV);
    memcpy(pV, &vecPNTVertex[0], vecPNTVertex.size() * sizeof(VertexPNT));
    pMesh->UnlockVertexBuffer();

    // 메쉬 인덱스 버퍼 기록
    WORD* pI = NULL;
    pMesh->LockIndexBuffer(NULL, (LPVOID*)&pI);
    for (WORD i = 0; i < vecPNTVertex.size(); ++i)
        pI[i] = i;
    pMesh->UnlockIndexBuffer();

    // 메쉬 속성 버퍼 기록
    DWORD* pA = NULL;
    pMesh->LockAttributeBuffer(NULL, &pA);
    memcpy(pA, &vecAttbuf[0], vecAttbuf.size() * sizeof(DWORD));
    pMesh->UnlockAttributeBuffer();

    // 메쉬 최적화 : 버텍스 개수 만큼 인접정보를 담을 공간을 확보
    vector<DWORD> vecAdjBuf(pMesh->GetNumFaces() * 3);
    pMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf[0]);

    pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                           &vecAdjBuf[0], 0, 0, 0);

    return pMesh;
}

void cObjLoader::LoadMtlLib(char* szFilePath)
{
    FILE* fp = NULL;
    fopen_s(&fp, szFilePath, "r");

    string sMtlName;
    int nRef = 0;

    while (!feof(fp))
    {
        char szTemp[1024];
        fgets(szTemp, 1024, fp);

        if (szTemp[0] == 'n')
        {
            char szMtlName[1024];
            sscanf_s(szTemp, "%*s %s", szMtlName, (UINT)sizeof(szMtlName));
            sMtlName = szMtlName;

            SAFE_RELEASE(m_mapMtlTex[sMtlName]);
            m_mapMtlTex[sMtlName] = new cMtlTex;
            // 속성별 고유번호 셋팅
            m_mapMtlTex[sMtlName]->SetMtlRef(nRef++);
        }
        else if (szTemp[0] == 'K')
        {
            ColorVal c;
            sscanf_s(szTemp, "%*s %f %f %f", &c.r, &c.g, &c.b);
            c.a = 1.0f;
            MATERIAL9* stMtl = m_mapMtlTex[sMtlName]->GetMtl();

            if (szTemp[1] == 'a')
                stMtl->Ambient = c;
            else if (szTemp[1] == 'd')
                stMtl->Diffuse = c;
            else if (szTemp[1] == 's')
                stMtl->Specular = c;
        }
        else if (szTemp[0] == 'm')
        {
            char szTexturePath[1024];
            char szFullPath[1024];
            sscanf_s(szTemp, "%*s %s", szTexturePath, (UINT)sizeof(szTexturePath));
            sprintf_s(szFullPath, "obj/%s", szTexturePath);

            g_pTextureManager->AddTexture(szTexturePath, szFullPath);
            LPTEXTURE9* lpTexture = g_pTextureManager->GetTexture(szTexturePath);
            m_mapMtlTex[sMtlName]->SetTexture(*lpTexture);
        }
    }

    fclose(fp);
}