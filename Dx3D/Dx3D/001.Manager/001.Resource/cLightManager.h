#pragma once
#define g_pLightManager     cLightManager::GetInstance()

class cLightManager
{
    SINGLETON(cLightManager);

private:
public:
    D3DLIGHT9 InitDirectional(Vector3* dir, XColor* c);
    D3DLIGHT9 InitPoint(Vector3* pos, XColor* c);
    D3DLIGHT9 InitSpot(Vector3* dir, Vector3* pos, XColor* c);
};

