#include "menu.h"

MenuAnimation g_MenuAnimation;


float EaseInOutCubic(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float EaseOutBack(float t) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    return 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
}

float EaseInElastic(float t) {
    const float c4 = (2.0f * 3.14159f) / 3.0f;
    return t == 0.0f ? 0.0f :
        t == 1.0f ? 1.0f :
        -pow(2.0f, 10.0f * t - 10.0f) * sin((t * 10.0f - 10.75f) * c4);
}

float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t) {
    return ImVec4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}

namespace ImVec2Helpers {
    inline ImVec2 Add(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }
    inline ImVec2 Subtract(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x - b.x, a.y - b.y); }
    inline ImVec2 Multiply(const ImVec2& a, float b) { return ImVec2(a.x * b, a.y * b); }
    inline ImVec2 Multiply(float a, const ImVec2& b) { return ImVec2(a * b.x, a * b.y); }
    inline ImVec2 Divide(const ImVec2& a, float b) { return ImVec2(a.x / b, a.y / b); }
}

// Enhanced animated button with multiple effects
bool AnimatedButtonEx(const char* label, const ImVec2& size, int effectType) {
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

    // Advanced animation states
    static std::map<ImGuiID, float> hoverAnims;
    static std::map<ImGuiID, float> clickAnims;
    static std::map<ImGuiID, ImVec2> rippleOrigins;
    static std::map<ImGuiID, float> rippleAnims;

    float& hoverAnim = hoverAnims[id];
    float& clickAnim = clickAnims[id];
    float& rippleAnim = rippleAnims[id];
    ImVec2& rippleOrigin = rippleOrigins[id];

    // Update animations
    if (hovered) hoverAnim = ImMin(hoverAnim + g.IO.DeltaTime * 10.0f, 1.0f);
    else hoverAnim = ImMax(hoverAnim - g.IO.DeltaTime * 5.0f, 0.0f);

    if (pressed) {
        clickAnim = 0.0f;
        rippleOrigin = ImVec2(pos.x + actual_size.x * 0.5f, pos.y + actual_size.y * 0.5f);
        rippleAnim = 0.0f;
    }

    if (clickAnim < 1.0f) clickAnim = ImMin(clickAnim + g.IO.DeltaTime * 15.0f, 1.0f);
    if (rippleAnim < 1.0f) rippleAnim = ImMin(rippleAnim + g.IO.DeltaTime * 8.0f, 1.0f);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float rounding = themeCustomButtons ? themeRounding : style.FrameRounding;

    // Render based on effect type
    switch (effectType) {
    case 0: // Default with ripple
    {
        ImU32 bg_col = ImGui::GetColorU32(held ? activeColor : (hovered ? hoverColor : themeAccent));
        draw_list->AddRectFilled(bb.Min, bb.Max, bg_col, rounding);

        // Ripple effect
        if (rippleAnim < 1.0f) {
            float ripple_radius = ImMax(actual_size.x, actual_size.y) * 0.8f * rippleAnim;
            ImU32 ripple_col = IM_COL32(255, 255, 255, (int)(100 * (1.0f - rippleAnim)));
            draw_list->AddCircle(rippleOrigin, ripple_radius, ripple_col, 0, 2.0f);
        }
        break;
    }
    case 1: // Glow effect
    {
        ImU32 bg_col = ImGui::GetColorU32(ImLerp(themeAccent, hoverColor, hoverAnim));
        draw_list->AddRectFilled(bb.Min, bb.Max, bg_col, rounding);

        if (hovered) {
            float glow_intensity = hoverAnim * 0.3f;
            ImU32 glow_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, glow_intensity));
            for (int i = 0; i < 3; i++) {
                float thickness = 1.0f + i * 2.0f;
                draw_list->AddRect(bb.Min, bb.Max, glow_col, rounding, ImDrawFlags_RoundCornersAll, thickness);
            }
        }
        break;
    }
    case 2: // Elastic fill
    {
        ImU32 bg_col = ImGui::GetColorU32(themeAccent);
        draw_list->AddRectFilled(bb.Min, bb.Max, bg_col, rounding);

        if (hoverAnim > 0.0f) {
            float elastic_scale = 1.0f + EaseOutBack(hoverAnim) * 0.1f;
            ImVec2 center = ImVec2((bb.Min.x + bb.Max.x) * 0.5f, (bb.Min.y + bb.Max.y) * 0.5f);
            ImVec2 new_size = ImVec2Helpers::Multiply(actual_size, elastic_scale);
            ImVec2 new_min = ImVec2Helpers::Subtract(center, ImVec2Helpers::Divide(new_size, 2.0f));
            ImVec2 new_max = ImVec2Helpers::Add(center, ImVec2Helpers::Divide(new_size, 2.0f));

            ImU32 hover_col = ImGui::GetColorU32(hoverColor);
            draw_list->AddRectFilled(new_min, new_max, hover_col, rounding);
        }
        break;
    }
    }

    // Click animation
    if (clickAnim < 1.0f) {
        float click_scale = 1.0f - EaseInElastic(clickAnim) * 0.1f;
        ImVec2 center = ImVec2((bb.Min.x + bb.Max.x) * 0.5f, (bb.Min.y + bb.Max.y) * 0.5f);
        ImVec2 new_size = ImVec2Helpers::Multiply(actual_size, click_scale);
        ImVec2 new_min = ImVec2Helpers::Subtract(center, ImVec2Helpers::Divide(new_size, 2.0f));
        ImVec2 new_max = ImVec2Helpers::Add(center, ImVec2Helpers::Divide(new_size, 2.0f));

        draw_list->AddRectFilled(new_min, new_max, IM_COL32(255, 255, 255, 50), rounding);
    }

    // Text with animation
    ImVec4 textColor = style.Colors[ImGuiCol_Text];
    if (hovered) {
        textColor = ImLerp(textColor, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), hoverAnim * 0.3f);
    }

    ImGui::RenderTextClipped(bb.Min, bb.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    return pressed;
}

