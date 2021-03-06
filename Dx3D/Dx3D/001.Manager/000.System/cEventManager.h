#pragma once
#include "iSingletonManager.h"

#define g_pEventManager cEventManager::GetInstance()

class cEvent;
class cEventElem;

class cEventManager : public iSingletonManager
{
    SINGLETON(cEventManager);

private:
    cEvent*                 m_pCurrEvent;
    map<string, cEvent*>    m_mapEvent;
    float                   m_fStandByTime;

public:
    //  Event 데이터 전체를 메모리에 로드.
    HRESULT Load(int nMapId);

    // iSingletonManager을(를) 통해 상속됨
    virtual HRESULT Setup() override;
    virtual HRESULT Update() override;
    virtual HRESULT Render() override;
    virtual HRESULT Destroy() override;
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