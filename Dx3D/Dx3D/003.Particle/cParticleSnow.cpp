#include "stdafx.h"
#include "cParticleSnow.h"


cParticleSnow::cParticleSnow()
{}

cParticleSnow::~cParticleSnow()
{}

cParticleSnow::cParticleSnow(cBoundingBox* stBoundingbox, int numParticles)
{
    m_pBoundingBox = stBoundingbox;     // ������ ����(���� �� ������ ����� �ٽ� ���õ�)
    m_fSize = 5.0f;                     // �� ũ��
    m_vbBufferSize = 2048;         // ���ؽ� ������ ũ�� 
    m_vbOffset = 0;          // ���� ������ 
    m_vbBatchSize = 512;		            // ��ġ ������

    for (int i = 0; i < numParticles; i++) // ��ƼŬ�� ����ŭ ����Ʈ�� ��ƼŬ�� �߰�
    {
        AddParticle();
    }
}

// ��� ���� �� ������ x�� z��ǥ ��ġ���� �����̸� �����, y��ǥ�� ��� ������ �ֻ�ܰ� ������ ����, �̾� �����̿� �ӵ��� �ο��Ͽ� �Ʒ� �������� �׸��� �ణ �������� ���������� �Ѵ�. �����̴� ������� ǥ��. 
void cParticleSnow::ResetParticle(ST_PARTICLE_ATTR* attribute)
{
    attribute->isAlive = true;
    // �ּ��� min�� �ִ��� max�� ���ǵ� ���� ���� ���� ���͸� ���, �������� ��ġ ������ ���� ������ x, y z ��ǥ�� ��´�. 
    attribute->vPos = GetRandomVector3(m_pBoundingBox->GetMin(), m_pBoundingBox->GetMax());
    // ���� (y��ǥ) �� �׻� ��� ������ �ֻ��
    attribute->vPos.y = m_pBoundingBox->GetMax().y;

    // ���� �������� �ӵ�, �Ʒ������� �������� �ణ ������ ���Ѵ�. 
    attribute->vSpeed.x = GetRandomFloat(0.0f, 1.0f) * -5.0f;
    attribute->vSpeed.y = GetRandomFloat(0.0f, 1.0f) * -12.0f;
    attribute->vSpeed.z = 0.0f;
    attribute->life = 2;            //���� �Ҹ�Ǳ� ������ �ð�

    attribute->color = D3DCOLOR_ARGB(150, 255, 255, 255); // �� ���� 
}

// ��ƼŬ�� ��ġ�� �����ϸ�, �ý����� ��� ���ڸ� ������� Ȯ��. ���� ��� ���ڸ� ����ٸ� �ش� ��ƼŬ�� �ʱ�ȭ. 
void cParticleSnow::Update()
{
    float fTimeDelta = g_pTimerManager->GetDeltaTime();
    list<ST_PARTICLE_ATTR>::iterator iter;
    for (iter = m_particleList.begin(); iter != m_particleList.end(); iter++)
    {
        // �ð��� �ӵ��� ������ ���� �̵��Ѵ� 
        iter->vPos += iter->vSpeed * fTimeDelta;

        // ��ƼŬ�� ��ġ�� �����ڸ� ������� Ȯ�� 
        if (m_pBoundingBox->IsPointInside(iter->vPos) == false)
        {
            // ��踦 ��� ��ƼŬ�� �ʱ�ȭ �Ͽ� ��Ȱ����
            ResetParticle(&(*iter));
        }
    }
}

void cParticleSnow::PreRender()
{
    cParticle::PreRender();
    // z ���� �б�� ���� ������ ����� ������� �ʴ´�. 
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void cParticleSnow::PostRender()
{
    cParticle::PostRender();
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
