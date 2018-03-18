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

    virtual void GetHeight(OUT Vector3& vPos) override;                                         // �� ���� üũ
    virtual bool CheckObstacle(OUT float& fDist, IN cRay ray) override;                         // ��ֹ� üũ
    virtual bool CheckEvent(OUT string& szEventName, IN Vector3 vPos) override;                 // �̺�Ʈ Ʈ�� üũ

    virtual void RendObstacle() override;                                                       // ��ֹ� ���
    virtual void RendEventTrap() override;                                                      // �̺�Ʈ Ʈ�� ���
};

