#pragma once
#include "stdafx.h"
#include "cUILayer.h"
#include "cUIObject.h"
#include "cUIButton.h"
#include "cUIImageView.h"

cUILayer::cUILayer()
    : m_pSprite(NULL)
    , m_stLayerSize(0, 0)
    , m_vPosition(Vector3(0, 0, 0))
    , m_textureBackground(NULL)
    , m_fRatioX(0.0f)
    , m_fRatioY(0.0f)
    , m_pParent(NULL)
    , m_pUIobjectRoot(NULL)
    , m_eUseBackground(E_UI_BACKGROUND_OFF)
    , m_isVertexBackground(false)
    , m_isMove(false)
    , m_isActive(false)
{
    D3DXMatrixIdentity(&m_matWorld);			// 메트릭스 초기화 
    SetRect(&m_rtLayer, 0, 0, 0, 0);			// 렉트 초기화 
    D3DXCreateSprite(g_pDevice, &m_pSprite);	// 스프라이트 생성
}

cUILayer::~cUILayer()
{
    if (!m_vecUILayerChilds.empty())
    {
        for (int i = 0; i< m_vecUILayerChilds.size(); i++)
        {
            SAFE_DELETE(m_vecUILayerChilds[i]);
        }
        m_vecUILayerChilds.clear();
    }

    SAFE_RELEASE(m_pUIobjectRoot);
    SAFE_RELEASE(m_pSprite);
}

HRESULT cUILayer::Setup()
{
    return E_NOTIMPL;
}

HRESULT cUILayer::Update()
{
    UpdateWorldMatrix();			// 월드 매트릭스 갱신
    UpdateChildren();				// 자식 갱신

    if (m_pUIobjectRoot)
        m_pUIobjectRoot->Update();

    return E_NOTIMPL;
}

#pragma region 수정중(변화에 따라 계속 수정)
HRESULT cUILayer::Render()
{
    if (m_isActive)
    {
        HRESULT hr;
        // 백그라운드 그리기 
        if (m_eUseBackground == E_UI_BACKGROUND_ON)
        {
            // 버텍스 or 텍스쳐로 그리기
            if (m_isVertexBackground)
            {
                hr = RenderVertexBackground();
            }
            else
            {
                hr = RenderTextureBackground();
            }
        }
        // 가이드라인 그리기
        hr = RenderGuideLine();

        if (m_pUIobjectRoot)
        {
            m_pUIobjectRoot->Render(m_pSprite);
        }

        // 레이어 자식이 있을 경우 
        if (!m_vecUILayerChilds.empty())
        {
            for each(auto p in m_vecUILayerChilds)
            {
                p->SetActive(m_isActive);
                p->Render();
            }
        }
    }
    return E_NOTIMPL;
}
#pragma endregion

// INVERSE 매트릭스는 다시 수정
HRESULT cUILayer::UpdateWorldMatrix()
{
    m_matWorld._41 = m_vPosition.x;
    m_matWorld._42 = m_vPosition.y;
    m_matWorld._43 = m_vPosition.z;

    if (m_pParent)
    {
        m_matWorld._41 += m_pParent->m_matWorld._41;
        m_matWorld._42 += m_pParent->m_matWorld._42;
        m_matWorld._43 += m_pParent->m_matWorld._43;
    }

    UpdateRect();

    return S_OK;
}

HRESULT cUILayer::UpdateChildren()
{
    for each(auto p in m_vecUILayerChilds)
    {
        p->Update();
    }

    return S_OK;
}

HRESULT cUILayer::UpdateRect()
{
    m_rtLayer.left = (int)m_matWorld._41;
    m_rtLayer.top = (int)m_matWorld._42;
    m_rtLayer.right = m_rtLayer.left + (int)(m_stLayerSize.w * m_matWorld._11);
    m_rtLayer.bottom = m_rtLayer.top + (int)(m_stLayerSize.h * m_matWorld._22);

    return S_OK;
}

HRESULT cUILayer::AddUILayerChild(IN cUILayer* pChild)
{
    if (pChild == NULL)
    {
        return E_INVALIDARG;
    }

    pChild->m_pParent = this;
    m_vecUILayerChilds.push_back(pChild);

    return S_OK;
}

