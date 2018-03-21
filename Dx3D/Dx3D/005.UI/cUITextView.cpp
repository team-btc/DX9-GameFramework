#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView()
    : m_pFont(NULL)
    , m_dwDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP)
    , m_dwColor(D3DCOLOR_XRGB(255, 255, 255))
{
}


cUITextView::~cUITextView()
{
    SAFE_RELEASE(m_pFont);
}

void cUITextView::Render(LPSPRITE pSprite)
{
    if (m_isAxtive)
    {

        RECT rc;	// 텍스트를 그릴 영역 설정
        SetRect(&rc,
            (int)m_matWorld._41,
            (int)m_matWorld._42,
            (int)(m_matWorld._41 + m_stSize.x),
            (int)(m_matWorld._42 + m_stSize.y));

#ifdef _DEBUG
        // 수정하기
        Color color = D3DCOLOR_XRGB(255, 255, 255);

        vector<VertexRHWC> vecVertex;
        vecVertex.reserve(8);

        vecVertex.push_back(VertexRHWC(Vector4((float)rc.left, (float)rc.top, 0, 1), color));
        vecVertex.push_back(VertexRHWC(Vector4((float)rc.right, (float)rc.top, 0, 1), color));

        vecVertex.push_back(VertexRHWC(Vector4((float)rc.right, (float)rc.top, 0, 1), color));
        vecVertex.push_back(VertexRHWC(Vector4((float)rc.right, (float)rc.bottom, 0, 1), color));

        vecVertex.push_back(VertexRHWC(Vector4((float)rc.right, (float)rc.bottom, 0, 1), color));
        vecVertex.push_back(VertexRHWC(Vector4((float)rc.left, (float)rc.bottom, 0, 1), color));

        vecVertex.push_back(VertexRHWC(Vector4((float)rc.left, (float)rc.bottom, 0, 1), color));
        vecVertex.push_back(VertexRHWC(Vector4((float)rc.left, (float)rc.top, 0, 1), color));

        g_pDevice->SetFVF(VertexRHWC::FVF);
        g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecVertex[0], sizeof(VertexRHWC));
#endif // _DEBUG


        m_pFont->DrawTextA(NULL, m_sText.c_str(), (INT)m_sText.length(), &rc,
            m_dwDrawTextFormat, m_dwColor);
    }

    cUIObject::Render(pSprite);
}
