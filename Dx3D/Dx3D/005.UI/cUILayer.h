#pragma once
#include "iUILayer.h"
#include "cUIButton.h"

class cUIObject;

class cUILayer : public iUILayer, public iUIButton // ������ UIDraw ���� ��Ȱ
{
private:
    enum E_UI_BACKGROUND
    {
        E_UI_BACKGROUND_ON = 0, E_UI_BACKGROUND_OFF
    };
private:
    LPD3DXSPRITE        m_pSprite;
    ST_SIZE             m_stLayerSize;
    RECT                m_rtLayer;                  // UILayer Collison Area
    string              m_strName;                  // UILayer Name (Identify UILayer other UILayers) 
    Vector3             m_vPosition;                // UILayer Position
    Matrix4             m_matWorld;                 // UILayer World Matrix
    LPTEXTURE9          m_textureBackground;        // UILayer Background Texture
    ST_SIZE             m_stBackgroundTextureSize;  // UILayer Layout Size (Widht, Height)
    Color               m_colorBackground;          // UILayer Backgorund Color
    float               m_fRatioX;
    float               m_fRatioY;
    cUILayer*           m_pParent;                  // Parent UILayer Pointer 
    vector<cUILayer*>   m_vecUILayerChilds;         // Store Child UILayer Pointers
    cUIObject*          m_pUIobjectRoot;
    vector<cUIObject*>  m_vecUIObjects;             // Store UIObject Pointers
    E_UI_BACKGROUND     m_eUseBackground;           // UILayer Background ON / OFF toggle
    bool                m_isVertexBackground;
    bool                m_isMove;                   // ������ ���󰡴� �Ŷ�� �ʿ� 
    bool                m_isActive;                 // Ȱ��(true) / ��Ȱ��(false)
    bool                m_isSelect;                 // ����

    vector<cUIButton*>  m_vecClickButton;           // Ŭ���� ��ư ��Ƴ���
private:
    // cUILayer private function
    HRESULT RenderGuideLine();
    HRESULT RenderVertexBackground();
    HRESULT RenderTextureBackground();
    HRESULT UpdateWorldMatrix();
    HRESULT UpdateChildren();
    HRESULT UpdateRect();

public:
    // cUILayer public function
    cUILayer();
    virtual ~cUILayer();

    virtual HRESULT Setup();
    virtual HRESULT Update();
    virtual HRESULT Render();

    HRESULT AddUILayerChild(IN cUILayer* pChild);
    HRESULT FindUILayerChild(OUT cUILayer** pChild, IN string strChildName);
    HRESULT AddUIObject(IN cUIObject* pObject);
    HRESULT FindUIObject(OUT cUIObject** pObject, IN string strObjectName);
    
    // Derived iUILayer interface
    virtual HRESULT SetPosition(IN Vector3 vPosition) override;
    virtual HRESULT GetPosition(OUT Vector3& vPosition) override;
    virtual HRESULT SetSize(IN float fWidth, IN float fHeight) override;
    virtual HRESULT GetLayerRect(OUT RECT& rtLayer) override;
    virtual HRESULT SetBackGroundColor(IN Color colorARGB) override;
    virtual HRESULT GetActive(OUT bool& isActiveState) override;
    virtual HRESULT SetActive(IN bool isActiveState) override;
    virtual HRESULT GetWorldMatrix(OUT Matrix4 matWorld) override;
    virtual HRESULT SetWorldMatrix(IN Matrix4 matWorld) override;
    virtual HRESULT SetLayer(IN string strLayerName, IN Vector3 vPosition, IN ST_SIZE stLayerSize,
        IN bool useBackground = false, IN Color colorBackGround = D3DCOLOR_ARGB(0, 0, 0, 0), IN string strTextureName = "" /*�� ��Ʈ��*/) override;

    virtual void OnClick(cUIButton* pSender) override;
    virtual void OnDrag(cUIButton* pSender) override;
    virtual void OnRelease(cUIButton* pSender) override;

    bool IsClickButton() { return !m_vecClickButton.empty(); }
    string GetClickButtonName();
};