#include "stdafx.h"
#include "cSkyBoxShader.h"


cSkyBoxShader::cSkyBoxShader()
    :m_pMesh(NULL)
{
    g_pShaderManager->AddEffect("SkyBox", SHADER_PATH + (string)"FX/SkyBox.fx");
    m_pSkyBoxShader = g_pShaderManager->GetEffect("SkyBox");
    g_pMeshManager->LoadMesh("Box", SHADER_PATH + (string)"Model/Box.x");
    m_pMesh = g_pMeshManager->GetBasicMesh("Box");
}


cSkyBoxShader::~cSkyBoxShader()
{
}

void cSkyBoxShader::Render(D3DXVECTOR4 vCameraPosition)
{
    D3DXMATRIXA16 matW, matView, matProjection, matViewProjection;
    D3DXMatrixIdentity(&matW);
   
    D3DXVECTOR4 gWorldCameraPosition = vCameraPosition;

    g_pDevice->GetTransform(D3DTS_VIEW, &matView);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
    matViewProjection = matView * matProjection;

    m_pSkyBoxShader->SetMatrix("ViewProjection", &matViewProjection);
    m_pSkyBoxShader->SetVector("ViewPosition", &gWorldCameraPosition);
    m_pSkyBoxShader->SetTexture("Sky_Tex", m_pCubeTexture);

    UINT numPasses = 0;
    m_pSkyBoxShader->Begin(&numPasses, NULL);
    {
        for (UINT i = 0; i < numPasses; ++i)
        {
            m_pSkyBoxShader->BeginPass(i);
            {
                m_pMesh->DrawSubset(0);
            }
            m_pSkyBoxShader->EndPass();
        }
    }
    m_pSkyBoxShader->End();
}



