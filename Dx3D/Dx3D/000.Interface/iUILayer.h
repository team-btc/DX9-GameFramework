#pragma once
#include "stdafx.h"

interface iUILayer
{
    virtual HRESULT SetPosition(IN Vector3 vPosition) PURE;         // UI ���̾��� ��ġ ����
    virtual HRESULT GetPosition(OUT Vector3& vPostion) PURE;        // UI ���̾��� ������ ����

    virtual HRESULT SetSize(IN float fWidth, IN float fHeight) PURE;    // UI ���̾��� ũ�� ����
    virtual HRESULT GetLayerRect(OUT RECT& rtLayer) PURE;               // UI ���̾� ��Ʈ ũ�� ��������
    virtual HRESULT SetBackGroundColor(IN Color colorARGB) PURE;     // UI ���̾��� ��׶��� �÷�

    virtual HRESULT GetActive(OUT bool& isActiveState) PURE;            // UI ���̾� Ȱ��ȭ ���� 
    virtual HRESULT SetActive(IN bool isActiveState) PURE;              // UI ���̾� Ȱ��ȭ ���� 

    virtual HRESULT GetWorldMatrix(OUT Matrix4 matWorld) PURE;
    virtual HRESULT SetWorldMatrix(IN Matrix4 matWorld) PURE;

    virtual HRESULT SetLayer(IN string strLayerName, IN Vector3 vPosition, IN ST_SIZE stLayerSize,
        IN bool useBackground, IN Color colorBackGround, IN string strTextureName) PURE;

};