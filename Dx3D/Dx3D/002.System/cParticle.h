#pragma once
#include "cBoundingBox.h"

// 파티클 시스템의 요소들.
// 파티클 시스템은 파티클들의 모임이며 파티클을 보여주고 관리하는 역할을 담당한다.
// 파티클의 크기나 파티클 원천의 위치, 파티클에 적용할 텍스처 등 
// 시스템 내의 모든 파티클에 영향을 주는 전역 특성들을 관리.
// 기능적 관점에서 보면
// 파티클 갱신과 디스플레이, 소멸, 생성 등을 관장하는 역할.

class cParticle
{
protected:
    Vector3                 m_vOriginPos;       //  시스템 내에서 파티클이 시작되는 곳.
    cBoundingBox*           m_pBoundingBox;
    float                   m_fEmitRate;        //  시스템에 새로운 파티클이 추가되는 비율. 보통은 초당 파티클 수로 기록.
    float                   m_fSize;            //  시스템 내 모든 파티클의 크기
    float                   m_fGenTime;         //  파티클 생성 시간
    float                   m_fGenTerm;         //  파티클 생성 주기
    LPTEXTURE9              m_vTexture;         //  시스템이 사용할 텍스쳐 
    LPVTXBUFFER9            m_vertexBuffer;     //  시스템이 사용할 버텍스 버퍼

    ST_PARTICLE_ATTR        m_particleOrigin;   //  파티클 원본
    list<ST_PARTICLE_ATTR>  m_particleList;     //  시스템 내 파티클 속성의 리스트.
                                                //  파티클을 만들고 제거하고 갱신하는 데 이 리스트를 이용.
                                                //  파티클을 그릴 준비가 완료되면 
                                                //  리스트 노드의 일부를 버텍스 버퍼로 복사하고
                                                //  파티클 드로잉 과정을 거친다.
                                                //  이어 다음 단계의 데이터를 복사하고 다시 파티클을 그리며,
                                                //  모든 파티클을 그릴 때까지 이 과정을 반복.

    int                     m_nMaxParticle;     //  주어진 시간 동안 시스템이 가질 수 있는 최대 파티클의 수.
                                                //  예, 파티클이 파괴되는 속도보다 만들어지는 속도가 빠르다면 
                                                //  엄청나게 많은 수의 파티클이 동시에 존재할 수 있다.
                                                //  이 멤버는 이와 같은 상황을 막는다.
    DWORD m_vbBufferSize;                       //  버텍스 버퍼가 보관할 수 있는 파티클의 수. 
                                                //  이 값은 실제 파티클 시스템 내의 파티클 수와는 독립적.
    DWORD m_vbOffset;                           //  파티클 시스템의 렌더링에 이용.
    DWORD m_vbBatchSize;                        //  파티클 시스템의 렌더링에 이용.

public:
    cParticle();                                //  생성자
    cParticle(Vector3* vOriginPos, int nGenNum, int nMaxNum);
    virtual ~cParticle();                       //  소멸자 (버텍스 버퍼, 텍스쳐를 해제) 
    virtual bool Init(string szKey);            //  init - 포인트 스프라이트를 저장하기 위한 버텍스 버퍼를 만들고 텍스쳐를 설정
    virtual void Reset();                       //  시스템 내의 모든 파티클 속성을 리셋
    virtual void Reset(ST_PARTICLE_ATTR attrOrigin);

    // 시스템에 파티클을 추가.
    virtual void AddParticle();

    // 렌더링에 앞서 지정해야 할 초기 렌더 상태를 지정.
    // 이 메서드는 시스템에 따라 달라질 수 있으므로 가상 함수로 선언.
    virtual void PreRender();

    // 시스템 내의 모든 파티클들을 렌더링
    virtual void Render();

    // 특정 파티클 시스템이 지정했을 수 있는 렌더 상태를 복구하는 데 이용.
    // 이 메서드는 시스템에 따라 달라질 수 있으므로 가상 메서드로 선언.
    virtual void PostRender();

    // 현재 시스템에 파티클이 없는 경우 true 리턴.
    bool IsEmpty();

    // 시스템 내의 파티클이 모두 죽은 경우 true 리턴.
    bool IsDead();

    virtual void ResetParticle(ST_PARTICLE_ATTR* attribute);
    virtual void ResetParticle(ST_PARTICLE_ATTR* attrDest, ST_PARTICLE_ATTR attrOrigin);
    virtual void Update();   // 시스템 내의 모든 파티클들을 갱신.

    void SetGenTerm(float fTerm) { m_fGenTerm = fTerm; }
    void SetSize(float size) { m_fSize = size; }
    void SetPosition(Vector3 vPos) { m_vOriginPos = vPos; }

protected:
    // 속성 리스트 _particle을 검색하여 죽은 파티클을 리스트에서 제거.
    virtual void RemoveDeadParticles();
};