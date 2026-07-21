	#pragma once
#pragma once
#ifndef TF_HUD_PLAYMENU_H
#define TF_HUD_PLAYMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>

class CTFPlayMenu : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CTFPlayMenu, vgui::Frame);

public:
	CTFPlayMenu(vgui::Panel* parent);
	virtual ~CTFPlayMenu();

	virtual void OnCommand(const char* command);
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);
	virtual void PerformLayout();

	void SwitchTab(int tabIndex);

public:
	vgui::Button* m_pTabMapBtn;
	vgui::Button* m_pTabDifficultyBtn;

	vgui::Panel* m_pMapPage;
	vgui::Panel* m_pDifficultyPage;

	vgui::ComboBox* m_pMapCombo;
	vgui::ComboBox* m_pDifficultyCombo;

	vgui::Button* m_pPlayButton;

	int m_iActiveTab;
};

void ToggleplayMenu();

#endif // TF_HUD_PLAYMENU_H