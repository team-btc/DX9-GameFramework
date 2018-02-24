#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cFrame.h"


cAseLoader::cAseLoader()
    : m_pRoot(NULL)
{
}


cAseLoader::~cAseLoader()
{
}

char* cAseLoader::GetToken()
{
    bool isQuote = false;
    int nReadCount = 0;

    while (true)
    {
        unsigned char c = fgetc(m_fp);  // 한 문자씩 읽는다.

        if (feof(m_fp)) break;          // 오픈 한 파일이 끝났다면 종료

        if (!isQuote && IsWhiteSpace(c))// "가 이전에 나온적이 없고 공백을 만난 경우
        {
            if (nReadCount == 0)        // 현재까지 읽어드린 문자가 없다면 다음 문자를 읽어라
                continue;
            else
                break;                  // 현재까지 읽어드린 문자가 있을 경우 한문장 읽기 끝
        }

        if (c == '\"')                  // "를 읽은 경우
        {
            if (isQuote)                // 현재 읽은 "가 클로즈인 경우 문장 읽기 끝
                break;

            isQuote = true;             // 현재 읽은 "가 오픈인 경우 문장 읽기 시작(다음 문자 부터)
            continue;
        }

        m_szToken[nReadCount++] = c;
    }

    if (nReadCount == 0) return NULL;

    m_szToken[nReadCount] = '\0';   // 마지막에 문장의 끝을 알리는 널 문자 추가

    return m_szToken;   // 읽어드린 문자를 반환
}

int cAseLoader::GetInteger()
{
    return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
    return (float)atof(GetToken());
}

bool cAseLoader::IsWhiteSpace(char c)
{
    return c < 33;
}

bool cAseLoader::IsEqual(char* str1, char* str2)
{
    return strcmp(str1, str2) == 0;
}

void cAseLoader::ProcessSCENE(OUT ST_ASE_SCENE* pScene)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_FIRSTFRAME))
        {
            pScene->nFirstFrame = GetInteger();
        }
        else if (IsEqual(szToken, ID_LASTFRAME))
        {
            pScene->nLastFrame = GetInteger();
        }
        else if (IsEqual(szToken, ID_FRAMESPEED))
        {
            pScene->nFrameSpeed = GetInteger();
        }
        else if (IsEqual(szToken, ID_TICKSPERFRAME))
        {
            pScene->nTicksPerFrame = GetInteger();
        }
    } while (nLevel > 0);
}

cFrame* cAseLoader::Load(IN char * szFullPath, OUT ST_ASE_SCENE* pScene)
{
    fopen_s(&m_fp, szFullPath, "r");

    while (true)
    {
        char* szToken = GetToken();
        if (szToken == NULL) break;

        if (IsEqual(szToken, ID_SCENE))
        {
            ProcessSCENE(pScene);
        }
        else if (IsEqual(szToken, ID_MATERIAL_LIST))
        {
            ProcessMATERIAL_LIST();
        }
        else if (IsEqual(szToken, ID_GEOMETRY))
        {
            cFrame* pFrame = ProcessGEOMOBJECT();
            if (m_pRoot == NULL)
                m_pRoot = pFrame;
        }
    }

    fclose(m_fp);

    m_pRoot->CalcLocalTM(NULL);

    return m_pRoot;
}

void cAseLoader::ProcessMATERIAL_LIST()
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MATERIAL_COUNT))
        {
            if (!m_vecMtlTex.empty())
            {
                for each (auto p in m_vecMtlTex)
                    SAFE_RELEASE(p);
                m_vecMtlTex.clear();
            }
            m_vecMtlTex.resize(GetInteger());
        }
        else if (IsEqual(szToken, ID_MATERIAL))
        {
            int nMtlRef = GetInteger();
            SAFE_RELEASE(m_vecMtlTex[nMtlRef]);
            m_vecMtlTex[nMtlRef] = new cMtlTex;
            g_pAutoReleasePool->AddObject(m_vecMtlTex[nMtlRef]);
            ProcessMATERIAL(m_vecMtlTex[nMtlRef]);
        }

    } while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL(OUT cMtlTex* pMtlTex)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_AMBIENT))
        {
            MATERIAL9* stMtl = pMtlTex->GetMtl();
            stMtl->Ambient.r = GetFloat();
            stMtl->Ambient.g = GetFloat();
            stMtl->Ambient.b = GetFloat();
            stMtl->Ambient.a = 1.0f;
        }
        else if (IsEqual(szToken, ID_DIFFUSE))
        {
            MATERIAL9* stMtl = pMtlTex->GetMtl();
            stMtl->Diffuse.r = GetFloat();
            stMtl->Diffuse.g = GetFloat();
            stMtl->Diffuse.b = GetFloat();
            stMtl->Diffuse.a = 1.0f;
        }
        else if (IsEqual(szToken, ID_SPECULAR))
        {
            MATERIAL9* stMtl = pMtlTex->GetMtl();
            stMtl->Specular.r = GetFloat();
            stMtl->Specular.g = GetFloat();
            stMtl->Specular.b = GetFloat();
            stMtl->Specular.a = 1.0f;
        }
        else if (IsEqual(szToken, ID_MAP_DIFFUSE))
        {
            ProcessMAP_DIFFUSE(pMtlTex);
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_BITMAP))
        {
            char szFullPath[1024];
            sprintf_s(szFullPath, "%s", GetToken());

            g_pTextureManager->AddTexture(szFullPath, szFullPath);
            pMtlTex->SetTexture(*g_pTextureManager->GetTexture(szFullPath));
        }
    } while (nLevel > 0);
}

