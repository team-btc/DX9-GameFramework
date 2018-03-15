#pragma once
class cSkyBoxShader
{
private:
    LPD3DXMESH				m_pMesh; 
    LPD3DXEFFECT			m_pSkyBoxShader;
    LPD3DXBUFFER            m_pMaterial;                // x파일 불러오기 위해 필요함
    LPDIRECT3DTEXTURE9	    m_pSkyBoxTexture;           // 
    LPCUBETEXTURE9         m_pCubeTexture;             // dds 파일 입힐때 필요함

public:
    cSkyBoxShader();
    ~cSkyBoxShader();

    void SetBox(LPCUBETEXTURE9 pSkyBox) { m_pCubeTexture = pSkyBox; }
    void Render(D3DXVECTOR4 vCameraPosition);
};

