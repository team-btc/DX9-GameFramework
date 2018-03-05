#pragma once
#include "cObject.h"
class cFrustum : public cObject
{
private:
    vector<Vector3>     m_vecOrigProjVertex;
    vector<Plane>       m_vecPlane;

public:
    cFrustum();
    ~cFrustum();

    HRESULT Setup();
    HRESULT Update();
    HRESULT IsInFrustum(OUT bool& result, ST_SPHERE* pSphere);
};

