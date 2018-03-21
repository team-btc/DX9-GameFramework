#pragma once

class cMapLoader
{
private:
    ST_MAP_INFO*        m_stMapInfo;
    string              m_szKey;

private:
    void LoadMapMesh();
    void LoadTextureMap();
    void LoadTexture(json jTexture);
    void LoadWater(json jWater);
    void LoadSky(json jSky);
    void LoadObstacle(json jObstacle);
    void LoadEvent(json jEvent);
    void LoadObject(json jObject);

public:
    cMapLoader();
    ~cMapLoader();

    void LoadMap(string szKey);
};

