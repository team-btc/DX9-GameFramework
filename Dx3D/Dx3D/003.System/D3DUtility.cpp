#include "stdafx.h"
#include "D3DUtility.h"

namespace D3DUTILITY
{
    MATERIAL9 InitMtrl(XColor a, XColor d, XColor s, XColor e, float p)
    {
        MATERIAL9 mtrl;
        mtrl.Ambient = a;
        mtrl.Diffuse = d;
        mtrl.Specular = s;
        mtrl.Emissive = e;
        mtrl.Power = p;

        return mtrl;
    }

    void ComputeNormal(Vector3* out, Vector3* p0, Vector3* p1, Vector3* p2)
    {
        Vector3 v01 = *p1 - *p0;
        Vector3 v02 = *p2 - *p0;

        D3DXVec3Cross(out, &v01, &v02);
        D3DXVec3Normalize(out, out);
    }

    LIGHT9 InitDirectional(Vector3* dir, XColor* c)
    {
        LIGHT9 stLight;
        ZeroMemory(&stLight, sizeof(LIGHT9));

        stLight.Type = D3DLIGHT_DIRECTIONAL;
        stLight.Ambient = *c * 0.4f;
        stLight.Diffuse = *c;
        stLight.Specular = *c * 0.6f;
        stLight.Direction = *dir;

        return stLight;
    }

    LIGHT9 InitPoint(Vector3* pos, XColor* c)
    {
        LIGHT9 stLight;
        ZeroMemory(&stLight, sizeof(LIGHT9));

        stLight.Type = D3DLIGHT_POINT;
        stLight.Ambient = *c * 0.4f;
        stLight.Diffuse = *c;
        stLight.Specular = *c * 0.6f;
        stLight.Position = *pos;
        stLight.Range = 20.0f;
        stLight.Falloff = 1.0f;
        stLight.Attenuation0 = 0.005f;
        stLight.Attenuation1 = 0.005f;
        stLight.Attenuation2 = 0.005f;

        return stLight;
    }

    LIGHT9 InitSpot(Vector3* dir, Vector3* pos, XColor* c)
    {
        LIGHT9 stLight;
        ZeroMemory(&stLight, sizeof(LIGHT9));

        stLight.Type = D3DLIGHT_SPOT;
        stLight.Ambient = *c * 0.4f;
        stLight.Diffuse = *c;
        stLight.Specular = *c * 0.6f;
        stLight.Direction = *dir;
        stLight.Position = *pos;
        stLight.Range = 50.0f;
        stLight.Falloff = 1.0f;
        stLight.Theta = 0.0f;
        stLight.Phi = D3DX_PI / 6;
        stLight.Attenuation0 = 0.005f;
        stLight.Attenuation1 = 0.005f;
        stLight.Attenuation2 = 0.005f;

        return stLight;
    }
}