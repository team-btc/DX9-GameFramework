#include "stdafx.h"
#include "cParticleExplosion.h"

cParticleExplosion::cParticleExplosion()
{}

cParticleExplosion::~cParticleExplosion()
{}

cParticleExplosion::cParticleExplosion(Vector3* origin, int numParticles)
{
    m_vOriginPos = *origin;
    m_fSize = 0.9f;
    m_vbBufferSize = 2048;
    m_vbOffset = 0;
    m_vbBatchSize = 512;

    for (int i = 0; i < numParticles; i++)
    {
        AddParticle();
    }
}

// �ý��� ��õ�� ��ƼŬ�� �ʱ�ȭ�ϰ� ��ü ������ ������ �ӵ��� �����,
// �ý��� ���� ��ƼŬ���� ������ �÷��� �ο�.
// �� ��ƼŬ���� 2�� ���� �����ϵ��� ������ ����. 
void cParticleExplosion::ResetParticle(ST_PARTICLE_ATTR* attribute)
{
    attribute->isAlive = true;
    attribute->vPos = m_vOriginPos;

    Vector3 min = Vector3(-1.0f, -1.0f, -1.0f);
    Vector3 max = Vector3(1.0f, 1.0f, 1.0f);

    // ������ ������ ������ ���͸� ����
    attribute->vSpeed = GetRandomVector3(min, max);

    D3DXVec3Normalize(
        &attribute->vSpeed,
        &attribute->vSpeed);
    attribute->vSpeed *= 100.0f;
    attribute->color = XColor(
        GetRandomFloat(0.0f, 1.0f),
        GetRandomFloat(0.0f, 1.0f),
        GetRandomFloat(0.0f, 1.0f),
        1.0f);
    attribute->age = 0.0f;
    attribute->life = 2.0f;
    // 2�� ������ ������ ������. 
}

// �� ��ƼŬ�� ��ġ�� �����ϰ� ������ �ʰ�����ƼŬ�� ������ ó��.
// �� �ý����� ���� ��ƼŬ�� �������� �ʴ´�.
// �̰��� ���ο� �Ҳ��� ���� �� ������ ���� Firework �ý����� ��Ȱ���� �� �ֱ� ����.
// ��, ��ƼŬ�� ����� �����ϴ� ���ŷο� ������ �ּ�ȭ. 
void cParticleExplosion::Update()
{
    float timeDelta = g_pTimerManager->GetDeltaTime();
    list<ST_PARTICLE_ATTR>::iterator i;
    for (i = m_particleList.begin(); i != m_particleList.end(); i++)
    {
        // ������ ��ƼŬ�� ����. 
        if (i->isAlive)
        {
            i->vPos += i->vSpeed * timeDelta;
            i->age += timeDelta;
            if (i->age > i->life)
            {
                // ���δ�.
                i->isAlive = false;
            }
        }
    }
}

// �������� �ռ� �����ؾ� �� �ʱ� ���� ���¸� ����.
// �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �Լ��� ����. 
// �ٸ� ���� �μ��� �̿�.
// ���� ���۷��� ���⵵ ������ ����.
// ����Ʈ ���� �μ��� ���� ���θ� �����ϰ��� �Ѵٸ�
// pSystem::PreRender�� pSystem::PostRender �޼��带 �������̵��ϸ� ��. 
void cParticleExplosion::PreRender()
{
    cParticle::PreRender();
    // �θ� ������ �޼��带 ȣ��. 
    g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    // z ���� �б�� ���� ������ ����� ������� �ʴ´�. 
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void cParticleExplosion::PostRender()
{
    cParticle::PostRender();
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
