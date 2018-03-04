#include "stdafx.h"
#include "cParticle.h"
#include "d3dUtility.h"


cParticle::cParticle()
{
    m_vertexBuffer = 0; //���ؽ� ����
    m_vTexture = NULL;  //�ؽ�ó
}

cParticle::~cParticle()
{
    SAFE_RELEASE(m_vertexBuffer);
    m_vTexture = NULL;  //�ؽ��� �Ŵ������� ���� ���ִ� ���⼱ NULL�θ� ó��
}

// init - ����Ʈ ��������Ʈ�� �����ϱ� ���� ���ؽ� ���۸� �����
// �ؽ�ó�� ����� ���� Direct3D�� ��ġ �������� �ʱ�ȭ �۾��� ó��.
bool cParticle::init(string texFileName)
{
    // �츮�� ���� ���ؽ� ���۸� �̿��� ����.
    // �� ������ ���� ��ƼŬ�� �����ؾ� �ϸ� �̴� �� ���ؽ� ������ �޸𸮿� �����ؾ� ���� �ǹ�. 
    // ���� ���ؽ� ���۷��� ���� �ӵ��� ����� �����ٴ� �� ����. 
    // ���� ���ؽ� ���۸� �̿��ϴ� ���� �ٷ� �� ����.
    
    HRESULT hr = 0;

    // ���ؽ� ���� ����
    hr = g_pDevice->CreateVertexBuffer(
        // ���ؽ� ���� ũ�Ⱑ m_dvertexBufferSize�� ���� �̸� ���ǵǸ�, �ý��� ���� ��ƼŬ ���ʹ� ������ ������ ����. 
        m_dvertexBufferSize * sizeof(Particle),
        // ���ؽ� ���۰� ����Ʈ ��������Ʈ�� ������ ������ �����ϴ� D3DUSAGE_POINTS�� �̿�.
        D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
        Particle::FVF,
        // ���� ���ؽ� ���۴� ���� �޸� Ǯ�� ������ �� �����Ƿ� ����Ʈ �޸� Ǯ�� �̿�.
        D3DPOOL_DEFAULT,
        &m_vertexBuffer,
        0);
    
    // ���ؽ� ���� ������ ���� ��
    if (FAILED(hr))
    {
        ::MessageBox(0, L"���ؽ� ���� ���� ����", L"cParticle", 0);
        return false;
    }

    g_pTextureManager->AddTexture(texFileName, texFileName, true);
    m_vTexture = g_pTextureManager->GetTexture(texFileName);

    // �ؽ��� �ε� ���� ��
    if (!m_vTexture)
    {
        ::MessageBox(0, L"�ؽ��� �ε� ����", L"cParticle", 0);
        return false;
    }
    return true;
}

// �ý��� ���� ��� ��ƼŬ �Ӽ��� ����.
void cParticle::reset()
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // �� ��ƼŬ�� �Ӽ��� ����.
        // ��ƼŬ�� �Ӽ��� ���µǴ� ����� ��ƼŬ �ý��ۿ� ���� �޶���.
        // ���� ���� Ŭ�������� �޼��带 �����ϵ��� �߻� �޼���� ����.
        resetParticle(&(*i));
    }
}

// �ý��ۿ� ��ƼŬ�� �߰�.
// �� �޼���� ����Ʈ�� �߰� �ϱ����� ��ƼŬ�� �ʱ�ȭ �ϴµ� resetPaticle �޼��带 �̿�.
void cParticle::addParticle()
{
    Attribute attribute;
    resetParticle(&attribute);
    _particles.push_back(attribute);
}
// ����Ʈ ��������Ʈ ���� ����.
void cParticle::preRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
    // ���� ������ ��ü �ؽ�ó�� ����Ʈ ��������Ʈ�� �ؽ�ó ���ο� �̿��Ұ����� �ǹ�.
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
    // ����Ʈ ũ�⸦ �� �����̽� ������ �ؼ��ϵ��� ����.
    // �� �����̽� ������ ������ ī�޶� ���� ���� 3D ����Ʈ�� ��Ÿ����,
    // ����Ʈ ��������Ʈ�� ũ��� ī�޶���� �Ÿ��� ���� �����ϰ� ������.
    // ��, ī�޶�� �ָ� ������ ��ƼŬ�� ����� ��ƼŬ�� �����۰� ��Ÿ��.
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
    // ����Ʈ ��������Ʈ�� ũ�⸦ ����.
    // �� ���� D3DRS_POINTSCALEENABLE ���� ���� ���� �� �����̽� ���� ũ�⳪ 
    // ��ũ�� �����̽� ���� ũ��� �ؼ�. 
    // FtoDw �Լ��� float�� DWORD�� �� ��ȯ�Ѵ�.
    // �� �Լ��� �ʿ��� ���� �Ϲ����� IDirect3DDevice9::SetRenderState ȣ����
    // float�� �ƴ� DWORD�� �ʿ�� �ϱ� ����.
    g_pDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDword(_size));

    // ����Ʈ ��������Ʈ�� ������ �� �ִ� �ּ� ũ�⸦ ����.
    g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDword(0.0f));

    // �� �� ���� ����� �Ÿ��� ���� ����Ʈ ��������Ʈ�� ũ�Ⱑ ���ϴ� ����� ����.
    // ���⿡�� ���ϴ� �Ÿ��� ī�޶�� ����Ʈ ��������Ʈ ���� �Ÿ�.
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDword(0.0f));
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDword(0.0f));
    g_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDword(1.0f));

    // �ؽ�ó�� ���ĸ� �̿�.
    g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    // ���� ������ Ȱ��ȭ�Ͽ� �ؽ�ó�� ���� ä���� �ؽ�ó�� �ȼ� ������ �����ϵ��� ����.
    // �̸� ���� �پ��� ȿ���� ���� �� ������,
    // ���� ��ǥ���� ���� �簢���� �ƴ� �ٸ� ��ƼŬ ���¸� ���� �ϴ� ��.
    // ��, "�����̿� �����" �ձ� ��ƼŬ�� ��� ���ؼ��� 
    // ����� ������ �������� ���� ä���� ���� ��� �ؽ�ó�� �̿��ϸ� ��.
    // �̷��� �ϸ� �簢���� ��� �ؽ�ó ��ü���ƴ� ��� �� ����� ��ƼŬ�� ���� �� �ִ�.
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����ϴ� �� �̿�.
// �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �޼���� ����.
void cParticle::postRender()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
    g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
    g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

