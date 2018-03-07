//--------------------------------------------------------------------------------------
// File: MeshLoader.cpp
//
// Wrapper class for ID3DXMesh interface. Handles loading mesh data from an .obj file
// and resource management for material textures.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "cMeshLoader.h"

// Vertex declaration
//D3DVERTEXELEMENT9 VERTEX_DECL[] =
VTXELEMENT9 VERTEX_DECL[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0},
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
};


//--------------------------------------------------------------------------------------
cObjMeshLoader::cObjMeshLoader()
{
    m_pMesh = NULL;

    ZeroMemory(m_strMediaDir, sizeof(m_strMediaDir));
}


//--------------------------------------------------------------------------------------
cObjMeshLoader::~cObjMeshLoader()
{
    Destroy();
}


//--------------------------------------------------------------------------------------
void cObjMeshLoader::Destroy()
{
    for (int iMaterial = 0; iMaterial < m_Materials.size(); iMaterial++)
    {
        Material* pMaterial = m_Materials.at(iMaterial);

        // Avoid releasing the same texture twice
        for (int x = iMaterial + 1; x < m_Materials.size(); x++)
        {
            Material* pCur = m_Materials.at(x);
            if (pCur->pTexture == pMaterial->pTexture)
                pCur->pTexture = NULL;
        }

        SAFE_RELEASE(pMaterial->pTexture);
        SAFE_DELETE(pMaterial);
    }

    m_Materials.clear();
    m_Vertices.clear();
    m_Indices.clear();
    m_Attributes.clear();

    SAFE_RELEASE(m_pMesh);
}


//--------------------------------------------------------------------------------------
HRESULT cObjMeshLoader::Create(LPDEVICE9 pd3dDevice, const WCHAR* strFilename)
{
    HRESULT hr;
    WCHAR str[MAX_PATH] = { 0 };

    // Start clean
    Destroy();

    // Load the vertex buffer, index buffer, and subset information from a file. In this case, 
    // an .obj file was chosen for simplicity, but it's meant to illustrate that ID3DXMesh objects
    // can be filled from any mesh file format once the necessary data is extracted from file.
    hr = LoadGeometryFromOBJ(strFilename);

    // Set the current directory based on where the mesh was found
    WCHAR wstrOldDir[MAX_PATH] = { 0 };
    hr = GetCurrentDirectory(MAX_PATH, wstrOldDir);
    hr = SetCurrentDirectory(m_strMediaDir);

    // Load material textures
    for (int iMaterial = 0; iMaterial < m_Materials.size(); iMaterial++)
    {
        Material* pMaterial = m_Materials.at(iMaterial);
        if (pMaterial->strTexture[0])
        {
            // Avoid loading the same texture twice
            bool bFound = false;
            for (int x = 0; x < iMaterial; x++)
            {
                Material* pCur = m_Materials.at(x);
                if (0 == wcscmp(pCur->strTexture, pMaterial->strTexture))
                {
                    bFound = true;
                    pMaterial->pTexture = pCur->pTexture;
                    break;
                }
            }

            // Not found, load the texture
            if (!bFound)
            {
                hr = D3DXCreateTextureFromFile(pd3dDevice, pMaterial->strTexture,
                    &(pMaterial->pTexture));
            }
        }
    }

    // Restore the original current directory
    hr = SetCurrentDirectory(wstrOldDir);
    // Create the encapsulated mesh
    ID3DXMesh* pMesh = NULL;
    hr = D3DXCreateMesh((DWORD)m_Indices.size() / 3, (DWORD)m_Vertices.size(),
        D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL,
        pd3dDevice, &pMesh);

    // Copy the vertex data
    VertexPNT* pVertex;
    hr = pMesh->LockVertexBuffer(0, (void**)&pVertex);
    memcpy(pVertex, &m_Vertices[0], (DWORD)m_Vertices.size() * sizeof(VertexPNT));
    hr = pMesh->UnlockVertexBuffer();
    m_Vertices.clear();

    // Copy the index data
    DWORD* pIndex;
    hr = pMesh->LockIndexBuffer(0, (void**)&pIndex);
    memcpy(pIndex, &m_Indices[0], (DWORD)m_Indices.size() * sizeof(DWORD));
    hr = pMesh->UnlockIndexBuffer();
    m_Indices.clear();

    // Copy the attribute data
    DWORD* pSubset;
    hr = pMesh->LockAttributeBuffer(0, &pSubset);
    memcpy(pSubset, &m_Attributes[0], (DWORD)m_Attributes.size() * sizeof(DWORD));
    hr = pMesh->UnlockAttributeBuffer();
    m_Attributes.clear();

    // Reorder the vertices according to subset and optimize the mesh for this graphics 
    // card's vertex cache. When rendering the mesh's triangle list the vertices will 
    // cache hit more often so it won't have to re-execute the vertex shader.
    DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
    if (aAdjacency == NULL)
        return E_OUTOFMEMORY;

    hr = pMesh->GenerateAdjacency(1e-6f, aAdjacency);
    hr = pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL);

    SAFE_DELETE_ARRAY(aAdjacency);
    m_pMesh = pMesh;

    return hr;
}


