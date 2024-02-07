#pragma once

#include "imgui.h"

//
// Theme tweak utilities for ImGui.
// Reuse and adaptation of imgui_theme.h and imgui_theme.cpp file is granted for other projects,
// provided the origin of those files is stated in the copied version
// Some themes were adapted by themes posted by ImGui users at https://github.com/ocornut/imgui/issues/707
//
// From: https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/imgui_theme.h
//

namespace ImGuiTheme
{
    enum class Theme
    {
        ImGuiColorsClassic = 0,
        ImGuiColorsDark,
        ImGuiColorsLight,
        MaterialFlat,
        PhotoshopStyle,
        GrayVariations,
        GrayVariations_Darker,
        MicrosoftStyle,
        Cherry,
        Darcula,
        DarculaDarker,
        LightRounded,
        SoDark_AccentBlue,
        SoDark_AccentYellow,
        SoDark_AccentRed,
        BlackIsBlack,
        WhiteIsWhite,
        Count
    };
    const char* ImGuiTheme_Name(Theme theme);
    Theme ImGuiTheme_FromName(const char* themeName);
    ImGuiStyle ThemeToStyle(Theme theme);
    void ApplyTheme(Theme theme);


    struct ImGuiThemeTweaks
    {
        // Common rounding for widgets. If < 0, this is ignored.
        float Rounding = -1.f;
        // If rounding is applied, scrollbar rounding needs to be adjusted to be visually pleasing in conjunction with other widgets roundings. Only applied if Rounding > 0.f)
        float RoundingScrollbarRatio = 4.f;
        // Change the alpha that will be applied to windows, popups, etc. If < 0, this is ignored.
        float AlphaMultiplier = -1.f;

        //
        // HSV Color tweaks
        //
        // Change the hue of all widgets (gray widgets will remain gray, since their saturation is zero). If < 0, this is ignored.
        float Hue = -1.f;
        // Multiply the saturation of all widgets (gray widgets will remain gray, since their saturation is zero). If < 0, this is ignored.
        float SaturationMultiplier = -1.f;
        // Multiply the value (luminance) of all front widgets. If < 0, this is ignored.
        float ValueMultiplierFront = -1.f;
        // Multiply the value (luminance) of all backgrounds. If < 0, this is ignored.
        float ValueMultiplierBg = -1.f;
        // Multiply the value (luminance) of text. If < 0, this is ignored.
        float ValueMultiplierText = -1.f;
        // Multiply the value (luminance) of FrameBg. If < 0, this is ignored.
        // (Background of checkbox, radio button, plot, slider, text input)
        float ValueMultiplierFrameBg = -1.f;

        ImGuiThemeTweaks() {}
    };

    struct ImGuiTweakedTheme
    {
        Theme theme = Theme::DarculaDarker;
        ImGuiThemeTweaks tweaks = ImGuiThemeTweaks();
    };

    ImGuiStyle TweakedThemeThemeToStyle(const ImGuiTweakedTheme& tweaked_theme);
    void ApplyTweakedTheme(const ImGuiTweakedTheme& tweaked_theme);

    // Show the theme selection listbox, the theme tweak widgets, as well as ImGui::ShowStyleEditor. Returns true if modified (Warning, when using ShowStyleEditor, no info about modification is transmitted)
    bool ShowThemeTweakGui(ImGuiTweakedTheme *tweaked_theme);

    // Some tweakable themes
    ImGuiStyle SoDark(float hue);
    ImGuiStyle ShadesOfGray(float rounding=0.f, float value_multiplier_front=1.f, float value_multiplier_bg=1.f);
    ImGuiStyle Darcula(
        float rounding=1.f,
        float hue=-1.f,
        float saturation_multiplier=1.f,
        float value_multiplier_front=1.f,
        float value_multiplier_bg=1.f,
        float alpha_bg_transparency=1.f
    );


} // namespace ImGuiTheme
