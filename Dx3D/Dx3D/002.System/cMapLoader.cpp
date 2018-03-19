#include "stdafx.h"
#include "cMapLoader.h"


cMapLoader::cMapLoader()
{
}

cMapLoader::~cMapLoader()
{
}

void cMapLoader::LoadMap(string szKey)
{
    // ���࿡ �ε� �ߴ� ���̶�� �ε����� �ʰ� ���� ������ ����
    g_pMapManager->SetCurrMap(szKey);
    if (g_pMapManager->IsLoadMapInfo(szKey))
    {
        return;
    }

    m_stMapInfo = new ST_MAP_INFO;
    m_szKey = szKey;

    json jLoad;
    ifstream iFile;
    iFile.open(MAP_PATH + szKey + "/" + szKey + ".json");
    iFile >> jLoad;
    iFile.close();
    
    // ���� �Ž�
    LoadMapMesh();

    // �ؽ��� ��
    LoadTextureMap();

    // �ؽ���
    if (!jLoad["texture"].is_null())
    {
        LoadTexture(jLoad["texture"]);
    }

    // ��
    if (!jLoad["water"].is_null())
    {
        LoadWater(jLoad["water"]);
    }
    
    // �ϴ�
    if (!jLoad["skybox"].is_null())
    {
        LoadSky(jLoad["skybox"]);
    }

    // ��ֹ�
    if (!jLoad["skybox"].is_null())
    {
        LoadObstacle(jLoad["block-group"]);
    }

    // �̺�Ʈ -> ���⼭ �÷��̾� ���� ��ġ ����
    if (!jLoad["event"].is_null())
    {
        LoadEvent(jLoad["event"]);
    }

    // ������Ʈ
    if (!jLoad["object"].is_null())
    {
        LoadObject(jLoad["object"]);
    }

    // �� �Ŵ����� ���� (���� ������ ������)
    g_pMapManager->SetMapInfo(szKey, m_stMapInfo);
}

void cMapLoader::LoadMapMesh()
{
    g_pMeshManager->LoadMesh(m_szKey, MAP_PATH + m_szKey + "/" + m_szKey + ".x");
    m_stMapInfo->pTerrainMesh = g_pMeshManager->GetMesh(m_szKey);
}

void cMapLoader::LoadTextureMap()
{
    g_pTextureManager->AddTexture(m_szKey, MAP_PATH + m_szKey + "/" + m_szKey + ".png");
    m_stMapInfo->pTextureMap = (LPTEXTURE9)g_pTextureManager->GetTexture(m_szKey);
}

void cMapLoader::LoadTexture(json jTexture)
{
    json jTexBG = jTexture["bgtex"];
    string szBGTex = jTexBG["key"];
    szBGTex = TERRAIN_PATH + szBGTex;
    g_pTextureManager->AddTexture(jTexBG["key"], szBGTex);
    m_stMapInfo->arrTextureInfo[0].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTexBG["key"]);
    m_stMapInfo->arrTextureInfo[0].fDensity = jTexBG["density"];

    json jTex1 = jTexture["tex1"];
    string szTex1 = jTex1["key"];
    szTex1 = TERRAIN_PATH + szTex1;
    g_pTextureManager->AddTexture(jTex1["key"], szTex1);
    m_stMapInfo->arrTextureInfo[1].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTex1["key"]);
    m_stMapInfo->arrTextureInfo[1].fDensity = jTex1["density"];

    json jTex2 = jTexture["tex2"];
    string szTex2 = jTex2["key"];
    szTex2 = TERRAIN_PATH + szTex2;
    g_pTextureManager->AddTexture(jTex2["key"], szTex2);
    m_stMapInfo->arrTextureInfo[2].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTex2["key"]);
    m_stMapInfo->arrTextureInfo[2].fDensity = jTex2["density"];

    json jTex3 = jTexture["tex3"];
    string szTex3 = jTex3["key"];
    szTex3 = TERRAIN_PATH + szTex3;
    g_pTextureManager->AddTexture(jTex3["key"], szTex3);
    m_stMapInfo->arrTextureInfo[3].pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jTex3["key"]);
    m_stMapInfo->arrTextureInfo[3].fDensity = jTex3["density"];
}