//--------------------------------------------------------------------------------------
HRESULT cObjMeshLoader::LoadGeometryFromOBJ(const WCHAR* strFileName)
{
    WCHAR strMaterialFilename[MAX_PATH] = { 0 };
    WCHAR wstr[MAX_PATH];
    char str[MAX_PATH];
    HRESULT hr = S_OK;

    // Find the file
    //V_RETURN(DXUTFindDXSDKMediaFileCch(wstr, MAX_PATH, strFileName));
    wcscpy_s(wstr, MAX_PATH, strFileName);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, MAX_PATH, NULL, NULL);

    // Store the directory where the mesh was found
    wcscpy_s(m_strMediaDir, MAX_PATH - 1, wstr);
    WCHAR* pch = wcsrchr(m_strMediaDir, L'\\');
    if (pch)
        *pch = NULL;

    // Create temporary storage for the input data. Once the data has been loaded into
    // a reasonable format we can create a D3DXMesh object and load it with the mesh data.
    vector<Vector3> Positions;
    vector<Vector2> TexCoords;
    vector<Vector3> Normals;

    // The first subset uses the default material
    Material* pMaterial = new Material();
    if (pMaterial == NULL)
        return E_OUTOFMEMORY;

    InitMaterial(pMaterial);
    wcscpy_s(pMaterial->strName, MAX_PATH - 1, L"default");
    m_Materials.push_back(pMaterial);

    DWORD dwCurSubset = 0;

    // File input
    WCHAR strCommand[256] = { 0 };
    wifstream InFile(str);
    if (!InFile)
        return E_FAIL;//DXTRACE_ERR( L"wifstream::open", E_FAIL );

    for (; ; )
    {
        InFile >> strCommand;
        if (!InFile)
            break;

        if (0 == wcscmp(strCommand, L"#"))
        {
            // Comment
        }
        else if (0 == wcscmp(strCommand, L"v"))
        {
            // Vertex Position
            float x, y, z;
            InFile >> x >> y >> z;
            Positions.push_back(Vector3(x, y, z));
        }
        else if (0 == wcscmp(strCommand, L"vt"))
        {
            // Vertex TexCoord
            float u, v;
            InFile >> u >> v;
            TexCoords.push_back(Vector2(u, v));
        }
        else if (0 == wcscmp(strCommand, L"vn"))
        {
            // Vertex Normal
            float x, y, z;
            InFile >> x >> y >> z;
            Normals.push_back(Vector3(x, y, z));
        }
        else if (0 == wcscmp(strCommand, L"f"))
        {
            // Face
            UINT iPosition, iTexCoord, iNormal;
            VertexPNT vertex;

            for (UINT iFace = 0; iFace < 3; iFace++)
            {
                ZeroMemory(&vertex, sizeof(VertexPNT));

                // OBJ format uses 1-based arrays
                InFile >> iPosition;
                vertex.p = Positions[iPosition - 1];

                if ('/' == InFile.peek())
                {
                    InFile.ignore();

                    if ('/' != InFile.peek())
                    {
                        // Optional texture coordinate
                        InFile >> iTexCoord;
                        vertex.t = TexCoords[iTexCoord - 1];
                    }

                    if ('/' == InFile.peek())
                    {
                        InFile.ignore();

                        // Optional vertex normal
                        InFile >> iNormal;
                        vertex.n = Normals[iNormal - 1];
                    }
                }

                // If a duplicate vertex doesn't exist, add this vertex to the Vertices
                // list. Store the index in the Indices array. The Vertices and Indices
                // lists will eventually become the Vertex Buffer and Index Buffer for
                // the mesh.
                DWORD index = AddVertex(iPosition, &vertex);
                if (index == (DWORD)-1)
                    return E_OUTOFMEMORY;

                m_Indices.push_back(index);
            }
            m_Attributes.push_back(dwCurSubset);
        }
        else if (0 == wcscmp(strCommand, L"mtllib"))
        {
            // Material library
            InFile >> strMaterialFilename;
        }
        else if (0 == wcscmp(strCommand, L"usemtl"))
        {
            // Material
            WCHAR strName[MAX_PATH] = { 0 };
            InFile >> strName;

            bool bFound = false;
            for (int iMaterial = 0; iMaterial < m_Materials.size(); iMaterial++)
            {
                Material* pCurMaterial = m_Materials.at(iMaterial);
                if (0 == wcscmp(pCurMaterial->strName, strName))
                {
                    bFound = true;
                    dwCurSubset = iMaterial;
                    break;
                }
            }

            if (!bFound)
            {
                pMaterial = new Material();
                if (pMaterial == NULL)
                    return E_OUTOFMEMORY;

                dwCurSubset = (DWORD)m_Materials.size();

                InitMaterial(pMaterial);
                wcscpy_s(pMaterial->strName, MAX_PATH - 1, strName);

                m_Materials.push_back(pMaterial);
            }
        }
        else
        {
            // Unimplemented or unrecognized command
        }

        InFile.ignore(1000, '\n');
    }

    // Cleanup
    InFile.close();
    DeleteCache();

    // If an associated material file was found, read that in as well.
    if (strMaterialFilename[0])
    {
        hr = LoadMaterialsFromMTL(strMaterialFilename);
    }

    return hr;
}


