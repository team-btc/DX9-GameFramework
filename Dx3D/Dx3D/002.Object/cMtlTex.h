#pragma once
#include "cObject.h"

class cMtlTex : public cObject
{
private:
    SYNTHESIZE_PASS_BY_PT(MATERIAL9, m_stMtl, Mtl);
    SYNTHESIZE_ADD_REF(LPTEXTURE9, m_pTexture, Texture);
    SYNTHESIZE(DWORD, m_nMtlRef, MtlRef);

public:
    cMtlTex();
    virtual ~cMtlTex();
};

