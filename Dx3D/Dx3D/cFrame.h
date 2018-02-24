#pragma once
#include "cObject.h"
#include "cMtlTex.h"

struct ST_ASE_SCENE
{
    int nFirstFrame;
    int nLastFrame;
    int nFrameSpeed;
    int nTicksPerFrame;
};

struct ST_POS_SAMPLE
{
    int     n;      // ������ ��
    Vector3	v;      // �ش� �����ӿ����� ��ġ ��
};

struct ST_ROT_SAMPLE
{
    int             n;  // ������ ��
    Quaternion  	q;  // �ش� �����ӿ����� ���ʹϿ� ��
};

class cFrame : public cObject
{
private:
    SYNTHESIZE_PASS_BY_REF(Matrix4, m_matWorldTM, WorldTM);
    SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecPNTVertex, PNTVertex);
    SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

private:
    vector<cFrame*>         m_vecChild;

    Matrix4           m_matLocalTM;
    vector<ST_POS_SAMPLE>   m_vecPosTrack;
    vector<ST_ROT_SAMPLE>   m_vecRotTrack;

public:
    cFrame();
    ~cFrame();

    void Update(int nKeyFrame, Matrix4* pParent);
    void Render();
    void AddChild(cFrame* pChild);

    void CalcLocalTM(cFrame* pParent);      // ������ �������� ���� ���������� ó�� �� �� �ֵ��� ���� ��Ʈ������ ����
    void CalcLocalTranslationMatrix(IN int nKeyFrame, OUT Matrix4& mat);  // �̵� ��� ����
    void CalcLocalRotationMatrix(IN int nKeyFrame, OUT Matrix4& mat);     // ȸ�� ��� ����

    vector<ST_POS_SAMPLE>& GetPosTrack() { return m_vecPosTrack; }
    vector<ST_ROT_SAMPLE>& GetRotTrack() { return m_vecRotTrack; }
};

