#pragma once

class cSoundObject
{
public:
    //  Sound object
    Sound*      m_pSound;
    Channel*    m_pChannel;
private:
    //  Info
    string  m_szScnName;
    bool    m_isBgm;
    bool    m_isLoop;
    string  m_szFilename;

public:
    void Setup();
    void Destroy();

    cSoundObject();
    ~cSoundObject();

    bool    IsBgm() { return m_isBgm; }
    bool    IsLoop() { return m_isLoop; }
    string  GetFilename() { return m_szFilename; }
    string  GetScnName() { return m_szScnName; }

    void SetBgm(bool Bgm) { m_isBgm = Bgm; }
    void SetLoop(bool Loop) { m_isLoop = Loop; }
    void SetFilename(string Filename) { m_szFilename = Filename; }
    void SetScnName(string ScnName) { m_szScnName = ScnName; }
};