cFrame* cAseLoader::ProcessGEOMOBJECT()
{
    cFrame* pFrame = new cFrame;
    g_pAutoReleasePool->AddObject(pFrame);

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_NODE_NAME)) // 현재 프레임을 맵에 셋팅
        {
            char* szNodeName = GetToken();
            m_mapFrame[szNodeName] = pFrame;
        }
        else if (IsEqual(szToken, ID_NODE_PARENT)) // 현재 프레임의 부모를 찾아서 연결
        {
            char* szParentNodeName = GetToken();
            m_mapFrame[szParentNodeName]->AddChild(pFrame);
        }
        else if (IsEqual(szToken, ID_NODE_TM)) // 현재 프레임의 월드 매트릭스
        {
            ProcessNODE_TM(pFrame);
        }
        else if (IsEqual(szToken, ID_MESH)) // PNT 버텍스 정보 셋팅
        {
            ProcessMESH(pFrame);
        }
        else if (IsEqual(szToken, ID_TM_ANIMATION)) // 애니메이션 정보 셋팅
        {
            ProcessTM_ANIMATION(pFrame);
        }
        else if (IsEqual(szToken, ID_MATERIAL_REF)) // 사용할 속성값 셋팅
        {
            pFrame->SetMtlTex(m_vecMtlTex[GetInteger()]);
        }
    } while (nLevel > 0);

    return pFrame;
}

void cAseLoader::ProcessNODE_TM(OUT cFrame* pFrame)
{
    Matrix4 mat;
    D3DXMatrixIdentity(&mat);

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_TM_ROW0))
        {
            mat._11 = GetFloat();
            mat._13 = GetFloat();
            mat._12 = GetFloat();
        }
        else if (IsEqual(szToken, ID_TM_ROW1))
        {
            mat._31 = GetFloat();
            mat._33 = GetFloat();
            mat._32 = GetFloat();
        }
        else if (IsEqual(szToken, ID_TM_ROW2))
        {
            mat._21 = GetFloat();
            mat._23 = GetFloat();
            mat._22 = GetFloat();
        }
        else if (IsEqual(szToken, ID_TM_ROW3))
        {
            mat._41 = GetFloat();
            mat._43 = GetFloat();
            mat._42 = GetFloat();
        }
    } while (nLevel > 0);

    pFrame->SetWorldTM(mat);
}

void cAseLoader::ProcessMESH(OUT cFrame* pFrame)
{
    vector<Vector3>		vecP;
    vector<Vector3>		vecN;
    vector<Vector2>		vecT;
    vector<VertexPNT>	vecPNTVertex;

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_NUMVERTEX))   // 버텍스 갯수
            vecP.resize(GetInteger());
        else if (IsEqual(szToken, ID_MESH_NUMFACES))    // 페이스(면, PNT) 갯수
            vecPNTVertex.resize(GetInteger() * 3); // 면 1개당 정점 3개
        else if (IsEqual(szToken, ID_MESH_VERTEX_LIST)) // 버텍스 정보
            ProcessMESH_VERTEX_LIST(vecP);
        else if (IsEqual(szToken, ID_MESH_FACE_LIST))   // 페이스 정보(P)
            ProcessMESH_FACE_LIST(vecP, vecPNTVertex);
        else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))  // 텍스쳐 버텍스 갯수
            vecT.resize(GetInteger());
        else if (IsEqual(szToken, ID_MESH_TVERTLIST))   // 텍스쳐 버텍스 정보
            ProcessMESH_TVERTLIST(vecT);
        else if (IsEqual(szToken, ID_MESH_TFACELIST))   // 페이스 정보(T)
            ProcessMESH_TFACELIST(vecT, vecPNTVertex);
        else if (IsEqual(szToken, ID_MESH_NORMALS))     // 노말 정보(N)
            ProcessMESH_NORMALS(vecPNTVertex);
    } while (nLevel > 0);

    /*
    파일에서 읽어드린 정점 정보들은 이미 월드매트릭스가 적용 된 상태이다.
    애니메이션을 위해서는 월드매트릭스를 변형 생성하며 사용할 것이기 때문에
    기존 정점 정보들을 로컬 포지션으로 돌려 보내야 한다.
    */
    Matrix4 matInvNodeTM;
    D3DXMatrixInverse(&matInvNodeTM, NULL, &pFrame->GetWorldTM());
    for (int i = 0; i < (int)vecPNTVertex.size(); ++i)
    {
        D3DXVec3TransformCoord(&vecPNTVertex[i].p, &vecPNTVertex[i].p, &matInvNodeTM);
        D3DXVec3TransformNormal(&vecPNTVertex[i].n, &vecPNTVertex[i].n, &matInvNodeTM);
    }

    pFrame->SetPNTVertex(vecPNTVertex);
}

