#include "stdafx.h"
#include "cParticleSnow.h"


cParticleSnow::cParticleSnow()
{}

cParticleSnow::~cParticleSnow()
{}

cParticleSnow::cParticleSnow(cBoundingBox* stBoundingbox, int numParticles)
{
    m_pBoundingBox = stBoundingbox;     // 경계상자 세팅(눈이 이 범위를 벗어나면 다시 세팅됨)
    m_fSize = 5.0f;                     // 눈 크기
    m_vbBufferSize = 2048;         // 버텍스 버퍼의 크기 
    m_vbOffset = 0;          // 시작 오프셋 
    m_vbBatchSize = 512;		            // 배치 사이즈

    for (int i = 0; i < numParticles; i++) // 파티클의 수만큼 리스트에 파티클을 추가
    {
        AddParticle();
    }
}

// 경계 상자 내 임의의 x와 z좌표 위치에서 눈송이를 만들고, y좌표를 경계 상자의 최상단과 같도록 지정, 이어 눈송이에 속도를 부여하여 아래 방향으로 그리고 약간 왼쪽으로 떨어지도록 한다. 눈송이는 흰색으로 표현. 
void cParticleSnow::ResetParticle(ST_PARTICLE_ATTR* attribute)
{
    attribute->isAlive = true;
    // 최소점 min과 최대점 max로 정의된 상자 내의 임의 벡터를 출력, 눈송이의 위치 지정을 위해 임의의 x, y z 좌표를 얻는다. 
    attribute->p = GetRandomVector3(m_pBoundingBox->GetMin(), m_pBoundingBox->GetMax());
    // 높이 (y좌표) 는 항상 경계 상자의 최상단
    attribute->p.y = m_pBoundingBox->GetMax().y;

    // 눈이 떨어지는 속도, 아래쪽으로 떨어지며 약간 왼쪽을 향한다. 
    attribute->v.x = GetRandomFloat(0.0f, 1.0f) * -5.0f;
    attribute->v.y = GetRandomFloat(0.0f, 1.0f) * -12.0f;
    attribute->v.z = 0.0f;
    attribute->life = 2;            //눈이 소멸되기 까지의 시간

    attribute->c = D3DCOLOR_ARGB(150, 255, 255, 255); // 눈 색상 
}

// 파티클의 위치를 갱신하며, 시스템의 경계 상자를 벗어났는지 확인. 만약 경계 상자를 벗어났다면 해당 파티클을 초기화. 
void cParticleSnow::Update()
{
    float fTimeDelta = g_pTimerManager->GetDeltaTime();
    list<ST_PARTICLE_ATTR>::iterator iter;
    for (iter = m_particleList.begin(); iter != m_particleList.end(); iter++)
    {
        // 시간과 속도의 곱으로 눈이 이동한다 
        iter->p += iter->v * fTimeDelta;

        // 파티클의 위치가 경계상자를 벗어났는지 확인 
        if (m_pBoundingBox->IsPointInside(iter->p) == false)
        {
            // 경계를 벗어난 파티클을 초기화 하여 재활용함
            ResetParticle(&(*iter));
        }
    }
}

void cParticleSnow::PreRender()
{
    cParticle::PreRender();
    // z 버퍼 읽기는 가능 하지만 쓰기는 허용하지 않는다. 
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void cParticleSnow::PostRender()
{
    cParticle::PostRender();
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
