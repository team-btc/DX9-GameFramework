#include "stdafx.h"
#include "cDbManager.h"


cDbManager::cDbManager()
    : m_pDatabase(NULL)
{
}


cDbManager::~cDbManager()
{
}

HRESULT cDbManager::Setup()
{
    return S_OK;
    m_pDatabase = new cAdo;

//    if (!m_pDatabase->Connect(".\\SQLEXPRESS", "fire", "guest"))
//    {
//#ifdef _DEBUG
//        cout << "DB connection failed" << endl;
//#endif // _DEBUG
//    }

    return S_OK;
}

HRESULT cDbManager::Update()
{
    return E_NOTIMPL;
}

HRESULT cDbManager::Render()
{
    return E_NOTIMPL;
}

HRESULT cDbManager::Destroy()
{
    return S_OK;
    if (m_pDatabase)
    {
        m_pDatabase->Disconnect();
    }

    return S_OK;
}
