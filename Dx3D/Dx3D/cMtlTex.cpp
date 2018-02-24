#include "stdafx.h"
#include "cMtlTex.h"


cMtlTex::cMtlTex()
    : m_pTexture(NULL)
{
    ZeroMemory(&m_stMtl, sizeof(MATERIAL9));
}


cMtlTex::~cMtlTex()
{
    SAFE_RELEASE(m_pTexture);
}