// ������ �޼���
void cParticle::render()
{
    //
    // Remarks:  The render method works by filling a section of the vertex buffer with data,
    //           then we render that section.  While that section is rendering we lock a new
    //           section and begin to fill that section.  Once that sections filled we render it.
    //           This process continues until all the particles have been drawn.  The benifit
    //           of this method is that we keep the video card and the CPU busy.  
    if (!_particles.empty())
    {
        //���� ���¸� ����.
        preRender();

        g_pDevice->SetTexture(0, m_vTexture);
        g_pDevice->SetFVF(Particle::FVF);
        g_pDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(Particle));

        // render batches one by one
        //
        // ���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.

        // _vbOffset - ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ �� ���� �ܰ���� ������(����Ʈ�� �ƴ� ��ƼŬ ����)
        // ��, �ܰ� 1�� 0���� 499���� �׸��̶�� �ܰ� 2���� �������� 500�� �ȴ�.
        if (_vbOffset >= m_dvertexBufferSize)
            _vbOffset = 0;
        Particle* v = 0;
        // _vbBatchSize - �ϳ��� �ܰ迡 ���ǵ� ��ƼŬ�� ��.
        m_vertexBuffer->Lock(
            _vbOffset * sizeof(Particle),
            _vbBatchSize * sizeof(Particle),
            (void**)&v,
            _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
        DWORD numParticlesInBatch = 0;

        // ��� ��ƼŬ�� �������� ������
        //
        std::list<Attribute>::iterator i;
        for (i = _particles.begin(); i != _particles.end(); i++)
        {
            if (i->isAlive)
            {

                // �Ѵܰ��� ������ ��ƼŬ������ ���ؽ� ���� ���׸�Ʈ�� ����.
                //
                v->position = i->position;
                v->color = (D3DCOLOR)i->color;
                v++; // next element;
                numParticlesInBatch++; //�ܰ� ī���͸� ������Ų��.
                                       // ���� �ܰ谡 ��� ä���� �ִ°�?
                if (numParticlesInBatch == _vbBatchSize)
                {
                    //
                    // ���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ���� �׸���.
                    //
                    m_vertexBuffer->Unlock();
                    g_pDevice->DrawPrimitive(
                        D3DPT_POINTLIST,
                        _vbOffset,
                        _vbBatchSize);
                    //
                    // �ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.
                    //
                    // ���� �ܰ��� ó�� ���������� �̵��Ѵ�.
                    _vbOffset += _vbBatchSize;

                    // ���ؽ� ������ ��踦 �Ѵ¸޸𸮷� �������� �������� �ʴ´�.
                    // ��踦 ���� ��� ó������ ����.
                    if (_vbOffset >= m_dvertexBufferSize)
                        _vbOffset = 0;
                    m_vertexBuffer->Lock(
                        _vbOffset * sizeof(Particle),
                        _vbBatchSize * sizeof(Particle),
                        (void**)&v,
                        _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
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
                _vbOffset,
                numParticlesInBatch);
        }
        // ���� ���
        _vbOffset += _vbBatchSize;
        //
        // reset render states
        //
        postRender();
    }
}
// ���� �ý��ۿ� ��ƼŬ�� ���� ��� true ����.
bool cParticle::isEmpty()
{
    return _particles.empty();
}

// �ý��� ���� ��ƼŬ�� ��� ���� ��� true ����.
// ��� ��ƼŬ�� ���� ���¿� �ý����� �� ���¸� ȥ������ �ʵ���.
// �� ���´� �ý��� ���� ��ƼŬ�� ���� ���¸� �ǹ�
// ���� ���´� ��ƼŬ�� ���������� ���� ������ ǥ�õ� ���¸� �ǹ�.
bool cParticle::isDead()
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // is there at least one living particle?  If yes,
        // the system is not dead.
        if (i->isAlive)
            return false;
    }
    // no living particles found, the system must be dead.
    return true;
}

// �Ӽ� ����Ʈ _particle�� �˻��Ͽ� ���� ��ƼŬ�� ����Ʈ���� ����.
void cParticle::removeDeadParticles()
{
    std::list<Attribute>::iterator i;
    i = _particles.begin();
    while (i != _particles.end())
    {
        if (i->isAlive == false)
        {
            // erase�� ���� �ݺ��ڸ� �����ϹǷ�
            // �츮�� �ݺ��ڸ� ������ų �ʿ䰡 ����.
            i = _particles.erase(i);
        }
        else
        {
            i++; // next in list
        }
    }
}