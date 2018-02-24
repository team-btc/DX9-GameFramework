#pragma once
#include "cObject.h"
#include "iMap.h"

/*
�������̽��� ��� ���� Ŭ������ �������̽��� ���� �Ǿ� �ִ� ���� �����Լ�����
'������' ���� ���� �ؾ��Ѵ�.
*/

class cObjMap : public cObject, public iMap
{
private:
    vector<Vector3> m_vecVertex;

public:
    cObjMap();
    ~cObjMap();

    virtual void Load(IN char* szFilePath, IN Matrix4* pMat) override;
    virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) override;
    //virtual void Test() override { int a = 10; }
};

