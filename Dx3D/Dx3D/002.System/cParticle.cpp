#include "stdafx.h"
#include "cParticle.h"


cParticle::cParticle()
{
    m_vertexBuffer = 0; //버텍스 버퍼
    m_vTexture = NULL;  //텍스처
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
    HRESULT hr = 0;

    // 버텍스 버퍼 생성
    hr = g_pDevice->CreateVertexBuffer(
        // 버텍스 버퍼 크기가 m_dvertexBufferSize에 의해 미리 정의되며, 시스템 내의 파티클 수와는 관련이 없음을 주의. 
        m_dvertexBufferSize * sizeof(Particle),
        // 버텍스 버퍼가 포인트 스프라이트를 보관할 것임을 지정하는 D3DUSAGE_POINTS를 이용.
        D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
        Particle::FVF,
        // 동적 버텍스 버퍼는 관리 메모리 풀에 보관할 수 없으므로 디폴트 메모리 풀을 이용.
        D3DPOOL_DEFAULT,
        &m_vertexBuffer,
        0);
    
    // 버텍스 버퍼 생성의 실패 시
    if (FAILED(hr))
    {
        MessageBox(0, L"버텍스 버퍼 생성 실패", L"cParticle", 0);
        return false;
    }

    m_vTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(szKey);

    // 텍스쳐 로딩 실패 시
    if (!m_vTexture)
    {
        MessageBox(0, L"텍스쳐 로딩 실패", L"cParticle", 0);

        return false;
    }

    return true;
}

// 시스템 내의 모든 파티클 속성을 리셋.
void cParticle::Reset()
{
    list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // 한 파티클의 속성을 리셋.
        // 파티클의 속성이 리셋되는 방식은 파티클 시스템에 따라 달라짐.
        // 따라서 하위 클래스에서 메서드를 구현하도록 추상 메서드로 선언.
        ResetParticle(&(*i));
    }
}

// 시스템에 파티클을 추가.
// 이 메서드는 리스트에 추가 하기전에 파티클을 초기화 하는데 resetPaticle 메서드를 이용.
void cParticle::AddParticle()
{
    Attribute attribute;
    ResetParticle(&attribute);
    _particles.push_back(attribute);
}

// 포인트 스프라이트 렌더 상태.
void cParticle::PreRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
    // 현재 지정된 전체 텍스처를 포인트 스프라이트의 텍스처 매핑에 이용할것임을 의미.
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
    // 포인트 크기를 뷰 스페이스 단위로 해석하도록 지정.
    // 뷰 스페이스 단위는 간단히 카메라 공간 내의 3D 포인트를 나타내며,
    // 포인트 스프라이트의 크기는 카메라와의 거리에 따라 적절하게 조정됨.
    // 즉, 카메라와 멀리 떨어진 파티클은 가까운 파티클에 비해작게 나타남.
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
    // 포인트 스프라이트의 크기를 지정.
    // 이 값은 D3DRS_POINTSCALEENABLE 상태 값에 따라서 뷰 스페이스 내의 크기나 
    // 스크린 스페이스 내의 크기로 해석. 
    // FtoDw 함수는 float을 DWORD로 형 변환한다.
    // 이 함수가 필요한 것은 일반적인 IDirect3DDevice9::SetRenderState 호출이
    // float이 아닌 DWORD를 필요로 하기 때문.
    g_pDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDword(_size));

    // 포인트 스프라이트의 지정할 수 있는 최소 크기를 지정.
    g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDword(0.0f));

    // 이 세 개의 상수는 거리에 따라 포인트 스프라이트의 크기가 변하는 방법을 제어.
    // 여기에서 말하는 거리란 카메라와 포인트 스프라이트 간의 거리.
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDword(0.0f));
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDword(0.0f));
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDword(1.0f));

    // 텍스처의 알파를 이용.
    g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    // 알파 블렌딩을 활성화하여 텍스처의 알파 채널이 텍스처의 픽셀 투명도를 결정하도록 했음.
    // 이를 통해 다양한 효과를 얻을 수 있으며,
    // 가장 대표적인 것이 사각형이 아닌 다른 파티클 형태를 구현 하는 것.
    // 예, "눈덩이와 비슷한" 둥근 파티클을 얻기 위해서는 
    // 흰색의 원형과 검은색의 알파 채널을 갖는 흰색 텍스처를 이용하면 됨.
    // 이렇게 하면 사각형의 흰색 텍스처 전체가아닌 흰색 원 모양의 파티클을 만들 수 있다.
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// 특정 파티클 시스템이 지정했을 수 있는 렌더 상태를 복구하는 데 이용.
// 이 메서드는 시스템에 따라 달라질 수 있으므로 가상 메서드로 선언.
void cParticle::PostRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

