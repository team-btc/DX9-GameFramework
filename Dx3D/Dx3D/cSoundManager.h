#pragma once
#include <fmod.hpp>
#pragma comment(lib, "fmod64_vc.lib")   //  x64
using namespace FMOD;

#include "cSoundObject.h"

#define SOUND_BUFFER 20
#define EXTRA_CHANNEL_BUFFER 5
#define TOTAL_SOUND_BUFFER (SOUND_BUFFER + EXTRA_CHANNEL_BUFFER)

#define g_pSndManager  cSoundManager::GetInstance()

typedef map<string, cSoundObject*>           SoundMap;
typedef map<string, cSoundObject*>::iterator SoundIter;
class cSoundManager
{
private:
    SoundMap    m_mapSounds;
    SoundIter   m_iterSounds;

    System*     m_pSystem;

public:
    cSoundManager();
    ~cSoundManager();

    void Update();
    void AddSound(string Key, string Scene, string Filename, bool IsBgm = false, bool IsLoop = false);

    void Play(string Key, float volume = 1.0f);
    void Stop(string Key);
    string StopBgm();
    void Pause(string Key);
    void Resume(string Key);
    bool IsPlay(string Key);
    bool IsPause(string Key);

    void PlayAndPause(string Key);

    void ReleaseAll();
    void ReleaseByScene(string Scene);
};

