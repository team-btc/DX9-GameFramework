#include "stdafx.h"
#include "cParticle.h"


cParticle::cParticle()
{
    m_vertexBuffer = 0; //버텍스 버퍼
    m_vTexture = NULL;  //텍스처
    m_isRandomPos = false;
    m_vMinRange = Vector3(0, 0, 0);
    m_vMaxRange = Vector3(0, 0, 0);
    m_isBlack = false;
}

cParticle::cParticle(Vector3* vOriginPos, int nGenNum, int nMaxNum)
{
    m_vOriginPos = *vOriginPos;
    m_fSize = 0.9f;
    m_vbBufferSize = 2048;
    m_vbOffset = 0;
    m_vbBatchSize = 512;
    m_fGenTerm = 0.0f;
    m_nMaxParticle = nMaxNum;

    for (int i = 0; i < nGenNum; i++)
    {
        AddParticle();
    }
}

cParticle::cParticle(Vector3 * vOriginPos, int nGenNum, int nMaxNum, bool bRandomPos, Vector3 vMinRange, Vector3 vMaxRange)
{
    m_vOriginPos = *vOriginPos;
    m_fSize = 0.9f;
    m_vbBufferSize = 2048;
    m_vbOffset = 0;
    m_vbBatchSize = 512;
    m_fGenTerm = 0.0f;
    m_nMaxParticle = nMaxNum;

    UseRandomPosition(bRandomPos);
    SetRandomRange(vMinRange, vMaxRange);

    for (int i = 0; i < nGenNum; i++)
    {
        AddParticle();
    }
}

cParticle::cParticle(Vector3 * vOriginPos, int nMaxNum)
{
    m_vOriginPos = *vOriginPos;
    m_fSize = 0.9f;
    m_vbBufferSize = 2048;
    m_vbOffset = 0;
    m_vbBatchSize = 512;
    m_fGenTerm = 0.0f;
    m_nMaxParticle = nMaxNum;

    for (int i = 0; i < nMaxNum; i++)
    {
        AddParticleCircle();
    }
}

cParticle::~cParticle()
{
    SAFE_RELEASE(m_vertexBuffer);
    m_vTexture = NULL;  //텍스쳐 매니저에서 관리 해주니 여기선 NULL로만 처리
}

//  init - 포인트 스프라이트를 저장하기 위한 버텍스 버퍼를 만들고
//  텍스처를 만드는 등의 Direct3D의 장치 의존적인 초기화 작업을 처리.
//  인자값 szKey는 TextureManager->GetTexture에 사용.
bool cParticle::Init(string szKey)
{
    // 우리는 동적 버텍스 버퍼를 이용할 것임.
    // 매 프레임 마다 파티클을 갱신해야 하며 이는 곧 버텍스 버퍼의 메모리에 접근해야 함을 의미. 
    // 정적 버텍스 버퍼로의 접근 속도는 상당히 느리다는 데 주의. 
    // 동적 버텍스 버퍼를 이용하는 것은 바로 이 때문.
    HRESULT hr = S_OK;

    // 버텍스 버퍼 생성
    hr = g_pDevice->CreateVertexBuffer(
        // 버텍스 버퍼 크기가 m_vbBufferSize에 의해 미리 정의되며, 시스템 내의 파티클 수와는 관련이 없음을 주의. 
        m_vbBufferSize * sizeof(ST_PARTICLE),
        // 버텍스 버퍼가 포인트 스프라이트를 보관할 것임을 지정하는 D3DUSAGE_POINTS를 이용.
        D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
        ST_PARTICLE::FVF,
        // 동적 버텍스 버퍼는 관리 메모리 풀에 보관할 수 없으므로 디폴트 메모리 풀을 이용.
        D3DPOOL_DEFAULT,
        &m_vertexBuffer,
        0);
    
    // 버텍스 버퍼 생성의 실패 시
    if (FAILED(hr))
    {
        MessageBox(0, L"버텍스 버퍼 생성 실패", L"cParticle", 0);
        g_pLogManager->WriteLog(EL_ERROR, "Creation FAILED : VertexBuffer - cParticle");

        return false;
    }

    m_vTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(szKey);

    // 텍스쳐 로딩 실패 시
    if (!m_vTexture)
    {
        MessageBox(0, L"텍스쳐 로딩 실패", L"cParticle", 0);
        g_pLogManager->WriteLog(EL_ERROR, "Creation FAILED : Texture - cParticle");

        return false;
    }

    return true;
}

