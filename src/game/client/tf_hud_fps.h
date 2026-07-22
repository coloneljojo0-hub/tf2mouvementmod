#pragma once
//========= Copyright Valve Corporation, All rights reserved. ============//
//
// TF FPS Counter HUD Element
//
//=============================================================================
#ifndef TF_HUD_FPS_H
#define TF_HUD_FPS_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include <vgui_controls/Panel.h>

class CTFHudFPS : public CHudElement, public vgui::Panel
{
    DECLARE_CLASS_SIMPLE(CTFHudFPS, vgui::Panel);

public:
    CTFHudFPS(const char* pElementName);

    virtual void Init() OVERRIDE;
    virtual void Reset() OVERRIDE;
    virtual bool ShouldDraw() OVERRIDE;
    virtual void OnThink() OVERRIDE;
    virtual void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;

protected:
    virtual void Paint() OVERRIDE;

private:
    void UpdateFont();

    vgui::HFont m_hFont;
    float m_flFramerate;
    int m_iCachedFontSize;
};

#endif // TF_HUD_FPS_H