#pragma once
#include "iSceneObject.h"

class cEndingScene : public iSceneObject
{
public:
    cEndingScene();
    ~cEndingScene();

    // iSceneObject��(��) ���� ��ӵ�
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
};