//--------------------------------------------------------------------------------------
DWORD cObjMeshLoader::AddVertex(UINT hash, VertexPNT* pVertex)
{
    // If this vertex doesn't already exist in the Vertices list, create a new entry.
    // Add the index of the vertex to the Indices list.
    bool bFoundInList = false;
    DWORD index = 0;

    // Since it's very slow to check every element in the vertex list, a hashtable stores
    // vertex indices according to the vertex position's index as reported by the OBJ file
    if ((UINT)m_VertexCache.size() > hash)
    {
        CacheEntry* pEntry = m_VertexCache.at(hash);
        while (pEntry != NULL)
        {
            VertexPNT* pCacheVertex = &m_Vertices[0] + pEntry->index;

            // If this vertex is identical to the vertex already in the list, simply
            // point the index buffer to the existing vertex
            if (0 == memcmp(pVertex, pCacheVertex, sizeof(VertexPNT)))
            {
                bFoundInList = true;
                index = pEntry->index;
                break;
            }

            pEntry = pEntry->pNext;
        }
    }

    // Vertex was not found in the list. Create a new entry, both within the Vertices list
    // and also within the hashtable cache
    if (!bFoundInList)
    {
        // Add to the Vertices list
        index = (DWORD)m_Vertices.size();
        m_Vertices.push_back(*pVertex);

        // Add this to the hashtable
        CacheEntry* pNewEntry = new CacheEntry;
        if (pNewEntry == NULL)
            return (DWORD)-1;

        pNewEntry->index = index;
        pNewEntry->pNext = NULL;

        // Grow the cache if needed
        while ((UINT)m_VertexCache.size() <= hash)
        {
            m_VertexCache.push_back(NULL);
        }

        // Add to the end of the linked list
        CacheEntry* pCurEntry = m_VertexCache.at(hash);
        if (pCurEntry == NULL)
        {
            // This is the head element
            //m_VertexCache.SetAt( hash, pNewEntry );
            m_VertexCache[hash] = pNewEntry;
        }
        else
        {
            // Find the tail
            while (pCurEntry->pNext != NULL)
            {
                pCurEntry = pCurEntry->pNext;
            }

            pCurEntry->pNext = pNewEntry;
        }
    }

    return index;
}


//--------------------------------------------------------------------------------------
void cObjMeshLoader::DeleteCache()
{
    // Iterate through all the elements in the cache and subsequent linked lists
    for (int i = 0; i < m_VertexCache.size(); i++)
    {
        CacheEntry* pEntry = m_VertexCache.at(i);
        while (pEntry != NULL)
        {
            CacheEntry* pNext = pEntry->pNext;
            SAFE_DELETE(pEntry);
            pEntry = pNext;
        }
    }

    m_VertexCache.clear();
}


