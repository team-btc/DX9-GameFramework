#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cGroup : public cObject
{
private:
    SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecPNTVertex, PNTVertex);
    SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
    cGroup();
    ~cGroup();

    void Render();
};

