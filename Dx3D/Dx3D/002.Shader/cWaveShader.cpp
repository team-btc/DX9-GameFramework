#include "stdafx.h"
#include "cWaveShader.h"


cWaveShader::cWaveShader()
    : gWorldLightPosition(10.0f, 1500.0f, 0.0, 1.0f)
    , gLightColor(0.7f, 0.7f, 1.0f, 1.0f)
    , m_pMesh(NULL)
    , m_pTexture(NULL)
{
    g_pShaderManager->AddEffect("Wave", SHADER_PATH + (string)"FX/WaveShader.fx");
    m_pWaveShader = g_pShaderManager->GetEffect("Wave");
}

cWaveShader::~cWaveShader()
{
}

void cWaveShader::SetShader(float fHeight, float fWaveHeight, float fSpeed, float fUVSpeed, float fWaveFrequency, float fTransparent, float fDensity)
{
    m_pHeight = fHeight;
    m_pWaveHeight = fWaveHeight;
    m_pSpeed = fSpeed;
    m_pUVSpeed = fUVSpeed;
    m_pWaveFrequency = fWaveFrequency;
    m_pTransparent = fTransparent;
    m_fDensity = fDensity;
}

void cWaveShader::Render(D3DXVECTOR4 vCameraPosition)
{
    if (m_pTexture == NULL || m_pMesh == NULL)
    {
        return;
    }

    D3DXMATRIXA16 matW, matView, matProjection;
    D3DXMatrixIdentity(&matW);
    matW._42 += m_pHeight - 128;

    D3DXVECTOR4 gWorldCameraPosition = vCameraPosition;
    g_pDevice->GetTransform(D3DTS_VIEW, &matView);
    g_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

    m_pWaveShader->SetMatrix("gWorldMatrix", &matW);
    m_pWaveShader->SetMatrix("gViewMatrix", &matView);
    m_pWaveShader->SetMatrix("gProjectionMatrix", &matProjection);
    m_pWaveShader->SetTexture("DiffuseMap_Tex", m_pTexture);

    m_pWaveShader->SetVector("gWorldLightPosition", &gWorldLightPosition);
    m_pWaveShader->SetVector("gWorldCameraPosition", &gWorldCameraPosition);
    m_pWaveShader->SetVector("gLightColor", &gLightColor);

    m_pWaveShader->SetFloat("gWaveHeight", m_pWaveHeight);                          // �ö󰡴� ����(����)
    m_pWaveShader->SetFloat("gSpeed", m_pSpeed);                                    // �������������ϴ� �ӵ�
    m_pWaveShader->SetFloat("gWaveFrequency", m_pWaveFrequency);                    // �ⷷ�̴� ��
    m_pWaveShader->SetFloat("gUVSpeed", m_pUVSpeed);                                // �ؽ��İ� �����̴� �ӵ�
    m_pWaveShader->SetFloat("gTransparent", m_pTransparent);                        // ����
    m_pWaveShader->SetFloat("Density", m_fDensity);                                 // ����

                                                      
    ULONGLONG tick = GetTickCount64();                                               // �ý��� �ð��� ����
    m_pWaveShader->SetFloat("gTime", tick / 1000.0f);

    UINT numPasses = 0;
    m_pWaveShader->Begin(&numPasses, NULL);
    {
        for (UINT i = 0; i < numPasses; ++i)
        {
            m_pWaveShader->BeginPass(i);
            {
                m_pMesh->DrawSubset(0);
            }
            m_pWaveShader->EndPass();
        }
    }
    m_pWaveShader->End();
}
