#pragma once

class cProjectile// : public cObject
{
public:
    enum E_SHAPE
    {
        SPHERE = 0, CUBE
    };

private:
    LPMESH  m_pMesh;
    SYNTHESIZE(Vector3, m_scale, Scale);
    SYNTHESIZE(Vector3, m_rotation, Rotation);
    SYNTHESIZE(Vector3, m_position, Position);
    SYNTHESIZE(Vector3, m_speed, Speed);
    SYNTHESIZE(ST_BOUNDING_SPHERE, m_stBoundingSphere, BoundingSphere);
    Matrix4     m_matWorld;
    float       m_fLifetime;
    bool        m_isAlive;

public:
    cProjectile();
    ~cProjectile();

    void Setup(E_SHAPE eShape, Vector3 size, Vector3 rotation, Vector3 position);
    void Update();
    void Render();
    bool IsCollision(ST_BOUNDING_SPHERE bs);
    bool IsAlive() { return m_isAlive; }
    void Destroy() { m_isAlive = false; }
    void SetLifetime(float t) { m_fLifetime = t; }
};

