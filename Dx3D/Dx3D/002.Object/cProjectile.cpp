#include "stdafx.h"
#include "cProjectile.h"


cProjectile::cProjectile()
    : m_scale(1, 1, 1)
    , m_rotation(0, 0, 0)
    , m_position(0, 0, 0)
    , m_speed(0, 0, 0)
    , m_pMesh(NULL)
    , m_isAlive(true)
{
    D3DXMatrixIdentity(&m_matWorld);
}


cProjectile::~cProjectile()
{
    m_pMesh->Release();
}

void cProjectile::Setup(E_SHAPE eShape, Vector3 size, Vector3 rotation, Vector3 position)
{
    switch (eShape)
    {
        case cProjectile::SPHERE:
        {
            D3DXCreateSphere(g_pDevice, size.x, 20, 20, &m_pMesh, NULL);
            break;
        }
        case cProjectile::CUBE:
        {
            D3DXCreateBox(g_pDevice, size.x, size.y, size.z, &m_pMesh, NULL);
            break;
        }
    }
    m_rotation = rotation;
    m_position = position;
}

void cProjectile::Update()
{
    if (m_fLifetime < g_pTimerManager->GetWorldTime())
    {
        m_isAlive = false;
        m_pMesh->Release();
    }

    if (m_isAlive)
    {
        m_stBoundingSphere.p = m_position;
        Matrix4 matS, matR, matT;
        D3DXMatrixScaling(&matS, m_scale.x, m_scale.y, m_scale.z);
        D3DXMatrixRotationYawPitchRoll(&matR,
                                       D3DXToRadian(m_rotation.y),
                                       D3DXToRadian(m_rotation.x),
                                       D3DXToRadian(m_rotation.z));

        m_position += m_speed;
        D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
        m_matWorld = matS * matR * matT;
    }
}

void cProjectile::Render()
{
    if (m_pMesh)
    {
        g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
        g_pDevice->SetMaterial(&WHITE_MTRL);
        m_pMesh->DrawSubset(0);
    }
}

bool cProjectile::IsCollision(ST_BOUNDING_SPHERE bs)
{
    Vector3 v = m_stBoundingSphere.p - bs.p;
    float dist = D3DXVec3Length(&v);
    if (dist <= (bs.d + m_stBoundingSphere.d))
    {
        //  COLLISION
        return true;
    }
    else
    {
        return false;
    }
}
