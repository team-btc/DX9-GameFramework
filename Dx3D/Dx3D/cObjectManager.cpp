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
    assert(m_setObject.empty() && "����� ������Ʈ �߿� ���� ���� ���� ��ü�� ����");
}
