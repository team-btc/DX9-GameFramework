#pragma once
#include "stdafx.h"

interface iUILayer
{
    virtual HRESULT SetPosition(IN Vector3 vPosition) PURE;         // UI 레이어의 위치 설정
    virtual HRESULT GetPosition(OUT Vector3& vPostion) PURE;        // UI 레이어의 포지션 설정

    virtual HRESULT SetSize(IN float fWidth, IN float fHeight) PURE;    // UI 레이어의 크기 설정
    virtual HRESULT GetLayerRect(OUT RECT& rtLayer) PURE;               // UI 레이어 렉트 크기 가져오기
    virtual HRESULT SetBackGroundColor(IN Color colorARGB) PURE;     // UI 레이어의 백그라운드 컬러

    virtual HRESULT GetActive(OUT bool& isActiveState) PURE;            // UI 레이어 활성화 여부 
    virtual HRESULT SetActive(IN bool isActiveState) PURE;              // UI 레이어 활성화 세팅 

    virtual HRESULT GetWorldMatrix(OUT Matrix4 matWorld) PURE;
    virtual HRESULT SetWorldMatrix(IN Matrix4 matWorld) PURE;

    virtual HRESULT SetLayer(IN string strLayerName, IN Vector3 vPosition, IN ST_SIZE stLayerSize,
        IN bool useBackground, IN Color colerBackGround, IN string strTextureName) PURE;

};