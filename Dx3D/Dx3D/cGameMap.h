#pragma once

#include "iMap.h"

class cGameMap : public iMap
{
private:
    LPMESH              m_pMapMesh;
    vector<LPMESH>      m_vecObstacleMesh;

public:
    cGameMap();
    ~cGameMap();

    void SetCurrMapMesh(LPMESH mesh) { m_pMapMesh = mesh; }
    void SetCurrObstacleMesh(vector<LPMESH> vecMesh);

    virtual HRESULT GetHeight(OUT Vector3& vPos) override;                                    // 맵 높이 체크
    virtual bool CheckObstacle(IN cRay ray) override;                                         // 장애물 체크

    void RendObstacle();                                                                      // 장애물 출력
};

