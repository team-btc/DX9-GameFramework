#pragma once

#define g_pMapDataManager cMapDataManager::GetInstance()

class cMapDataManager
{
    SINGLETON(cMapDataManager);

private:
    map<string, json>       m_mMapData;     // �� ������
    map<string, LPMESH>     m_mMesh;        // ���� �Ž� ������

public:

    void LoadMapData();                     // ��� �� ������ �ε� �ؿ���
    void Destroy();
};

