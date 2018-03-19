#pragma once

#include "iMap.h"

class cGameMap : public iMap
{
private:
    ST_MAP_INFO*        m_stCurrMapInfo;

public:
    cGameMap();
    ~cGameMap();

    void SetCurrMapInfo(ST_MAP_INFO* mpInfo) { m_stCurrMapInfo = mpInfo; }

    virtual void GetHeight(OUT Vector3& vPos) override;                                         // 맵 높이 체크
    virtual bool CheckObstacle(OUT float& fDist, IN cRay ray) override;                         // 장애물 체크
    virtual bool CheckEvent(OUT string& szEventName, IN Vector3 vPos) override;                 // 이벤트 트랩 체크

    virtual void RendObstacle() override;                                                       // 장애물 출력
    virtual void RendEventTrap() override;                                                      // 이벤트 트랩 출력
};

