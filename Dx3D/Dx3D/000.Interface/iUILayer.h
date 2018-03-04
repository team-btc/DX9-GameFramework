#pragma once
#include "stdafx.h"

interface iUILayer
{
    virtual HRESULT SetPosition(IN D3DXVECTOR3 vPosition) PURE;         // UI 레이어의 위치 설정
    virtual HRESULT GetPosition(OUT D3DXVECTOR3& vPostion) PURE;        // UI 레이어의 포지션 설정

    virtual HRESULT SetSize(IN float fWidth, IN float fHeight) PURE;    // UI 레이어의 크기 설정
    virtual HRESULT GetLayerRect(OUT RECT& rtLayer) PURE;               // UI 레이어 렉트 크기 가져오기
    virtual HRESULT SetBackGroundColor(IN D3DCOLOR colorARGB) PURE;     // UI 레이어의 백그라운드 컬러

    virtual HRESULT GetActive(OUT bool& isActiveState) PURE;            // UI 레이어 활성화 여부 
    virtual HRESULT SetActive(IN bool isActiveState) PURE;              // UI 레이어 활성화 세팅 

    virtual HRESULT GetWorldMatrix(OUT D3DXMATRIXA16 matWorld) PURE;
    virtual HRESULT SetWorldMatrix(IN D3DXMATRIXA16 matWorld) PURE;

    virtual HRESULT SetLayer(IN string strLayerName, IN D3DXVECTOR3 vPosition, IN ST_SIZE stLayerSize,
        IN bool useBackground, IN D3DCOLOR colerBackGround, IN string strTextureName) PURE;

};