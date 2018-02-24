#include "stdafx.h"
#include "cTimerManager.h"

cTimerManager::cTimerManager()
{
    m_pTimer = new cTimer;
    m_fDeltaTime = 0.0f;
}

cTimerManager::~cTimerManager()
{
    SAFE_DELETE(m_pTimer);
}

void cTimerManager::Update(float lock)
{
    if (m_pTimer)
    {
        m_fDeltaTime = GetWorldTime();
        m_pTimer->Tick(lock);
        m_fDeltaTime = GetWorldTime() - m_fDeltaTime;
    }
}

void cTimerManager::Render()
{
    RECT rt = { 0, 0, W_WIDTH, W_HEIGHT };
    ST_TIME_INFO info = g_pTimerManager->GetTimeInfo();
    g_pFontManager->GetFont(cFontManager::E_DEBUG)->DrawTextA(NULL, info.szFPS.c_str(), -1, &rt, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
    rt.top = 15;
    g_pFontManager->GetFont(cFontManager::E_DEBUG)->DrawTextA(NULL, info.szWorldTime.c_str(), -1, &rt, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
}

ST_TIME_INFO cTimerManager::GetTimeInfo()
{
    ST_TIME_INFO info;
    info.szFPS = "FPS : " + to_string(m_pTimer->GetFPS());
    info.szWorldTime = "WORLD TIME : " + to_string(GetWorldTime());
    return info;
}


cTimer::cTimer()
{
    //  고성능 타이머 지원 여부 체크
    if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPeriodFrequency))
    {
        m_isHardware = true;
        QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);

        m_fTimeScale = 1.0f / m_nPeriodFrequency;
    }
    else
    {
        m_isHardware = false;
        m_nLastTime = timeGetTime();
        m_fTimeScale = 0.001000f;           //  FIXED VALUE 1ms
    }

    //  타이머 관련 변수들 초기화
    m_nFrameRate = 0;
    m_nFpsCount = 0;
    m_fFpsTimeElapsed = 0.0f;
    m_fWorldTime = 0.0f;
}


cTimer::~cTimer()
{
}

void cTimer::Tick(float lockFPS)
{
    //  고성능 타이머 확인
    if (m_isHardware)
    {
        //  마이크로세컨드 단위로 계산
        QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrTime);
    }
    else
    {
        m_nCurrTime = timeGetTime();
    }

    //  DELTA TIME
    m_fTimeElapsed = (m_nCurrTime - m_nLastTime) * m_fTimeScale;

    if (lockFPS > 0.0f)
    {
        while (m_fTimeElapsed < (1.0f / lockFPS))
        {
            if (m_isHardware)
            {
                QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrTime);
            }
            else
            {
                m_nCurrTime = timeGetTime();
            }

            m_fTimeElapsed = (m_nCurrTime - m_nLastTime) * m_fTimeScale;
        }
    }

    m_nLastTime = m_nCurrTime;
    m_nFpsCount++;
    m_fFpsTimeElapsed += m_fTimeElapsed;
    m_fWorldTime += m_fTimeElapsed;

    if (m_fFpsTimeElapsed > 1.0f)
    {
        m_nFrameRate = m_nFpsCount;
        m_nFpsCount = 0;
        m_fFpsTimeElapsed = 0.0f;
    }
}

unsigned int cTimer::GetFPS(char * str) const
{
    return m_nFrameRate;
}