// 시스템 내의 모든 파티클 속성을 리셋.
void cParticle::Reset()
{
    auto i = m_particleList.begin();
    for (; i != m_particleList.end(); i++)
    {
        // 한 파티클의 속성을 리셋.
        // 파티클의 속성이 리셋되는 방식은 파티클 시스템에 따라 달라짐.
        // 따라서 하위 클래스에서 메서드를 구현하도록 추상 메서드로 선언.
        ResetParticle(&(*i));
    }
}

void cParticle::Reset(ST_PARTICLE_ATTR attrOrigin)
{
    m_particleOrigin = attrOrigin;
    auto i = m_particleList.begin();
    for (; i != m_particleList.end(); i++)
    {
        // 한 파티클의 속성을 리셋.
        // 파티클의 속성이 리셋되는 방식은 파티클 시스템에 따라 달라짐.
        // 따라서 하위 클래스에서 메서드를 구현하도록 추상 메서드로 선언.
        ResetParticle(&(*i), m_particleOrigin);
    }
}

// 시스템에 파티클을 추가.
// 이 메서드는 리스트에 추가 하기전에 파티클을 초기화 하는데 resetPaticle 메서드를 이용.
void cParticle::AddParticle()
{
    ST_PARTICLE_ATTR attribute;
    attribute = m_particleOrigin;

    attribute.vPos = m_vOriginPos;

    ResetParticle(&attribute, m_particleOrigin);
    m_particleList.push_back(attribute);
}

// 포인트 스프라이트 렌더 상태.
void cParticle::PreRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
    g_pDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDword(m_fSize));
    g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDword(0.0f));

    /*
    (1 / (A + B * D + C * D^2)) * SIZE
    */
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDword(0.0f));
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDword(0.0f));
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDword(1.0f));

    g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


    if (m_isBlack)
    {
        g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }
}

// 특정 파티클 시스템이 지정했을 수 있는 렌더 상태를 복구하는 데 이용.
// 이 메서드는 시스템에 따라 달라질 수 있으므로 가상 메서드로 선언.
void cParticle::PostRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

