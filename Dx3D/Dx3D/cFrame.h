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
    int     n;      // 프레임 값
    Vector3	v;      // 해당 프레임에서의 위치 값
};

struct ST_ROT_SAMPLE
{
    int             n;  // 프레임 값
    Quaternion  	q;  // 해당 프레임에서의 퀀터니온 값
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

    void CalcLocalTM(cFrame* pParent);      // 각각의 프레임을 로컬 기준축으로 처리 할 수 있도록 로컬 매트릭스를 생성
    void CalcLocalTranslationMatrix(IN int nKeyFrame, OUT Matrix4& mat);  // 이동 행렬 생성
    void CalcLocalRotationMatrix(IN int nKeyFrame, OUT Matrix4& mat);     // 회전 행렬 생성

    vector<ST_POS_SAMPLE>& GetPosTrack() { return m_vecPosTrack; }
    vector<ST_ROT_SAMPLE>& GetRotTrack() { return m_vecRotTrack; }
};

