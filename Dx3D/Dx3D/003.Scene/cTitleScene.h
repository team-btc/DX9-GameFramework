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
    cUILayer*           m_pSCLayer;

    cSkinnedMesh*       m_pArthas;
    cSkinnedMesh*       m_pSindragosa;

    vector<ST_PNT_VERTEX>       m_vecWPNTVertex;
    vector<int>                 m_vecWVertexIndex;
    
    Vector3             m_vSindraPos;
    Vector3             m_vArthasPos;

    Vector3             m_vStartPos;
    Vector3             m_vCameraPos;
    Vector3             m_vArthusStartPos;
    Vector3             m_vArthusLastPos;

    int                 m_nCurrIndex;

    float               m_fWorldTime;
    float               m_fSpeed;
    
    bool                m_isPressSpace;

    bool                m_isPopup;
    
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