//--------------------------------------------------------------------------------------
HRESULT cObjMeshLoader::LoadMaterialsFromMTL(const WCHAR* strFileName)
{
    HRESULT hr = S_OK;

    // Set the current directory based on where the mesh was found
    WCHAR wstrOldDir[MAX_PATH] = { 0 };
    GetCurrentDirectory(MAX_PATH, wstrOldDir);
    SetCurrentDirectory(m_strMediaDir);

    // Find the file
    WCHAR strPath[MAX_PATH];
    char cstrPath[MAX_PATH];
    //DXUTFindDXSDKMediaFileCch(strPath, MAX_PATH, strFileName);
    wcscpy_s(strPath, MAX_PATH, strFileName);
    WideCharToMultiByte(CP_ACP, 0, strPath, -1, cstrPath, MAX_PATH, NULL, NULL);

    // File input
    WCHAR strCommand[256] = { 0 };
    wifstream InFile(cstrPath);
    if (!InFile)
    {
        g_pLogManager->WriteLog(EL_ERROR, "wifstream::open failed - obj loader");
    }
        //return DXTRACE_ERR( L"wifstream::open", E_FAIL );

    // Restore the original current directory
    SetCurrentDirectory(wstrOldDir);

    Material* pMaterial = NULL;

    for (; ; )
    {
        InFile >> strCommand;
        if (!InFile)
            break;

        if (0 == wcscmp(strCommand, L"newmtl"))
        {
            // Switching active materials
            WCHAR strName[MAX_PATH] = { 0 };
            InFile >> strName;

            pMaterial = NULL;
            for (int i = 0; i < m_Materials.size(); i++)
            {
                Material* pCurMaterial = m_Materials.at(i);
                if (0 == wcscmp(pCurMaterial->strName, strName))
                {
                    pMaterial = pCurMaterial;
                    break;
                }
            }
        }

        // The rest of the commands rely on an active material
        if (pMaterial == NULL)
            continue;

        if (0 == wcscmp(strCommand, L"#"))
        {
            // Comment
        }
        else if (0 == wcscmp(strCommand, L"Ka"))
        {
            // Ambient color
            float r, g, b;
            InFile >> r >> g >> b;
            pMaterial->vAmbient = Vector3(r, g, b);
            pMaterial->mMaterial.Ambient = D3DXCOLOR(r, g, b, 1.0f);
        }
        else if (0 == wcscmp(strCommand, L"Kd"))
        {
            // Diffuse color
            float r, g, b;
            InFile >> r >> g >> b;
            pMaterial->vDiffuse = Vector3(r, g, b);
        }
        else if (0 == wcscmp(strCommand, L"Ks"))
        {
            // Specular color
            float r, g, b;
            InFile >> r >> g >> b;
            pMaterial->vSpecular = Vector3(r, g, b);
        }
        else if (0 == wcscmp(strCommand, L"d") ||
            0 == wcscmp(strCommand, L"Tr"))
        {
            // Alpha
            InFile >> pMaterial->fAlpha;
        }
        else if (0 == wcscmp(strCommand, L"Ns"))
        {
            // Shininess
            int nShininess;
            InFile >> nShininess;
            pMaterial->nShininess = nShininess;
        }
        else if (0 == wcscmp(strCommand, L"illum"))
        {
            // Specular on/off
            int illumination;
            InFile >> illumination;
            pMaterial->bSpecular = (illumination == 2);
        }
        else if (0 == wcscmp(strCommand, L"map_Kd"))
        {
            // Texture
            InFile >> pMaterial->strTexture;
        }

        else
        {
            // Unimplemented or unrecognized command
        }

        InFile.ignore(1000, L'\n');
    }

    InFile.close();

    return hr;
}


//--------------------------------------------------------------------------------------
void cObjMeshLoader::InitMaterial(Material* pMaterial)
{
    ZeroMemory(pMaterial, sizeof(Material));

    pMaterial->vAmbient = Vector3(0.2f, 0.2f, 0.2f);
    pMaterial->mMaterial.Ambient = XColor(0.2f, 0.2f, 0.2f, 1.0f);
    pMaterial->vDiffuse = Vector3(0.8f, 0.8f, 0.8f);
    pMaterial->mMaterial.Diffuse = XColor(0.8f, 0.8f, 0.8f, 1.0f);
    pMaterial->vSpecular = Vector3(1.0f, 1.0f, 1.0f);
    pMaterial->mMaterial.Specular = XColor(1.0f, 1.0f, 1.0f, 1.0f);
    pMaterial->nShininess = 0;
    pMaterial->fAlpha = 1.0f;
    pMaterial->bSpecular = false;
    pMaterial->pTexture = NULL;
}