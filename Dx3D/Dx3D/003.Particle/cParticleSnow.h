#pragma once
#include "cParticle.h"

class cParticleSnow : public cParticle
{
private:

public:
    // 생성자
    cParticleSnow();

    // 경계 상자 구조체를 가리키는 포인터와 시스템 내 파티클의 수, 경계 상자는 눈송이가 떨어질 부피를 정의하며, 
    // 만약 눈송이가 이 범위 밖으로 벗어 나면 즉시 소멸하고 다시 만들어짐. 항상 같은 수의 파티클을 유지. 
    cParticleSnow(cBoundingBox* stBoundingbox, int numParticles);
    virtual ~cParticleSnow();

    virtual void ResetParticle(ST_PARTICLE_ATTR* attribute) override;
    virtual void Update() override;

    virtual void PreRender() override;
    virtual void PostRender() override;
};


