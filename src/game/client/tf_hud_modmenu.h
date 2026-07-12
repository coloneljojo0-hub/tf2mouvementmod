#pragma once
#ifndef TF_HUD_MODMENU_H
#define TF_HUD_MODMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Panel.h>

class CTFModMenu : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CTFModMenu, vgui::Frame);

public:
	CTFModMenu(vgui::Panel* parent);
	virtual ~CTFModMenu();

	virtual void OnCommand(const char* command);
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);
	virtual void PerformLayout();

	void SwitchTab(int tabIndex);

public:
	vgui::CheckButton* m_pSpeedometerCheck;

	vgui::Button* m_pTabMovementBtn;
	vgui::Button* m_pTabHudBtn;

	vgui::Panel* m_pMovementPage;
	vgui::Panel* m_pHudPage;

	int m_iActiveTab;
};

void ToggleModMenu();

#endif // TF_HUD_MODMENU_H