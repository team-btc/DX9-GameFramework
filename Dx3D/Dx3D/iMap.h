#pragma once
#include "stdafx.h"

#define TILE_N 256
#define VERTEX_DIM (TILE_N + 1)

/*
�������̽� : ���� ���� �Լ��� ����
- �������̽��� ��� ���� Ŭ�����鿡 ���� �ؾߵ� �Լ����� ���� ���� �Լ��� ������ ���´�.
*/

interface iMap
{
    virtual void Load(IN char* szFilePath, IN Matrix4* pMat) PURE;
    virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) PURE;
    virtual vector<Vector3>& GetVertex() PURE;
    virtual vector<DWORD>& GetIndex() PURE;
};