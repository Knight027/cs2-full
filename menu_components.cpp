#include "menu.h"

// Themed Checkbox implementation
bool ThemedCheckbox(const char* label, bool* v, const char* tooltip) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    const float square_size = ImGui::GetFrameHeight();

    // Calculate actual width based on checkbox style
    float actual_width = square_size;
    if (currentCheckboxStyle == CHECKBOX_SWITCH) {
        actual_width = square_size * 1.6f; // Switch is wider
    }

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + actual_width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + label_size.y + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    // Tooltip
    if (tooltip && hovered && ImGui::BeginTooltip()) {
        ImGui::TextUnformatted(tooltip);
        ImGui::EndTooltip();
    }

    // Animation states
    static std::map<ImGuiID, float> checkAnimations;
    static std::map<ImGuiID, float> hoverAnimations;
    static std::map<ImGuiID, ImVec2> rippleOrigins;
    static std::map<ImGuiID, float> rippleAnimations;

    float& checkAnim = checkAnimations[id];
    float& hoverAnim = hoverAnimations[id];
    float& rippleAnim = rippleAnimations[id];
    ImVec2& rippleOrigin = rippleOrigins[id];

    // Update animations
    if (*v) checkAnim = ImMin(checkAnim + g.IO.DeltaTime * 8.0f * checkboxAnimationSpeed, 1.0f);
    else checkAnim = ImMax(checkAnim - g.IO.DeltaTime * 8.0f * checkboxAnimationSpeed, 0.0f);

    if (hovered) hoverAnim = ImMin(hoverAnim + g.IO.DeltaTime * 6.0f * checkboxAnimationSpeed, 1.0f);
    else hoverAnim = ImMax(hoverAnim - g.IO.DeltaTime * 4.0f * checkboxAnimationSpeed, 0.0f);

    // Handle ripple effect
    if (pressed && checkboxRippleEffect) {
        rippleOrigin = ImVec2(pos.x + actual_width * 0.5f, pos.y + square_size * 0.5f);
        rippleAnim = 0.0f;
    }
    if (rippleAnim < 1.0f) {
        rippleAnim = ImMin(rippleAnim + g.IO.DeltaTime * 6.0f * checkboxAnimationSpeed, 1.0f);
    }

    const ImRect check_bb(pos, ImVec2(pos.x + actual_width, pos.y + square_size));
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Calculate text position with proper spacing
    float text_pos_x = pos.x + actual_width + style.ItemInnerSpacing.x;

    // Extra spacing for toggle switch when active to prevent text overlap
    if (currentCheckboxStyle == CHECKBOX_SWITCH && *v) {
        text_pos_x += square_size; // Additional space when switch is on
    }

    // Render based on checkbox style
    switch (currentCheckboxStyle) {
    case CHECKBOX_DEFAULT: {
        // Default style with theme colors
        ImU32 bg_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, checkboxActiveColor, checkAnim));
        ImU32 border_col = ImGui::GetColorU32(ImLerp(themeBorderColor, themeAccent, checkAnim));

        draw_list->AddRectFilled(check_bb.Min, check_bb.Max, bg_col, style.FrameRounding);
        draw_list->AddRect(check_bb.Min, check_bb.Max, border_col, style.FrameRounding, ImDrawFlags_RoundCornersAll, 1.5f);

        // Checkmark
        if (checkAnim > 0.0f) {
            const float pad = ImMax(1.0f, IM_FLOOR(square_size / 6.0f));
            const ImVec2 checkSize = ImVec2(square_size - pad * 2.0f, square_size - pad * 2.0f);
            const ImVec2 checkPos = ImVec2(pos.x + pad, pos.y + pad);

            ImU32 check_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, checkAnim));
            draw_list->AddRectFilled(checkPos, ImVec2(checkPos.x + checkSize.x, checkPos.y + checkSize.y),
                check_col, style.FrameRounding * 0.5f);
        }
        break;
    }

    case CHECKBOX_ROUNDED: {
        // Rounded style (circular)
        float radius = square_size * 0.5f;
        ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);

        ImU32 bg_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, checkboxActiveColor, checkAnim));
        ImU32 border_col = ImGui::GetColorU32(ImLerp(themeBorderColor, themeAccent, checkAnim));

        draw_list->AddCircleFilled(center, radius, bg_col);
        draw_list->AddCircle(center, radius, border_col, 0, 2.0f);

        // Checkmark (dot in center)
        if (checkAnim > 0.0f) {
            float inner_radius = radius * 0.4f * checkAnim;
            ImU32 check_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, checkAnim));
            draw_list->AddCircleFilled(center, inner_radius, check_col);
        }
        break;
    }

    case CHECKBOX_SWITCH: {
        // Toggle switch style
        float radius = square_size * 0.5f;
        float switch_width = actual_width;
        ImRect switch_bb(pos, ImVec2(pos.x + switch_width, pos.y + square_size));

        ImU32 track_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, checkboxActiveColor, checkAnim * 0.3f));
        ImU32 thumb_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, checkboxActiveColor, checkAnim));

        // Track
        draw_list->AddRectFilled(switch_bb.Min, switch_bb.Max, track_col, radius);

        // Thumb - with smooth animation
        float thumb_pos = checkAnim * (switch_width - square_size);
        ImVec2 thumb_min = ImVec2(pos.x + thumb_pos, pos.y);
        ImVec2 thumb_max = ImVec2(thumb_min.x + square_size, thumb_min.y + square_size);
        draw_list->AddCircleFilled(ImVec2(thumb_min.x + radius, thumb_min.y + radius), radius * 0.9f, thumb_col);

        // Add a subtle border to the track for better visibility
        draw_list->AddRect(switch_bb.Min, switch_bb.Max, ImGui::GetColorU32(themeBorderColor), radius, ImDrawFlags_RoundCornersAll, 1.0f);

        break;
    }

    case CHECKBOX_FILLED: {
        // Filled style with icon
        ImU32 bg_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, checkboxActiveColor, checkAnim));
        ImU32 border_col = ImGui::GetColorU32(ImLerp(themeBorderColor, themeAccent, checkAnim));

        draw_list->AddRectFilled(check_bb.Min, check_bb.Max, bg_col, style.FrameRounding);
        draw_list->AddRect(check_bb.Min, check_bb.Max, border_col, style.FrameRounding, ImDrawFlags_RoundCornersAll, 1.0f);

        // Checkmark icon
        if (checkAnim > 0.0f) {
            const float pad = square_size * 0.2f;
            const ImVec2 p0 = ImVec2(pos.x + pad, pos.y + square_size * 0.5f);
            const ImVec2 p1 = ImVec2(pos.x + square_size * 0.4f, pos.y + square_size - pad);
            const ImVec2 p2 = ImVec2(pos.x + square_size - pad, pos.y + pad);

            ImU32 check_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, checkAnim));
            draw_list->AddLine(p0, p1, check_col, 2.0f * checkAnim);
            draw_list->AddLine(p1, p2, check_col, 2.0f * checkAnim);
        }
        break;
    }

    case CHECKBOX_OUTLINE: {
        // Outline only style
        ImU32 border_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, themeAccent, checkAnim));
        float border_thickness = 1.5f + hoverAnim * 1.0f;

        draw_list->AddRect(check_bb.Min, check_bb.Max, border_col, style.FrameRounding, ImDrawFlags_RoundCornersAll, border_thickness);

        // Checkmark (X shape)
        if (checkAnim > 0.0f) {
            const float pad = square_size * 0.3f;
            const ImVec2 p0 = ImVec2(pos.x + pad, pos.y + pad);
            const ImVec2 p1 = ImVec2(pos.x + square_size - pad, pos.y + square_size - pad);
            const ImVec2 p2 = ImVec2(pos.x + square_size - pad, pos.y + pad);
            const ImVec2 p3 = ImVec2(pos.x + pad, pos.y + square_size - pad);

            ImU32 check_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, themeAccent, checkAnim));
            draw_list->AddLine(p0, p1, check_col, 2.0f * checkAnim);
            draw_list->AddLine(p2, p3, check_col, 2.0f * checkAnim);
        }
        break;
    }

    case CHECKBOX_ANIMATED: {
        // Animated style with ripple
        ImU32 bg_col = ImGui::GetColorU32(ImLerp(checkboxInactiveColor, checkboxActiveColor, checkAnim));
        ImU32 border_col = ImGui::GetColorU32(ImLerp(themeBorderColor, themeAccent, checkAnim));

        draw_list->AddRectFilled(check_bb.Min, check_bb.Max, bg_col, style.FrameRounding);
        draw_list->AddRect(check_bb.Min, check_bb.Max, border_col, style.FrameRounding, ImDrawFlags_RoundCornersAll, 1.5f);

        // Ripple effect
        if (rippleAnim < 1.0f) {
            float ripple_radius = square_size * 0.8f * rippleAnim;
            ImU32 ripple_col = IM_COL32(255, 255, 255, (int)(100 * (1.0f - rippleAnim)));
            draw_list->AddCircle(rippleOrigin, ripple_radius, ripple_col, 0, 2.0f);
        }

        // Pulsing checkmark
        if (checkAnim > 0.0f) {
            static float pulse = 0.0f;
            pulse += g.IO.DeltaTime * 4.0f;
            float pulse_alpha = (sin(pulse) + 1.0f) * 0.2f + 0.6f;

            const float pad = square_size * 0.25f;
            const ImVec2 center = ImVec2(pos.x + square_size * 0.5f, pos.y + square_size * 0.5f);
            float check_size = square_size * 0.4f * checkAnim;

            ImU32 check_col = IM_COL32(255, 255, 255, (int)(255 * checkAnim * pulse_alpha));
            draw_list->AddCircleFilled(center, check_size, check_col);
        }
        break;
    }
    }

    // Hover effect
    if (hoverAnim > 0.0f) {
        ImU32 hover_col = IM_COL32(255, 255, 255, (int)(30 * hoverAnim));
        draw_list->AddRectFilled(check_bb.Min, check_bb.Max, hover_col, style.FrameRounding);
    }

    // Label - USING PROPERLY CALCULATED TEXT POSITION
    if (label_size.x > 0.0f) {
        ImGui::RenderText(ImVec2(text_pos_x, pos.y + style.FramePadding.y), label);
    }

    return pressed;
}

