#include "stdafx.h"
#include "cUIProgressBar.h"
#include "cUIObject.h"
#include "cUIImageView.h"
#include "cUITextView.h"

cUIProgressBar::cUIProgressBar()
    : m_fMaxGuage(0.0f)
    , m_fCurGuage(0.0f)
{

}

cUIProgressBar::~cUIProgressBar()
{

}

// Max ������ ���� 
// 1) �ƽ� ������(0���� ū ���� �Է��ؾ���)
HRESULT cUIProgressBar::SetMaxGuage(IN float fMaxGuage)
{
    if (fMaxGuage <= 0.0f)
    {
        return E_INVALIDARG;
    }
    else
    {
        m_fMaxGuage = fMaxGuage;
        return S_OK;
    }
}

HRESULT cUIProgressBar::SetCurrentGuage(IN float fCurGuage)
{
    m_fCurGuage = fCurGuage;
    return E_NOTIMPL;
}


// �ؽ�Ʈ ����
HRESULT cUIProgressBar::AddText(IN LPFONTDX pFont, IN DWORD TextFormat, IN D3DCOLOR color)
{
    cUITextView* pText = new cUITextView;
    pText->SetParent(this);
    pText->SetDebugRender(DEBUG_RENDER);
    pText->SetLocalPos(Vector3(0, 0, 0));
    pText->SetSize(m_stSize);
    pText->SetName("UI_PROGRESS_TEXT");
    pText->SetColor(color);
    pText->SetFont(pFont);
    pText->SetText("");
    pText->SetTag((int)E_PROGRESSBAR_TEXT);
    AddChild(pText);

    return S_OK;
}

// ���α׷����� ������ �ؽ��� ����
// 1)�ؽ��� ���(" ", "" �� �����޽��� ��ȯ), 2)������ �� �� ����(Default(FRONT) : 0, BACK : 1), 3)�÷�(Default D3DCOLOR_ARGB(255,255,255,255))
HRESULT cUIProgressBar::AddGuageTexture(IN string GuageImgPath, IN int imgType, IN ST_SIZE stSize, IN D3DCOLOR color)
{
    if (imgType < 0 || imgType > 1)
    {
        return E_INVALIDARG;
    }

    if (GuageImgPath.empty() || GuageImgPath[0] == NULL)
    {
        return E_INVALIDARG;
    }

    cUIImageView* pImgView = new cUIImageView;
    g_pTextureManager->AddTexture(GuageImgPath, GuageImgPath, true);

    D3DXIMAGE_INFO imgInfo;
    LPTEXTURE9 pTexture = (LPTEXTURE9)g_pTextureManager->GetTexture(GuageImgPath, &imgInfo);

    pImgView->SetParent(this);
    pImgView->SetDebugRender(DEBUG_RENDER);
    pImgView->SetLocalPos(Vector3((m_stSize.x * 0.5f) - (stSize.w * 0.5f), (m_stSize.y * 0.5f) - (stSize.h * 0.5f), 0));
    pImgView->SetSize(Vector2((float)imgInfo.Width, (float)imgInfo.Height));
    pImgView->SetTexture(pTexture);
    pImgView->SetColor(color);
    pImgView->SetScale(stSize.w, stSize.h);

    // ������ Ÿ��  
    if (imgType == 0)
    {
        pImgView->SetName("UI_PROGRESS_FRONT");
        pImgView->SetTag((int)E_PROGRESSBAR_TEXTURE_FRONT);
        m_stFrontSize = stSize;
        AddChild(pImgView);
    }
    else if (imgType == 1)
    {
        pImgView->SetName("UI_PROGRESS_BACK");
        pImgView->SetTag((int)E_PROGRESSBAR_TEXTURE_BACK);
        AddChild(pImgView);
    }

    return S_OK;
}

/*���� : ���α׷������� �̸�, ��ġ, ũ��, ������ ����
���� : 1)���α׷����� �̸�("", " " �Է½� ����), 2)��ġ, 3)ũ��(����, ����), 4)�ƽ������� ����*/
HRESULT cUIProgressBar::Setup(IN string strProgressBarName, IN Vector3 vPosition, IN Vector2 vSize, IN float fGuageMax)
{
    if (strProgressBarName.empty() || strProgressBarName[0] == NULL)
    {
        return E_INVALIDARG;
    }
    else
    {
        this->SetName(strProgressBarName);
    }

    this->SetLocalPos(vPosition);

    if (vSize.x <= 0.0f || vSize.y <= 0.0f)
    {
        return E_INVALIDARG;
    }
    else
    {
        this->SetSize(vSize);
    }

    // ��Ʈ ���� ������?

    HRESULT hr;
    hr = SetMaxGuage(fGuageMax);

    if (hr == E_INVALIDARG)
    {
        return E_INVALIDARG;
    }

    m_isDebugRender = DEBUG_RENDER;
    return E_NOTIMPL;
}

void cUIProgressBar::Update(IN float fCurrentGuage, IN Vector3* vFollowPosition /*= NULL*/)
{
    if (vFollowPosition)
    {
        m_vLocalPos = m_vLocalPos + *vFollowPosition;
    }

    cUITextView* obj = NULL;
    obj = (cUITextView*)GetChildByTag(E_PROGRESSBAR_TEXT);

    if (obj)
    {
        string text;
        char buffer[128];
        sprintf_s(buffer, "%.1f / %.1f", fCurrentGuage, m_fMaxGuage);
        text = buffer;
        obj->SetText(text);
    }


    // ���� ����
    if (!m_vecChild.empty())
    {
        for each(auto vPos in m_vecChild)
        {
            switch ((E_PROGRESSBAR_INNER_TYPE)vPos->GetTag())
            {
            case E_PROGRESSBAR_TEXTURE_FRONT:
            {
                // ���� ��� �� ����
                float width = fCurrentGuage / m_fMaxGuage * m_stFrontSize.w;

                if (width <= 0.0f)
                {
                    width = 0.0f;
                }
                else if (width >= m_stFrontSize.w)
                {
                    width = m_stFrontSize.w;
                }
                ((cUIImageView*)vPos)->SetScale(width, m_stSize.y);
            }
                break;
            case E_PROGRESSBAR_TEXTURE_BACK:
            {
                ((cUIImageView*)vPos)->SetScale(m_stSize.x, m_stSize.y);
            }
                break;
            case E_PROGRESSBAR_TEXT:
            {
                ((cUITextView*)vPos)->SetSize(m_stSize);
            }
                break;
            }
        }
    }
    cUIObject::Update();
}

void cUIProgressBar::Update()
{
    Update(m_fCurGuage);
}

void cUIProgressBar::Render(LPSPRITE pSprite)
{
    cUIObject* obj = GetChildByTag(E_PROGRESSBAR_TEXTURE_BACK);

    if (obj)
    {
        obj->Render(pSprite);
    }
    obj = GetChildByTag(E_PROGRESSBAR_TEXTURE_FRONT);

    if (obj)
    {
        obj->Render(pSprite);
    }
    obj = GetChildByTag(E_PROGRESSBAR_TEXT);

    if (obj)
    {
        obj->Render(pSprite);
    }
}

ULONG cUIProgressBar::Release(void)
{
    cObject::Release();

    return 0;
}
