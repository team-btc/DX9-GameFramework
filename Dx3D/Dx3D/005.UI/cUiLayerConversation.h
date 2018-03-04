#pragma once
#include "cUILayer.h"

class cUiLayerConversation : public cUILayer
{
private:
    enum E_IMG_PLACE
    {
        E_IMG_PLACE_CENTER, E_IMG_PLACE_LEFT, E_IMG_PLACE_RIGHT
    };
    string strPersonName;
public:
    cUiLayerConversation();
    virtual ~cUiLayerConversation();
};