#pragma region 수정중 (반복 설정이 맞게 됬는지)
// 1)반환될 자식의 주소(찾지 못할시 NULL반환), 2)찾을 자식의 이름
// 이름을 입력하여 자식을 찾는다.
HRESULT cUILayer::FindUILayerChild(OUT cUILayer** pChild, IN string strChildName)
{
    if (strChildName.empty() || strChildName[0] == NULL)
    {
        return E_INVALIDARG;
    }
    if (m_strName == strChildName)
    {
        *pChild = this;
        return S_OK;
    }
    else
    {
        if (!m_vecUILayerChilds.empty())
        {
            for each (auto p in m_vecUILayerChilds)
            {
                p->FindUILayerChild(pChild, strChildName);

                if (p->m_strName == strChildName)
                {
                    *pChild = p;
                    return S_OK;
                }
            }
        }
    }

    // 없는 경우
    pChild = NULL;
    return S_OK;
}
HRESULT cUILayer::AddUIObject(IN cUIObject * pObject)
{
    pObject->SetParent(m_pUIobjectRoot);
    m_pUIobjectRoot->AddChild(pObject);

    return S_OK;
}
HRESULT cUILayer::FindUIObject(OUT cUIObject ** pObject, IN string strObjectName)
{
    if (strObjectName.empty() || strObjectName[0] == NULL)
    {
        return E_INVALIDARG;
    }

    if (m_pUIobjectRoot->GetName() == strObjectName)
    {
        *pObject = m_pUIobjectRoot;
    }
    else
    {
        *pObject = m_pUIobjectRoot->GetChildByName(strObjectName);
    }

    return S_OK;
}
#pragma endregion

HRESULT cUILayer::SetPosition(IN Vector3 vPosition)
{
    m_vPosition = vPosition;
    return S_OK;
}

HRESULT cUILayer::GetPosition(OUT Vector3& vPosition)
{
    vPosition = m_vPosition;
    return S_OK;
}

HRESULT cUILayer::GetLayerRect(OUT RECT& rtLayer)
{
    rtLayer = m_rtLayer;
    return E_NOTIMPL;
}

HRESULT cUILayer::SetBackGroundColor(IN Color colorARGB)
{
    m_colorBackground = colorARGB;
    return S_OK;
}

HRESULT cUILayer::GetActive(OUT bool&  isActiveState)
{
    isActiveState = m_isActive;
    return S_OK;
}

HRESULT cUILayer::SetActive(IN bool isActiveState)
{
    m_isActive = isActiveState;
    return S_OK;
}

HRESULT cUILayer::GetWorldMatrix(OUT Matrix4 matWorld)
{
    return E_NOTIMPL;
}

HRESULT cUILayer::SetWorldMatrix(IN Matrix4 matWorld)
{
    return E_NOTIMPL;
}

//1)이름, 2)위치, 3)사이즈, 4)백그라운드 표시 = false, 5)백그라운드 컬러 = 0, 6)백그라운드 텍스쳐 ="")) 
//입력된 값을 통해 레이어를 설정한다.	
HRESULT cUILayer::SetLayer(IN string strLayerName, IN Vector3 vPosition, IN ST_SIZE stLayerSize, IN bool useBackground, IN Color colerBackGround, IN string strTextureName)
{
    HRESULT hr;

    if (strLayerName.empty() || strLayerName[0] == NULL)
    {
        return hr = E_INVALIDARG;
    }
    else
    {
        m_strName = strLayerName;
    }

    if (m_pParent)
    {
        Vector3 vParentPos, vMyPos = vPosition;
        hr = m_pParent->GetPosition(vParentPos);
        vMyPos += vParentPos;
        hr = SetPosition(vMyPos);
    }
    else
    {
        hr = SetPosition(vPosition);
    }

    hr = SetSize(stLayerSize.w, stLayerSize.h);

    // 월드 매트릭스 세팅 
    m_matWorld._41 = m_vPosition.x;
    m_matWorld._42 = m_vPosition.y;
    m_matWorld._43 = m_vPosition.z;

    // 렉트 설정
    SetRect(&m_rtLayer,
        (int)m_vPosition.x,
        (int)m_vPosition.y,
        (int)(m_vPosition.x + m_stLayerSize.w),
        (int)(m_vPosition.y + m_stLayerSize.h));

    // 백그라운드 사용여부 설정
    if (useBackground)
    {
        m_eUseBackground = E_UI_BACKGROUND_ON;
        string str = strTextureName;

        // 뛰어 쓰기일 경우 잘못된 입력
        if ((int)str[0] == NULL)
        {
            return E_INVALIDARG;
        }
        // 빈 스트링일 경우는 버텍스로 배경을 그리게 
        if (str.empty())
        {
            m_isVertexBackground = true;
        }
        // 스트링이 비어있지 않을 경우 텍스쳐로 그리게 
        else
        {
            D3DXIMAGE_INFO imgInfo;
            m_textureBackground = g_pTextureManager->GetTexture(strTextureName, &imgInfo);
            m_stBackgroundTextureSize.w = (float)imgInfo.Width;
            m_stBackgroundTextureSize.h = (float)imgInfo.Height;

            m_fRatioX = m_stLayerSize.w / m_stBackgroundTextureSize.w;
            m_fRatioY = m_stLayerSize.h / m_stBackgroundTextureSize.h;
            m_isVertexBackground = false;
        }
        // 백그라운드 컬러를 설정 
        hr = SetBackGroundColor(colerBackGround);
    }

    m_pUIobjectRoot = new cUIObject;
    m_pUIobjectRoot->SetLocalPos(m_vPosition);
    m_pUIobjectRoot->SetParent(NULL);
    m_pUIobjectRoot->SetName("UI_OBJECT_ROOT");
    m_pUIobjectRoot->SetRectBody(m_rtLayer);

    return S_OK;
}

