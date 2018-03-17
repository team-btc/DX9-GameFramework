#pragma once
class cBoundingBox
{
private:
    Vector3     m_vMin;
    Vector3     m_vMax;

public:
    cBoundingBox();
    cBoundingBox(Vector3 vMin, Vector3 vMax);
    ~cBoundingBox();

    Vector3 GetMin() { return m_vMin; }
    Vector3 GetMax() { return m_vMax; }

    void SetMin(Vector3 vMin) { m_vMin = vMin; }
    void SetMax(Vector3 vMax) { m_vMax = vMax; }
    bool IsPointInside(Vector3 p);
};

