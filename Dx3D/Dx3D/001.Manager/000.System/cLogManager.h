#pragma once

#define g_pLogManager cLogManager::GetInstance()

enum LOG_LEVEL
{
    EL_DEBUG = 0, EL_INFO, EL_WARNING, EL_ALERT, EL_ERROR
};

class cLogManager
{
    SINGLETON(cLogManager);
private:
    string m_szLogPath;
    string m_szLogFilename;
    string m_szDate;

    void CreateLogDir();

    string GetTimeString();
    string GetTimeStampSimple();
    string GetCurrentPath();

public:

    void Setup(string LogPath);
    void WriteLog(LOG_LEVEL LogLevel, string Message);
};

