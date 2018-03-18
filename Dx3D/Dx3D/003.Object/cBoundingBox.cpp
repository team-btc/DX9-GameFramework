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

bool cBoundingBox::IsPointInside(Vector3 vPos)
{
    if (vPos.x >= m_vMin.x &&
        vPos.y >= m_vMin.y &&
        vPos.z >= m_vMin.z &&
        vPos.x <= m_vMax.x &&
        vPos.y <= m_vMax.y &&
        vPos.z <= m_vMax.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}
