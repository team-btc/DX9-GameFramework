#pragma once

#define g_pShaderManager cShaderManager::GetInstance()

class cShaderManager
{
    SINGLETON(cShaderManager);
private:
    map<string, LPEFFECT>      m_mapEffect;

public:
    void Destroy();

    HRESULT AddEffect(string key, string filepath);
    LPEFFECT GetEffect(string key);

};

