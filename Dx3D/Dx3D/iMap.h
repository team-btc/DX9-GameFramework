#pragma once
#include "stdafx.h"

interface iMap
{
    virtual HRESULT Load(IN char* szFilePath, IN Matrix4* pMat) PURE;
    virtual HRESULT GetHeight(OUT float& h, IN const Vector3 vPos) PURE;
};