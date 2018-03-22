#include "stdafx.h"
#include "cParticle.h"


cParticle::cParticle()
{
    m_vertexBuffer = 0; //���ؽ� ����
    m_vTexture = NULL;  //�ؽ�ó
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
    m_vTexture = NULL;  //�ؽ��� �Ŵ������� ���� ���ִ� ���⼱ NULL�θ� ó��
}

//  init - ����Ʈ ��������Ʈ�� �����ϱ� ���� ���ؽ� ���۸� �����
//  �ؽ�ó�� ����� ���� Direct3D�� ��ġ �������� �ʱ�ȭ �۾��� ó��.
//  ���ڰ� szKey�� TextureManager->GetTexture�� ���.
bool cParticle::Init(string szKey)
{
    // �츮�� ���� ���ؽ� ���۸� �̿��� ����.
    // �� ������ ���� ��ƼŬ�� �����ؾ� �ϸ� �̴� �� ���ؽ� ������ �޸𸮿� �����ؾ� ���� �ǹ�. 
    // ���� ���ؽ� ���۷��� ���� �ӵ��� ����� �����ٴ� �� ����. 
    // ���� ���ؽ� ���۸� �̿��ϴ� ���� �ٷ� �� ����.
    HRESULT hr = S_OK;

    // ���ؽ� ���� ����
    hr = g_pDevice->CreateVertexBuffer(
        // ���ؽ� ���� ũ�Ⱑ m_vbBufferSize�� ���� �̸� ���ǵǸ�, �ý��� ���� ��ƼŬ ���ʹ� ������ ������ ����. 
        m_vbBufferSize * sizeof(ST_PARTICLE),
        // ���ؽ� ���۰� ����Ʈ ��������Ʈ�� ������ ������ �����ϴ� D3DUSAGE_POINTS�� �̿�.
        D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
        ST_PARTICLE::FVF,
        // ���� ���ؽ� ���۴� ���� �޸� Ǯ�� ������ �� �����Ƿ� ����Ʈ �޸� Ǯ�� �̿�.
        D3DPOOL_DEFAULT,
        &m_vertexBuffer,
        0);
    
    // ���ؽ� ���� ������ ���� ��
    if (FAILED(hr))
    {
        MessageBox(0, L"���ؽ� ���� ���� ����", L"cParticle", 0);
        g_pLogManager->WriteLog(EL_ERROR, "Creation FAILED : VertexBuffer - cParticle");

        return false;
    }

    m_vTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(szKey);

    // �ؽ��� �ε� ���� ��
    if (!m_vTexture)
    {
        MessageBox(0, L"�ؽ��� �ε� ����", L"cParticle", 0);
        g_pLogManager->WriteLog(EL_ERROR, "Creation FAILED : Texture - cParticle");

        return false;
    }

    return true;
}

// �ý��� ���� ��� ��ƼŬ �Ӽ��� ����.
void cParticle::Reset()
{
    auto i = m_particleList.begin();
    for (; i != m_particleList.end(); i++)
    {
        // �� ��ƼŬ�� �Ӽ��� ����.
        // ��ƼŬ�� �Ӽ��� ���µǴ� ����� ��ƼŬ �ý��ۿ� ���� �޶���.
        // ���� ���� Ŭ�������� �޼��带 �����ϵ��� �߻� �޼���� ����.
        ResetParticle(&(*i));
    }
}

void cParticle::Reset(ST_PARTICLE_ATTR attrOrigin)
{
    m_particleOrigin = attrOrigin;
    auto i = m_particleList.begin();
    for (; i != m_particleList.end(); i++)
    {
        // �� ��ƼŬ�� �Ӽ��� ����.
        // ��ƼŬ�� �Ӽ��� ���µǴ� ����� ��ƼŬ �ý��ۿ� ���� �޶���.
        // ���� ���� Ŭ�������� �޼��带 �����ϵ��� �߻� �޼���� ����.
        ResetParticle(&(*i), m_particleOrigin);
    }
}

// �ý��ۿ� ��ƼŬ�� �߰�.
// �� �޼���� ����Ʈ�� �߰� �ϱ����� ��ƼŬ�� �ʱ�ȭ �ϴµ� resetPaticle �޼��带 �̿�.
void cParticle::AddParticle()
{
    ST_PARTICLE_ATTR attribute;
    attribute = m_particleOrigin;

    attribute.vPos = m_vOriginPos;

    ResetParticle(&attribute, m_particleOrigin);
    m_particleList.push_back(attribute);
}

// ����Ʈ ��������Ʈ ���� ����.
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

// Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����ϴ� �� �̿�.
// �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �޼���� ����.
void cParticle::PostRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

