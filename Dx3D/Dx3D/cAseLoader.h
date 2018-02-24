#pragma once
#include "cObject.h"

class cMtlTex;
class cFrame;
struct ST_ASE_SCENE;

class cAseLoader : public cObject
{
private:
    FILE*                   m_fp;
    char                    m_szToken[1024];
    vector<cMtlTex*>        m_vecMtlTex;
    map<string, cFrame*>    m_mapFrame;
    cFrame*                 m_pRoot;

private:
    char*	GetToken();
    int		GetInteger();
    float	GetFloat();
    bool	IsWhiteSpace(char c);
    bool	IsEqual(char* str1, char* str2); // strcmp : 같다 == 0

    void	ProcessSCENE(OUT ST_ASE_SCENE* pScene);

    void	ProcessMATERIAL_LIST();
    void	ProcessMATERIAL(OUT cMtlTex* pMtlTex);
    void	ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex);

    cFrame*	ProcessGEOMOBJECT();
    void	ProcessNODE_TM(OUT cFrame* pFrame);
    void	ProcessMESH(OUT cFrame* pFrame);

    void	ProcessMESH_VERTEX_LIST(OUT vector<Vector3>& vecV);
    void	ProcessMESH_FACE_LIST(IN vector<Vector3>& vecV, OUT vector<ST_PNT_VERTEX>& vecPNTVertex);

    void	ProcessMESH_TVERTLIST(OUT vector<Vector2>& vecT);
    void	ProcessMESH_TFACELIST(IN vector<Vector2>& vecT, OUT vector<ST_PNT_VERTEX>& vecPNTVertex);

    void	ProcessMESH_NORMALS(OUT vector<ST_PNT_VERTEX>& vecPNTVertex);

    void	ProcessTM_ANIMATION(OUT cFrame* pFrame);
    void	ProcessCONTROL_POS_TRACK(OUT cFrame* pFrame);
    void	ProcessCONTROL_ROT_TRACK(OUT cFrame* pFrame);

    /* 로테이션 트랙 정보는 퀀티니온 값으로 변경해서 담어야 한다.
    키 값은 그대로 담는다.

    stRotSample.q.x = stRotSample.q.x * sinf(stRotSample.q.w / 2.0f)
    stRotSample.q.y = stRotSample.q.y * sinf(stRotSample.q.w / 2.0f)
    stRotSample.q.z = stRotSample.q.z * sinf(stRotSample.q.w / 2.0f)
    stRotSample.q.w = cosf(stRotSample.q.w / 2.0f)

    회전 값은 '누적' 연산 처리 된다.
    if (!pFrame->m_vecRotTrack.empty())
    마지막으로 계산 된 값에 위에서 계산 된 값을 곱해서 넣는다.

    위에서 계산 된 퀀터니언 값을 그대로 대입
    */

public:
    cAseLoader();
    ~cAseLoader();

    cFrame* Load(IN char* szFullPath, OUT ST_ASE_SCENE* pScene);
};

