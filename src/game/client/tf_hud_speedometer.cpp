#include "cbase.h"
#include "tf_hud_speedometer.h"
#include "c_tf_player.h"
#include "clientmode_shared.h"
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <vgui/IScheme.h>

ConVar cl_speedometer_enabled("cl_speedometer_enabled", "1", FCVAR_ARCHIVE, "Show/hide the speedometer");
ConVar cl_speedometer_unit("cl_speedometer_unit", "0", FCVAR_ARCHIVE, "Speedometer unit: 0 = u/s, 1 = MPH, 2 = km/h, 3 = None");
ConVar cl_speedometer_color("cl_speedometer_color", "0", FCVAR_ARCHIVE, "Speedometer color: 0 = White, 1 = Green, 2 = Cyan, 3 = Yellow, 4 = Red");
ConVar cl_speedometer_size("cl_speedometer_size", "30", FCVAR_ARCHIVE, "Speedometer font size");
ConVar cl_speedometer_x("cl_speedometer_x", "-1", FCVAR_ARCHIVE, "Speedometer X position (-1 for default center)");
ConVar cl_speedometer_y("cl_speedometer_y", "-1", FCVAR_ARCHIVE, "Speedometer Y position (-1 for default center)");

using namespace vgui;

DECLARE_HUDELEMENT(CTFHudSpeedometer);

CTFHudSpeedometer::CTFHudSpeedometer(const char* pElementName)
    : CHudElement(pElementName), BaseClass(NULL, "HudSpeedometer")
{
    Panel* pParent = g_pClientMode->GetViewport();
    SetParent(pParent);

    SetHiddenBits(HIDEHUD_MISCSTATUS);
    m_flCurrentSpeed = 0.0f;
    m_iCachedFontSize = 0;

    SetSize(200, 60);
}

void CTFHudSpeedometer::Init()
{
    m_flCurrentSpeed = 0.0f;
    surface()->AddCustomFontFile("Roboto", "resource/Roboto-Regular.ttf");
}

void CTFHudSpeedometer::Reset()
{
    m_flCurrentSpeed = 0.0f;
}

void CTFHudSpeedometer::UpdateFont()
{
    int fontSize = cl_speedometer_size.GetInt();
    if (fontSize < 12) fontSize = 12;
    if (fontSize > 72) fontSize = 72;

    if (m_iCachedFontSize != fontSize || !m_hFont)
    {
        m_hFont = surface()->CreateFont();
        surface()->SetFontGlyphSet(
            m_hFont,
            "Roboto",
            fontSize,
            500,
            0,
            0,
            0x001   // FONTFLAG_ANTIALIAS
        );
        m_iCachedFontSize = fontSize;
    }
}

void CTFHudSpeedometer::ApplySchemeSettings(IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);

    UpdateFont();

    int screenWide, screenTall;
    surface()->GetScreenSize(screenWide, screenTall);

    int panelWide, panelTall;
    GetSize(panelWide, panelTall);

    int posX = cl_speedometer_x.GetInt();
    int posY = cl_speedometer_y.GetInt();

    if (posX < 0) posX = (screenWide - panelWide) / 2;
    if (posY < 0) posY = (screenTall / 2) + 30;

    SetPos(posX, posY);
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

    Vector vecVelocity = pPlayer->GetAbsVelocity();
    m_flCurrentSpeed = vecVelocity.Length2D();

    UpdateFont();

    int screenWide, screenTall;
    surface()->GetScreenSize(screenWide, screenTall);

    int panelWide, panelTall;
    GetSize(panelWide, panelTall);

    int posX = cl_speedometer_x.GetInt();
    int posY = cl_speedometer_y.GetInt();

    if (posX < 0) posX = (screenWide - panelWide) / 2;
    if (posY < 0) posY = (screenTall / 2) + 30;

    SetPos(posX, posY);
}

void CTFHudSpeedometer::Paint()
{
    wchar_t wszSpeedText[64];
    float displaySpeed = m_flCurrentSpeed;

    int unitMode = cl_speedometer_unit.GetInt();
    if (unitMode == 1)
    {
        // Convert to MPH
        displaySpeed = m_flCurrentSpeed * (0.75f / 12.0f) * (3600.0f / 5280.0f);
        V_swprintf_safe(wszSpeedText, L"%.1f MPH", displaySpeed);
    }
    else if (unitMode == 2)
    {
        // Convert to km/h
        displaySpeed = (m_flCurrentSpeed * (0.75f / 12.0f) * (3600.0f / 5280.0f)) * 1.60934f;
        V_swprintf_safe(wszSpeedText, L"%.1f km/h", displaySpeed);
    }
    else if (unitMode == 3)
    {
        // None (Numbers only)
        V_swprintf_safe(wszSpeedText, L"%.1f", displaySpeed);
    }
    else
    {
        // Default u/s
        V_swprintf_safe(wszSpeedText, L"%.1f u/s", displaySpeed);
    }

    // Determine text color based on ConVar
    Color textColor(255, 255, 255, 255);
    switch (cl_speedometer_color.GetInt())
    {
    case 1: textColor = Color(100, 255, 100, 255); break; // Green
    case 2: textColor = Color(0, 225, 255, 255);   break; // Cyan
    case 3: textColor = Color(255, 225, 50, 255);  break; // Yellow
    case 4: textColor = Color(255, 80, 80, 255);   break; // Red
    default: textColor = Color(255, 255, 255, 255); break; // White
    }

    int textWide, textTall;
    surface()->GetTextSize(m_hFont, wszSpeedText, textWide, textTall);

    int panelWide, panelTall;
    GetSize(panelWide, panelTall);

    surface()->DrawSetTextFont(m_hFont);
    surface()->DrawSetTextColor(textColor);
    surface()->DrawSetTextPos((panelWide - textWide) / 2, (panelTall - textTall) / 2);
    surface()->DrawPrintText(wszSpeedText, wcslen(wszSpeedText));
}