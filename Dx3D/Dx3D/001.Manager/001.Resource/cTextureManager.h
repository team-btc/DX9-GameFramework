#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
    SINGLETON(cTextureManager);

private:
    map<string, BASETEXTURE9*>  m_mapTexture;
    map<string, IMAGE_INFO>     m_mapImageInfo;

public:
    void Destroy();

    void AddTexture(string szKey, string szFilepath, bool saveImageInfo = false);
    void AddCubeTexture(string szKey, string szFilepath);
    BASETEXTURE9* GetTexture(string szKey, OUT IMAGE_INFO* pImageInfo = NULL);
};