void cMapLoader::LoadWater(json jWater)
{
    g_pMeshManager->LoadMesh(m_szKey + "-water", MAP_PATH + m_szKey + "/" + m_szKey + "-water.x");
    m_stMapInfo->pWaterMesh = g_pMeshManager->GetMesh(m_szKey + "-water");
    string szWaterTex = jWater["filename"];
    szWaterTex = WATER_PATH + szWaterTex;
    g_pTextureManager->AddTexture(jWater["filename"], szWaterTex);
    m_stMapInfo->pWaterTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(jWater["filename"]);
    m_stMapInfo->fWaterDensity = jWater["density"];
    m_stMapInfo->isEnableWater = jWater["enable"];
    m_stMapInfo->fWaterHeight = jWater["height"];
    m_stMapInfo->fWaterUVSpeed = jWater["uvspeed"];
    m_stMapInfo->fWaterWaveHeight = jWater["waveheight"];
    m_stMapInfo->fWaterHeightSpeed = jWater["heightspeed"];
    m_stMapInfo->fWaterfrequency = jWater["frequency"];
    m_stMapInfo->fWaterTransparent = jWater["transparent"];
}

void cMapLoader::LoadSky(json jSky)
{
    string szSkyTex = jSky["key"];
    szSkyTex = SKY_PATH + szSkyTex;
    g_pTextureManager->AddCubeTexture(jSky["key"], szSkyTex);
    m_stMapInfo->pSkyTexture = (LPCUBETEXTURE9)g_pTextureManager->GetTexture(jSky["key"]);
}

void cMapLoader::LoadObstacle(json jObstacle)
{
    vector<Vector3> vecObstacle;
    // ���ؽ�
    vector<ST_PC_VERTEX> vecPCVertex;
    // �ε���
    vector<DWORD> vecIndex;
    // �ε��� �ѹ�
    int nIndex = 0;
    for (int nBG = 0; nBG < jObstacle.size(); ++nBG)
    {
        for (int nBG_Point = 0; nBG_Point < jObstacle[nBG]["block-group-point"].size(); ++nBG_Point)
        {
            json jBGPoint = jObstacle[nBG]["block-group-point"][nBG_Point];
            Vector3 vPos;
            vPos.x = (float)jBGPoint["block-point-x"];
            vPos.y = (float)jBGPoint["block-point-y"];
            vPos.z = (float)jBGPoint["block-point-z"];
            vecObstacle.push_back(vPos);
        }
        for (int i = 0; i < vecObstacle.size() - 1; ++i)
        {
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i].x, 0, vecObstacle[i].z), Color(RED)));       // 0
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i].x, 255, vecObstacle[i].z), Color(RED)));	    // 1
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i + 1].x, 255, vecObstacle[i + 1].z), Color(RED)));	    // 2
            vecPCVertex.push_back(ST_PC_VERTEX(Vector3(vecObstacle[i + 1].x, 0, vecObstacle[i + 1].z), Color(RED)));	    // 3

            vecIndex.push_back(nIndex + 0);
            vecIndex.push_back(nIndex + 1);
            vecIndex.push_back(nIndex + 2);
            vecIndex.push_back(nIndex + 0);
            vecIndex.push_back(nIndex + 2);
            vecIndex.push_back(nIndex + 3);

            vecIndex.push_back(nIndex + 3);
            vecIndex.push_back(nIndex + 2);
            vecIndex.push_back(nIndex + 1);
            vecIndex.push_back(nIndex + 3);
            vecIndex.push_back(nIndex + 1);
            vecIndex.push_back(nIndex + 0);

            nIndex += 4;
        }
    }

    // ��ֹ� �Ž�
    D3DXCreateMeshFVF((DWORD)vecIndex.size() / 3, (DWORD)vecPCVertex.size(),
        D3DXMESH_MANAGED | D3DXMESH_32BIT, ST_PC_VERTEX::FVF, g_pDevice, &m_stMapInfo->pObstacleMesh);

    // ���ؽ� ���� ���
    Vector3* pV = NULL;
    m_stMapInfo->pObstacleMesh->LockVertexBuffer(NULL, (LPVOID*)&pV);
    memcpy(pV, &vecPCVertex[0], vecPCVertex.size() * sizeof(ST_PC_VERTEX));
    m_stMapInfo->pObstacleMesh->UnlockVertexBuffer();

    // �ε��� ���� ���
    DWORD* pI = NULL;
    m_stMapInfo->pObstacleMesh->LockIndexBuffer(NULL, (LPVOID*)&pI);
    memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
    m_stMapInfo->pObstacleMesh->UnlockIndexBuffer();

    // �Ӽ� ���� ���
    DWORD* pA = NULL;
    m_stMapInfo->pObstacleMesh->LockAttributeBuffer(NULL, &pA);
    for (int i = 0; i < vecIndex.size() / 3; ++i) // ���̽����� �ϳ��� ���
        pA[i] = (DWORD)0;
    m_stMapInfo->pObstacleMesh->UnlockAttributeBuffer();

    // �޽� ����ȭ : ���ؽ� ���� ��ŭ ���������� ���� ������ Ȯ��
    vector<DWORD> vecAdjBuf2(m_stMapInfo->pObstacleMesh->GetNumFaces() * 3);

    m_stMapInfo->pObstacleMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf2[0]);

    m_stMapInfo->pObstacleMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, &vecAdjBuf2[0], 0, 0, 0);

    // �Ž� �Ŵ����� ����
    g_pMeshManager->AddMesh(m_szKey + "Obstacle", &m_stMapInfo->pObstacleMesh);
}