// 렌더링 메서드
void cParticle::Render()
{
    // Remarks:  The Render method works by filling a section of the vertex buffer with data,
    //           then we Render that section.  While that section is rendering we lock a new
    //           section and begin to fill that section.  Once that sections filled we Render it.
    //           This process continues until all the particles have been drawn.  The benifit
    //           of this method is that we keep the video card and the CPU busy.  
    if (!m_particleList.empty())
    {
        //렌더 상태를 지정.
        PreRender();

        g_pDevice->SetTexture(0, m_vTexture);
        g_pDevice->SetFVF(ST_PARTICLE::FVF);
        g_pDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(ST_PARTICLE));

        // Render batches one by one
        // 버텍스 버퍼를 벗어날 경우 처음부터 시작한다.

        // _vbOffset - 버텍스 버퍼에서 복사를 시작할 파티클 내 다음 단계로의 오프셋(바이트가 아닌 파티클 단위)
        // 예, 단계 1이 0부터 499까지 항목이라면 단계 2로의 오프셋은 500이 된다.
        if (m_vbOffset >= m_vbBufferSize)
        {
            m_vbOffset = 0;
        }

        ST_PARTICLE* v = NULL;
        // _vbBatchSize - 하나의 단계에 정의된 파티클의 수.
        m_vertexBuffer->Lock(
            m_vbOffset * sizeof(ST_PARTICLE),
            m_vbBatchSize * sizeof(ST_PARTICLE),
            (void**)&v,
            m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
        DWORD numParticlesInBatch = 0;

        // 모든 파티클이 렌더링될 때까지
        list<ST_PARTICLE_ATTR>::iterator i;
        for (i = m_particleList.begin(); i != m_particleList.end(); i++)
        {
            if (i->isAlive)
            {
                // 한단계의 생존한 파티클을다음 버텍스 버퍼 세그먼트로 복사.
                v->vPos = i->vPos;
                v->c = (Color)i->color;
                v++; // next element;
                numParticlesInBatch++; //단계 카운터를 증가시킨다.
                                       // 현재 단계가 모두 채워져 있는가?
                if (numParticlesInBatch == m_vbBatchSize)
                {
                    // 버텍스 버퍼로 복사된 마지막 단계의 파티클들을 그린다.
                    m_vertexBuffer->Unlock();
                    g_pDevice->DrawPrimitive(
                        D3DPT_POINTLIST,
                        m_vbOffset,
                        m_vbBatchSize);
                    // 단계가 그려지는 동안 다음 단계를 파티클로 채운다.
                    // 다음 단계의 처음 오프셋으로 이동한다.
                    m_vbOffset += m_vbBatchSize;

                    // 버텍스 버퍼의 경계를 넘는메모리로 오프셋을 설정하지 않는다.
                    // 경계를 넘을 경우 처음부터 시작.
                    if (m_vbOffset >= m_vbBufferSize)
                    {
                        m_vbOffset = 0;
                    }

                    m_vertexBuffer->Lock(
                        m_vbOffset * sizeof(ST_PARTICLE),
                        m_vbBatchSize * sizeof(ST_PARTICLE),
                        (void**)&v,
                        m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
                    numParticlesInBatch = 0; // 다음 단계를 위한 리셋
                }
            }
        }
        m_vertexBuffer->Unlock();

        // (numParticlesInBatch == _vbBatchSize) 조건이 
        // 만족되지 못하여 마지막 단계가 렌더링되지 않는 경우가 발생할 수 있다.
        // 일부만 채워진 단계는 바로 이곳에서 렌더링 된다.  
        if (numParticlesInBatch)
        {
            g_pDevice->DrawPrimitive(
                D3DPT_POINTLIST,
                m_vbOffset,
                numParticlesInBatch);
        }
        // 다음 블록
        m_vbOffset += m_vbBatchSize;

        // reset Render states
        PostRender();
    }
}
// 현재 시스템에 파티클이 없는 경우 true 리턴.
bool cParticle::IsEmpty()
{
    return m_particleList.empty();
}

// 시스템 내의 파티클이 모두 죽은 경우 true 리턴.
// 모든 파티클이 죽은 상태와 시스템이 빈 상태를 혼동하지 않도록.
// 빈 상태는 시스템 내에 파티클이 없는 상태를 의미
// 죽은 상태는 파티클이 존재하지만 죽은 것으로 표시된 상태를 의미.
bool cParticle::IsDead()
{
    list<ST_PARTICLE_ATTR>::iterator i;
    for (i = m_particleList.begin(); i != m_particleList.end(); i++)
    {
        // is there at least one living particle?  If yes,
        // the system is not dead.
        if (i->isAlive)
        {
            return false;
        }
    }
    // no living particles found, the system must be dead.
    return true;
}

void cParticle::ResetParticle(ST_PARTICLE_ATTR* attribute)
{
    attribute->isAlive = true;
    attribute->vPos = m_vOriginPos;

    Vector3 min = Vector3(-1.0f, -1.0f, -1.0f);
    Vector3 max = Vector3(1.0f, 1.0f, 1.0f);

    // 지정된 범위의 랜덤한 벡터를 저장
    attribute->vSpeed = GetRandomVector3(min, max);

    D3DXVec3Normalize(
        &attribute->vSpeed,
        &attribute->vSpeed);

    attribute->vSpeed *= TEMP_MULTIPLE;
   
    //attribute->color = XColor(
    //    GetRandomFloat(0.0f, 1.0f),
    //    GetRandomFloat(0.0f, 1.0f),
    //    GetRandomFloat(0.0f, 1.0f),
    //    1.0f);

    attribute->color = XColor(1, 1, 1, 1);

   
    attribute->age = 0.0f;
    attribute->life = TEMP_AGE;
    // 0.5초 동안의 수명을 가진다. 
}

void cParticle::ResetParticle(ST_PARTICLE_ATTR* attrDest, ST_PARTICLE_ATTR attrOrigin)
{
    *attrDest = attrOrigin;

    if (m_isRandomPos)
    {
        attrDest->vPos = m_vOriginPos + GetRandomVector3(m_vMinRange, m_vMaxRange);
    }
    else
    {
        attrDest->vPos = m_vOriginPos;
    }

    Vector3 min = attrOrigin.deltaAccelMin;
    Vector3 max = attrOrigin.deltaAccelMax;

    // 랜덤한 속도 생성
    attrDest->vSpeed = GetRandomVector3(min, max);

  /*  D3DXVec3Normalize(
        &attrDest->vSpeed,
        &attrDest->vSpeed);*/
    attrDest->vSpeed *= attrDest->fSpeed;

    attrDest->life = GetRandomFloat(attrDest->fMinLife, attrDest->fMaxLife);
}

void cParticle::Update()
{
    float timeDelta = g_pTimerManager->GetDeltaTime();
    if ((int)m_particleList.size() < m_nMaxParticle &&
        m_fGenTime < g_pTimerManager->GetWorldTime())
    {
        AddParticle();
        m_fGenTime = g_pTimerManager->GetWorldTime() + m_fGenTerm;
    }

    auto i = m_particleList.begin();
    for (; i != m_particleList.end(); i++)
    {
        // 생존한 파티클만 갱신.
        if (i->isAlive)
        {
            if (i->isFade)
            {
                Vector4 c1, c2;
                c1 = Vector4(i->originColor.r, i->originColor.g, i->originColor.b, i->originColor.a);
                c2 = Vector4(i->fadeColor.r, i->fadeColor.g, i->fadeColor.b, i->fadeColor.a);
                D3DXVec4Lerp(&c1, &c1, &c2, (i->age / i->life));
                i->color = XColor(c1.x, c1.y, c1.z, c1.w);
            }

            i->vAccel.y -= i->fGravity;
            i->vPos += (i->vSpeed + i->vAccel) * timeDelta;
            i->vPos.y -= i->fGravity * timeDelta;
            i->age += timeDelta;
            if (i->age > i->life)
            {
                if (i->isLoop)
                {
                    i->vPos = m_vOriginPos;
                    i->age = 0.0f;
                    ResetParticle(&(*i), m_particleOrigin);
                }
                else
                {
                    i->isAlive = false;
                }
            }
        }
    }
}

void cParticle::AddParticleCircle()
{
    ST_PARTICLE_ATTR attribute;
    attribute.vPos = m_vOriginPos;

    ResetParticle(&attribute);
    m_particleList.push_back(attribute);

}

void cParticle::UpdateCircle()
{
    for (auto iter = m_particleList.begin(); iter != m_particleList.end(); ++iter)
    {
        // 살아있는 것만 업데이트
        if (iter->isAlive)
        {
            float time = g_pTimerManager->GetDeltaTime();

            iter->vPos += (iter->vSpeed * time);

            iter->age += time;

            if (iter->age > iter->life)	//수명이 끝났음
            {
                iter->isAlive = false;
            }
        }
    }
}

// 속성 리스트 _particle을 검색하여 죽은 파티클을 리스트에서 제거.
void cParticle::RemoveDeadParticles()
{
    list<ST_PARTICLE_ATTR>::iterator i;
    i = m_particleList.begin();
    while (i != m_particleList.end())
    {
        if (i->isAlive == false)
        {
            // erase는 다음 반복자를 리턴하므로
            // 우리가 반복자를 증가시킬 필요가 없다.
            i = m_particleList.erase(i);
        }
        else
        {
            i++; // next in list
        }
    }
}