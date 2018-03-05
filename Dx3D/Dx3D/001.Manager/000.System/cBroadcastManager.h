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
    ST_NEWS* GetNews(IN ST_NEWS* pNews, IN string szViewer);
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

    /*  pNews에 호출자가 보유한 news를 담아서 호출
    호출자의 news가 업데이트 되어야 한다면 isNew = true 아니면 false
    isNew가 true라면 pNews에 최신 news를 세팅 */
    HRESULT UpdateNews(OUT bool& isNew, IN ST_NEWS* pNews, IN string szChannel, IN string szViewer);

    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
};

