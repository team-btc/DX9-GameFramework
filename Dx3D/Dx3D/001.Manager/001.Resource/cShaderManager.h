#pragma once

#define g_pShaderManager cShaderManager::GetInstance()

class cShaderManager
{
    SINGLETON(cShaderManager);

private:
    map<string, LPEFFECT>      m_mapEffect;

public:
    //  �Ŵ��� ����
    HRESULT Destroy();

    //  CRUD
    HRESULT AddEffect(string key, string filepath);
    HRESULT GetEffect(LPEFFECT* effect, string key);
    HRESULT DelEffect(string key);
};

