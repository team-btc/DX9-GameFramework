#pragma once

// ���ӽ����̽� ���� ����ü 
namespace psystem
{
    // ��ƼŬ�� ��ġ�� �÷��� ǥ���ϴ� ����ü 
    struct Particle
    {
        D3DXVECTOR3         position;   // ��ġ 
        D3DCOLOR            color;      // ����
        static const DWORD  FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;  // ������ ���ؽ� ����
    };

    // ��ƼŬ�� �Ӽ� ����ü
    struct Attribute
    {
        D3DXVECTOR3         position;       // ���彺���̽� ���� ��ƼŬ ��ġ
        D3DXVECTOR3         velocity;       // ��ƼŬ�� �ӵ�, ������ �ʴ� �̵� ������ ���
        D3DXVECTOR3         acceleration;   // ��ƼŬ�� ����,
        float               lifeTime;       // ��ƼŬ�� �Ҹ� �Ҷ����� �����Ǵ� �ð�
        float               age;            // ��ƼŬ�� ���� ����
        D3DXCOLOR           color;          // ��ƼŬ�� �÷�
        D3DXCOLOR           colorFade;      // ��ƼŬ�� �÷��� �ð��� �帧�� ���� ����ϴ� ���
        bool                isAlive;        // ��ƼŬ�� ���� true, �Ҹ� false

        // ��ƼŬ �Ӽ� ����ü�� �ʱⰪ ����
        Attribute()
        {
            lifeTime = 0.0f;
            age = 0.0f;
            isAlive = true;
        }
    };
}

using namespace psystem;

// ��ƼŬ �ý����� ��ҵ�.
// ��ƼŬ �ý����� ��ƼŬ���� �����̸� ��ƼŬ�� �����ְ� �����ϴ� ������ ����Ѵ�.
// ��ƼŬ�� ũ�⳪ ��ƼŬ ��õ�� ��ġ, ��ƼŬ�� ������ �ؽ�ó �� 
// �ý��� ���� ��� ��ƼŬ�� ������ �ִ� ���� Ư������ ����.
// ����� �������� ����
// ��ƼŬ ���Ű� ���÷���, �Ҹ�, ���� ���� �����ϴ� ����.

class cParticle
{
protected:
    D3DXVECTOR3             m_vOriginPos;       // �ý��� ������ ��ƼŬ�� ���۵Ǵ� ��.
    float                   _emitRate;          // �ý��ۿ� ���ο� ��ƼŬ�� �߰��Ǵ� ����. ������ �ʴ� ��ƼŬ ���� ���.
    float                   _size;              // �ý��� �� ��� ��ƼŬ�� ũ��

    LPTEXTURE9			    m_vTexture;         // �ý����� ����� �ؽ��� 
    IDirect3DVertexBuffer9* m_vertexBuffer;     // �ý����� ����� ���ؽ� ����

    list<Attribute>         _particles;         // �ý��� �� ��ƼŬ �Ӽ��� ����Ʈ.
                                                // �츮�� ��ƼŬ�� ����� �����ϰ� �����ϴ� �� �� ����Ʈ�� �̿�.
                                                // ��ƼŬ�� �׸� �غ� �Ϸ�Ǹ� 
                                                // ����Ʈ ����� �Ϻθ� ���ؽ� ���۷� �����ϰ�
                                                // ��ƼŬ ����� ������ ��ģ��.
                                                // �̾� ���� �ܰ��� �����͸� �����ϰ� �ٽ� ��ƼŬ�� �׸���,
                                                // ��� ��ƼŬ�� �׸� ������ �� ������ �ݺ�.

    int                     _maxParticles;      // �־��� �ð� ���� �ý����� ���� �� �ִ� �ִ� ��ƼŬ�� ��.
                                                // ��, ��ƼŬ�� �ı��Ǵ� �ӵ����� ��������� �ӵ��� �����ٸ� 
                                                // ��û���� ���� ���� ��ƼŬ�� ���ÿ� ������ �� �ִ�.
                                                // �� ����� �̿� ���� ��Ȳ�� ���´�.
    DWORD m_dvertexBufferSize;                  // ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ�� ��. 
                                                // �� ���� ���� ��ƼŬ �ý��� ���� ��ƼŬ ���ʹ� ������.
    DWORD _vbOffset;                            // ��ƼŬ �ý����� �������� �̿�.
    DWORD _vbBatchSize;                         // ��ƼŬ �ý����� �������� �̿�.

public:
    cParticle();                                // ������ 
    virtual ~cParticle();                       // �Ҹ��� (���ؽ� ����, �ؽ��ĸ� ����) 
    virtual bool init(string texFileName);      // init - ����Ʈ ��������Ʈ�� �����ϱ� ���� ���ؽ� ���۸� ����� �ؽ��ĸ� ����
    virtual void reset();                       // �ý��� ���� ��� ��ƼŬ �Ӽ��� ����

    // �ý��ۿ� ��ƼŬ�� �߰�.
    virtual void addParticle();

    // �������� �ռ� �����ؾ� �� �ʱ� ���� ���¸� ����.
    // �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �Լ��� ����.
    virtual void preRender();

    // �ý��� ���� ��� ��ƼŬ���� ������
    virtual void render();

    // Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����ϴ� �� �̿�.
    // �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �޼���� ����.
    virtual void postRender();

    // ���� �ý��ۿ� ��ƼŬ�� ���� ��� true ����.
    bool isEmpty();

    // �ý��� ���� ��ƼŬ�� ��� ���� ��� true ����.
    bool isDead();

    virtual void resetParticle(Attribute* attribute) PURE;   // ���� �����Լ�
    virtual void update(float timeDelta) PURE;   // �ý��� ���� ��� ��ƼŬ���� ����.
protected:
    // �Ӽ� ����Ʈ _particle�� �˻��Ͽ� ���� ��ƼŬ�� ����Ʈ���� ����.
    virtual void removeDeadParticles();
};