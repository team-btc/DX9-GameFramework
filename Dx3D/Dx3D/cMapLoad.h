#pragma once
#include "iSceneObject.h"

class cGameMap;
class cWaveShader;
class cTextureShader;
class cSkyBoxShader;

class cMapLoad : public iSceneObject
{
private:
    ST_MAP_INFO*        m_stMapInfo;

    cGameMap*           m_pGameMap;

    cTextureShader*     m_pTextureShader;
    cSkyBoxShader*      m_pSkyBoxShader;
    cWaveShader*        m_pWaveShader;

    LPMESH              m_pSphereMesh;
    Vector3             m_vSpherePos;
    Vector3             m_vDirection;
    float               m_fRotY;
    Matrix4             m_matWorld;

public:
    cMapLoad();
    ~cMapLoad();

    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
};

