#include "stdafx.h"
#include "cMonster.h"
#include "005.UI\\cUILayer.h"


cMonster::cMonster(string szKey, string szFolder, string szFilename ,string szJsonName)
{
    m_eTag = MONSTER;
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename, szJsonName);
    g_pAutoReleasePool->AddObject(m_pMesh);

    for (int i = 0; i < m_pMesh->GetJson()["State"].size(); i++)
    {
        ST_STATE state;
        string str1 = m_pMesh->GetJson()["State"][i]["index"];
        state.nStateNum = atoi(str1.c_str());
        for (int j = 0; j < m_pMesh->GetJson()["State"][i]["Position"].size(); j++)
        {
            string str1 = m_pMesh->GetJson()["State"][i]["Position"][j]["Name"];
            string str2 = m_pMesh->GetJson()["State"][i]["Position"][j]["Value"];
            float pos = atof(str2.c_str());
            state.mapPosition.insert(make_pair(str1, pos));
        }
        string str = m_pMesh->GetJson()["State"][i]["Name"];
        m_mapStateInfo.insert(make_pair(str, state));
    }

    IdleAnim();

    m_stSphere.fRadius = 1.0f;
    m_stSphere.vCenter = m_vPosition;

    m_pPikingMesh = *g_pMeshManager->GetBasicMesh("sphere");
}

cMonster::cMonster()
{
}


cMonster::~cMonster()
{
    SAFE_DELETE(m_pUILayer);
}

void cMonster::Setup()
{
}

void cMonster::Update()
{
    if (!isActive &&  m_pMesh->GetCurPos() >= 1)
    {
        isAlive = false;
        g_pCharacterManager->PushMonster(this);
    }

    if (m_fHP <= 0 && isActive)
    {
        isActive = false;
        m_pTarget->SetTarget(NULL);
        m_stSphere.fRadius = 0;
        DeadAnim();
    }


    if (isActive)
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
                    m_pMesh->SetDescZeroPos();
                    Action("Attack", "5");
                }
            }
            else
            {
                //공격중이었는가?
                if (isAttack)
                {
                    RunAnim();
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
          
        }
    }
    else
    {
     
    }

    Matrix4 matR,matW;
    D3DXMatrixRotationY(&matR, -D3DX_PI/2);
    D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    m_stSphere.vCenter = m_vPosition;
    D3DXMatrixScaling(&m_MatScale, 2, 2, 2);
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
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pPikingMesh->DrawSubset(0);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
