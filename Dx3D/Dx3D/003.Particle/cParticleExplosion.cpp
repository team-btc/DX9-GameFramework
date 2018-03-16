#include "stdafx.h"
#include "cParticleExplosion.h"

cParticleExplosion::cParticleExplosion()
{}

cParticleExplosion::~cParticleExplosion()
{}

cParticleExplosion::cParticleExplosion(Vector3* origin, int numParticles)
{
    m_vOriginPos = *origin;
    _size = 0.9f;
    m_dvertexBufferSize = 2048;
    _vbOffset = 0;
    _vbBatchSize = 512;

    for (int i = 0; i < numParticles; i++)
    {
        AddParticle();
    }
}

// �ý��� ��õ�� ��ƼŬ�� �ʱ�ȭ�ϰ� ��ü ������ ������ �ӵ��� �����,
// �ý��� ���� ��ƼŬ���� ������ �÷��� �ο�.
// �� ��ƼŬ���� 2�� ���� �����ϵ��� ������ ����. 
void cParticleExplosion::ResetParticle(Attribute* attribute)
{
    attribute->isAlive = true;
    attribute->position = m_vOriginPos;

    Vector3 min = Vector3(-1.0f, -1.0f, -1.0f);
    Vector3 max = Vector3(1.0f, 1.0f, 1.0f);

    // ������ ������ ������ ���͸� ����
    attribute->velocity = GetRandomVector3(min, max);

    D3DXVec3Normalize(
        &attribute->velocity,
        &attribute->velocity);
    attribute->velocity *= 100.0f;
    attribute->color = XColor(
        GetRandomFloat(0.0f, 1.0f),
        GetRandomFloat(0.0f, 1.0f),
        GetRandomFloat(0.0f, 1.0f),
        1.0f);
    attribute->age = 0.0f;
    attribute->lifeTime = 2.0f;
    // 2�� ������ ������ ������. 
}

// �� ��ƼŬ�� ��ġ�� �����ϰ� ������ �ʰ�����ƼŬ�� ������ ó��.
// �� �ý����� ���� ��ƼŬ�� �������� �ʴ´�.
// �̰��� ���ο� �Ҳ��� ���� �� ������ ���� Firework �ý����� ��Ȱ���� �� �ֱ� ����.
// ��, ��ƼŬ�� ����� �����ϴ� ���ŷο� ������ �ּ�ȭ. 
void cParticleExplosion::Update()
{
    float timeDelta = g_pTimerManager->GetDeltaTime();
    list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // ������ ��ƼŬ�� ����. 
        if (i->isAlive)
        {
            i->position += i->velocity * timeDelta;
            i->age += timeDelta;
            if (i->age > i->lifeTime)
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
