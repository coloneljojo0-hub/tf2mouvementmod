#include "cbase.h"
#include "tf_hud_playmenu.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include "iclientmode.h"
#include <vgui/IScheme.h>
#include <vgui_controls/Controls.h>

using namespace vgui;

static CTFPlayMenu* g_pPlayMenu = NULL;

CTFPlayMenu::CTFPlayMenu(vgui::Panel* parent) : BaseClass(parent, "TFPlayMenu")
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme");
	SetScheme(scheme);

	SetTitle("Play Game", false);
	SetMoveable(true);
	SetSizeable(true);
	SetVisible(false);
	SetProportional(false);

	SetMinimumSize(340, 260);

	const int panelWide = 360;
	const int panelTall = 280;
	SetSize(panelWide, panelTall);

	// --- Tab buttons ---
	m_pTabMapBtn = new Button(this, "TabMapBtn", "Map", this, "tab_map");
	m_pTabMapBtn->SetPos(10, 30);
	m_pTabMapBtn->SetSize(160, 24);

	m_pTabDifficultyBtn = new Button(this, "TabDifficultyBtn", "Difficulty", this, "tab_difficulty");
	m_pTabDifficultyBtn->SetPos(180, 30);
	m_pTabDifficultyBtn->SetSize(160, 24);

	// --- Map page ---
	m_pMapPage = new Panel(this, "MapPage");
	m_pMapPage->SetPos(10, 65);
	m_pMapPage->SetSize(340, 190);

	m_pMapCombo = new ComboBox(m_pMapPage, "MapCombo", 6, false);
	m_pMapCombo->SetPos(10, 10);
	m_pMapCombo->SetSize(320, 24);
	m_pMapCombo->AddItem("Sandbox", NULL);
	m_pMapCombo->AddItem("Map 1 (placeholder)", NULL);
	m_pMapCombo->AddItem("Map 2 (placeholder)", NULL);
	m_pMapCombo->AddItem("Map 3 (placeholder)", NULL);
	m_pMapCombo->ActivateItem(0);

	m_pPlayButton = new Button(m_pMapPage, "BigPlayButton", "PLAY", this, "start_game");
	m_pPlayButton->SetPos(10, 60);
	m_pPlayButton->SetSize(320, 50);

	// --- Difficulty page ---
	m_pDifficultyPage = new Panel(this, "DifficultyPage");
	m_pDifficultyPage->SetPos(10, 65);
	m_pDifficultyPage->SetSize(340, 190);

	m_pDifficultyCombo = new ComboBox(m_pDifficultyPage, "DifficultyCombo", 6, false);
	m_pDifficultyCombo->SetPos(10, 10);
	m_pDifficultyCombo->SetSize(320, 24);
	m_pDifficultyCombo->AddItem("Easy (placeholder)", NULL);
	m_pDifficultyCombo->AddItem("Normal (placeholder)", NULL);
	m_pDifficultyCombo->AddItem("Hard (placeholder)", NULL);
	m_pDifficultyCombo->ActivateItem(0);

	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	SetPos((screenWide - panelWide) / 2, (screenTall - panelTall) / 2);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	m_iActiveTab = 0;
	SwitchTab(0);
}

CTFPlayMenu::~CTFPlayMenu()
{
}

void CTFPlayMenu::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	SetBgColor(Color(20, 20, 20, 230));
}

void CTFPlayMenu::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide, tall;
	GetSize(wide, tall);

	m_pTabMapBtn->SetPos(10, 30);
	m_pTabDifficultyBtn->SetPos(180, 30);

	int pageWide = wide - 20;
	int pageTall = tall - 75;

	m_pMapPage->SetPos(10, 65);
	m_pMapPage->SetSize(pageWide, pageTall);

	m_pDifficultyPage->SetPos(10, 65);
	m_pDifficultyPage->SetSize(pageWide, pageTall);

	if (m_pMapCombo)
	{
		m_pMapCombo->SetSize(pageWide - 20, 24);
	}
	if (m_pPlayButton)
	{
		m_pPlayButton->SetPos(10, 60);
		m_pPlayButton->SetSize(pageWide - 20, 50);
	}
	if (m_pDifficultyCombo)
	{
		m_pDifficultyCombo->SetSize(pageWide - 20, 24);
	}
}

void CTFPlayMenu::SwitchTab(int tabIndex)
{
	m_iActiveTab = tabIndex;

	m_pMapPage->SetVisible(tabIndex == 0);
	m_pDifficultyPage->SetVisible(tabIndex == 1);
}

void CTFPlayMenu::OnCommand(const char* command)
{
	if (!Q_stricmp(command, "tab_map"))
	{
		SwitchTab(0);
		return;
	}
	else if (!Q_stricmp(command, "tab_difficulty"))
	{
		SwitchTab(1);
		return;
	}
	else if (!Q_stricmp(command, "start_game"))
	{
		// Placeholder: map names not wired to real files yet.
		// Once real maps exist, translate the selected combo index to a real map name here.
		int iMapIndex = m_pMapCombo->GetActiveItem();
		Msg("Play pressed — selected map index: %d, difficulty index: %d\n",
			iMapIndex, m_pDifficultyCombo->GetActiveItem());

		// Example of what real launch will look like once maps exist:
		// engine->ClientCmd_Unrestricted( "map <realmapname>" );

		return;
	}

	BaseClass::OnCommand(command);
}

CON_COMMAND(toggleplaymenu, "Toggle the custom play menu")
{
	ToggleplayMenu();
}

void ToggleplayMenu()
{
	if (!g_pPlayMenu)
	{
		g_pPlayMenu = new CTFPlayMenu(NULL);
		g_pPlayMenu->InvalidateLayout(true, true);
		g_pPlayMenu->SetVisible(false);
	}

	if (g_pPlayMenu->IsVisible())
	{
		g_pPlayMenu->SetVisible(false);
	}
	else
	{
		g_pPlayMenu->SetVisible(true);
		g_pPlayMenu->SetEnabled(true);
		g_pPlayMenu->MoveToFront();
		g_pPlayMenu->RequestFocus();
		g_pPlayMenu->Activate();
	}
}