// 렌더링 메서드
void cParticle::Render()
{
    // Remarks:  The Render method works by filling a section of the vertex buffer with data,
    //           then we Render that section.  While that section is rendering we lock a new
    //           section and begin to fill that section.  Once that sections filled we Render it.
    //           This process continues until all the particles have been drawn.  The benifit
    //           of this method is that we keep the video card and the CPU busy.  
    if (!_particles.empty())
    {
        //렌더 상태를 지정.
        PreRender();

        g_pDevice->SetTexture(0, m_vTexture);
        g_pDevice->SetFVF(Particle::FVF);
        g_pDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(Particle));

        // Render batches one by one
        // 버텍스 버퍼를 벗어날 경우 처음부터 시작한다.

        // _vbOffset - 버텍스 버퍼에서 복사를 시작할 파티클 내 다음 단계로의 오프셋(바이트가 아닌 파티클 단위)
        // 예, 단계 1이 0부터 499까지 항목이라면 단계 2로의 오프셋은 500이 된다.
        if (_vbOffset >= m_dvertexBufferSize)
        {
            _vbOffset = 0;
        }

        Particle* v = 0;
        // _vbBatchSize - 하나의 단계에 정의된 파티클의 수.
        m_vertexBuffer->Lock(
            _vbOffset * sizeof(Particle),
            _vbBatchSize * sizeof(Particle),
            (void**)&v,
            _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
        DWORD numParticlesInBatch = 0;

        // 모든 파티클이 렌더링될 때까지
        list<Attribute>::iterator i;
        for (i = _particles.begin(); i != _particles.end(); i++)
        {
            if (i->isAlive)
            {
                // 한단계의 생존한 파티클을다음 버텍스 버퍼 세그먼트로 복사.
                v->p = i->position;
                v->c = (Color)i->color;
                v++; // next element;
                numParticlesInBatch++; //단계 카운터를 증가시킨다.
                                       // 현재 단계가 모두 채워져 있는가?
                if (numParticlesInBatch == _vbBatchSize)
                {
                    // 버텍스 버퍼로 복사된 마지막 단계의 파티클들을 그린다.
                    m_vertexBuffer->Unlock();
                    g_pDevice->DrawPrimitive(
                        D3DPT_POINTLIST,
                        _vbOffset,
                        _vbBatchSize);
                    // 단계가 그려지는 동안 다음 단계를 파티클로 채운다.
                    // 다음 단계의 처음 오프셋으로 이동한다.
                    _vbOffset += _vbBatchSize;

                    // 버텍스 버퍼의 경계를 넘는메모리로 오프셋을 설정하지 않는다.
                    // 경계를 넘을 경우 처음부터 시작.
                    if (_vbOffset >= m_dvertexBufferSize)
                    {
                        _vbOffset = 0;
                    }

                    m_vertexBuffer->Lock(
                        _vbOffset * sizeof(Particle),
                        _vbBatchSize * sizeof(Particle),
                        (void**)&v,
                        _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
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
                _vbOffset,
                numParticlesInBatch);
        }
        // 다음 블록
        _vbOffset += _vbBatchSize;

        // reset Render states
        PostRender();
    }
}
// 현재 시스템에 파티클이 없는 경우 true 리턴.
bool cParticle::IsEmpty()
{
    return _particles.empty();
}

// 시스템 내의 파티클이 모두 죽은 경우 true 리턴.
// 모든 파티클이 죽은 상태와 시스템이 빈 상태를 혼동하지 않도록.
// 빈 상태는 시스템 내에 파티클이 없는 상태를 의미
// 죽은 상태는 파티클이 존재하지만 죽은 것으로 표시된 상태를 의미.
bool cParticle::IsDead()
{
    list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
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

// 속성 리스트 _particle을 검색하여 죽은 파티클을 리스트에서 제거.
void cParticle::RemoveDeadParticles()
{
    list<Attribute>::iterator i;
    i = _particles.begin();
    while (i != _particles.end())
    {
        if (i->isAlive == false)
        {
            // erase는 다음 반복자를 리턴하므로
            // 우리가 반복자를 증가시킬 필요가 없다.
            i = _particles.erase(i);
        }
        else
        {
            i++; // next in list
        }
    }
}