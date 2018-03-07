#pragma once
#include "cCamera.h"
#include "cMeshLoader.h"
class cMainGame
{
private:
    cCamera*    m_pCamera;
    LPMESH      m_pMesh;
    cMeshLoader* m_pMeshLoader;
    LPEFFECT    m_pEffect;

    D3DXHANDLE  g_hAmbient = NULL;
    D3DXHANDLE  g_hDiffuse = NULL;
    D3DXHANDLE  g_hSpecular = NULL;
    D3DXHANDLE  g_hOpacity = NULL;
    D3DXHANDLE  g_hSpecularPower = NULL;
    D3DXHANDLE  g_hLightColor = NULL;
    D3DXHANDLE  g_hLightPosition = NULL;
    D3DXHANDLE  g_hCameraPosition = NULL;
    D3DXHANDLE  g_hTexture = NULL;
    D3DXHANDLE  g_hTime = NULL;
    D3DXHANDLE  g_hWorld = NULL;
    D3DXHANDLE  g_hWorldViewProjection = NULL;

public:
    cMainGame();
    ~cMainGame();

    void Setup();
    void Update();
    void Render();

    void RenderSubset(UINT i);

    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};