// ������ �޼���
void cParticle::Render()
{
    // Remarks:  The Render method works by filling a section of the vertex buffer with data,
    //           then we Render that section.  While that section is rendering we lock a new
    //           section and begin to fill that section.  Once that sections filled we Render it.
    //           This process continues until all the particles have been drawn.  The benifit
    //           of this method is that we keep the video card and the CPU busy.  
    if (!m_particleList.empty())
    {
        //���� ���¸� ����.
        PreRender();

        g_pDevice->SetTexture(0, m_vTexture);
        g_pDevice->SetFVF(ST_PARTICLE::FVF);
        g_pDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(ST_PARTICLE));

        // Render batches one by one
        // ���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.

        // _vbOffset - ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ �� ���� �ܰ���� ������(����Ʈ�� �ƴ� ��ƼŬ ����)
        // ��, �ܰ� 1�� 0���� 499���� �׸��̶�� �ܰ� 2���� �������� 500�� �ȴ�.
        if (m_vbOffset >= m_vbBufferSize)
        {
            m_vbOffset = 0;
        }

        ST_PARTICLE* v = NULL;
        // _vbBatchSize - �ϳ��� �ܰ迡 ���ǵ� ��ƼŬ�� ��.
        m_vertexBuffer->Lock(
            m_vbOffset * sizeof(ST_PARTICLE),
            m_vbBatchSize * sizeof(ST_PARTICLE),
            (void**)&v,
            m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
        DWORD numParticlesInBatch = 0;

        // ��� ��ƼŬ�� �������� ������
        list<ST_PARTICLE_ATTR>::iterator i;
        for (i = m_particleList.begin(); i != m_particleList.end(); i++)
        {
            if (i->isAlive)
            {
                // �Ѵܰ��� ������ ��ƼŬ������ ���ؽ� ���� ���׸�Ʈ�� ����.
                v->vPos = i->vPos;
                v->c = (Color)i->color;
                v++; // next element;
                numParticlesInBatch++; //�ܰ� ī���͸� ������Ų��.
                                       // ���� �ܰ谡 ��� ä���� �ִ°�?
                if (numParticlesInBatch == m_vbBatchSize)
                {
                    // ���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ���� �׸���.
                    m_vertexBuffer->Unlock();
                    g_pDevice->DrawPrimitive(
                        D3DPT_POINTLIST,
                        m_vbOffset,
                        m_vbBatchSize);
                    // �ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.
                    // ���� �ܰ��� ó�� ���������� �̵��Ѵ�.
                    m_vbOffset += m_vbBatchSize;

                    // ���ؽ� ������ ��踦 �Ѵ¸޸𸮷� �������� �������� �ʴ´�.
                    // ��踦 ���� ��� ó������ ����.
                    if (m_vbOffset >= m_vbBufferSize)
                    {
                        m_vbOffset = 0;
                    }

                    m_vertexBuffer->Lock(
                        m_vbOffset * sizeof(ST_PARTICLE),
                        m_vbBatchSize * sizeof(ST_PARTICLE),
                        (void**)&v,
                        m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
                    numParticlesInBatch = 0; // ���� �ܰ踦 ���� ����
                }
            }
        }
        m_vertexBuffer->Unlock();

        // (numParticlesInBatch == _vbBatchSize) ������ 
        // �������� ���Ͽ� ������ �ܰ谡 ���������� �ʴ� ��찡 �߻��� �� �ִ�.
        // �Ϻθ� ä���� �ܰ�� �ٷ� �̰����� ������ �ȴ�.  
        if (numParticlesInBatch)
        {
            g_pDevice->DrawPrimitive(
                D3DPT_POINTLIST,
                m_vbOffset,
                numParticlesInBatch);
        }
        // ���� ���
        m_vbOffset += m_vbBatchSize;

        // reset Render states
        PostRender();
    }
}
// ���� �ý��ۿ� ��ƼŬ�� ���� ��� true ����.
bool cParticle::IsEmpty()
{
    return m_particleList.empty();
}

// �ý��� ���� ��ƼŬ�� ��� ���� ��� true ����.
// ��� ��ƼŬ�� ���� ���¿� �ý����� �� ���¸� ȥ������ �ʵ���.
// �� ���´� �ý��� ���� ��ƼŬ�� ���� ���¸� �ǹ�
// ���� ���´� ��ƼŬ�� ���������� ���� ������ ǥ�õ� ���¸� �ǹ�.
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

    // ������ ������ ������ ���͸� ����
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
    // 0.5�� ������ ������ ������. 
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

    // ������ �ӵ� ����
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
        // ������ ��ƼŬ�� ����.
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
        // ����ִ� �͸� ������Ʈ
        if (iter->isAlive)
        {
            float time = g_pTimerManager->GetDeltaTime();

            iter->vPos += (iter->vSpeed * time);

            iter->age += time;

            if (iter->age > iter->life)	//������ ������
            {
                iter->isAlive = false;
            }
        }
    }
}

// �Ӽ� ����Ʈ _particle�� �˻��Ͽ� ���� ��ƼŬ�� ����Ʈ���� ����.
void cParticle::RemoveDeadParticles()
{
    list<ST_PARTICLE_ATTR>::iterator i;
    i = m_particleList.begin();
    while (i != m_particleList.end())
    {
        if (i->isAlive == false)
        {
            // erase�� ���� �ݺ��ڸ� �����ϹǷ�
            // �츮�� �ݺ��ڸ� ������ų �ʿ䰡 ����.
            i = m_particleList.erase(i);
        }
        else
        {
            i++; // next in list
        }
    }
}