// Custom checkbox with size parameter
bool ThemedCheckbox(const char* label, bool* v, float size, const char* tooltip) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    const float square_size = size;

    // Calculate actual width based on checkbox style
    float actual_width = square_size;
    if (currentCheckboxStyle == CHECKBOX_SWITCH) {
        actual_width = square_size * 1.6f; // Switch is wider
    }

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + actual_width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + ImMax(square_size, label_size.y) + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    // Tooltip
    if (tooltip && hovered && ImGui::BeginTooltip()) {
        ImGui::TextUnformatted(tooltip);
        ImGui::EndTooltip();
    }

    const ImRect check_bb(pos, ImVec2(pos.x + actual_width, pos.y + square_size));
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Calculate text position with proper spacing
    float text_pos_x = pos.x + actual_width + style.ItemInnerSpacing.x;

    // Extra spacing for toggle switch when active
    if (currentCheckboxStyle == CHECKBOX_SWITCH && *v) {
        text_pos_x += square_size * 0.3f;
    }

    // Simplified rendering for custom size
    ImU32 bg_col = ImGui::GetColorU32(*v ? checkboxActiveColor : checkboxInactiveColor);
    ImU32 border_col = ImGui::GetColorU32(themeBorderColor);

    // Handle switch style differently for custom size
    if (currentCheckboxStyle == CHECKBOX_SWITCH) {
        float radius = square_size * 0.5f;
        float switch_width = actual_width;
        ImRect switch_bb(pos, ImVec2(pos.x + switch_width, pos.y + square_size));

        ImU32 track_col = ImGui::GetColorU32(*v ? checkboxActiveColor : checkboxInactiveColor);
        ImU32 thumb_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

        // Track
        draw_list->AddRectFilled(switch_bb.Min, switch_bb.Max, track_col, radius);

        // Thumb
        float thumb_pos = *v ? (switch_width - square_size) : 0.0f;
        ImVec2 thumb_min = ImVec2(pos.x + thumb_pos, pos.y);
        ImVec2 thumb_max = ImVec2(thumb_min.x + square_size, thumb_min.y + square_size);
        draw_list->AddCircleFilled(ImVec2(thumb_min.x + radius, thumb_min.y + radius), radius * 0.9f, thumb_col);

        // Border
        draw_list->AddRect(switch_bb.Min, switch_bb.Max, border_col, radius, ImDrawFlags_RoundCornersAll, 1.0f);
    }
    else {
        // Default rendering for other styles
        draw_list->AddRectFilled(check_bb.Min, check_bb.Max, bg_col, style.FrameRounding);
        draw_list->AddRect(check_bb.Min, check_bb.Max, border_col, style.FrameRounding, ImDrawFlags_RoundCornersAll, 1.5f);

        // Checkmark for non-switch styles
        if (*v) {
            const float pad = ImMax(1.0f, IM_FLOOR(square_size / 6.0f));
            const ImVec2 checkSize = ImVec2(square_size - pad * 2.0f, square_size - pad * 2.0f);
            const ImVec2 checkPos = ImVec2(pos.x + pad, pos.y + pad);

            ImU32 check_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            draw_list->AddRectFilled(checkPos, ImVec2(checkPos.x + checkSize.x, checkPos.y + checkSize.y),
                check_col, style.FrameRounding * 0.5f);
        }
    }

    // Label with proper positioning
    if (label_size.x > 0.0f) {
        float text_offset_y = (square_size - label_size.y) * 0.5f;
        ImGui::RenderText(ImVec2(text_pos_x, pos.y + text_offset_y), label);
    }

    return pressed;
}

