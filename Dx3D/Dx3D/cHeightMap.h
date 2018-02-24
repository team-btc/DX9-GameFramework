#pragma once
#include "cObject.h"
#include "iMap.h"

class cMtlTex;

class cHeightMap : public cObject, public iMap
{
private:
    LPMESH                  m_pMesh;
    vector<Vector3>         m_vecVertex;
    vector<DWORD>           m_vecIndex;
    cMtlTex*                m_pMtlTex;

    float                   m_fSizeX;
    float                   m_fSizeZ;

public:
    cHeightMap();
    ~cHeightMap();

    virtual void Load(IN char* szFilePath, IN Matrix4* pMat = NULL) override;
    virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) override;
    virtual vector<Vector3>& GetVertex() override;
    virtual vector<DWORD>& GetIndex() override;
    void Render();
};

/*
fopen 으로 읽어서 fgetc 를 사용하여 픽셀 하나씩 정보를 받는다.
char : 1byte, 8bit ==> 2^8 = 0 ~ 255

하이트맵 사이즈 257 * 257

버텍스 갯수 257 * 257
타일의 갯수 256 * 256

1. PNT	=> P : X, Z 읽어드린 순서대로
인덱스 번호 = z * 257 + x
y = 색 정보 / 10.0f
=> N : 0, 1, 0
=> T : 0 ~ 1

2. 인덱스 구성 (시계 방향)
1--3	0 = z * 257 + x
|\ |	1 = (z + 1) * 257 + x
| \|	2 = z * 257 + x + 1
0--2	3 = (z + 1) * 257 + x + 1

3. 노말값 셋팅
---u---
|\ |\ |
| \| \|
L--n--r
|\ |\ |
| \| \|
---d---
du 벡터와 lr 벡터를 외적해서 현재 위치의 노말 값을 구한다.

4. 메쉬 생성 및 기록, 최적화

5. MtlTex 정보 셋팅

================================================================
GetHeight 함수
캐릭터의 높이 설정

1. 기준 페이스 선택
1--3	0의 x, z 좌표 찾기
|\ |	x = (int)캐릭터 위치x
| \|	z = (int)캐릭터 위치z
0--2
deltaX = 위치x - x
deltaZ = 위치z - z

deltaX + deltaZ < 1 : 아래쪽 삼각형
deltaX + deltaZ >= 1 : 윗쪽 삼각형

두 점 사이의 높이차이 계산
1--3
|\ |	아래쪽 삼각형일 경우 : 0.y + x 축 높이 차이 * 델타x + z 축 높이 차이 * 델타z
| \|	윗쪽 삼각형일 경우 : 3.y + x 축 높이 차이 * (1.0f - 델타x) + z 축 높이 차이 * (1.0f - 델타z)
0--2
*/