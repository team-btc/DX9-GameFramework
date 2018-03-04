#pragma once
#include "cParticle.h"
using namespace psystem;

class cParticleExplosion : public cParticle
{
public:
    cParticleExplosion();
    cParticleExplosion(D3DXVECTOR3* origin, int numParticles);	// 생성자 초기화 
    ~cParticleExplosion();

    virtual void resetParticle(Attribute* attribute) override;
    virtual void update(float timeDelta) override;
    void preRender();
    void postRender();
};
