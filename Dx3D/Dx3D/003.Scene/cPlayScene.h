#pragma once
#include "iSceneObject.h"

class cPlayScene : public iSceneObject
{
public:
    cPlayScene();
    ~cPlayScene();

    // iSceneObject��(��) ���� ��ӵ�
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
};

