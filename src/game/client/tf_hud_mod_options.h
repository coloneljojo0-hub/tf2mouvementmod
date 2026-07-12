#ifndef TF_HUD_MOD_OPTIONS_H
#define TF_HUD_MOD_OPTIONS_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/PropertySheet.h>
#include <vgui_controls/PropertyPage.h>
#include <vgui_controls/Slider.h>
#include <vgui_controls/Label.h>

class CModOptionsPage_HUD : public vgui::PropertyPage
{
    DECLARE_CLASS_SIMPLE(CModOptionsPage_HUD, vgui::PropertyPage);

public:
    CModOptionsPage_HUD(vgui::Panel* pParent, const char* pName);

    virtual void OnThink() OVERRIDE;

private:
    vgui::Slider* m_pSliderXPos;
    vgui::Slider* m_pSliderYPos;
    vgui::Slider* m_pSliderSize;
    vgui::Slider* m_pSliderR;
    vgui::Slider* m_pSliderG;
    vgui::Slider* m_pSliderB;

    vgui::Label* m_pLabelXPos;
    vgui::Label* m_pLabelYPos;
    vgui::Label* m_pLabelSize;
    vgui::Label* m_pLabelR;
    vgui::Label* m_pLabelG;
    vgui::Label* m_pLabelB;
};

class CTFModOptionsPanel : public vgui::Frame
{
    DECLARE_CLASS_SIMPLE(CTFModOptionsPanel, vgui::Frame);

public:
    CTFModOptionsPanel(vgui::Panel* pParent);

private:
    vgui::PropertySheet* m_pSheet;
};

void OpenModOptionsPanel();

#endif // TF_HUD_MOD_OPTIONS_H