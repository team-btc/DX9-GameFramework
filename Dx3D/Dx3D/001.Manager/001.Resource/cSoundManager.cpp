#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{
    System_Create(&m_pSystem);
    FMOD_RESULT fr;
    fr = m_pSystem->init(TOTAL_SOUND_BUFFER, FMOD_INIT_NORMAL, NULL);
}


cSoundManager::~cSoundManager()
{
    if (m_pSystem != NULL)
    {
        FMOD_RESULT fr;
        fr = m_pSystem->release();
        fr = m_pSystem->close();
    }
}

void cSoundManager::Setup()
{
    AddSound("player-attack-01", "play", "Assets\\Sound\\Effect\\Arthaslichking\\hr_lich king_attack01.ogg", false, false);
    AddSound("player-attack-02", "play", "Assets\\Sound\\Effect\\Arthaslichking\\hr_lich king_attack02.ogg", false, false);
    AddSound("player-attack-03", "play", "Assets\\Sound\\Effect\\Arthaslichking\\hr_lich king_attack03.ogg", false, false);
    AddSound("store-open", "play", "Assets\\Sound\\Effect\\Store\\auctionwindowclose.ogg", false, false);
    AddSound("store-close", "play", "Assets\\Sound\\Effect\\Store\\auctionwindowopen.ogg", false, false);
    AddSound("shop-01", "play", "Assets\\Sound\\Effect\\Store\\humanmalemerchantcstartshopping01.ogg", false, false);
    AddSound("shop-02", "play", "Assets\\Sound\\Effect\\Store\\humanmalemerchantcstartshopping04.ogg", false, false);
    AddSound("shop-03", "play", "Assets\\Sound\\Effect\\Store\\humanmalemerchantcstartshopping09.ogg", false, false);
    AddSound("shop-04", "play", "Assets\\Sound\\Effect\\Store\\humanmalemerchantcstartshopping13.ogg", false, false);
    AddSound("shop-05", "play", "Assets\\Sound\\Effect\\Store\\humanmalemerchantcstartshopping18.ogg", false, false);
    AddSound("shop-06", "play", "Assets\\Sound\\Effect\\Store\\humanmalemerchantcstartshopping19.ogg", false, false);
}

void cSoundManager::Update()
{
    FMOD_RESULT fr = m_pSystem->update();
}

void cSoundManager::AddSound(string Key, string Scene, string Filename, bool IsBgm, bool IsLoop)
{
    FMOD_RESULT fr;

    g_pLogManager->WriteLog(EL_INFO, "Add new sound");
    g_pLogManager->WriteLog(EL_INFO, Key + " : " + Filename);

    cSoundObject* newSound;
    m_mapSounds.insert(make_pair(Key, new cSoundObject));
    newSound = m_mapSounds.find(Key)->second;
    newSound->SetFilename(Filename);
    newSound->SetScnName(Scene);
    newSound->SetBgm(IsBgm);
    newSound->SetLoop(IsLoop);

    if (IsLoop)
        if (IsBgm)
            fr = m_pSystem->createStream(Filename.c_str(), FMOD_LOOP_NORMAL, NULL, &newSound->m_pSound);
        else
            fr = m_pSystem->createSound(Filename.c_str(), FMOD_LOOP_NORMAL, NULL, &newSound->m_pSound);
    else
        fr = m_pSystem->createSound(Filename.c_str(), FMOD_DEFAULT, NULL, &newSound->m_pSound);
}

void cSoundManager::Play(string Key, float volume)
{
    m_iterSounds = m_mapSounds.find(Key);
    if (m_iterSounds != m_mapSounds.end())
    {
        if (m_iterSounds->second->IsBgm()) { StopBgm(); }
        FMOD_RESULT fr = m_pSystem->playSound(m_iterSounds->second->m_pSound,
                             NULL,
                             false,
                             &m_iterSounds->second->m_pChannel);
        fr = m_iterSounds->second->m_pChannel->setVolume(volume);
    }
    else
    {
        string szError = Key + " NO CHANNEL TO PLAY";
        g_pLogManager->WriteLog(EL_ERROR, szError);
    }
    
}

