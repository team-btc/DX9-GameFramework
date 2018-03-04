#pragma once
#include "iUILayer.h"

class cUIObject;
class cUILayer : public iUILayer // ������ UIDraw ���� ��Ȱ
{
private:
    enum E_UI_BACKGROUND
    {
        E_UI_BACKGROUND_ON, E_UI_BACKGROUND_OFF
    };
private:
    LPD3DXSPRITE			m_pSprite;
    ST_SIZE					m_stLayerSize;
    RECT					m_rtLayer;					// UILayer Collison Area
    string					m_strName;					// UILayer Name (Identify UILayer other UILayers) 
    D3DXVECTOR3				m_vPosition;				// UILayer Position
    D3DXMATRIXA16			m_matWorld;					// UILayer World Matrix
    LPDIRECT3DTEXTURE9		m_textureBackground;		// UILayer Background Texture
    ST_SIZE					m_stBackgroundTextureSize;	// UILayer Layout Size (Widht, Height)
    D3DCOLOR				m_colorBackground;			// UILayer Backgorund Color
    float					m_fRatioX;
    float					m_fRatioY;
    cUILayer*				m_pParent;					// Parent UILayer Pointer 
    vector<cUILayer*>		m_vecUILayerChilds;			// Store Child UILayer Pointers
    cUIObject*				m_pUIobjectRoot;
    vector<cUIObject*>		m_vecUIObjects;				// Store UIObject Pointers
    E_UI_BACKGROUND			m_eUseBackground;			// UILayer Background ON / OFF toggle
    bool					m_isVertexBackground;
    bool					m_isMove;					// ������ ���󰡴� �Ŷ�� �ʿ� 
    bool					m_isActive;					// Ȱ��(true) / ��Ȱ��(false)
    bool					m_isSelect;					// ����
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
    virtual HRESULT SetPosition(IN D3DXVECTOR3 vPosition) override;
    virtual HRESULT GetPosition(OUT D3DXVECTOR3& vPosition) override;
    virtual HRESULT SetSize(IN float fWidth, IN float fHeight) override;
    virtual HRESULT GetLayerRect(OUT RECT& rtLayer) override;
    virtual HRESULT SetBackGroundColor(IN D3DCOLOR colorARGB) override;
    virtual HRESULT GetActive(OUT bool& isActiveState) override;
    virtual HRESULT SetActive(IN bool isActiveState) override;
    virtual HRESULT GetWorldMatrix(OUT D3DXMATRIXA16 matWorld) override;
    virtual HRESULT SetWorldMatrix(IN D3DXMATRIXA16 matWorld) override;
    virtual HRESULT SetLayer(IN string strLayerName, IN D3DXVECTOR3 vPosition, IN ST_SIZE stLayerSize,
        IN bool useBackground = false, IN D3DCOLOR colerBackGround = D3DCOLOR_ARGB(0, 0, 0, 0), IN string strTextureName = "" /*�� ��Ʈ��*/) override;
};