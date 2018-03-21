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

    string              m_szMapKey;
    ST_MAP_INFO*        m_stMapInfo;

    cGameMap*           m_pGameMap;

    cPlayer*            m_pPlayer;
    cFrustum*           m_pFrustum;
    cSindragosa*        m_pSindragosa;
    cUILayer*           m_pBGLayer;
    cUILayer*           m_pSGLayer;
    cTextureShader*     m_pTextureShader;
    cSkyBoxShader*      m_pSkyBoxShader;
    cWaveShader*        m_pWaveShader;
    cSkinnedMesh*       m_pArthus;
    //=================================== �ٴ�..
    LPMESH              m_pBlackMesh;
    vector<ST_PNT_VERTEX>       m_vecWPNTVertex;
    vector<int>                 m_vecWVertexIndex;
    // ����ŸƮ, ���� ��ŸƮ
    bool                m_isMovieStart;
    bool                m_isStart;
    bool                m_isArthusMove;
    bool                m_isArthusRender;
    bool                m_isReady;
    //���ŵǴ� ������
    Vector3             m_vSindraPos;
    Vector3             m_vArtuhsPos;

    Vector3             m_vStartPos;
    Vector3             m_vCameraPos;
    Vector3             m_vArthusStartPos;
    Vector3             m_vArthusLastPos;
    Vector3             m_vFlyRotation;

    vector<Vector3>     m_vecSindraJumpTarget;
    vector<Vector3>     m_vecSindraJumpBezier;
    //  �������� �����ߴ°�
    vector<bool>        m_vecIsArriveSindra;
    int                 m_nCurrIndex;

    bool                m_isRoar;
    // ��������
    float               m_ft;
    float               m_fWorldTime;


public:
    cTitleScene();
    ~cTitleScene();

    // iSceneObject��(��) ���� ��ӵ�
    virtual HRESULT Start() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual ULONG Release() override;
    virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

    // ���� ������ �̵� ���� int�� �̵� �� �ε���
    void MoveSindra(Vector3 vSpot, int n);
    void MoveArthus();
    void MoveSindraAllRoute();
    
    void SetSindragosa();
};
