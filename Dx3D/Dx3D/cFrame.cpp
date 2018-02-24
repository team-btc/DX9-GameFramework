#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
    : m_pMtlTex(NULL)
{
    D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
}

void cFrame::Update(int nKeyFrame, Matrix4* pParent)
{
    /*
    월드TM 계산 : 로컬T * 로컬R
    로컬T : CalcLocalTranslationMatrix()
    로컬R : CalcLocalRotationMatrix()

    부모가 존재 할 경우
    월드TM = 월드TM * 부모TM

    모든 자식들은 재귀적으로 처리 한다.
    */

    Matrix4 matT, matR;
    CalcLocalTranslationMatrix(nKeyFrame, matT);
    CalcLocalRotationMatrix(nKeyFrame, matR);
    m_matWorldTM = matR * matT;

    if (pParent)
        m_matWorldTM *= *pParent;

    for each (auto p in m_vecChild)
        p->Update(nKeyFrame, &m_matWorldTM);
}

void cFrame::Render()
{
    if (m_pMtlTex)
    {
        g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
        g_pDevice->SetTexture(0, m_pMtlTex->GetTexture());
        g_pDevice->SetMaterial(m_pMtlTex->GetMtl());
        g_pDevice->SetFVF(VertexPNT::FVF);
        g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
            (int)(m_vecPNTVertex.size() / 3),
                                      &m_vecPNTVertex[0],
                                      sizeof(VertexPNT));
    }

    for each (auto p in m_vecChild)
        p->Render();
}

void cFrame::AddChild(cFrame* pChild)
{
    if (pChild == NULL) return;

    m_vecChild.push_back(pChild);
}

void cFrame::CalcLocalTM(cFrame* pParent)
{
    // 로컬TM을 계산한다.
    // NODE_TM 은 부모의 로컬TM까지 적용이 된 월드 매트릭스 이다.
    // 로컬TM = NODE_TM * 부모Inv
    // 자식들까지 재귀적으로 적용

    m_matLocalTM = m_matWorldTM;

    if (pParent)
    {
        Matrix4 matInvParent;
        D3DXMatrixInverse(&matInvParent, NULL, &pParent->GetWorldTM());
        m_matLocalTM *= matInvParent;
    }

    for each (auto p in m_vecChild)
        p->CalcLocalTM(this);
}

void cFrame::CalcLocalTranslationMatrix(IN int nKeyFrame, OUT Matrix4& mat)
{
    D3DXMatrixIdentity(&mat);

    if (m_vecPosTrack.empty())	// 애니메이션 트랙 정보가 없을 경우
    {
        mat._41 = m_matLocalTM._41;
        mat._42 = m_matLocalTM._42;
        mat._43 = m_matLocalTM._43;
    }
    else if (nKeyFrame <= m_vecPosTrack.front().n) // 현재 키프레임이 포즈트랙 최소 키프레임 값보다 작은 경우
    {
        mat._41 = m_vecPosTrack.front().v.x;
        mat._42 = m_vecPosTrack.front().v.y;
        mat._43 = m_vecPosTrack.front().v.z;
    }
    else if (nKeyFrame >= m_vecPosTrack.back().n) // 가장 큰 보다 큰 경우
    {
        mat._41 = m_vecPosTrack.back().v.x;
        mat._42 = m_vecPosTrack.back().v.y;
        mat._43 = m_vecPosTrack.back().v.z;
    }
    else // 사이값은 보간
    {
        int nNext;
        for (int i = 0; i < (int)m_vecPosTrack.size(); ++i)
        {
            if (m_vecPosTrack[i].n > nKeyFrame)
            {
                nNext = i;
                break;
            }
        }
        int nPrev = nNext - 1;

        // 현재 위치 / 전체 구간
        float t = (nKeyFrame - m_vecPosTrack[nPrev].n) /
            (float)(m_vecPosTrack[nNext].n - m_vecPosTrack[nPrev].n);

        Vector3 pos;
        D3DXVec3Lerp(&pos, &m_vecPosTrack[nPrev].v, &m_vecPosTrack[nNext].v, t);
        mat._41 = pos.x;
        mat._42 = pos.y;
        mat._43 = pos.z;
    }
}

void cFrame::CalcLocalRotationMatrix(IN int nKeyFrame, OUT Matrix4& mat)
{
    // D3DXMatrixRotationQuaternion(&mat, 퀀터니언 값)
    // D3DXQuaternionSlerp() 퀀터니언 보간(회전 보간)

    if (m_vecRotTrack.empty())	// 애니메이션 트랙 정보가 없을 경우
    {
        mat = m_matLocalTM;
        mat._41 = 0.0f;
        mat._42 = 0.0f;
        mat._43 = 0.0f;
    }
    else if (nKeyFrame <= m_vecRotTrack.front().n) // 현재 키프레임이 포즈트랙 최소 키프레임 값보다 작은 경우
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
    }
    else if (nKeyFrame >= m_vecRotTrack.back().n) // 가장 큰 보다 큰 경우
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
    }
    else // 사이값은 보간
    {
        int nNext;
        for (int i = 0; i < (int)m_vecRotTrack.size(); ++i)
        {
            if (m_vecRotTrack[i].n > nKeyFrame)
            {
                nNext = i;
                break;
            }
        }
        int nPrev = nNext - 1;

        // 현재 위치 / 전체 구간
        float t = (nKeyFrame - m_vecRotTrack[nPrev].n) /
            (float)(m_vecRotTrack[nNext].n - m_vecRotTrack[nPrev].n);

        Quaternion rot;
        D3DXQuaternionSlerp(&rot, &m_vecRotTrack[nPrev].q, &m_vecRotTrack[nNext].q, t);
        D3DXMatrixRotationQuaternion(&mat, &rot);
    }
}