// Themed Button implementation
bool ThemedButton(const char* label, const ImVec2& size, bool special, const char* tooltip) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 actual_size = ImGui::CalcItemSize(size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, ImVec2(pos.x + actual_size.x, pos.y + actual_size.y));
    ImGui::ItemSize(actual_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Animation
    static std::map<ImGuiID, float> hoverAnims;
    float& hoverAnim = hoverAnims[id];
    if (hovered) hoverAnim = ImMin(hoverAnim + g.IO.DeltaTime * 8.0f * themeAnimationSpeed, 1.0f);
    else hoverAnim = ImMax(hoverAnim - g.IO.DeltaTime * 4.0f * themeAnimationSpeed, 0.0f);

    // Tooltip
    if (tooltip && hovered && ImGui::BeginTooltip()) {
        ImGui::TextUnformatted(tooltip);
        ImGui::EndTooltip();
    }

    // Render based on theme style
    ImU32 bg_col = ImGui::GetColorU32(held ? activeColor : (hovered ? hoverColor : themeAccent));
    float buttonRounding = themeCustomButtons ? themeRounding : style.FrameRounding;

    if (useGradientButtons && hovered) {
        // Gradient effect for neon theme
        ImU32 top_col = ImGui::GetColorU32(ImLerp(themeAccent, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.2f));
        ImU32 bottom_col = ImGui::GetColorU32(themeAccent);
        window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, top_col, top_col, bottom_col, bottom_col, buttonRounding);
    }
    else if (useOutlineButtons) {
        // Outline style for retro theme
        window->DrawList->AddRectFilled(bb.Min, bb.Max, IM_COL32(0, 0, 0, 0), buttonRounding);
        window->DrawList->AddRect(bb.Min, bb.Max, bg_col, buttonRounding, ImDrawFlags_RoundCornersAll, 2.0f);

        // Fill on hover
        if (hovered) {
            ImU32 fill_col = ImGui::GetColorU32(ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.2f));
            window->DrawList->AddRectFilled(bb.Min, bb.Max, fill_col, buttonRounding);
        }
    }
    else if (useGlassButtons) {
        // Glass effect for midnight theme
        ImU32 glass_col = IM_COL32(255, 255, 255, hovered ? 30 : 15);
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col, buttonRounding);
        window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Max.x, bb.Min.y + actual_size.y * 0.5f), glass_col, buttonRounding, ImDrawFlags_RoundCornersTop);
    }
    else if (useModernButtons) {
        // Modern flat design
        float alpha = held ? 0.9f : (hovered ? 0.7f : 0.4f);
        ImVec4 buttonColor = ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, alpha);
        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(buttonColor), buttonRounding);
    }
    else {
        // Default button
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col, buttonRounding);
    }

    // Special effects for important buttons
    if (special) {
        if (hovered) {
            ImVec4 glowColor = ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.3f);
            window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(glowColor), buttonRounding, ImDrawFlags_RoundCornersAll, 3.0f);
        }

        // Pulse animation for special buttons
        static float pulse = 0.0f;
        pulse += g.IO.DeltaTime * 2.0f;
        float pulseAlpha = (sin(pulse) + 1.0f) * 0.2f + 0.1f;
        ImVec4 pulseColor = ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, pulseAlpha);
        window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(pulseColor), buttonRounding, ImDrawFlags_RoundCornersAll, 1.5f);
    }

    // Text rendering with animation
    ImVec4 textColor = style.Colors[ImGuiCol_Text];
    if (hovered) {
        textColor = ImLerp(textColor, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), hoverAnim * 0.3f);
    }

    ImGui::RenderTextClipped(bb.Min, bb.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    return pressed;
}

