#pragma once
#include "cParticle.h"
using namespace psystem;

class cParticleExplosion : public cParticle
{
public:
    cParticleExplosion();
    cParticleExplosion(Vector3* origin, int numParticles);	// 생성자 초기화 
    ~cParticleExplosion();

    virtual void ResetParticle(Attribute* attribute) override;
    virtual void Update() override;
    void PreRender();
    void PostRender();
};
