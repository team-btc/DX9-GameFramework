#pragma once
#include "cBoundingBox.h"

#define TEMP_AGE (0.5f)
#define TEMP_MULTIPLE (10.0f)

// ��ƼŬ �ý����� ��ҵ�.
// ��ƼŬ �ý����� ��ƼŬ���� �����̸� ��ƼŬ�� �����ְ� �����ϴ� ������ ����Ѵ�.
// ��ƼŬ�� ũ�⳪ ��ƼŬ ��õ�� ��ġ, ��ƼŬ�� ������ �ؽ�ó �� 
// �ý��� ���� ��� ��ƼŬ�� ������ �ִ� ���� Ư������ ����.
// ����� �������� ����
// ��ƼŬ ���Ű� ���÷���, �Ҹ�, ���� ���� �����ϴ� ����.

class cParticle
{
protected:
    Vector3                 m_vOriginPos;       //  �ý��� ������ ��ƼŬ�� ���۵Ǵ� ��.
    cBoundingBox*           m_pBoundingBox;
    float                   m_fEmitRate;        //  �ý��ۿ� ���ο� ��ƼŬ�� �߰��Ǵ� ����. ������ �ʴ� ��ƼŬ ���� ���.
    float                   m_fSize;            //  �ý��� �� ��� ��ƼŬ�� ũ��
    float                   m_fGenTime;         //  ��ƼŬ ���� �ð�
    float                   m_fGenTerm;         //  ��ƼŬ ���� �ֱ�
    LPTEXTURE9              m_vTexture;         //  �ý����� ����� �ؽ��� 
    LPVTXBUFFER9            m_vertexBuffer;     //  �ý����� ����� ���ؽ� ����

    ST_PARTICLE_ATTR        m_particleOrigin;   //  ��ƼŬ ����
    list<ST_PARTICLE_ATTR>  m_particleList;     //  �ý��� �� ��ƼŬ �Ӽ��� ����Ʈ.
                                                //  ��ƼŬ�� ����� �����ϰ� �����ϴ� �� �� ����Ʈ�� �̿�.
                                                //  ��ƼŬ�� �׸� �غ� �Ϸ�Ǹ� 
                                                //  ����Ʈ ����� �Ϻθ� ���ؽ� ���۷� �����ϰ�
                                                //  ��ƼŬ ����� ������ ��ģ��.
                                                //  �̾� ���� �ܰ��� �����͸� �����ϰ� �ٽ� ��ƼŬ�� �׸���,
                                                //  ��� ��ƼŬ�� �׸� ������ �� ������ �ݺ�.

    int                     m_nMaxParticle;     //  �־��� �ð� ���� �ý����� ���� �� �ִ� �ִ� ��ƼŬ�� ��.
                                                //  ��, ��ƼŬ�� �ı��Ǵ� �ӵ����� ��������� �ӵ��� �����ٸ� 
                                                //  ��û���� ���� ���� ��ƼŬ�� ���ÿ� ������ �� �ִ�.
                                                //  �� ����� �̿� ���� ��Ȳ�� ���´�.
    DWORD m_vbBufferSize;                       //  ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ�� ��. 
                                                //  �� ���� ���� ��ƼŬ �ý��� ���� ��ƼŬ ���ʹ� ������.
    DWORD m_vbOffset;                           //  ��ƼŬ �ý����� �������� �̿�.
    DWORD m_vbBatchSize;                        //  ��ƼŬ �ý����� �������� �̿�.

    bool        m_isRandomPos;
    Vector3     m_vMinRange;
    Vector3     m_vMaxRange;
    bool        m_isBlack;
private:
    void UseRandomPosition(bool bUseRandom) { m_isRandomPos = bUseRandom; }
    void SetRandomRange(Vector3 vMinRange, Vector3 vMaxRange) { m_vMinRange = vMinRange, m_vMaxRange = vMaxRange; }
    void AddParticleCircle();
public:
    //  ������
    cParticle();             
    cParticle(Vector3* vOriginPos, int nGenNum, int nMaxNum);
    cParticle(Vector3* vOriginPos, int nGenNum, int nMaxNum, bool bRandomPos, Vector3 vMinRange, Vector3 vMaxRange);
    cParticle(Vector3* vOriginPos, int nMaxNum);

    virtual ~cParticle();                       //  �Ҹ��� (���ؽ� ����, �ؽ��ĸ� ����) 
    virtual bool Init(string szKey);            //  init - ����Ʈ ��������Ʈ�� �����ϱ� ���� ���ؽ� ���۸� ����� �ؽ��ĸ� ����
    virtual void Reset();                       //  �ý��� ���� ��� ��ƼŬ �Ӽ��� ����
    virtual void Reset(ST_PARTICLE_ATTR attrOrigin);

    // �ý��ۿ� ��ƼŬ�� �߰�.
    virtual void AddParticle();

    // �������� �ռ� �����ؾ� �� �ʱ� ���� ���¸� ����.
    // �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �Լ��� ����.
    virtual void PreRender();

    // �ý��� ���� ��� ��ƼŬ���� ������
    virtual void Render();

    // Ư�� ��ƼŬ �ý����� �������� �� �ִ� ���� ���¸� �����ϴ� �� �̿�.
    // �� �޼���� �ý��ۿ� ���� �޶��� �� �����Ƿ� ���� �޼���� ����.
    virtual void PostRender();

    // ���� �ý��ۿ� ��ƼŬ�� ���� ��� true ����.
    bool IsEmpty();

    // �ý��� ���� ��ƼŬ�� ��� ���� ��� true ����.
    bool IsDead();

    virtual void ResetParticle(ST_PARTICLE_ATTR* attribute);
    virtual void ResetParticle(ST_PARTICLE_ATTR* attrDest, ST_PARTICLE_ATTR attrOrigin);
    virtual void Update();   // �ý��� ���� ��� ��ƼŬ���� ����.

    void SetGenTerm(float fTerm) { m_fGenTerm = fTerm; }
    void SetSize(float size) { m_fSize = size; }
    
    void SetPosition(Vector3 vPos) { m_vOriginPos = vPos; }
    void RemoveBlack(bool bRemove) { m_isBlack = bRemove; }

    void SetOriginParticle(ST_PARTICLE_ATTR origin) { m_particleOrigin = origin; }
    void UpdateCircle();

protected:
    // �Ӽ� ����Ʈ _particle�� �˻��Ͽ� ���� ��ƼŬ�� ����Ʈ���� ����.
    virtual void RemoveDeadParticles();
};