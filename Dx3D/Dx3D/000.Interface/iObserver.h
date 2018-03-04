#pragma once
#include "stdafx.h"

class Entity;
interface iObserver
{
public:
    virtual ~iObserver();
    virtual void OnNotify(const Entity& object, string szEvent) PURE;
};