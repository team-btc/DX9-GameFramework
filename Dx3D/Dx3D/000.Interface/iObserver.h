#pragma once
#include "stdafx.h"

interface iObserver
{
public:
    template<typename T>
    virtual void OnNotify(string szName, T _t) PURE;
};