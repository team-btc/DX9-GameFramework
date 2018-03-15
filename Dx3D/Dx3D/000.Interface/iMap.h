#pragma once
#include "stdafx.h"

interface iMap
{
    virtual HRESULT Load(IN char* szFilePath, IN Matrix4* pMat) PURE;                   // 맵 로드
    virtual HRESULT GetHeight(OUT bool& isWalkable, OUT Vector3& vPos) PURE;            // 맵 높이 및 진입가능 여부 체크
    virtual HRESULT GetGroundInfo(OUT E_GROUND_TYPE& eGroundType, IN Vector3 vPos);     // 맵의 지형 정보
    virtual HRESULT GetEventTrap(OUT E_EVENT_TRAP_TYPE& eTrapType, IN Vector3 vPos);    // 이벤트 트랩 정보
};