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
fopen ���� �о fgetc �� ����Ͽ� �ȼ� �ϳ��� ������ �޴´�.
char : 1byte, 8bit ==> 2^8 = 0 ~ 255

����Ʈ�� ������ 257 * 257

���ؽ� ���� 257 * 257
Ÿ���� ���� 256 * 256

1. PNT	=> P : X, Z �о�帰 �������
�ε��� ��ȣ = z * 257 + x
y = �� ���� / 10.0f
=> N : 0, 1, 0
=> T : 0 ~ 1

2. �ε��� ���� (�ð� ����)
1--3	0 = z * 257 + x
|\ |	1 = (z + 1) * 257 + x
| \|	2 = z * 257 + x + 1
0--2	3 = (z + 1) * 257 + x + 1

3. �븻�� ����
---u---
|\ |\ |
| \| \|
L--n--r
|\ |\ |
| \| \|
---d---
du ���Ϳ� lr ���͸� �����ؼ� ���� ��ġ�� �븻 ���� ���Ѵ�.

4. �޽� ���� �� ���, ����ȭ

5. MtlTex ���� ����

================================================================
GetHeight �Լ�
ĳ������ ���� ����

1. ���� ���̽� ����
1--3	0�� x, z ��ǥ ã��
|\ |	x = (int)ĳ���� ��ġx
| \|	z = (int)ĳ���� ��ġz
0--2
deltaX = ��ġx - x
deltaZ = ��ġz - z

deltaX + deltaZ < 1 : �Ʒ��� �ﰢ��
deltaX + deltaZ >= 1 : ���� �ﰢ��

�� �� ������ �������� ���
1--3
|\ |	�Ʒ��� �ﰢ���� ��� : 0.y + x �� ���� ���� * ��Ÿx + z �� ���� ���� * ��Ÿz
| \|	���� �ﰢ���� ��� : 3.y + x �� ���� ���� * (1.0f - ��Ÿx) + z �� ���� ���� * (1.0f - ��Ÿz)
0--2
*/