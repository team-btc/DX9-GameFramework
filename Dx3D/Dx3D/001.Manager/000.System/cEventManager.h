#pragma once

#define g_pEventManager cEventManager::GetInstance()

class cEvent;
class cEventElem;

class cEventManager
{
    SINGLETON(cEventManager);

private:
    cEvent*                 m_pCurrEvent;
    map<string, cEvent*>    m_mapEvent;
    float                   m_fStandByTime;

public:
    //  Event ������ ��ü�� �޸𸮿� �ε�.
    HRESULT Load(int nMapId);

    //  Event �߻� üũ.
    HRESULT Update();

    //  EventManager ����
    HRESULT Destroy();
};

class cEvent
{
public:
    enum E_TYPE
    {
        WAIT = 0, MOVE, ANIMATION, DIALOG, BGM, SFX, UI_TOGGLE, QUEST, WARP
    };

private:
    list<cEventElem*>    m_listEventElem;

public:
    cEvent();
    ~cEvent();

    HRESULT Destroy();
};

class cEventElem
{
public:
    enum E_STATUS
    {
        STOP = 0, PLAY, OVER, EXPIRE
    };

    int                 m_nId;
    string              m_szName;
    cEvent::E_TYPE      m_eType;
    E_STATUS            m_eStatus;

public:
    cEventElem();
    ~cEventElem();
};