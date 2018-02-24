#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
    SINGLETON(cTextureManager);

private:
    map<string, LPTEXTURE9> m_mapTexture;
    map<string, IMAGE_INFO>     m_mapImageInfo;

public:
    void Destroy();

    void AddTexture(string key, string fileName, bool saveImageInfo = false);
    LPTEXTURE9* GetTexture(string key, OUT IMAGE_INFO* pImageInfo = NULL);
};

