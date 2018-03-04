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

// 시스템 원천의 파티클을 초기화하고 구체 내에서 임의의 속도를 만들며,
// 시스템 내의 파티클들은 임의의 컬러를 부여.
// 각 파티클들이 2초 동안 유지하도록 수명을 지정. 
void cParticleExplosion::resetParticle(Attribute * attribute)
{
    attribute->isAlive = true;
    attribute->position = m_vOriginPos;

    D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
    D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    // 지정된 범위의 랜덤한 벡터를 저장
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
    // 2초 동안의 수명을 가진다. 
}

// 각 파티클의 위치를 갱신하고 수명을 초과한파티클의 죽음을 처리.
// 이 시스템은 죽은 파티클을 제거하지 않는다.
// 이것은 새로운 불꽃을 만들 때 기존의 죽은 Firework 시스템을 재활용할 수 있기 때문.
// 즉, 파티클을 만들고 제거하는 번거로운 과정을 최소화. 
void cParticleExplosion::update(float timeDelta)
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // 생존한 파티클만 갱신. 
        if (i->isAlive)
        {
            i->position += i->velocity * timeDelta;
            i->age += timeDelta;
            if (i->age >i->lifeTime)
                // 죽인다. 
                i->isAlive = false;
        }
    }
}

// 렌더링에 앞서 지정해야 할 초기 렌더 상태를 지정.
// 이 메서드는 시스템에 따라 달라질 수 있으므로 가상 함수로 선언. 
// 다른 블렌드 인수가 이용.
// 깊이 버퍼로의 쓰기도 허용되지 않음.
// 디폴트 블렌드 인수와 쓰기 여부를 변경하고자 한다면
// pSystem::preRender와 pSystem::postRender 메서드를 오버라이드하면 됨. 
void cParticleExplosion::preRender()
{
    cParticle::preRender();
    // 부모 버전의 메서드를 호출. 
    g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    // z 버퍼 읽기는 가능 하지만 쓰기는 허용하지 않는다. 
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void cParticleExplosion::postRender()
{
    cParticle::postRender();
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
