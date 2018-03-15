#pragma once
#include "iSceneObject.h"

class cWaveShader;
class cTextureShader;
class cSkyBoxShader;

class cMapLoad : public iSceneObject
{
private:
    ST_MAP*             m_stMapInfo;

    cTextureShader*     m_pTextureShader;
    cSkyBoxShader*      m_pSkyBoxShader;
    cWaveShader*        m_pWaveShader;

public:
    cMapLoad();
    ~cMapLoad();

    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
};

