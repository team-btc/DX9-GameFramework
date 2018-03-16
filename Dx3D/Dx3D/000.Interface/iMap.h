#pragma once
#include "stdafx.h"

interface iMap
{
    //LPMESH m_pMesh;
    //virtual HRESULT SetMesh(IN LPMESH mesh) PURE;                                       // 맵 매쉬 셋팅
    virtual HRESULT GetHeight(OUT Vector3& vPos) PURE;                                    // 맵 높이 체크
    virtual bool CheckObstacle(IN cRay ray) PURE;                                         // 장애물 체크
    //virtual HRESULT GetGroundInfo(OUT E_GROUND_TYPE& eGroundType, IN Vector3 vPos);     // 맵의 지형 정보
    //virtual HRESULT GetEventTrap(OUT E_EVENT_TRAP_TYPE& eTrapType, IN Vector3 vPos);    // 이벤트 트랩 정보
};