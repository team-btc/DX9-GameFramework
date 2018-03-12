#include "stdafx.h"
#include "cLogManager.h"

cLogManager::cLogManager()
{

}

cLogManager::~cLogManager()
{

}

HRESULT cLogManager::Setup()
{
    return E_NOTIMPL;
}

HRESULT cLogManager::Update()
{
    return E_NOTIMPL;
}

HRESULT cLogManager::Render()
{
    return E_NOTIMPL;
}

HRESULT cLogManager::Destroy()
{
    return E_NOTIMPL;
}

void cLogManager::CreateLogDir()
{
    if (CreateDirectoryA(m_szLogPath.c_str(), NULL) ||
        ERROR_ALREADY_EXISTS == GetLastError())
    {
#ifdef _DEBUG
        // CopyFile(...)
        cout << "SUC" << endl;
    }
    else
    {
        // Failed to create directory.
        cout << "FAILED" << endl;
#endif // _DEBUG
    }
}

string cLogManager::GetTimeString()
{
    time_t rawTime;
    struct tm now;
    char buffer[80];
    
    time(&rawTime);
    localtime_s(&now, &rawTime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M:%S", &now);
    string retTimeString(buffer);

    return retTimeString;
}

string cLogManager::GetTimeStampSimple()
{
    time_t rawTime;
    struct tm now;
    char buffer[80];

    time(&rawTime);
    localtime_s(&now, &rawTime);

    strftime(buffer, sizeof(buffer), "%Y%m%d-%I%M%S", &now);
    string retTimeString(buffer);

    return retTimeString;
}

string cLogManager::GetCurrentPath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");

    return string(buffer).substr(0, pos);
}

void cLogManager::Setup(string LogPath)
{
    m_szDate = GetTimeStampSimple();
    m_szLogPath = GetCurrentPath();
    m_szLogPath.append(LogPath);
    CreateLogDir();
    m_szLogFilename = m_szLogPath + m_szDate + "-Log.txt";
}

void cLogManager::WriteLog(LOG_LEVEL LogLevel, string Message)
{
    string szLog = "";
    string szJoint = "";
    szLog.append(GetTimeString());
    switch (LogLevel)
    {
    case EL_DEBUG:
        szJoint = "[DEBUG]";
        break;
    case EL_INFO:
        szJoint = "[INFO]";
        break;
    case EL_ALERT:
        szJoint = "[ALERT]";
        break;
    case EL_WARNING:
        szJoint = "[WARNING]";
        break;
    case EL_ERROR:
        szJoint = "[ERROR]";
        break;
    }
    szLog.append(szJoint);
    szLog.append(Message + "\n");

    ofstream writeLog;
    writeLog.open(m_szLogFilename, ios_base::app);
    writeLog << szLog;
}