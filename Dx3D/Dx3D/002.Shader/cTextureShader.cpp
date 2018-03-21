#include "stdafx.h"
#include "cTextureShader.h"

cTextureShader::cTextureShader()
{
    g_pShaderManager->AddEffect("rendtex", SHADER_PATH + (string)"FX/Ingame_TextureShader.fx");
    m_pTextureShader = g_pShaderManager->GetEffect("rendtex");
    ZeroMemory(m_pTexture, 3);

    for (int i = 1; i < 3; ++i)
    {
        m_TexDensity[i] = 0.0f;
    }
}

cTextureShader::~cTextureShader()
{
}

void cTextureShader::SetBGTexture(LPTEXTURE9 pTex, float fDensity)
{
    m_pBGTexture = pTex;
    m_fBGDensity = fDensity * 0.1f;
}

void cTextureShader::SetTexture1(LPTEXTURE9 pTex, float fDensity)
{
    m_pTexture[0] = pTex;
    m_TexDensity[0] = fDensity * 0.1f;
}

void cTextureShader::SetTexture2(LPTEXTURE9 pTex, float fDensity)
{
    m_pTexture[1] = pTex;
    m_TexDensity[1] = fDensity * 0.1f;
}

void cTextureShader::SetTexture3(LPTEXTURE9 pTex, float fDensity)
{
    m_pTexture[2] = pTex;
    m_TexDensity[2] = fDensity * 0.1f;
}

void cTextureShader::Render()
{
    D3DXMATRIXA16 matW, matView, matProjection;
    D3DXMatrixIdentity(&matW);

    g_pDevice->GetTransform(D3DTS_VIEW, &matView);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

    m_pTextureShader->SetMatrix("gWorldMatrix", &matW);
    m_pTextureShader->SetMatrix("gViewMatrix", &matView);
    m_pTextureShader->SetMatrix("gProjectionMatrix", &matProjection);

    m_pTextureShader->SetTexture("BackGroundTexture", m_pBGTexture);
    m_pTextureShader->SetTexture("texture1", m_pTexture[0]);
    m_pTextureShader->SetTexture("texture2", m_pTexture[1]);
    m_pTextureShader->SetTexture("texture3", m_pTexture[2]);
    
    m_pTextureShader->SetTexture("AlphaMap", m_pAlphaDraw);

    m_pTextureShader->SetFloat("Density", 0);

    m_pTextureShader->SetFloat("BackGroundDensity", m_fBGDensity);
    m_pTextureShader->SetFloat("Tex1Density", m_TexDensity[0]);
    m_pTextureShader->SetFloat("Tex2Density", m_TexDensity[1]);
    m_pTextureShader->SetFloat("Tex3Density", m_TexDensity[2]);
    
   UINT numPasses = 0;
   m_pTextureShader->Begin(&numPasses, NULL);
   {
       for (UINT i = 0; i < numPasses; ++i)
       {
           m_pTextureShader->BeginPass(i);
           {
               m_pMesh->DrawSubset(0);
           }
           m_pTextureShader->EndPass();
       }
   }
   m_pTextureShader->End();
}

void cTextureShader::SetPlayerPos(Vector3 pos)
{
    Vector4 playerPos(pos.x, pos.y, pos.z, 1.0f);
    m_pTextureShader->SetVector("PlayerPos", &playerPos);
    m_pTextureShader->SetVector("PlayerColor", &Vector4(10, 255, 10, 255));
    m_pTextureShader->SetFloat("PlayerScale", 0.015f);
}

void cTextureShader::SetTargetPos(Vector3* target)
{
    if (target)
    {
        Vector4 targetPos(target->x, target->y, target->z, 1.0f);
        m_pTextureShader->SetVector("TargetPos", &targetPos);
        m_pTextureShader->SetVector("TargetColor", &Vector4(255, 10, 10, 255));
        m_pTextureShader->SetFloat("TargetScale", 0.02f);
    }
    else
    {
        m_pTextureShader->SetVector("TargetColor", &Vector4(0, 0, 0, 0));
    }
}