// Animated Tab Button
bool AnimatedTabButton(const char* label, bool selected, const ImVec2& size) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 actual_size = ImGui::CalcItemSize(size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, ImVec2(pos.x + actual_size.x, pos.y + actual_size.y));
    ImGui::ItemSize(actual_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Animation states
    static std::map<ImGuiID, float> hoverAnimations;
    static std::map<ImGuiID, float> activeAnimations;

    float& hoverAnim = hoverAnimations[id];
    float& activeAnim = activeAnimations[id];

    if (hovered) hoverAnim = ImMin(hoverAnim + g.IO.DeltaTime * 8.0f * themeAnimationSpeed, 1.0f);
    else hoverAnim = ImMax(hoverAnim - g.IO.DeltaTime * 4.0f * themeAnimationSpeed, 0.0f);

    if (selected) activeAnim = ImMin(activeAnim + g.IO.DeltaTime * 6.0f * themeAnimationSpeed, 1.0f);
    else activeAnim = ImMax(activeAnim - g.IO.DeltaTime * 6.0f * themeAnimationSpeed, 0.0f);

    // Calculate colors with animations - using theme colors
    ImVec4 bgColor = ImLerp(style.Colors[ImGuiCol_Tab], hoverColor, hoverAnim);
    bgColor = ImLerp(bgColor, activeColor, activeAnim);

    ImVec4 textColor = ImLerp(style.Colors[ImGuiCol_Text], ImVec4(1.0f, 1.0f, 1.0f, 1.0f), activeAnim);

    // Render
    ImGui::RenderNavHighlight(bb, id);
    float tabRounding = themeRoundedTabs ? style.FrameRounding : 0.0f;
    ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(bgColor), true, tabRounding);

    // Underline for active tab
    if (activeAnim > 0.0f) {
        ImVec2 lineStart = ImVec2(bb.Min.x, bb.Max.y - 2.0f);
        ImVec2 lineEnd = ImVec2(bb.Max.x, bb.Max.y - 2.0f);
        window->DrawList->AddLine(lineStart, lineEnd,
            ImGui::GetColorU32(ImLerp(ImVec4(0, 0, 0, 0), themeAccent, activeAnim)),
            3.0f * activeAnim);
    }

    ImVec2 textPadding = ImVec2(style.FramePadding.x, style.FramePadding.y);
    ImGui::RenderTextClipped(
        ImVec2Helpers::Add(bb.Min, textPadding),
        ImVec2Helpers::Subtract(bb.Max, textPadding),
        label, NULL, &label_size, style.ButtonTextAlign, &bb
    );

    return pressed;
}

