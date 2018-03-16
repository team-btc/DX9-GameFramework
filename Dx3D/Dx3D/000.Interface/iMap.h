#pragma once
#include "stdafx.h"

interface iMap
{
    //LPMESH m_pMesh;
    //virtual HRESULT SetMesh(IN LPMESH mesh) PURE;                                         // 맵 매쉬 셋팅
    //virtual HRESULT GetGroundInfo(OUT E_GROUND_TYPE& eGroundType, IN Vector3 vPos);       // 맵의 지형 정보
    //virtual HRESULT GetEventTrap(OUT E_EVENT_TRAP_TYPE& eTrapType, IN Vector3 vPos);      // 이벤트 트랩 정보
                                                                                            
    virtual void GetHeight(OUT Vector3& vPos) PURE;                                         // 맵 높이 체크
    virtual bool CheckObstacle(OUT float& fDist, IN cRay ray) PURE;                         // 장애물 체크
    virtual bool CheckEvent(OUT string& szEventName, IN Vector3 vPos) PURE;                 // 이벤트 트랩 체크
    virtual void RendObstacle() PURE;                                                       // 장애물 출력
    virtual void RendEventTrap() PURE;                                                      // 이벤트트랩 출력
};