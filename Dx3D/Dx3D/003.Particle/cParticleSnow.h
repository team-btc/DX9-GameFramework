#pragma once
#include "cParticle.h"

class cParticleSnow : public cParticle
{
private:

public:
    // ������
    cParticleSnow();

    // ��� ���� ����ü�� ����Ű�� �����Ϳ� �ý��� �� ��ƼŬ�� ��, ��� ���ڴ� �����̰� ������ ���Ǹ� �����ϸ�, 
    // ���� �����̰� �� ���� ������ ���� ���� ��� �Ҹ��ϰ� �ٽ� �������. �׻� ���� ���� ��ƼŬ�� ����. 
    cParticleSnow(cBoundingBox* stBoundingbox, int numParticles);
    virtual ~cParticleSnow();

    virtual void ResetParticle(ST_PARTICLE_ATTR* attribute) override;
    virtual void Update() override;

    virtual void PreRender() override;
    virtual void PostRender() override;
};


