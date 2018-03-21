#include "stdafx.h"
#include "cGear.h"
#include "005.UI//cUILayer.h"
#include "005.UI//cUIImageView.h"
#include "005.UI//cUITextView.h"
#include "005.UI//cUIButton.h"

cGear::cGear()
    : m_pGearLayer(NULL)
    , m_isClickGear(false)
    , m_isOpen(false)
{
}

cGear::~cGear()
{
}

void cGear::Setup()
{
    

    SetGearUI();

    if (m_pGearLayer)
    {
        m_pGearLayer->Setup();
    }
}

void cGear::Update()
{
    m_isClickGear = false;

    if (!m_pGearLayer)
    {
        return;
    }

    m_pGearLayer->Setup();
}

void cGear::Render()
{
    if (m_pGearLayer)
    {
        m_pGearLayer->Render();
    }
}

void cGear::OpenGear()
{
}

void cGear::CloseGear()
{
}

ULONG cGear::Release(void)
{
    SAFE_DELETE(m_pGearLayer);

    return cObject::Release();
}

void cGear::SetGearUI()
{
}
