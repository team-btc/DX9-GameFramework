#include "stdafx.h"
#include "cObjectManager.h"


cObjectManager::cObjectManager()
{
}


cObjectManager::~cObjectManager()
{
}

void cObjectManager::AddObject(cObject* pObject)
{
    m_setObject.insert(pObject);
}

void cObjectManager::RemoveObejct(cObject* pObject)
{
    m_setObject.erase(pObject);
}

void cObjectManager::Destory()
{
    assert(m_setObject.empty() && "사용자 오브젝트 중에 삭제 되지 않은 객체가 있음");
}
