#include "stdafx.h"
#include "cObjMap.h"


cObjMap::cObjMap()
{
}


cObjMap::~cObjMap()
{
}

void cObjMap::Load(IN char* szFilePath, IN Matrix4* pMat)
{
    vector<Vector3> vecP;

    FILE* fp = NULL;
    fopen_s(&fp, szFilePath, "r");

    while (!feof(fp))
    {
        char szTemp[1024];
        fgets(szTemp, 1024, fp);

        if (szTemp[0] == 'v')
        {
            Vector3 v;
            sscanf_s(szTemp, "%*s %f %f %f", &v.x, &v.y, &v.z);

            if (szTemp[1] == ' ')
                vecP.push_back(v);
        }
        else if (szTemp[0] == 'f')
        {
            int aIndex[3];
            sscanf_s(szTemp, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                     &aIndex[0], &aIndex[1], &aIndex[2]);

            for (int i = 0; i < 3; ++i)
            {
                Vector3 v;
                v = vecP[aIndex[i] - 1];

                if (pMat)
                {
                    D3DXVec3TransformCoord(&v, &v, pMat);
                }
                m_vecVertex.push_back(v);
            }
        }
    }

    fclose(fp);
}

bool cObjMap::GetHeight(IN const float& x, OUT float& y, IN const float& z)
{
    Vector3 vRayPos(x, y + 2, z);
    Vector3 vRayDir(0, -1, 0);
    float distance;
    bool ret = false;

    for (int i = 0; i < (int)m_vecVertex.size(); i += 3)
    {
        if (D3DXIntersectTri(&m_vecVertex[i + 0], &m_vecVertex[i + 1], &m_vecVertex[i + 2],
                             &vRayPos, &vRayDir, NULL, NULL, &distance) && distance <= 2.5f)
        {
            y = vRayPos.y - distance;
            ret = true;
            break;
        }
    }

    return ret;
}