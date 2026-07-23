#include "cbase.h"
#include "tf_hud_playmenu.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include "iclientmode.h"
#include <vgui/IScheme.h>
#include <vgui_controls/Controls.h>
#include "cdll_int.h" // Needed for engine access

using namespace vgui;

static CTFPlayMenu* g_pPlayMenu = NULL;

CTFPlayMenu::CTFPlayMenu(vgui::Panel* parent) : BaseClass(parent, "TFPlayMenu")
{
	HScheme scheme = vgui::scheme()->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme");
	SetScheme(scheme);

	SetTitle("Play Game", false);
	SetMoveable(true);
	SetSizeable(true);
	SetVisible(false);
	SetProportional(false);

	SetMinimumSize(380, 420);
	SetSize(380, 420);

	// ==========================================
	// --- MAIN NAVIGATION TABS ---
	// ==========================================
	m_pTabSingleplayerBtn = new Button(this, "TabSingleplayerBtn", "Singleplayer", this, "tab_singleplayer");
	m_pTabMultiplayerBtn = new Button(this, "TabMultiplayerBtn", "Multiplayer", this, "tab_multiplayer");

	// ==========================================
	// --- SUB NAVIGATION TABS ---
	// ==========================================
	m_pSubTabCoopBtn = new Button(this, "SubTabCoopBtn", "Co-op", this, "subtab_coop");
	m_pSubTab1v1Btn = new Button(this, "SubTab1v1Btn", "1v1", this, "subtab_1v1");

	// ==========================================
	// --- PAGES ---
	// ==========================================
	m_pSingleplayerPage = new Panel(this, "SingleplayerPage");
	m_pCoopPage = new Panel(this, "CoopPage");
	m_p1v1Page = new Panel(this, "1v1Page");

	// ---------------- Singleplayer Page ----------------
	m_pSPMapLabel = new Label(m_pSingleplayerPage, "SPMapLabel", "Select Map:");
	m_pSPMapLabel->SetPos(10, 10);

	m_pSPMapCombo = new ComboBox(m_pSingleplayerPage, "SPMapCombo", 6, false);
	m_pSPMapCombo->SetPos(10, 30);
	m_pSPMapCombo->AddItem("Sandbox", NULL);
	m_pSPMapCombo->AddItem("Map 1 (placeholder)", NULL);
	m_pSPMapCombo->AddItem("Map 2 (placeholder)", NULL);
	m_pSPMapCombo->ActivateItem(0);

	m_pSPDiffLabel = new Label(m_pSingleplayerPage, "SPDiffLabel", "Select Difficulty:");
	m_pSPDiffLabel->SetPos(10, 65);

	m_pSPDifficultyCombo = new ComboBox(m_pSingleplayerPage, "SPDifficultyCombo", 6, false);
	m_pSPDifficultyCombo->SetPos(10, 85);
	m_pSPDifficultyCombo->AddItem("Easy", NULL);
	m_pSPDifficultyCombo->AddItem("Normal", NULL);
	m_pSPDifficultyCombo->AddItem("Hard", NULL);
	m_pSPDifficultyCombo->ActivateItem(0);

	m_pSPPlayButton = new Button(m_pSingleplayerPage, "SPPlayButton", "PLAY SINGLEPLAYER", this, "start_game");
	m_pSPPlayButton->SetPos(10, 130);

	// ---------------- Co-op Page ----------------
	m_pCoopMapLabel = new Label(m_pCoopPage, "CoopMapLabel", "Select Map:");
	m_pCoopMapLabel->SetPos(10, 5);

	m_pCoopMapCombo = new ComboBox(m_pCoopPage, "CoopMapCombo", 4, false);
	m_pCoopMapCombo->SetPos(10, 25);
	m_pCoopMapCombo->AddItem("Coop_Sandbox", NULL);
	m_pCoopMapCombo->AddItem("Coop_Map1", NULL);
	m_pCoopMapCombo->ActivateItem(0);

	m_pCoopDiffLabel = new Label(m_pCoopPage, "CoopDiffLabel", "Select Difficulty:");
	m_pCoopDiffLabel->SetPos(10, 55);

	m_pCoopDiffCombo = new ComboBox(m_pCoopPage, "CoopDiffCombo", 4, false);
	m_pCoopDiffCombo->SetPos(10, 75);
	m_pCoopDiffCombo->AddItem("Normal", NULL);
	m_pCoopDiffCombo->AddItem("Hard", NULL);
	m_pCoopDiffCombo->ActivateItem(0);

	m_pCoopIPLabel = new Label(m_pCoopPage, "CoopIPLabel", "Your Server IP (Share with friends):");
	m_pCoopIPLabel->SetPos(10, 105);

	m_pCoopIPEntry = new TextEntry(m_pCoopPage, "CoopIPEntry");
	m_pCoopIPEntry->SetPos(10, 125);
	m_pCoopIPEntry->SetText("127.0.0.1:27015");

	m_pCoopHostBtn = new Button(m_pCoopPage, "CoopHostBtn", "HOST & PLAY CO-OP", this, "start_coop");
	m_pCoopHostBtn->SetPos(10, 155);

	m_pCoopJoinLabel = new Label(m_pCoopPage, "CoopJoinLabel", "Join a Friend (Enter IP):");
	m_pCoopJoinLabel->SetPos(10, 205);

	m_pCoopJoinIPEntry = new TextEntry(m_pCoopPage, "CoopJoinIPEntry");
	m_pCoopJoinIPEntry->SetPos(10, 225);
	m_pCoopJoinIPEntry->SetText("");

	m_pCoopJoinBtn = new Button(m_pCoopPage, "CoopJoinBtn", "JOIN FRIEND", this, "join_coop");
	m_pCoopJoinBtn->SetPos(10, 255);

	// ---------------- 1v1 Page ----------------
	m_p1v1MapLabel = new Label(m_p1v1Page, "1v1MapLabel", "Select Map:");
	m_p1v1MapLabel->SetPos(10, 5);

	m_p1v1MapCombo = new ComboBox(m_p1v1Page, "1v1MapCombo", 4, false);
	m_p1v1MapCombo->SetPos(10, 25);
	m_p1v1MapCombo->AddItem("pvp_flatarena", NULL);
	m_p1v1MapCombo->AddItem("idk", NULL);
	m_p1v1MapCombo->ActivateItem(0);

	m_p1v1RoundsLabel = new Label(m_p1v1Page, "1v1RoundsLabel", "Rounds to Win:");
	m_p1v1RoundsLabel->SetPos(10, 55);

	m_p1v1RoundsEntry = new TextEntry(m_p1v1Page, "1v1RoundsEntry");
	m_p1v1RoundsEntry->SetPos(120, 55);
	m_p1v1RoundsEntry->SetSize(60, 20);
	m_p1v1RoundsEntry->SetText("20");

	m_p1v1NoHookCheck = new CheckButton(m_p1v1Page, "1v1NoHook", "No Grappling Hook");
	m_p1v1NoHookCheck->SetPos(10, 80);

	m_p1v1NoSapperCheck = new CheckButton(m_p1v1Page, "1v1NoSapper", "No Sapper");
	m_p1v1NoSapperCheck->SetPos(10, 105);

	m_p1v1NoKnifeCheck = new CheckButton(m_p1v1Page, "1v1NoKnife", "No Knife");
	m_p1v1NoKnifeCheck->SetPos(10, 130);

	m_p1v1IPLabel = new Label(m_p1v1Page, "1v1IPLabel", "Your Server IP (Share with opponent):");
	m_p1v1IPLabel->SetPos(10, 160);

	m_p1v1IPEntry = new TextEntry(m_p1v1Page, "1v1IPEntry");
	m_p1v1IPEntry->SetPos(10, 180);
	m_p1v1IPEntry->SetText("127.0.0.1:27015");

	m_p1v1HostBtn = new Button(m_p1v1Page, "1v1HostBtn", "HOST & PLAY 1v1", this, "start_1v1");
	m_p1v1HostBtn->SetPos(10, 210);

	m_p1v1JoinLabel = new Label(m_p1v1Page, "1v1JoinLabel", "Join Opponent (Enter IP):");
	m_p1v1JoinLabel->SetPos(10, 255);

	m_p1v1JoinIPEntry = new TextEntry(m_p1v1Page, "1v1JoinIPEntry");
	m_p1v1JoinIPEntry->SetPos(10, 275);
	m_p1v1JoinIPEntry->SetText("");

	m_p1v1JoinBtn = new Button(m_p1v1Page, "1v1JoinBtn", "JOIN OPPONENT", this, "join_1v1");
	m_p1v1JoinBtn->SetPos(10, 305);

	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	SetPos((screenWide - 380) / 2, (screenTall - 420) / 2);

	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);

	SwitchTab(0, 0);
}