// 입력받은 너비와 높이로 레이어의 크기를 설정한다.
HRESULT cUILayer::SetSize(IN float fWidth, IN float fHeight)
{
    if (fWidth <= 0 || fHeight <= 0.0f)
    {
        return E_INVALIDARG;
    }
    else
    {
        m_stLayerSize.w = fWidth;
        m_stLayerSize.h = fHeight;
        return S_OK;
    }
}

// RenderGuideLine() 가이드라인을 그린다.
HRESULT cUILayer::RenderGuideLine()
{
    float left = m_matWorld._41;
    float top = m_matWorld._42;
    float right = m_matWorld._41 + m_stLayerSize.w;
    float bottom = m_matWorld._42 + m_stLayerSize.h;

    // 수정하기
    float top10persent = top + (10.0f * m_stLayerSize.h / 100.0f);

    Color color = D3DCOLOR_XRGB(0, 0, 0);

    vector<VertexRHWC> vecVertex;
    vecVertex.reserve(10);

    vecVertex.push_back(VertexRHWC(Vector4(left, top, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(right, top, 0, 1), color));

    vecVertex.push_back(VertexRHWC(Vector4(right, top, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(right, bottom, 0, 1), color));

    vecVertex.push_back(VertexRHWC(Vector4(right, bottom, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(left, bottom, 0, 1), color));

    vecVertex.push_back(VertexRHWC(Vector4(left, bottom, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(left, top, 0, 1), color));

    vecVertex.push_back(VertexRHWC(Vector4(left, top10persent, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(right, top10persent, 0, 1), color));

    g_pDevice->SetFVF(VertexRHWC::FVF);
    g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 5, &vecVertex[0], sizeof(VertexRHWC));


    return S_OK;
}

#pragma region 수정중(알파 값이 먹어야되는데...)
// 버텍스로 백그라운드를 그린다. 
HRESULT cUILayer::RenderVertexBackground()
{
    float left = m_matWorld._41 + 0.5f;
    float top = m_matWorld._42 + 0.5f;
    float right = left + m_stLayerSize.w;
    float bottom = top + m_stLayerSize.h;

    vector<VertexRHWC> vecVertex;
    vecVertex.reserve(6);

    D3DXCOLOR color = m_colorBackground;

    vecVertex.push_back(VertexRHWC(Vector4(left, bottom, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(left, top, 0, 1), color));

    vecVertex.push_back(VertexRHWC(Vector4(right, top, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(left, bottom, 0, 1), color));

    vecVertex.push_back(VertexRHWC(Vector4(right, top, 0, 1), color));
    vecVertex.push_back(VertexRHWC(Vector4(right, bottom, 0, 1), color));

    g_pDevice->SetFVF(VertexRHWC::FVF);
    g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &vecVertex[0], sizeof(VertexRHWC));

    return E_NOTIMPL;
}
#pragma endregion

// 텍스쳐로 백그라운드를 그린다.
HRESULT cUILayer::RenderTextureBackground()
{
    // 이미지 영역 렉트(전체영역)
    RECT rc;
    SetRect(&rc,
        0,
        0,
        (int)m_stBackgroundTextureSize.w,
        (int)m_stBackgroundTextureSize.h);

    // 이미지 크기와 상관 없이 레이어 크기에 맞게 조정
    Matrix4 matScale, matWorld;
    D3DXMatrixScaling(&matScale, m_fRatioX, m_fRatioY, 1.0f);
    matWorld = matScale * m_matWorld;

    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

    m_pSprite->SetTransform(&matWorld);
    m_pSprite->Draw(m_textureBackground, &rc, &Vector3(0, 0, 0),
        &Vector3(0, 0, 0), m_colorBackground);

    m_pSprite->End();
    return S_OK;
}