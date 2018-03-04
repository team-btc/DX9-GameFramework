#pragma once
#include "stdafx.h"

interface iUILayer
{
    virtual HRESULT SetPosition(IN D3DXVECTOR3 vPosition) PURE;         // UI ���̾��� ��ġ ����
    virtual HRESULT GetPosition(OUT D3DXVECTOR3& vPostion) PURE;        // UI ���̾��� ������ ����

    virtual HRESULT SetSize(IN float fWidth, IN float fHeight) PURE;    // UI ���̾��� ũ�� ����
    virtual HRESULT GetLayerRect(OUT RECT& rtLayer) PURE;               // UI ���̾� ��Ʈ ũ�� ��������
    virtual HRESULT SetBackGroundColor(IN D3DCOLOR colorARGB) PURE;     // UI ���̾��� ��׶��� �÷�

    virtual HRESULT GetActive(OUT bool& isActiveState) PURE;            // UI ���̾� Ȱ��ȭ ���� 
    virtual HRESULT SetActive(IN bool isActiveState) PURE;              // UI ���̾� Ȱ��ȭ ���� 

    virtual HRESULT GetWorldMatrix(OUT D3DXMATRIXA16 matWorld) PURE;
    virtual HRESULT SetWorldMatrix(IN D3DXMATRIXA16 matWorld) PURE;

    virtual HRESULT SetLayer(IN string strLayerName, IN D3DXVECTOR3 vPosition, IN ST_SIZE stLayerSize,
        IN bool useBackground, IN D3DCOLOR colerBackGround, IN string strTextureName) PURE;

};