void cAseLoader::ProcessMESH_VERTEX_LIST(OUT vector<Vector3>& vecV)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_VERTEX))
        {
            int nIndex = GetInteger();
            vecV[nIndex].x = GetFloat();
            vecV[nIndex].z = GetFloat();
            vecV[nIndex].y = GetFloat();
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST(IN vector<Vector3>& vecV, OUT vector<VertexPNT>& vecPNTVertex)
{
    int nIndex = 0;

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_FACE))
        {
            GetToken(); // 페이스 인덱스 번호
            GetToken(); // A:
            int nA = GetInteger();
            GetToken(); // B:
            int nB = GetInteger();
            GetToken(); // C:
            int nC = GetInteger();

            vecPNTVertex[nIndex++].p = vecV[nA];
            vecPNTVertex[nIndex++].p = vecV[nC];
            vecPNTVertex[nIndex++].p = vecV[nB];
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTLIST(OUT vector<Vector2>& vecT)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_TVERT))
        {
            int nIndex = GetInteger();
            vecT[nIndex].x = GetFloat();
            vecT[nIndex].y = 1.0f - GetFloat();
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACELIST(IN vector<Vector2>& vecT, OUT vector<VertexPNT>& vecPNTVertex)
{
    int nIndex = 0;

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_TFACE))
        {
            GetToken();
            int nA = GetInteger();
            int nB = GetInteger();
            int nC = GetInteger();

            vecPNTVertex[nIndex++].t = vecT[nA];
            vecPNTVertex[nIndex++].t = vecT[nC];
            vecPNTVertex[nIndex++].t = vecT[nB];
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS(OUT vector<VertexPNT>& vecPNTVertex)
{
    int nIndex = 0;
    int nCount = 0;
    int aModCount[] = { 0, 2, 1 };

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
        {
            GetToken();
            int index = nIndex + aModCount[nCount];
            vecPNTVertex[index].n.x = GetFloat();
            vecPNTVertex[index].n.z = GetFloat();
            vecPNTVertex[index].n.y = GetFloat();

            ++nCount;
            if (nCount == 3)
            {
                nCount = 0;
                nIndex += 3;
            }
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessTM_ANIMATION(OUT cFrame* pFrame)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_POS_TRACK))
        {
            ProcessCONTROL_POS_TRACK(pFrame);
        }
        else if (IsEqual(szToken, ID_ROT_TRACK))
        {
            ProcessCONTROL_ROT_TRACK(pFrame);
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessCONTROL_POS_TRACK(OUT cFrame* pFrame)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_POS_SAMPLE))
        {
            ST_POS_SAMPLE stSample;
            stSample.n = GetInteger();
            stSample.v.x = GetFloat();
            stSample.v.z = GetFloat();
            stSample.v.y = GetFloat();

            pFrame->GetPosTrack().push_back(stSample);
        }
    } while (nLevel > 0);
}

void cAseLoader::ProcessCONTROL_ROT_TRACK(OUT cFrame* pFrame)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_ROT_SAMPLE))
        {
            ST_ROT_SAMPLE stSample;
            stSample.n = GetInteger();
            stSample.q.x = GetFloat();
            stSample.q.z = GetFloat();
            stSample.q.y = GetFloat();
            stSample.q.w = GetFloat();

            stSample.q.x = stSample.q.x * sinf(stSample.q.w / 2.0f);
            stSample.q.z = stSample.q.z * sinf(stSample.q.w / 2.0f);
            stSample.q.y = stSample.q.y * sinf(stSample.q.w / 2.0f);
            stSample.q.w = cosf(stSample.q.w / 2.0f);

            if (!pFrame->GetRotTrack().empty())
                stSample.q = pFrame->GetRotTrack().back().q * stSample.q;

            pFrame->GetRotTrack().push_back(stSample);
        }
    } while (nLevel > 0);
}
