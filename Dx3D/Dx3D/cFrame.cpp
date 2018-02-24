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
    ����TM ��� : ����T * ����R
    ����T : CalcLocalTranslationMatrix()
    ����R : CalcLocalRotationMatrix()

    �θ� ���� �� ���
    ����TM = ����TM * �θ�TM

    ��� �ڽĵ��� ��������� ó�� �Ѵ�.
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
    // ����TM�� ����Ѵ�.
    // NODE_TM �� �θ��� ����TM���� ������ �� ���� ��Ʈ���� �̴�.
    // ����TM = NODE_TM * �θ�Inv
    // �ڽĵ���� ��������� ����

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

    if (m_vecPosTrack.empty())	// �ִϸ��̼� Ʈ�� ������ ���� ���
    {
        mat._41 = m_matLocalTM._41;
        mat._42 = m_matLocalTM._42;
        mat._43 = m_matLocalTM._43;
    }
    else if (nKeyFrame <= m_vecPosTrack.front().n) // ���� Ű�������� ����Ʈ�� �ּ� Ű������ ������ ���� ���
    {
        mat._41 = m_vecPosTrack.front().v.x;
        mat._42 = m_vecPosTrack.front().v.y;
        mat._43 = m_vecPosTrack.front().v.z;
    }
    else if (nKeyFrame >= m_vecPosTrack.back().n) // ���� ū ���� ū ���
    {
        mat._41 = m_vecPosTrack.back().v.x;
        mat._42 = m_vecPosTrack.back().v.y;
        mat._43 = m_vecPosTrack.back().v.z;
    }
    else // ���̰��� ����
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

        // ���� ��ġ / ��ü ����
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
    // D3DXMatrixRotationQuaternion(&mat, ���ʹϾ� ��)
    // D3DXQuaternionSlerp() ���ʹϾ� ����(ȸ�� ����)

    if (m_vecRotTrack.empty())	// �ִϸ��̼� Ʈ�� ������ ���� ���
    {
        mat = m_matLocalTM;
        mat._41 = 0.0f;
        mat._42 = 0.0f;
        mat._43 = 0.0f;
    }
    else if (nKeyFrame <= m_vecRotTrack.front().n) // ���� Ű�������� ����Ʈ�� �ּ� Ű������ ������ ���� ���
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
    }
    else if (nKeyFrame >= m_vecRotTrack.back().n) // ���� ū ���� ū ���
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
    }
    else // ���̰��� ����
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

        // ���� ��ġ / ��ü ����
        float t = (nKeyFrame - m_vecRotTrack[nPrev].n) /
            (float)(m_vecRotTrack[nNext].n - m_vecRotTrack[nPrev].n);

        Quaternion rot;
        D3DXQuaternionSlerp(&rot, &m_vecRotTrack[nPrev].q, &m_vecRotTrack[nNext].q, t);
        D3DXMatrixRotationQuaternion(&mat, &rot);
    }
}