CTFPlayMenu::~CTFPlayMenu()
{
}

void CTFPlayMenu::ApplySchemeSettings(IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	SetBgColor(Color(20, 20, 20, 230));
}

void CTFPlayMenu::PerformLayout()
{
	BaseClass::PerformLayout();

	int wide, tall;
	GetSize(wide, tall);

	int mainBtnWide = (wide - 30) / 2;
	m_pTabSingleplayerBtn->SetPos(10, 30);
	m_pTabSingleplayerBtn->SetSize(mainBtnWide, 24);

	m_pTabMultiplayerBtn->SetPos(20 + mainBtnWide, 30);
	m_pTabMultiplayerBtn->SetSize(mainBtnWide, 24);

	bool isMultiplayer = (m_iActiveMainTab == 1);

	m_pSubTabCoopBtn->SetVisible(isMultiplayer);
	m_pSubTab1v1Btn->SetVisible(isMultiplayer);

	int pageTop = 65;
	if (isMultiplayer)
	{
		int subBtnWide = (wide - 30) / 2;
		m_pSubTabCoopBtn->SetPos(10, 60);
		m_pSubTabCoopBtn->SetSize(subBtnWide, 22);

		m_pSubTab1v1Btn->SetPos(20 + subBtnWide, 60);
		m_pSubTab1v1Btn->SetSize(subBtnWide, 22);

		pageTop = 90;
	}

	int pageWide = wide - 20;
	int pageTall = tall - pageTop - 10;

	m_pSingleplayerPage->SetPos(10, pageTop);
	m_pSingleplayerPage->SetSize(pageWide, pageTall);

	m_pCoopPage->SetPos(10, pageTop);
	m_pCoopPage->SetSize(pageWide, pageTall);

	m_p1v1Page->SetPos(10, pageTop);
	m_p1v1Page->SetSize(pageWide, pageTall);

	int contentWide = pageWide - 20;

	// Scale Singleplayer elements
	m_pSPMapCombo->SetSize(contentWide, 24);
	m_pSPDifficultyCombo->SetSize(contentWide, 24);
	m_pSPPlayButton->SetSize(contentWide, 45);

	// Scale Co-op elements
	m_pCoopMapCombo->SetSize(contentWide, 24);
	m_pCoopDiffCombo->SetSize(contentWide, 24);
	m_pCoopIPEntry->SetSize(contentWide, 22);
	m_pCoopHostBtn->SetSize(contentWide, 35);
	m_pCoopJoinIPEntry->SetSize(contentWide, 22);
	m_pCoopJoinBtn->SetSize(contentWide, 35);

	// Scale 1v1 elements
	m_p1v1MapCombo->SetSize(contentWide, 24);
	m_p1v1NoHookCheck->SetSize(contentWide, 20);
	m_p1v1NoSapperCheck->SetSize(contentWide, 20);
	m_p1v1NoKnifeCheck->SetSize(contentWide, 20);
	m_p1v1IPEntry->SetSize(contentWide, 22);
	m_p1v1HostBtn->SetSize(contentWide, 35);
	m_p1v1JoinIPEntry->SetSize(contentWide, 22);
	m_p1v1JoinBtn->SetSize(contentWide, 35);
}

