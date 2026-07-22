#include "cbase.h"
#include "tf_hud_fps.h"
#include "c_tf_player.h"
#include "clientmode_shared.h"
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <vgui/IScheme.h>

ConVar cl_fps_enabled("cl_fps_enabled", "0", FCVAR_ARCHIVE, "Show/hide custom FPS counter");
ConVar cl_fps_color("cl_fps_color", "0", FCVAR_ARCHIVE, "FPS color: 0 = White, 1 = Green, 2 = Cyan, 3 = Yellow, 4 = Red");
ConVar cl_fps_size("cl_fps_size", "24", FCVAR_ARCHIVE, "FPS font size");
ConVar cl_fps_x("cl_fps_x", "10", FCVAR_ARCHIVE, "FPS X position");
ConVar cl_fps_y("cl_fps_y", "10", FCVAR_ARCHIVE, "FPS Y position");

using namespace vgui;

DECLARE_HUDELEMENT(CTFHudFPS);

CTFHudFPS::CTFHudFPS(const char* pElementName)
    : CHudElement(pElementName), BaseClass(NULL, "HudFPS")
{
    Panel* pParent = g_pClientMode->GetViewport();
    SetParent(pParent);

    SetHiddenBits(HIDEHUD_MISCSTATUS);
    m_flFramerate = 0.0f;
    m_iCachedFontSize = 0;

    SetSize(160, 50);
}

void CTFHudFPS::Init()
{
    m_flFramerate = 0.0f;
    surface()->AddCustomFontFile("Roboto", "resource/Roboto-Regular.ttf");
}

void CTFHudFPS::Reset()
{
    m_flFramerate = 0.0f;
}

void CTFHudFPS::UpdateFont()
{
    int fontSize = cl_fps_size.GetInt();
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

void CTFHudFPS::ApplySchemeSettings(IScheme* pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);
    UpdateFont();

    SetPos(cl_fps_x.GetInt(), cl_fps_y.GetInt());
}

bool CTFHudFPS::ShouldDraw()
{
    if (!cl_fps_enabled.GetBool())
        return false;

    return CHudElement::ShouldDraw();
}

void CTFHudFPS::OnThink()
{
    // Compute current frame rate from frame time
    if (gpGlobals->absoluteframetime > 0.0f)
    {
        float flInstantFPS = 1.0f / gpGlobals->absoluteframetime;
        // Smooth frame rate slightly across frames
        m_flFramerate = (m_flFramerate * 0.9f) + (flInstantFPS * 0.1f);
    }

    UpdateFont();
    SetPos(cl_fps_x.GetInt(), cl_fps_y.GetInt());
}

void CTFHudFPS::Paint()
{
    wchar_t wszFPSText[32];
    V_swprintf_safe(wszFPSText, L"%.0f FPS", m_flFramerate);

    Color textColor(255, 255, 255, 255);
    switch (cl_fps_color.GetInt())
    {
    case 1: textColor = Color(100, 255, 100, 255); break; // Green
    case 2: textColor = Color(0, 225, 255, 255);   break; // Cyan
    case 3: textColor = Color(255, 225, 50, 255);  break; // Yellow
    case 4: textColor = Color(255, 80, 80, 255);   break; // Red
    default: textColor = Color(255, 255, 255, 255); break; // White
    }

    surface()->DrawSetTextFont(m_hFont);
    surface()->DrawSetTextColor(textColor);
    surface()->DrawSetTextPos(0, 0);
    surface()->DrawPrintText(wszFPSText, wcslen(wszFPSText));
}