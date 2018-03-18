#pragma once
#include "iSceneObject.h"

class cLoadingScene : public iSceneObject
{
public:
    cLoadingScene();
    ~cLoadingScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
};

