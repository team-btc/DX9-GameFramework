#pragma once
#include "iSceneObject.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cBoss.h"
#include "cFrustum.h"
#include "cMapLoader.h"
#include "cGameMap.h"
#include "cWaveShader.h"
#include "cTextureShader.h"
#include "cSkyBoxShader.h"
#include "cParticle.h"
#include "cUILayer.h"

class cUILayer;
class cShop;
class cInventory;

class cPlayScene : public iSceneObject
{
private:
    cUILayer*                       m_pPlayerStatUILayer;
    cUILayer*                       m_pHPUILayer;

    cCamera*                        m_pCamera;
    string                          m_szMapKey;
    ST_MAP_INFO*                    m_stMapInfo;

    cGameMap*                       m_pGameMap;

    cPlayer*                        m_pPlayer;
    cFrustum*                       m_pFrustum;
    vector<iCharacterObject*>*      m_vecMonster;

    cTextureShader*                 m_pTextureShader;
    cSkyBoxShader*                  m_pSkyBoxShader;
    cWaveShader*                    m_pWaveShader;
    
    cShop*                          m_pShop;
    cInventory*                     m_pInventory;

    cParticle*                      m_pParticleFrost;

    vector<string>                  m_VecSzDeathwingAttack;
    vector<string>                  m_VecSzPlayerAttack;
    
private:
    void SetUI();
    void UpdateUI();
    void SetupSound();
    bool                            m_isRoar;

    // 사운드 관련 변수
    bool                            m_isWalk;
    float                           m_fWalkTime;
public:
    cPlayScene();
    ~cPlayScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

    void ParseEvent(string szCommand);
    void TransportMap(string szMap);
};

