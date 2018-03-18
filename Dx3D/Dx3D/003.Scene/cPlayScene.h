#pragma once
#include "iSceneObject.h"

class cPlayScene : public iSceneObject
{
public:
    cPlayScene();
    ~cPlayScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
};

