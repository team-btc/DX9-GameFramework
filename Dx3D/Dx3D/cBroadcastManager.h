#pragma once
class cBroadcastManager
{
    SINGLETON(cBroadcastManager);

private:
    template<class T>
    queue<T*>   m_qMessage;

public:
    cBroadcastManager();
    ~cBroadcastManager();
};

