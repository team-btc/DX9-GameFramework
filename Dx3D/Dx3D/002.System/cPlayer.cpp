#include "stdafx.h"
#include "cPlayer.h"
#include "003.Object\cSkinnedMesh.h"
#include "cMonster.h"

cPlayer::cPlayer(string szKey, string szFolder, string szFilename,string szJsonName)
    :DestPoint(0,0,0)
    , isMoveToPoint(false)
    , isPoint(false)
{
    m_pMesh = new cSkinnedMesh(szKey, szFolder, szFilename , szJsonName);
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

cPlayer::cPlayer()
{
}


cPlayer::~cPlayer()
{
}

void cPlayer::Setup()
{
}

void cPlayer::Update()
{

    if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON) && !isAttack)
    {
        cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);
        BOOL isHit = false;
        float _dist = 0.0f;

        //���̵�
        D3DXIntersectSubset(*g_pMeshManager->GetBasicMesh("map"), 0, &ray.m_vOrg, &ray.m_vDir, &isHit, 0, 0, 0, &_dist, NULL, NULL);
        if (isHit)
        {
            if (!isRun)
                RunAnim();
            Vector3 _Dest = ray.m_vOrg + ray.m_vDir*_dist;
            isAttack = false;
            isMoveToTarget = false;
            isMoveToPoint = true;
            DestPoint = _Dest;
        }


        //�޽� ����
        for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end(); iter++)
        {
            if (ray.IsPicked(&(*iter)->GetSphere()))
            {
                if (!isRun)
                   RunAnim();
                isMoveToPoint = false;
                isMoveToTarget = true;
                RayCast(*iter);
            }
        }

    }

    // Ÿ�ϸ� �����ش�.
    if (g_pKeyManager->isOnceKeyDown(VK_SHIFT))
    {
        if (m_vecMonster.size() > 0)
        {
            NearestSearch(m_vecMonster);
        }
    }

    if (!isAttack)
    {
        if (g_pKeyManager->isOnceKeyDown('Q'))
        {
            // �ٷ� �׼��� �ƴ϶� ����� ���������� �׼��̰�����
            Action("Heal", "3");
        }

        if (g_pKeyManager->isOnceKeyDown('W'))
        {
            RunAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('W'))
        {
            MoveForword();
        }
        if (g_pKeyManager->isOnceKeyUp('W'))
        {
            IdleAnim();
        }

        if (g_pKeyManager->isOnceKeyDown('S'))
        {
            RunAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('S'))
        {
            MoveBackword();
        }
        if (g_pKeyManager->isOnceKeyUp('S'))
        {
            IdleAnim();
        }

        if (g_pKeyManager->isOnceKeyDown('A') && !isRun)
        {
            LeftAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('A'))
        {
            RotateLeft();
        }
        if (g_pKeyManager->isOnceKeyUp('A') && !isRun)
        {
            IdleAnim();
        }
        if (g_pKeyManager->isOnceKeyDown('D') && !isRun)
        {
            RightAnim();
        }
        else if (g_pKeyManager->isStayKeyDown('D'))
        {
            RotateRight();
        }
        if (g_pKeyManager->isOnceKeyUp('D') && !isRun)
        {
            IdleAnim();
        }
    }

    if (g_pKeyManager->isOnceKeyDown('1') && !isAttack)
    {
        isPoint = true;
        AttackAnim();
    }

    if(isAttack)
    {
        if (isPoint && m_pMesh->GetdescPos() >= m_mapStateInfo["Attack"].mapPosition["attack"])
        {
            isPoint = false;
            Action("Attack", "50");// �ٽ��ؾ���
            if (m_pTarget)
                m_pTarget->RayCast(this); // ��׷� �ְ�
        }
        if(m_pMesh->GetCurPos() >= 1)
            IdleAnim();
    }

    //���콺 �̵�
    if (isMoveToPoint)
    {
        Vector3 _Dir = DestPoint - m_vPosition;

        D3DXVec3Normalize(&_Dir, &_Dir);

        m_vPosition += _Dir * m_fSpeed;
        m_stSphere.vCenter = m_vPosition;

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &_Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);

        D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

        //����
        if (Distance(DestPoint) <m_fSpeed )
        {
            m_vPosition = DestPoint;
            IdleAnim();
            isMoveToPoint = false;
            DestPoint = Vector3(0, 0, 0);
        }
    }

    // Ÿ�����ϰ� ����������
    if (isMoveToTarget)
    {
        Vector3 Dir = m_pTarget->GetPosition() - m_vPosition;
        float Distance = D3DXVec3Length(&Dir);
        D3DXVec3Normalize(&Dir, &Dir);

        // ���ݸ������ ����
        if (Distance < m_stSphere.fRadius + m_pTarget->GetSphere().fRadius )
        {
            isMoveToTarget = false;
            isPoint = true;
            AttackAnim();
        }
        else
        {
            m_vPosition += Dir * m_fSpeed;
            m_stSphere.vCenter = m_vPosition;

            D3DXMatrixTranslation(&m_MatTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
        }

        D3DXMatrixLookAtLH(&m_MatRotate, &D3DXVECTOR3(0, 0, 0), &Dir, &D3DXVECTOR3(0, 1, 0));
        D3DXMatrixTranspose(&m_MatRotate, &m_MatRotate);
    }

    Matrix4 matR,matW;
    D3DXMatrixRotationY(&matR, -D3DX_PI/2);
    D3DXMatrixScaling(&m_MatScale, 5, 5, 5);
    matW = m_MatScale * m_MatRotate * matR * m_MatTrans ;
    m_stSphere.vCenter = m_vPosition;
    m_pMesh->SetWorldMatrix(matW);
}

void cPlayer::Render()
{
    m_pMesh->UpdateAndRender();

    Matrix4 matR,matT, matW;
    D3DXMatrixRotationY(&matR, -D3DX_PI/2);
    D3DXMatrixTranslation(&matT, m_stSphere.vCenter.x, m_stSphere.vCenter.y + 0.5f, m_stSphere.vCenter.z);
    D3DXMatrixScaling(&m_MatScale, 1, 1, 1);
    matW = m_MatScale * m_MatRotate * matR * matT;
    g_pDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pPikingMesh->DrawSubset(0);
    g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//void cPlayer::Destroy()
//{
//}
