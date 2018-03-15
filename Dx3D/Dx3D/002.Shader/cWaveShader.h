#pragma once

class cWaveShader
{
private:
    LPMESH				    m_pMesh;
    LPD3DXEFFECT			m_pWaveShader;
    LPTEXTURE9              m_pTexture;

    D3DXVECTOR4             gWorldLightPosition;
    D3DXVECTOR4             gLightColor;

    // Wave ����
    float m_pHeight;                  // ���� ���������� ����
    float m_pWaveHeight;              // ����
    float m_pSpeed;                   // �����ӵ�
    float m_pUVSpeed;                 // �ؽ��İ� �������� �ӵ�
    float m_pWaveFrequency;           // ������ ����
    float m_pTransparent;             // ���� ����
public:
    cWaveShader();
    ~cWaveShader();

    // Height : ���� ���������� ����, WaveHeight : ���� , Speed : �����ӵ�, UVSpeed : �ؽ��İ� �������� �ӵ�, WaveFrequency : ������ ����, Transparent : ���� ����
    void SetShader(/* char * sTexFile, char * sTexFileKey, */float fHeight = 0.0f, float fWaveHeight = 0.6f, float fSpeed = 2.4f,
        float fUVSpeed = 7.0f, float fWaveFrequency = 0.01f, float fTransparent = 0.6f); 
    void SetMesh(LPMESH mesh) { m_pMesh = mesh; }
    void SetWaveTexture(LPTEXTURE9 texture) { m_pTexture = texture; }

    void Render(D3DXVECTOR4 vCameraPosition);
};

