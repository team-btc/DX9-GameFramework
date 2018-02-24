#pragma once
#include "stdafx.h"

#define TILE_N 256
#define VERTEX_DIM (TILE_N + 1)

/*
인터페이스 : 순수 가상 함수로 구현
- 인터페이스를 상속 받은 클래스들에 구현 해야될 함수들을 순수 가상 함수로 설계해 놓는다.
*/

interface iMap
{
    virtual void Load(IN char* szFilePath, IN Matrix4* pMat) PURE;
    virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) PURE;
    virtual vector<Vector3>& GetVertex() PURE;
    virtual vector<DWORD>& GetIndex() PURE;
};