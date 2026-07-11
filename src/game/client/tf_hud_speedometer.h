#pragma once
#ifndef TF_HUD_SPEEDOMETER_H
#define TF_HUD_SPEEDOMETER_H

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>

class CTFHudSpeedometer : public CHudElement, public vgui::Panel
{
    DECLARE_CLASS_SIMPLE(CTFHudSpeedometer, vgui::Panel);

public:
    CTFHudSpeedometer(const char* pElementName);

    virtual void Init() OVERRIDE;
    virtual void Reset() OVERRIDE;
    virtual bool ShouldDraw() OVERRIDE;
    virtual void OnThink() OVERRIDE;
    virtual void ApplySchemeSettings(vgui::IScheme* pScheme) OVERRIDE;

protected:
    virtual void Paint() OVERRIDE;

private:
    vgui::HFont m_hFont;
    float m_flCurrentSpeed;
};

#endif // TF_HUD_SPEEDOMETER_H