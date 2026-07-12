#include "cbase.h"
#include "tf_hud_speedometer.h"
#include "c_tf_player.h"
#include "clientmode_shared.h"
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <vgui/IScheme.h>

ConVar cl_speedometer_enabled("cl_speedometer_enabled", "1", FCVAR_ARCHIVE, "Show/hide the speedometer");

using namespace vgui;

// Forward declaration — defined in tf_gamemovement.cpp

DECLARE_HUDELEMENT(CTFHudSpeedometer);

CTFHudSpeedometer::CTFHudSpeedometer(const char* pElementName)
    : CHudElement(pElementName), BaseClass(NULL, "HudSpeedometer")
{
    Panel* pParent = g_pClientMode->GetViewport();
    SetParent(pParent);

    SetHiddenBits(HIDEHUD_MISCSTATUS);
    m_flCurrentSpeed = 0.0f;

    SetSize(120, 40);
}

void CTFHudSpeedometer::Init()
{
    m_flCurrentSpeed = 0.0f;

    // Register the ttf file so "Roboto" is a valid family name to reference in ApplySchemeSettings.
    // File must exist at game/mod_tf/resource/Roboto-Regular.ttf
    surface()->AddCustomFontFile("Roboto", "resource/Roboto-Regular.ttf");
}

void CTFHudSpeedometer::Reset()
{
    m_flCurrentSpeed = 0.0f;
}

void CTFHudSpeedometer::ApplySchemeSettings(IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);

    // Build the font entirely in code, no .res file needed
    m_hFont = surface()->CreateFont();

    surface()->SetFontGlyphSet(
        m_hFont,
        "Roboto",
        30,
        500,
        0,
        0,
        0x001   // FONTFLAG_ANTIALIAS
    );

    SetFgColor(Color(255, 255, 255, 255));

    int screenWide, screenTall;
    surface()->GetScreenSize(screenWide, screenTall);

    int panelWide, panelTall;
    GetSize(panelWide, panelTall);

    // Centered horizontally, offset down from screen center (under crosshair)
    SetPos((screenWide - panelWide) / 2, (screenTall / 2) + 30);
}

bool CTFHudSpeedometer::ShouldDraw()
{
    if (!cl_speedometer_enabled.GetBool())
        return false;

    C_TFPlayer* pPlayer = C_TFPlayer::GetLocalTFPlayer();
    if (!pPlayer)
        return false;

    return CHudElement::ShouldDraw();
}

void CTFHudSpeedometer::OnThink()
{
    C_TFPlayer* pPlayer = C_TFPlayer::GetLocalTFPlayer();
    if (!pPlayer)
        return;

    // Horizontal speed only (ignore vertical) -- standard for KZ/bhop speedometers
    Vector vecVelocity = pPlayer->GetAbsVelocity();
    m_flCurrentSpeed = vecVelocity.Length2D();
}

void CTFHudSpeedometer::Paint()
{
    wchar_t wszSpeedText[32];
    V_swprintf_safe(wszSpeedText, L"%.1f u/s", m_flCurrentSpeed);

    int textWide, textTall;
    surface()->GetTextSize(m_hFont, wszSpeedText, textWide, textTall);

    int panelWide, panelTall;
    GetSize(panelWide, panelTall);

    surface()->DrawSetTextFont(m_hFont);
    surface()->DrawSetTextColor(GetFgColor());
    surface()->DrawSetTextPos((panelWide - textWide) / 2, (panelTall - textTall) / 2);
    surface()->DrawPrintText(wszSpeedText, wcslen(wszSpeedText));
}