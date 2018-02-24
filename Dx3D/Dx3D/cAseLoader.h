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
    bool	IsEqual(char* str1, char* str2); // strcmp : ���� == 0

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

    /* �����̼� Ʈ�� ������ ��Ƽ�Ͽ� ������ �����ؼ� ���� �Ѵ�.
    Ű ���� �״�� ��´�.

    stRotSample.q.x = stRotSample.q.x * sinf(stRotSample.q.w / 2.0f)
    stRotSample.q.y = stRotSample.q.y * sinf(stRotSample.q.w / 2.0f)
    stRotSample.q.z = stRotSample.q.z * sinf(stRotSample.q.w / 2.0f)
    stRotSample.q.w = cosf(stRotSample.q.w / 2.0f)

    ȸ�� ���� '����' ���� ó�� �ȴ�.
    if (!pFrame->m_vecRotTrack.empty())
    ���������� ��� �� ���� ������ ��� �� ���� ���ؼ� �ִ´�.

    ������ ��� �� ���ʹϾ� ���� �״�� ����
    */

public:
    cAseLoader();
    ~cAseLoader();

    cFrame* Load(IN char* szFullPath, OUT ST_ASE_SCENE* pScene);
};

