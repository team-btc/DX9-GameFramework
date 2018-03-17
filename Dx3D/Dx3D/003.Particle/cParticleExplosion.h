#pragma once
#include "cParticle.h"

class cParticleExplosion : public cParticle
{
public:
    cParticleExplosion();
    cParticleExplosion(Vector3* origin, int numParticles);	// ������ �ʱ�ȭ 
    ~cParticleExplosion();

    virtual void ResetParticle(ST_PARTICLE_ATTR* attribute) override;
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void PostRender() override;
};
