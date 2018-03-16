#pragma once
#include "cParticle.h"

class cParticleExplosion : public cParticle
{
public:
    cParticleExplosion();
    cParticleExplosion(Vector3* origin, int numParticles);	// 생성자 초기화 
    ~cParticleExplosion();

    virtual void ResetParticle(ST_PARTICLE_ATTR* attribute) override;
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void PostRender() override;
};
