#pragma once
#include "stdafx.h"

interface iSingletonManager
{
    virtual HRESULT Setup() PURE;
    virtual HRESULT Update() PURE;
    virtual HRESULT Render() PURE;
    virtual HRESULT Destroy() PURE;
};