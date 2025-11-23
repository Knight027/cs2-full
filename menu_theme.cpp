#include "menu.h"
#include "fontawesome.h"

// Theme application function
void ApplyTheme(int theme) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Reset all button styles
    useGradientButtons = false;
    useOutlineButtons = false;
    useGlassButtons = false;
    useModernButtons = false;
    themeCustomButtons = false;

    switch (theme) {
    case THEME_DEFAULT:
        colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.4f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        themePanelBg = ImVec4(0.12f, 0.12f, 0.14f, 0.4f); // New panel background
        colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.60f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.85f, 0.45f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.95f, 0.55f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.85f, 0.45f, 0.40f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.95f, 0.55f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.25f, 0.60f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.85f, 0.45f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.90f, 0.50f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.30f, 0.95f, 0.55f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.95f, 0.55f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.97f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.25f, 0.50f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.20f, 0.85f, 0.45f, 0.95f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.20f, 0.85f, 0.45f, 0.80f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);

        hoverColor = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
        activeColor = ImVec4(0.30f, 0.95f, 0.55f, 1.00f);
        themeAccent = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
        themeHeaderColor = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        themeBorderColor = ImVec4(0.20f, 0.20f, 0.25f, 0.60f);
        themeFrameBg = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        themeTextDisabled = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        themeWindowRounding = 12.0f;
        themeRoundedTabs = true;
        break;

    case THEME_DARK_PRO:
        // Professional dark theme with blue accent
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.08f, 0.4f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
        themePanelBg = ImVec4(0.10f, 0.10f, 0.12f, 0.4f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.11f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.15f, 0.15f, 0.20f, 0.60f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.14f, 0.17f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.00f, 0.47f, 0.78f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.55f, 0.85f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.47f, 0.78f, 0.40f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.55f, 0.85f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.65f, 0.95f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);

        hoverColor = ImVec4(0.00f, 0.55f, 0.85f, 1.00f);
        activeColor = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
        themeAccent = ImVec4(0.00f, 0.47f, 0.78f, 1.00f);
        themeHeaderColor = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
        themeBorderColor = ImVec4(0.15f, 0.15f, 0.20f, 0.60f);
        themeFrameBg = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        themeTextDisabled = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
        themeWindowRounding = 8.0f;
        themeRoundedTabs = false;
        themeCustomButtons = true;
        useModernButtons = true;
        break;

    case THEME_LIGHT:
        // Clean light theme
        colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.96f, 0.4f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);
        themePanelBg = ImVec4(0.40f, 0.40f, 0.45f, 0.4f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.99f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.60f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.91f, 0.93f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.88f, 0.89f, 0.91f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.94f, 0.94f, 0.96f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.65f, 0.35f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.75f, 0.45f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.65f, 0.35f, 0.40f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.75f, 0.45f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.75f, 0.45f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.65f, 1.00f);

        hoverColor = ImVec4(0.25f, 0.70f, 0.40f, 1.00f);
        activeColor = ImVec4(0.30f, 0.75f, 0.45f, 1.00f);
        themeAccent = ImVec4(0.20f, 0.65f, 0.35f, 1.00f);
        themeHeaderColor = ImVec4(0.94f, 0.94f, 0.96f, 1.00f);
        themeBorderColor = ImVec4(0.80f, 0.80f, 0.83f, 0.60f);
        themeFrameBg = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
        themeTextDisabled = ImVec4(0.60f, 0.60f, 0.65f, 1.00f);
        themeWindowRounding = 6.0f;
        themeRoundedTabs = false;
        break;

    case THEME_PURPLE_NEON:
        // Vibrant purple neon theme
        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.03f, 0.08f, 0.4f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.05f, 0.10f, 1.00f);
        themePanelBg = ImVec4(0.20f, 0.04f, 0.24f, 0.4f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.07f, 0.12f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.50f, 0.20f, 0.80f, 0.60f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.08f, 0.15f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.10f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.15f, 0.25f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.06f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.08f, 0.18f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.60f, 0.20f, 0.90f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.40f, 1.00f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.60f, 0.20f, 0.90f, 0.40f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.80f, 0.40f, 1.00f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.40f, 1.00f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.90f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.45f, 0.60f, 1.00f);

        hoverColor = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        activeColor = ImVec4(0.80f, 0.40f, 1.00f, 1.00f);
        themeAccent = ImVec4(0.60f, 0.20f, 0.90f, 1.00f);
        themeHeaderColor = ImVec4(0.12f, 0.08f, 0.18f, 1.00f);
        themeBorderColor = ImVec4(0.50f, 0.20f, 0.80f, 0.60f);
        themeFrameBg = ImVec4(0.10f, 0.08f, 0.15f, 1.00f);
        themeTextDisabled = ImVec4(0.50f, 0.45f, 0.60f, 1.00f);
        themeWindowRounding = 15.0f;
        themeRoundedTabs = true;
        themeCustomButtons = true;
        useGradientButtons = true;
        break;

    case THEME_RETRO:
        // Retro gaming theme (green terminal style)
        colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.10f, 0.05f, 0.4f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.08f, 0.04f, 1.00f);
        themePanelBg = ImVec4(0.06f, 0.40f, 0.06f, 0.4f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.12f, 0.06f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.60f, 0.30f, 0.60f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.15f, 0.08f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.20f, 0.10f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.25f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.12f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.18f, 0.09f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.00f, 0.50f, 0.25f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.70f, 0.35f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.80f, 0.40f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.50f, 0.25f, 0.40f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.70f, 0.35f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.80f, 0.40f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.80f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.80f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.20f, 1.00f);

        hoverColor = ImVec4(0.00f, 0.70f, 0.35f, 1.00f);
        activeColor = ImVec4(0.00f, 0.80f, 0.40f, 1.00f);
        themeAccent = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
        themeHeaderColor = ImVec4(0.00f, 0.18f, 0.09f, 1.00f);
        themeBorderColor = ImVec4(0.00f, 0.60f, 0.30f, 0.60f);
        themeFrameBg = ImVec4(0.00f, 0.15f, 0.08f, 1.00f);
        themeTextDisabled = ImVec4(0.00f, 0.40f, 0.20f, 1.00f);
        themeWindowRounding = 0.0f;
        themeRoundedTabs = false;
        themeCustomButtons = true;
        useOutlineButtons = true;
        break;

    case THEME_MIDNIGHT:
        // Deep blue midnight theme
        colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.04f, 0.10f, 0.4f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.06f, 0.12f, 1.00f);
        themePanelBg = ImVec4(0.06f, 0.06f, 0.2f, 0.4f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.04f, 0.08f, 0.15f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.10f, 0.30f, 0.60f, 0.60f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.10f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.07f, 0.14f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.18f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.03f, 0.06f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.10f, 0.18f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.10f, 0.40f, 0.75f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.50f, 0.85f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.60f, 0.95f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.10f, 0.40f, 0.75f, 0.40f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.50f, 0.85f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.60f, 0.95f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.15f, 0.50f, 0.85f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.15f, 0.50f, 0.85f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.20f, 0.60f, 0.95f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.80f, 0.90f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.40f, 0.60f, 1.00f);

        hoverColor = ImVec4(0.15f, 0.50f, 0.85f, 1.00f);
        activeColor = ImVec4(0.20f, 0.60f, 0.95f, 1.00f);
        themeAccent = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        themeHeaderColor = ImVec4(0.05f, 0.10f, 0.18f, 1.00f);
        themeBorderColor = ImVec4(0.10f, 0.30f, 0.60f, 0.60f);
        themeFrameBg = ImVec4(0.05f, 0.10f, 0.18f, 1.00f);
        themeTextDisabled = ImVec4(0.30f, 0.40f, 0.60f, 1.00f);
        themeWindowRounding = 10.0f;
        themeRoundedTabs = true;
        themeCustomButtons = true;
        useGlassButtons = true;
        break;
    }

    // Apply global rounding
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.WindowRounding = themeWindowRounding;
    style.ChildRounding = themeRounding + 2.0f;
    style.FrameRounding = themeRounding;
    style.PopupRounding = themeRounding;
    style.ScrollbarRounding = themeRounding;
    style.GrabRounding = themeRounding;
    style.TabRounding = themeRoundedTabs ? themeRounding : 0.0f;

    // Set checkbox colors based on theme
    switch (theme) {
    case THEME_DEFAULT:
        checkboxActiveColor = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
        checkboxInactiveColor = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        break;
    case THEME_DARK_PRO:
        checkboxActiveColor = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
        checkboxInactiveColor = ImVec4(0.50f, 0.50f, 0.55f, 0.50f);
        break;
    case THEME_LIGHT:
        checkboxActiveColor = ImVec4(0.25f, 0.70f, 0.40f, 1.00f);
        checkboxInactiveColor = ImVec4(0.60f, 0.60f, 0.65f, 0.50f);
        break;
    case THEME_PURPLE_NEON:
        checkboxActiveColor = ImVec4(0.70f, 0.30f, 1.00f, 1.00f);
        checkboxInactiveColor = ImVec4(0.50f, 0.45f, 0.60f, 0.50f);
        break;
    case THEME_RETRO:
        checkboxActiveColor = ImVec4(0.00f, 1.00f, 0.50f, 1.00f);
        checkboxInactiveColor = ImVec4(0.00f, 0.40f, 0.20f, 0.50f);
        break;
    case THEME_MIDNIGHT:
        checkboxActiveColor = ImVec4(0.15f, 0.50f, 0.85f, 1.00f);
        checkboxInactiveColor = ImVec4(0.30f, 0.40f, 0.60f, 0.50f);
        break;
    }
}

