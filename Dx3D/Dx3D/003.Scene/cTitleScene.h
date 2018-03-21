#pragma once
#include "iSceneObject.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cMonster.h"
#include "cFrustum.h"
#include "cMapLoader.h"
#include "cGameMap.h"
#include "cWaveShader.h"
#include "cTextureShader.h"
#include "cSkyBoxShader.h"
#include "cSindragosa.h"
#include "cSkinnedMesh.h"
#include "cUILayer.h"
#include "cUIImageView.h"

#define ARTHUS  1.27f
class cTitleScene : public iSceneObject
{
private:
    cCamera*            m_pCamera;

    LPMESH              m_pTerrain;
    LPMESH              m_pWater;
    cTextureShader*     m_pTextureShader;
    cSkyBoxShader*      m_pSkyBoxShader;
    cWaveShader*        m_pWaveShader;

    cUILayer*           m_pBGLayer;
    
    cSkinnedMesh*       m_pArthas;
    cSkinnedMesh*       m_pSindragosa;

    vector<ST_PNT_VERTEX>       m_vecWPNTVertex;
    vector<int>                 m_vecWVertexIndex;
    
    //갱신되는 포지션
    Vector3             m_vSindraPos;
    Vector3             m_vArthasPos;

    Vector3             m_vStartPos;
    Vector3             m_vCameraPos;
    Vector3             m_vArthusStartPos;
    Vector3             m_vArthusLastPos;

    vector<Vector3>     m_vecSindraJumpTarget;
    vector<Vector3>     m_vecSindraJumpBezier;

    //  목적지에 도착했는감
    vector<bool>        m_vecIsArriveSindra;
    int                 m_nCurrIndex;

    float               m_fWorldTime;

public:
    cTitleScene();
    ~cTitleScene();

    // iSceneObject을(를) 통해 상속됨
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

    void SetAnimation(string szAnimName, cSkinnedMesh* pMesh);
};
