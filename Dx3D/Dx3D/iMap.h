#pragma once
#include "stdafx.h"

interface iMap
{
    virtual HRESULT Load(IN char* szFilePath, IN Matrix4* pMat) PURE;                   // �� �ε�
    virtual HRESULT GetHeight(OUT bool& isWalkable, OUT Vector3& vPos) PURE;            // �� ���� �� ���԰��� ���� üũ
    virtual HRESULT GetGroundInfo(OUT E_GROUND_TYPE& eGroundType, IN Vector3 vPos);     // ���� ���� ����
    virtual HRESULT GetEventTrap(OUT E_EVENT_TRAP_TYPE& eTrapType, IN Vector3 vPos);    // �̺�Ʈ Ʈ�� ����
};