void cSoundManager::Stop(string Key)
{
    FMOD_RESULT fr;
    SoundIter iter = m_mapSounds.find(Key);
    if (iter != m_mapSounds.end())
    {
        fr = iter->second->m_pChannel->stop();
    }
    else
    {
        string szError = Key + " NO CHANNEL TO STOP";
        g_pLogManager->WriteLog(EL_ERROR, szError);
    }
}

string cSoundManager::StopBgm()
{
    string result = "";
    for (auto iter = m_mapSounds.begin(); iter != m_mapSounds.end(); iter++)
    {
        if (iter->second->IsBgm() && IsPlay(iter->first))
        {
            result = iter->first;
            Stop(result);
            break;
        }
    }
    return result;
}

void cSoundManager::Pause(string Key)
{
    FMOD_RESULT fr;
    m_iterSounds = m_mapSounds.find(Key); 
    if (m_iterSounds != m_mapSounds.end())
    {
        fr = m_iterSounds->second->m_pChannel->setPaused(true);
    }
    else
    {
        string szError = Key + " NO CHANNEL TO PAUSE";
        g_pLogManager->WriteLog(EL_ERROR, szError);
    }
}

void cSoundManager::Resume(string Key)
{
    FMOD_RESULT fr;
    m_iterSounds = m_mapSounds.find(Key);
    if (m_iterSounds != m_mapSounds.end())
    {
        fr = m_iterSounds->second->m_pChannel->setPaused(false);
    }
    else
    {
        string szError = Key + " NO CHANNEL TO RESUME";
        g_pLogManager->WriteLog(EL_ERROR, szError);
    }
}

bool cSoundManager::IsPlay(string Key)
{
    FMOD_RESULT fr;
    bool isPlay = false;
    SoundIter iter = m_mapSounds.find(Key);
    if (iter != m_mapSounds.end())
    {
        fr = iter->second->m_pChannel->isPlaying(&isPlay);
    }
    else
    {
        string szError = Key + " NO CHANNEL PLAYING";
        g_pLogManager->WriteLog(EL_ERROR, szError);
    }

    return isPlay;
}

bool cSoundManager::IsPause(string Key)
{
    FMOD_RESULT fr;
    bool isPause = false;
    m_iterSounds = m_mapSounds.find(Key);
    if (m_iterSounds != m_mapSounds.end())
    {
        fr = m_iterSounds->second->m_pChannel->getPaused(&isPause);
    }
    else
    {
        string szError = Key + " NO CHANNEL PAUSED";
        g_pLogManager->WriteLog(EL_ERROR, szError);
    }

    return isPause;
}

void cSoundManager::PlayAndPause(string Key)
{
    if (IsPause(Key))
        Resume(Key);
    else
        Pause(Key);
}

void cSoundManager::ReleaseAll()
{
    FMOD_RESULT fr;
    m_iterSounds = m_mapSounds.begin();
    while (m_iterSounds != m_mapSounds.end())
    {
        if (m_iterSounds->second != NULL)
        {
            fr = m_iterSounds->second->m_pChannel->stop();
            m_iterSounds->second->Destroy();
            SAFE_DELETE(m_iterSounds->second);
            m_iterSounds = m_mapSounds.erase(m_iterSounds);
        }
        else
        {
            ++m_iterSounds;
        }
    }
}

void cSoundManager::ReleaseByScene(string Scene)
{
    FMOD_RESULT fr;
    g_pLogManager->WriteLog(EL_INFO, "Sound release by scene " + Scene);
    m_iterSounds = m_mapSounds.begin();
    while (m_iterSounds != m_mapSounds.end())
    {
        if (m_iterSounds->second != NULL &&
            m_iterSounds->second->GetScnName() == Scene)
        {
            fr = m_iterSounds->second->m_pChannel->stop();
            m_iterSounds->second->Destroy();
            m_iterSounds = m_mapSounds.erase(m_iterSounds);
        }
        else
        {
            ++m_iterSounds;
        }
    }
}