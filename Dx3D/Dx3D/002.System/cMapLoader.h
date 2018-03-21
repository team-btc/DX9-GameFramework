#pragma once

class cMapLoader
{
private:
    ST_MAP_INFO*        m_stMapInfo;
    string              m_szKey;
    json                m_jObject;
    int                 m_nObjectMaxCnt;

private:
    void LoadMapMesh();
    void LoadTextureMap();
    void LoadTexture(json jTexture);
    void LoadWater(json jWater);
    void LoadSky(json jSky);
    void LoadObstacle(json jObstacle);
    void LoadEvent(json jEvent);

public:
    cMapLoader();
    ~cMapLoader();

    void SetKey(string szKey) { m_szKey = szKey; }
    void LoadMap();
    void LoadObject(int nIndex);
    int GetObjectMaxCnt() { return m_nObjectMaxCnt; }
};

