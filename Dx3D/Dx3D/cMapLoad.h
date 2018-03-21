#pragma once
#include "iSceneObject.h"
#include "cPlayer.h"
#include "cMonster.h"

class cFrustum;
class cGameMap;
class cWaveShader;
class cTextureShader;
class cSkyBoxShader;

class cMapLoad : public iSceneObject
{
private:
    ST_MAP_INFO*        m_stMapInfo;

    cGameMap*           m_pGameMap;

    cPlayer*                    m_pPlayer;
    cFrustum*                   m_pFrustum;
    vector<iCharacterObject*>*  m_vecMonster;

    cTextureShader*     m_pTextureShader;
    cSkyBoxShader*      m_pSkyBoxShader;
    cWaveShader*        m_pWaveShader;

public:
    cMapLoad();
    ~cMapLoad();

    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;

    Vector3 GetPlayerPos() { return m_pPlayer->GetPosition(); }
};

