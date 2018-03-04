#include "stdafx.h"
#include "cParticleExplosion.h"

cParticleExplosion::cParticleExplosion()
{}

cParticleExplosion::~cParticleExplosion()
{}

cParticleExplosion::cParticleExplosion(D3DXVECTOR3 * origin, int numParticles)
{
    m_vOriginPos = *origin;
    _size = 0.9f;
    m_dvertexBufferSize = 2048;
    _vbOffset = 0;
    _vbBatchSize = 512;

    for (int i = 0; i < numParticles; i++)
        addParticle();
}

// �ý��� ��õ�� ��ƼŬ�� �ʱ�ȭ�ϰ� ��ü ������ ������ �ӵ��� �����,
// �ý��� ���� ��ƼŬ���� ������ �÷��� �ο�.
// �� ��ƼŬ���� 2�� ���� �����ϵ��� ������ ����. 
void cParticleExplosion::resetParticle(Attribute * attribute)
{
    attribute->isAlive = true;
    attribute->position = m_vOriginPos;

    D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
    D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    // ������ ������ ������ ���͸� ����
    attribute->velocity = GetRandomVector3(min, max);

    D3DXVec3Normalize(
        &attribute->velocity,
        &attribute->velocity);
    attribute->velocity *= 100.0f;
    attribute->color = D3DXCOLOR(
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
void cParticleExplosion::update(float timeDelta)
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // ������ ��ƼŬ�� ����. 
        if (i->isAlive)
        {
            i->position += i->velocity * timeDelta;
            i->age += timeDelta;
            if (i->age >i->lifeTime)
                // ���δ�. 
                i->isAlive = false;
        }
    }
}

// �������� �ռ� �����ؾ� �� �ʱ� ���� ���¸� ����.
// �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �Լ��� ����. 
// �ٸ� ���� �μ��� �̿�.
// ���� ���۷��� ���⵵ ������ ����.
// ����Ʈ ���� �μ��� ���� ���θ� �����ϰ��� �Ѵٸ�
// pSystem::preRender�� pSystem::postRender �޼��带 �������̵��ϸ� ��. 
void cParticleExplosion::preRender()
{
    cParticle::preRender();
    // �θ� ������ �޼��带 ȣ��. 
    g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    // z ���� �б�� ���� ������ ����� ������� �ʴ´�. 
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void cParticleExplosion::postRender()
{
    cParticle::postRender();
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
