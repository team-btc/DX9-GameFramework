#include "stdafx.h"
#include "cBroadcastManager.h"


cBroadcastManager::cBroadcastManager()
{
}


cBroadcastManager::~cBroadcastManager()
{
}

HRESULT cBroadcastManager::AddChannel(IN string szChannel)
{
    HRESULT hr;
    auto iter = m_mapChannel.find(szChannel);
    if (iter == m_mapChannel.end())
    {
        m_mapChannel.insert(make_pair(szChannel, new cBroadChannel));
        hr = S_OK;
    }
    else
    {
        hr = E_ABORT;
    }

    return hr;
}

HRESULT cBroadcastManager::AddNews(IN string szChannel, IN ST_NEWS* pNews)
{
    HRESULT hr;
    auto iter = m_mapChannel.find(szChannel);
    if (iter != m_mapChannel.end())
    {
        iter->second->AddNews(pNews);

        hr = S_OK;
    }
    else
    {
        hr = E_ABORT;
    }

    return hr;
}

HRESULT cBroadcastManager::UpdateNews(OUT bool& isNew, OUT IN ST_NEWS** pNews, IN string szChannel, IN string szViewer)
{
    HRESULT hr;
    auto iter = m_mapChannel.find(szChannel);
    if (iter != m_mapChannel.end())
    {
        hr = iter->second->GetNews(pNews, szViewer);
        if (hr == S_OK)
        {
            isNew = true;
        }
        else
        {
            isNew = false;
        }
    }
    else
    {
        hr = E_ABORT;
    }

    return hr;
}

HRESULT cBroadcastManager::Destroy()
{
    HRESULT hr;
    for (auto iter = m_mapChannel.begin(); iter != m_mapChannel.end(); iter++)
    {
        hr = iter->second->Destroy();
    }

    return hr;
}

HRESULT cBroadChannel::AddNews(IN ST_NEWS* pNews)
{
    HRESULT hr;
    m_pListNews.push_back(pNews);
    hr = S_OK;

    return hr;
}

HRESULT cBroadChannel::GetNews(OUT ST_NEWS** pNews, IN string szViewer)
{
    HRESULT hr;
    if (!m_pListNews.empty())
    {
        ST_NEWS* pFront = m_pListNews.front();
        if (szViewer == pFront->szTarget &&
            *pNews != pFront)
        {
            *pNews = pFront;
            hr = S_OK;
            pFront->nReadCount--;
            if (pFront->nReadCount <= 0)
            {
                m_pListNews.pop_front();
            }
        }
        else
        {
            hr = E_FAIL;
        }
    }
    else
    {
        hr = E_ABORT;
    }

    return hr;
}

HRESULT cBroadChannel::Destroy()
{
    HRESULT hr = S_OK;
    for (auto iter = m_pListNews.begin(); iter != m_pListNews.end();)
    {
        ST_NEWS* pNews = *iter;
        iter = m_pListNews.erase(iter);
        SAFE_DELETE(pNews);
    }

    return hr;
}
