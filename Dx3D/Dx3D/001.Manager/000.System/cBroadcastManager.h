#pragma once

#define g_pBroadcastManager cBroadcastManager::GetInstance()

struct ST_NEWS
{
    int     nReadCount;     //  int,
    string  szTarget;       //  ???, target
    string  szMessage;      //  str, message

    ST_NEWS()
    {
        nReadCount = 0;
        szTarget = "";
        szMessage = "";
    }
};

class cBroadChannel
{
private:
    list<ST_NEWS*>     m_pListNews;

public:
    cBroadChannel() {}
    ~cBroadChannel() {}

    HRESULT AddNews(IN ST_NEWS* pNews);
    HRESULT GetNews(OUT ST_NEWS** pNews, IN string szViewer);
    HRESULT Destroy();
};

#include "iSingletonManager.h"

class cBroadcastManager : public iSingletonManager
{
    SINGLETON(cBroadcastManager);

private:
    map<string, cBroadChannel*> m_mapChannel;

public:
    HRESULT AddChannel(IN string szChannel);
    HRESULT AddNews(IN string szChannel, IN ST_NEWS* pNews);

    /*  pNews�� ȣ���ڰ� ������ news�� ��Ƽ� ȣ��
    ȣ������ news�� ������Ʈ �Ǿ�� �Ѵٸ� isNew = true �ƴϸ� false
    isNew�� true��� pNews�� �ֽ� news�� ���� */
    HRESULT UpdateNews(OUT bool& isNew, OUT IN ST_NEWS** pNews, IN string szChannel, IN string szViewer);

    // iSingletonManager��(��) ���� ��ӵ�
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};

