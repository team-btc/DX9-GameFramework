#include "stdafx.h"
#include "cBoundingBox.h"


cBoundingBox::cBoundingBox()
    : m_vMin(0, 0, 0)
    , m_vMax(1, 1, 1)
{
}

cBoundingBox::cBoundingBox(Vector3 vMin, Vector3 vMax)
{
    m_vMin = vMin;
    m_vMax = vMax;
}


cBoundingBox::~cBoundingBox()
{
}

bool cBoundingBox::IsPointInside(Vector3 p)
{
    if (p.x >= m_vMin.x &&
        p.y >= m_vMin.y &&
        p.z >= m_vMin.z &&
        p.x <= m_vMax.x &&
        p.y <= m_vMax.y &&
        p.z <= m_vMax.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}
