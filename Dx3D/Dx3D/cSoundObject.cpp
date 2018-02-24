#include "stdafx.h"
#include "cSoundObject.h"


void cSoundObject::Setup()
{
    m_pSound = NULL;
    m_pChannel = NULL;
    m_szScnName = "";
    m_isBgm = false;
    m_isLoop = false;
    m_szFilename = "";
}

void cSoundObject::Destroy()
{
    if (m_pSound != NULL)
    {
        m_pSound = NULL;
        SAFE_DELETE(m_pSound);
    }

    if (m_pChannel != NULL)
    {
        m_pChannel = NULL;
        SAFE_DELETE(m_pChannel);
    }
}

cSoundObject::cSoundObject()
{
    Setup();
}


cSoundObject::~cSoundObject()
{
}