// Enhanced tab transition system
void UpdateMenuAnimations() {
    if (!menuOpen && g_MenuAnimation.alpha <= 0.0f) return;

    float deltaTime = ImGui::GetIO().DeltaTime;

    // Menu open/close animation
    if (menuOpen) {
        g_MenuAnimation.alpha = ImMin(g_MenuAnimation.alpha + deltaTime * 8.0f, 1.0f);
        g_MenuAnimation.backgroundDim = ImMin(g_MenuAnimation.backgroundDim + deltaTime * 6.0f, 0.7f);
    }
    else {
        g_MenuAnimation.alpha = ImMax(g_MenuAnimation.alpha - deltaTime * 8.0f, 0.0f);
        g_MenuAnimation.backgroundDim = ImMax(g_MenuAnimation.backgroundDim - deltaTime * 6.0f, 0.0f);
    }

    // Tab transition animation
    if (g_MenuAnimation.currentTab != g_MenuAnimation.previousTab) {
        g_MenuAnimation.tabAnimationProgress += deltaTime * 8.0f;
        if (g_MenuAnimation.tabAnimationProgress >= 1.0f) {
            g_MenuAnimation.tabAnimationProgress = 1.0f;
            g_MenuAnimation.previousTab = g_MenuAnimation.currentTab;
        }
    }
    else {
        g_MenuAnimation.tabAnimationProgress = 1.0f;
    }
}

// Page transition effect
void RenderPageTransition(ImDrawList* draw_list, const ImVec2& pos, const ImVec2& size) {
    if (g_MenuAnimation.tabAnimationProgress < 1.0f) {
        // Slide transition
        float slide_offset = size.x * (1.0f - g_MenuAnimation.tabAnimationProgress);

        // Old content sliding out
        ImVec4 old_content_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f - g_MenuAnimation.tabAnimationProgress);
        draw_list->AddRectFilled(
            ImVec2(pos.x - slide_offset, pos.y),
            ImVec2(pos.x + size.x - slide_offset, pos.y + size.y),
            ImGui::GetColorU32(old_content_color)
        );

        // New content sliding in
        ImVec4 new_content_color = ImVec4(1.0f, 1.0f, 1.0f, g_MenuAnimation.tabAnimationProgress);
        draw_list->AddRectFilled(
            ImVec2(pos.x + slide_offset, pos.y),
            ImVec2(pos.x + size.x + slide_offset, pos.y + size.y),
            ImGui::GetColorU32(new_content_color)
        );
    }
}