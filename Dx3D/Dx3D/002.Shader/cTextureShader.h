#pragma once

class cTextureShader
{
    LPMESH                  m_pMesh;
    LPEFFECT                m_pTextureShader;
    Vector4                 m_vPickPos;
    float                   m_fBGDensity;
    LPTEXTURE9              m_pBGTexture;
    LPTEXTURE9              m_pTexture[3];
    float                   m_TexDensity[3];                                            // 타일의 밀도
    LPTEXTURE9              m_pAlphaDraw;

public:
    cTextureShader();
    ~cTextureShader();

    void SetBGTexture(LPTEXTURE9 pTex, float fDensity);
    void SetTexture1(LPTEXTURE9 pTex, float fDensity);
    void SetTexture2(LPTEXTURE9 pTex, float fDensity);
    void SetTexture3(LPTEXTURE9 pTex, float fDensity);
    void SetMesh(LPMESH mesh) { m_pMesh = mesh; }
    void SetAlphaDraw(LPTEXTURE9 AlphaDraw) { m_pAlphaDraw = AlphaDraw; }
    void Render();
    void SetPlayerPos(Vector3 pos);
    void SetTargetPos(Vector3* pos);
};