// Theme preview widget
void ThemePreviewWidget() {
    ImGui::BeginChild("ThemePreview", ImVec2(0, 120), true);

    // Show current theme colors
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, 80);

    // Draw theme colors preview
    float segment = size.x / 5.0f;

    // Background
    draw_list->AddRectFilled(pos, ImVec2(pos.x + segment, pos.y + size.y), ImGui::GetColorU32(themeBackground));
    // Child BG
    draw_list->AddRectFilled(ImVec2(pos.x + segment, pos.y), ImVec2(pos.x + segment * 2, pos.y + size.y), ImGui::GetColorU32(themeChildBg));
    // Accent
    draw_list->AddRectFilled(ImVec2(pos.x + segment * 2, pos.y), ImVec2(pos.x + segment * 3, pos.y + size.y), ImGui::GetColorU32(themeAccent));
    // Hover
    draw_list->AddRectFilled(ImVec2(pos.x + segment * 3, pos.y), ImVec2(pos.x + segment * 4, pos.y + size.y), ImGui::GetColorU32(hoverColor));
    // Active
    draw_list->AddRectFilled(ImVec2(pos.x + segment * 4, pos.y), ImVec2(pos.x + segment * 5, pos.y + size.y), ImGui::GetColorU32(activeColor));

    // Labels
    ImGui::SetCursorPosY(85);
    ImGui::Text("Background");
    ImGui::SameLine(segment);
    ImGui::Text("Child BG");
    ImGui::SameLine(segment * 2);
    ImGui::Text("Accent");
    ImGui::SameLine(segment * 3);
    ImGui::Text("Hover");
    ImGui::SameLine(segment * 4);
    ImGui::Text("Active");

    ImGui::EndChild();
}