// Additional UI components
bool ToggleButton(const char* label, bool* state, const ImVec2& size) {
    bool pressed = ThemedButton(label, size, *state);
    if (pressed) {
        *state = !*state;
    }
    return pressed;
}

bool BadgeButton(const char* label, int count, const ImVec2& size) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    bool pressed = ThemedButton(label, size);

    // Draw badge
    if (count > 0) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetItemRectMax();
        pos.x -= 5;
        pos.y = ImGui::GetItemRectMin().y + 5;

        std::string countStr = std::to_string(count);
        ImVec2 textSize = ImGui::CalcTextSize(countStr.c_str());
        float radius = textSize.x > textSize.y ? textSize.x * 0.7f : textSize.y * 0.7f;

        // Badge background
        draw_list->AddCircleFilled(pos, radius, ImGui::GetColorU32(ImVec4(1.0f, 0.3f, 0.3f, 1.0f)));

        // Badge text
        ImVec2 textPos = ImVec2(pos.x - textSize.x * 0.5f, pos.y - textSize.y * 0.5f);
        draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), countStr.c_str());
    }

    return pressed;
}

bool IconButton(const char* icon, const char* label, const ImVec2& size) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    // You would need to implement icon font for this
    // For now, we'll just use text as placeholder
    std::string buttonText = std::string(icon) + " " + label;
    return ThemedButton(buttonText.c_str(), size);
}

// Color Picker implementation
bool ColorPicker(const char* label, ImVec4& color) {
    bool changed = false;
    ImGui::PushID(label);

    // Color preview button with border
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x * 1.1f, color.y * 1.1f, color.z * 1.1f, color.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.x * 0.9f, color.y * 0.9f, color.z * 0.9f, color.w));
    if (ImGui::Button("##ColorBtn", ImVec2(40, 20))) {
        ImGui::OpenPopup("##ColorPickerPopup");
    }
    ImGui::PopStyleColor(3);

    // Border around color button
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p_min = ImGui::GetItemRectMin();
    ImVec2 p_max = ImGui::GetItemRectMax();
    draw_list->AddRect(p_min, p_max, ImGui::GetColorU32(ImGuiCol_Border), 4.0f);

    ImGui::SameLine();
    ImGui::Text("%s", label);

    // Color picker popup
    if (ImGui::BeginPopup("##ColorPickerPopup")) {
        ImGui::Text("Select Color");
        ImGui::Separator();
        changed = ImGui::ColorPicker4("##Picker", (float*)&color,
            ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::EndPopup();
    }

    ImGui::PopID();
    return changed;
}