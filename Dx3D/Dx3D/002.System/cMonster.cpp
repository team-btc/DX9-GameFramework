#include "stdafx.h"
#include "cMonster.h"


cMonster::cMonster(string szKey, string szFolder, string szFilename)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename);
    g_pAutoReleasePool->AddObject(m_pMesh);
    m_stSphere.fRadius = 1.0f;
    m_stSphere.vCenter = m_vPosition;
    

    m_pPikingMesh = *g_pMeshManager->GetBasicMesh("sphere");
}

cMonster::cMonster()
{
}


cMonster::~cMonster()
{
}

void cMonster::Setup()
{
}

void cMonster::Update()
{
    if (m_fHP <= 0)
        isAlive = false;
    if (isAlive)
    {
        if (m_pTarget)
        {
            Vector3 Dir = m_pTarget->GetPosition() - m_vPosition;
            float Distance = D3DXVec3Length(&Dir);
            D3DXVec3Normalize(&Dir, &Dir);

            if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius)
            {
                //뛰는중이었는가?
                if (!isAttack)
                {
                    AttackAnim();
                    isAttack = true;
                }

                if (m_pMesh->GetCurPos() >= 1)
                {
                    Action("Attack", "5");
                }
            }
            else
            {
                //공격중이었는가?
                if (isAttack)
                {
                    m_pMesh->SetAnimationIndex(3);
                    isAttack = false;
                }

                m_vPosition += Dir * 0.005;
                m_stSphere.vCenter = m_vPosition;

                D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
            }
            D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
            D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);
        }
        else
        {
            m_pMesh->SetAnimationIndex(4);
        }
    }
    else
    {
        m_pMesh->SetAnimationIndex(4);
    }

    Matrix4 matR,matW;
    D3DXMatrixRotationY(&matR, D3DX_PI);
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    m_stSphere.vCenter = m_vPosition;
    matW = m_MatScale * m_MatRotate* matR * m_MatTrans;
    m_pMesh->SetWorldMatrix(matW);
}

void cMonster::Render()
{
    m_pMesh->UpdateAndRender();

    Matrix4 matW;
    D3DXMatrixTranslation(&m_MatTrans, m_stSphere.vCenter.x, m_stSphere.vCenter.y + 0.5f, m_stSphere.vCenter.z);
    matW = m_MatScale * m_MatRotate * m_MatTrans;
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
   // m_pPikingMesh->DrawSubset(0);
}

void cMonster::Destroy()
{
}