void cMapLoader::LoadEvent(json jEvent)
{
    for (int i = 0; i < jEvent.size(); ++i)
    {
        ST_EVENT_INFO stEvent;

        string szName = jEvent[i]["event-name"];
        stEvent.szName = szName;
        stEvent.vPos.x = jEvent[i]["event-position-x"];
        stEvent.vPos.y = jEvent[i]["event-position-y"];
        stEvent.vPos.z = jEvent[i]["event-position-z"];
        stEvent.fRadius = 10.0f;

        // ���� Y���� �°� ����
        cRay ray;
        ray.m_vOrg = stEvent.vPos;
        ray.m_vOrg.y = 255.0f;
        ray.m_vDir = Vector3(0, -1, 0);
        BOOL isHit = false;
        float fDist;

        D3DXIntersectSubset(m_stMapInfo->pTerrainMesh, NULL, &ray.m_vOrg, &ray.m_vDir, &isHit,
            NULL, NULL, NULL, &fDist, NULL, NULL);

        if (isHit)
        {
            stEvent.vPos.y = 255.0f - fDist;
        }

        m_stMapInfo->vecEventInfo.push_back(stEvent);
    }
}

void cMapLoader::LoadObject(json jObject)
{
    for (int i = 0; i < jObject.size(); ++i)
    {
        ST_OBJECT_INFO stObject;
        string szName = jObject[i]["object-file-key"];
        string szPath = OBJECT_PATH + szName;
        stObject.pMesh = new cSkinnedMesh(szName, szPath, szName + ".x");
        float fScale = jObject[i]["object-scale"];
        Vector3 pos, rot;
        pos.x = jObject[i]["object-position-x"];
        pos.y = jObject[i]["object-position-y"];
        pos.z = jObject[i]["object-position-z"];
        rot.x = jObject[i]["object-rotation-x"];
        rot.y = jObject[i]["object-rotation-y"];
        rot.z = jObject[i]["object-rotation-z"];
        stObject.pMesh->SetScale(fScale);
        stObject.pMesh->SetPosition(pos);
        stObject.pMesh->SetRotation(rot);

        m_stMapInfo->vecObjectInfo.push_back(stObject);
    }
}