// Theme configuration tab
void RenderThemeTab() {
    // Remove global styling noise for this tab
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

    // --- Layout: 2 Columns (Left: Presets, Right: Detailed Customization) ---
    ImGui::Columns(2, "ThemeTabCols", false);

    // Adjust column width for better ratio
    if (themeInitialized) {
        ImGui::SetColumnWidth(0, 320.0f);
        themeInitialized = false; // reuse this flag or create a static bool for init
    }

    // ================= LEFT COLUMN: THEME PRESETS =================
    ImGui::BeginChild("ThemeSelector", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {
        ImGui::TextColored(themeAccent, ICON_FA_PALETTE "  Theme Presets");
        ImGui::Separator();
        ImGui::Spacing();

        // Use our new "Card" widget for professional selection
        if (ThemeSelectionCard("Default Green", "Classic dark theme with emerald accents", ImVec4(0.20f, 0.85f, 0.45f, 1.0f), currentTheme == THEME_DEFAULT)) {
            currentTheme = THEME_DEFAULT; ApplyTheme(currentTheme);
        }

        if (ThemeSelectionCard("Dark Professional", "Deep blue professional business style", ImVec4(0.00f, 0.47f, 0.78f, 1.0f), currentTheme == THEME_DARK_PRO)) {
            currentTheme = THEME_DARK_PRO; ApplyTheme(currentTheme);
        }

        if (ThemeSelectionCard("Midnight Blue", "High contrast night mode", ImVec4(0.10f, 0.40f, 0.75f, 1.0f), currentTheme == THEME_MIDNIGHT)) {
            currentTheme = THEME_MIDNIGHT; ApplyTheme(currentTheme);
        }

        if (ThemeSelectionCard("Purple Neon", "Vibrant cyberpunk aesthetic", ImVec4(0.60f, 0.20f, 0.90f, 1.0f), currentTheme == THEME_PURPLE_NEON)) {
            currentTheme = THEME_PURPLE_NEON; ApplyTheme(currentTheme);
        }

        if (ThemeSelectionCard("Retro Terminal", "Old school matrix terminal green", ImVec4(0.00f, 1.00f, 0.50f, 1.0f), currentTheme == THEME_RETRO)) {
            currentTheme = THEME_RETRO; ApplyTheme(currentTheme);
        }

        if (ThemeSelectionCard("Light Clean", "Bright minimal workspace", ImVec4(0.20f, 0.65f, 0.35f, 1.0f), currentTheme == THEME_LIGHT)) {
            currentTheme = THEME_LIGHT; ApplyTheme(currentTheme);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (CustomButton(ICON_FA_UNDO " Reset to Defaults", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
            currentTheme = THEME_DEFAULT;
            ApplyTheme(currentTheme);
            AddNotification("Themes reset to default", 2.0f, themeAccent);
        }

    }
    ImGui::EndChild();

    ImGui::NextColumn();

    // ================= RIGHT COLUMN: FINE TUNING =================
    ImGui::BeginChild("ThemeSettings", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {
        ImGui::TextColored(themeAccent, ICON_FA_SLIDERS_H "  Interface Customization");
        ImGui::Separator();
        ImGui::Spacing();

        // 1. Component Style
        if (CustomCollapsingHeader(ICON_FA_SHAPES " Component Style", true)) {
            ImGui::Spacing();

            ImGui::Text("Button Style:");
            ImGui::BeginGroup();
            bool s_modern = useModernButtons;
            bool s_grad = useGradientButtons;
            bool s_out = useOutlineButtons;

            // Use a radio-button like behavior logic
            if (ImGui::RadioButton("Modern", s_modern)) { useModernButtons = true; useGradientButtons = false; useOutlineButtons = false; useGlassButtons = false; } ImGui::SameLine();
            if (ImGui::RadioButton("Gradient", s_grad)) { useModernButtons = false; useGradientButtons = true; useOutlineButtons = false; useGlassButtons = false; } ImGui::SameLine();
            if (ImGui::RadioButton("Outline", s_out)) { useModernButtons = false; useGradientButtons = false; useOutlineButtons = true; useGlassButtons = false; }
            ImGui::EndGroup();

            ImGui::Spacing();
            CustomCheckbox("Rounded Corners", &themeRoundedIcons);
            CustomCheckbox("Glass Effect Panels", &useGlassButtons); // Re-purposing for panel logic if needed

            ImGui::Spacing();
            ImGui::Text("Animations:");
            CustomCheckbox("Enable Interface Animations", &themeAnimatedElements);
            if (themeAnimatedElements) {
                CustomSliderFloat("Anim Speed", &themeAnimationSpeed, 0.5f, 2.0f, "%.1fx");
            }
        }

        ImGui::Spacing();

        // 2. Checkbox Styling
        if (CustomCollapsingHeader(ICON_FA_CHECK_SQUARE " Controls & Inputs")) {
            ImGui::Spacing();
            ImGui::Text("Toggle Style:");
            const char* styles[] = { "Default", "Rounded", "Switch", "Filled", "Outline" };
            ImGui::Combo("##CheckStyle", &currentCheckboxStyle, styles, IM_ARRAYSIZE(styles));

            ImGui::Spacing();
            CustomCheckbox("Ripple Effects", &checkboxRippleEffect);

            ImGui::Spacing();
            ImGui::Text("Interaction Colors:");
            ColorPicker("Checkbox Active", checkboxActiveColor);
            ColorPicker("Checkbox Inactive", checkboxInactiveColor);
        }

        ImGui::Spacing();

        // 3. Colors
        if (CustomCollapsingHeader(ICON_FA_PAINT_BRUSH " Color Palette")) {
            ImGui::Spacing();
            ImGui::Text("System Colors:");
            ColorPicker("Global Accent", themeAccent);
            ColorPicker("Window Background", themeBackground);
            ColorPicker("Panel Background", themePanelBg);

            ImGui::Spacing();
            ImGui::Text("Text Colors:");
            ColorPicker("Primary Text", themeText);
            ColorPicker("Disabled Text", themeTextDisabled);

            ImGui::Spacing();
            if (CustomButton("Apply Color Changes", ImVec2(ImGui::GetContentRegionAvail().x, 30))) {
                ApplyTheme(currentTheme); // Re-apply with overrides
            }
        }
    }
    ImGui::EndChild();

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}

// Setup ImGui style
void SetupImGuiStyle() {
    if (!themeInitialized) {
        ApplyTheme(currentTheme);
        themeInitialized = true;
    }
}