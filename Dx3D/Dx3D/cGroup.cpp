#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
    : m_pMtlTex(NULL)
{
}


cGroup::~cGroup()
{
    SAFE_RELEASE(m_pMtlTex);
}

void cGroup::Render()
{
    g_pDevice->SetTexture(0, m_pMtlTex->GetTexture());
    g_pDevice->SetMaterial(m_pMtlTex->GetMtl());
    g_pDevice->SetFVF(VertexPNT::FVF);
    g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
        (UINT)(m_vecPNTVertex.size() / 3),
                                  &m_vecPNTVertex[0],
                                  (UINT)sizeof(VertexPNT));
}
