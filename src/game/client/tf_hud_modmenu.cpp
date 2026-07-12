#include "cbase.h"
#include "tf_hud_modmenu.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/Button.h>
#include "vgui_controls/Panel.h"
#include "iclientmode.h"
#include <vgui/IScheme.h>
#include <vgui_controls/Controls.h>

using namespace vgui;

static CTFModMenu* g_pModMenu = NULL;
extern ConVar cl_speedometer_enabled;

CTFModMenu::CTFModMenu(vgui::Panel* parent) : BaseClass(parent, "TFModMenu")
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme");
	SetScheme(scheme);

	SetTitle("Mod Menu", false);
	SetMoveable(true);
	SetSizeable(true);
	SetVisible(false);
	SetProportional(false);

	SetMinimumSize(300, 200);

	const int panelWide = 320;
	const int panelTall = 240;
	SetSize(panelWide, panelTall);

	// --- Tab buttons ---
	m_pTabMovementBtn = new Button(this, "TabMovementBtn", "Movement", this, "tab_movement");
	m_pTabMovementBtn->SetPos(10, 30);
	m_pTabMovementBtn->SetSize(140, 24);

	m_pTabHudBtn = new Button(this, "TabHudBtn", "HUD", this, "tab_hud");
	m_pTabHudBtn->SetPos(160, 30);
	m_pTabHudBtn->SetSize(140, 24);

	// --- Movement page (empty for now) ---
	m_pMovementPage = new Panel(this, "MovementPage");
	m_pMovementPage->SetPos(10, 65);
	m_pMovementPage->SetSize(300, 160);

	// --- HUD page ---
	m_pHudPage = new Panel(this, "HudPage");
	m_pHudPage->SetPos(10, 65);
	m_pHudPage->SetSize(300, 160);

	m_pSpeedometerCheck = new CheckButton(m_pHudPage, "SpeedometerCheck", "Show Speedometer");
	m_pSpeedometerCheck->SetPos(10, 10);
	m_pSpeedometerCheck->SetSize(250, 24);
	m_pSpeedometerCheck->SetSelected(cl_speedometer_enabled.GetBool());
	m_pSpeedometerCheck->AddActionSignalTarget(this);

	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	SetPos((screenWide - panelWide) / 2, (screenTall - panelTall) / 2);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	m_iActiveTab = 1; // default to HUD tab since that's the only functional one right now
	SwitchTab(1);
}

CTFModMenu::~CTFModMenu()
{
}

void CTFModMenu::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	SetBgColor(Color(20, 20, 20, 230));
}

void CTFModMenu::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide, tall;
	GetSize(wide, tall);

	// Keep tab buttons and pages stretched to the resized window
	m_pTabMovementBtn->SetPos(10, 30);
	m_pTabHudBtn->SetPos(160, 30);

	int pageWide = wide - 20;
	int pageTall = tall - 75;

	m_pMovementPage->SetPos(10, 65);
	m_pMovementPage->SetSize(pageWide, pageTall);

	m_pHudPage->SetPos(10, 65);
	m_pHudPage->SetSize(pageWide, pageTall);
}

void CTFModMenu::SwitchTab(int tabIndex)
{
	m_iActiveTab = tabIndex;

	m_pMovementPage->SetVisible(tabIndex == 0);
	m_pHudPage->SetVisible(tabIndex == 1);
}

void CTFModMenu::OnCommand(const char* command)
{
	if (!Q_stricmp(command, "tab_movement"))
	{
		SwitchTab(0);
		return;
	}
	else if (!Q_stricmp(command, "tab_hud"))
	{
		SwitchTab(1);
		return;
	}

	BaseClass::OnCommand(command);
}

CON_COMMAND(togglemodmenu, "Toggle the custom mod menu")
{
	ToggleModMenu();
}

void ToggleModMenu()
{
	if (!g_pModMenu)
	{
		g_pModMenu = new CTFModMenu(NULL);
		g_pModMenu->InvalidateLayout(true, true);
		g_pModMenu->SetVisible(false);
	}

	if (g_pModMenu->IsVisible())
	{
		cl_speedometer_enabled.SetValue(g_pModMenu->m_pSpeedometerCheck->IsSelected() ? 1 : 0);
		g_pModMenu->SetVisible(false);
	}
	else
	{
		g_pModMenu->m_pSpeedometerCheck->SetSelected(cl_speedometer_enabled.GetBool());
		g_pModMenu->SetVisible(true);
		g_pModMenu->SetEnabled(true);
		g_pModMenu->MoveToFront();
		g_pModMenu->RequestFocus();
		g_pModMenu->Activate();
	}
}