#pragma once
#include "iSceneObject.h"

class cTitleScene : public iSceneObject
{
private:


public:
    cTitleScene();
    ~cTitleScene();

    // iSceneObject��(��) ���� ��ӵ�
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
};