void CTFPlayMenu::SwitchTab(int mainTab, int subTab)
{
	m_iActiveMainTab = mainTab;
	m_iActiveSubTab = subTab;

	m_pSingleplayerPage->SetVisible(mainTab == 0);

	m_pCoopPage->SetVisible(mainTab == 1 && subTab == 0);
	m_p1v1Page->SetVisible(mainTab == 1 && subTab == 1);

	InvalidateLayout(true, false);
}

void CTFPlayMenu::OnCommand(const char* command)
{
	if (!Q_stricmp(command, "tab_singleplayer"))
	{
		SwitchTab(0);
		return;
	}
	else if (!Q_stricmp(command, "tab_multiplayer"))
	{
		SwitchTab(1, m_iActiveSubTab);
		return;
	}
	else if (!Q_stricmp(command, "subtab_coop"))
	{
		SwitchTab(1, 0);
		return;
	}
	else if (!Q_stricmp(command, "subtab_1v1"))
	{
		SwitchTab(1, 1);
		return;
	}
	else if (!Q_stricmp(command, "start_game"))
	{
		Msg("Starting Singleplayer. Selected map index: %d, difficulty index: %d\n",
			m_pSPMapCombo->GetActiveItem(), m_pSPDifficultyCombo->GetActiveItem());
		return;
	}
	else if (!Q_stricmp(command, "start_coop"))
	{
		Msg("Hosting Co-op on Map Index: %d, Diff: %d\n", m_pCoopMapCombo->GetActiveItem(), m_pCoopDiffCombo->GetActiveItem());
		return;
	}
	else if (!Q_stricmp(command, "join_coop"))
	{
		char szIP[64];
		m_pCoopJoinIPEntry->GetText(szIP, sizeof(szIP));
		Msg("Connecting to Co-op host: %s\n", szIP);
		return;
	}
	else if (!Q_stricmp(command, "start_1v1"))
	{
		// 1. Get the map name selected in the dropdown
		char szMapName[64] = "pvp_flatarena";
		if (m_p1v1MapCombo)
		{
			m_p1v1MapCombo->GetText(szMapName, sizeof(szMapName));
		}

		// 2. Get the round score limit entered by the user
		int iRounds = 20;
		if (m_p1v1RoundsEntry)
		{
			char szRounds[16];
			m_p1v1RoundsEntry->GetText(szRounds, sizeof(szRounds));
			if (Q_strlen(szRounds) > 0)
			{
				iRounds = atoi(szRounds);
			}
		}

		// 3. Construct the server launch command string
		char szCmd[256];
		Q_snprintf(szCmd, sizeof(szCmd), "map %s +sv_force_spy_mode 1 +start_1v1_mode %d\n", szMapName, iRounds);

		// 4. Send command to the client engine interface
		engine->ClientCmd_Unrestricted(szCmd);

		// Close menu after starting
		SetVisible(false);
		return;
	}
	else if (!Q_stricmp(command, "join_1v1"))
	{
		char szIP[64];
		m_p1v1JoinIPEntry->GetText(szIP, sizeof(szIP));
		if (Q_strlen(szIP) > 0)
		{
			char szConnectCmd[128];
			Q_snprintf(szConnectCmd, sizeof(szConnectCmd), "connect %s\n", szIP);
			engine->ClientCmd_Unrestricted(szConnectCmd);
			SetVisible(false);
		}
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