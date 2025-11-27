#include "menu.h"
#include <dwmapi.h>
#include "fontawesome.h"
#include <filesystem> // Required for modern directory/file operations (C++17)
#include <algorithm>

// Global Definitions
std::string g_currentConfigName = "default";
std::vector<std::string> g_configFiles;

// Parse color from string
ImVec4 parse_color(const std::string& value) {
    try {
        std::vector<float> colors;
        std::stringstream ss(value);
        std::string token;
        while (std::getline(ss, token, ',')) {
            colors.push_back(std::stof(token));
        }
        if (colors.size() == 4) {
            return ImVec4(colors[0], colors[1], colors[2], colors[3]);
        }
    }
    catch (...) {
        // Fallback to default color if parsing fails
    }
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

std::string sanitize_key(std::string text) {
    std::replace(text.begin(), text.end(), ' ', '_');
    return text;
}

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

#ifndef DWMSBT_TRANSIENTWINDOW
#define DWMSBT_TRANSIENTWINDOW 3 // Acrylic Effect
#endif

// --- DWM Blur Implementation ---
void SetupWindowBlur(HWND hwnd) {
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    int backdrop = DWMSBT_TRANSIENTWINDOW;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdrop, sizeof(backdrop));

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);
}

// --- Helper: Color Interpolation ---
ImVec4 LerpColor(const ImVec4& a, const ImVec4& b, float t) {
    return ImVec4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}



float SmoothStep(float t) {
    return t * t * (3.0f - 2.0f * t);
}

// --- NEW: Theme Selection Card (Professional Look) ---
bool ThemeSelectionCard(const char* label, const char* description, ImVec4 accent, bool selected) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    // Dimensions
    float width = ImGui::GetContentRegionAvail().x;
    float height = 50.0f;
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, ImVec2(pos.x + width, pos.y + height));

    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Animation State
    float t = selected ? 1.0f : 0.0f;
    float anim = ImGui::GetStateStorage()->GetFloat(id, t);
    float hoverAnim = ImGui::GetStateStorage()->GetFloat(id + 1, 0.0f);

    // Animate Selection
    float dt = ImGui::GetIO().DeltaTime * 10.0f;
    if (anim < t) anim += dt;
    if (anim > t) anim -= dt;
    anim = ImClamp(anim, 0.0f, 1.0f);

    // Animate Hover
    if (hovered) hoverAnim += dt;
    else hoverAnim -= dt;
    hoverAnim = ImClamp(hoverAnim, 0.0f, 1.0f);

    // Store
    ImGui::GetStateStorage()->SetFloat(id, anim);
    ImGui::GetStateStorage()->SetFloat(id + 1, hoverAnim);

    // --- DRAWING ---

    // 1. Background
    ImU32 bgCol = ImGui::GetColorU32(ImVec4(themePanelBg.x, themePanelBg.y, themePanelBg.z, 0.6f));
    ImU32 bgHover = ImGui::GetColorU32(ImVec4(themePanelBg.x + 0.05f, themePanelBg.y + 0.05f, themePanelBg.z + 0.05f, 0.8f));

    // Interpolate background based on hover
    window->DrawList->AddRectFilled(bb.Min, bb.Max, hovered ? bgHover : bgCol, themeRounding);

    // 2. Accent Border (if selected or hovering)
    if (anim > 0.01f || hoverAnim > 0.01f) {
        ImU32 borderCol = ImGui::GetColorU32(accent);
        float borderThickness = 1.0f + (1.0f * anim); // Thicker when selected

        // Draw left indicator strip
        window->DrawList->AddRectFilled(
            bb.Min,
            ImVec2(bb.Min.x + 4.0f, bb.Max.y),
            borderCol,
            themeRounding,
            ImDrawFlags_RoundCornersLeft
        );

        // Draw outline
        if (selected) {
            window->DrawList->AddRect(bb.Min, bb.Max, borderCol, themeRounding, 0, 1.5f);
        }
    }

    // 3. Text
    ImU32 textCol = ImGui::GetColorU32(themeText);
    ImU32 subTextCol = ImGui::GetColorU32(themeTextDisabled);

    // Title
    window->DrawList->AddText(ImVec2(bb.Min.x + 15.0f, bb.Min.y + 8.0f), textCol, label);

    // Description (Smaller)
    ImGui::PushFont(g.Font); // Ideally use a smaller font here if available
    window->DrawList->AddText(ImVec2(bb.Min.x + 15.0f, bb.Min.y + 28.0f), subTextCol, description);
    ImGui::PopFont();

    // 4. Checkmark icon if selected
    if (selected) {
        const char* icon = ICON_FA_CHECK;
        ImVec2 iconSz = ImGui::CalcTextSize(icon);
        window->DrawList->AddText(
            ImVec2(bb.Max.x - iconSz.x - 15.0f, bb.Min.y + (height - iconSz.y) / 2),
            ImGui::GetColorU32(accent),
            icon
        );
    }

    return pressed;
}



// --- Custom Toggle Checkbox (Clean Animation) ---
bool CustomCheckbox(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    // Layout
    float height = ImGui::GetFrameHeight();
    float width = height * 1.8f; // Wider for switch look
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + width + (label_size.x > 0 ? style.ItemInnerSpacing.x + label_size.x : 0), pos.y + height));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    // Animation Logic
    static std::map<ImGuiID, float> animMap;
    float t = *v ? 1.0f : 0.0f;
    float anim = animMap[id];

    // Smooth lerp for the switch
    float dt = ImGui::GetIO().DeltaTime * 12.0f; // Speed
    if (anim < t) anim = ImMin(anim + dt, t);
    if (anim > t) anim = ImMax(anim - dt, t);
    animMap[id] = anim;

    // Colors
    ImVec4 bg_inactive = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 bg_active = themeAccent;
    ImVec4 bg_col = LerpColor(bg_inactive, bg_active, anim);

    // Draw Switch Background (Rounded Pill)
    ImRect check_bb(pos, ImVec2(pos.x + width, pos.y + height));
    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, ImGui::GetColorU32(bg_col), height * 0.5f);

    // Draw Switch Circle (Knob)
    float pad = 3.0f;
    float circle_sz = height - (pad * 2);
    float circle_x_start = check_bb.Min.x + pad + (circle_sz * 0.5f);
    float circle_x_end = check_bb.Max.x - pad - (circle_sz * 0.5f);
    float circle_x = circle_x_start + (circle_x_end - circle_x_start) * EaseInOutCubic(anim); // Use cubic ease

    window->DrawList->AddCircleFilled(ImVec2(circle_x, check_bb.GetCenter().y), circle_sz * 0.5f, IM_COL32(255, 255, 255, 255));

    // Draw Label
    if (label_size.x > 0.0f) {
        ImVec4 text_col = hovered ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : themeText;
        window->DrawList->AddText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), ImGui::GetColorU32(text_col), label);
    }

    return pressed;
}

// --- Custom Slider (Modern Bar Style) ---
bool CustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = ImGui::CalcItemWidth();

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + ImGui::GetFrameHeight()));
    const ImRect total_bb(frame_bb.Min, ImVec2(frame_bb.Max.x + label_size.x + style.ItemInnerSpacing.x, frame_bb.Max.y));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb)) return false;

    const bool hovered = ImGui::ItemHoverable(frame_bb, id, 0);
    if (hovered) ImGui::SetHoveredID(id);

    const bool clicked = (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left));
    if (clicked || (g.HoveredId == id && g.IO.MouseDown[0])) {
        ImGui::SetActiveID(id, window);
        ImGui::SetFocusID(id, window);
        ImGui::FocusWindow(window);
    }

    bool value_changed = false;
    if (g.ActiveId == id) {
        // *** FIX START: Clear ActiveID if mouse is released ***
        if (g.IO.MouseDown[0]) {
            const float mouse_pos = g.IO.MousePos.x - frame_bb.Min.x;
            const float normalized_pos = ImClamp(mouse_pos / frame_bb.GetWidth(), 0.0f, 1.0f);
            const float new_value = v_min + (v_max - v_min) * normalized_pos;
            if (*v != new_value) { *v = new_value; value_changed = true; }
        }
        else {
            ImGui::ClearActiveID();
        }
        // *** FIX END ***
    }

    float fraction = ImClamp((*v - v_min) / (v_max - v_min), 0.0f, 1.0f);

    // Background Track
    float height = 4.0f;
    float y_center = frame_bb.GetCenter().y;
    window->DrawList->AddRectFilled(
        ImVec2(frame_bb.Min.x, y_center - height / 2),
        ImVec2(frame_bb.Max.x, y_center + height / 2),
        IM_COL32(40, 40, 45, 200),
        height
    );

    // Active Fill
    if (fraction > 0.0f) {
        window->DrawList->AddRectFilledMultiColor(
            ImVec2(frame_bb.Min.x, y_center - height / 2),
            ImVec2(frame_bb.Min.x + frame_bb.GetWidth() * fraction, y_center + height / 2),
            ImGui::GetColorU32(themeAccent),
            ImGui::GetColorU32(ImVec4(themeAccent.x + 0.1f, themeAccent.y + 0.1f, themeAccent.z + 0.1f, 1.0f)),
            ImGui::GetColorU32(ImVec4(themeAccent.x + 0.1f, themeAccent.y + 0.1f, themeAccent.z + 0.1f, 1.0f)),
            ImGui::GetColorU32(themeAccent)
        );
    }

    // Grab Handle
    ImVec2 grab_pos = ImVec2(frame_bb.Min.x + frame_bb.GetWidth() * fraction, y_center);

    if (g.ActiveId == id || g.HoveredId == id) {
        window->DrawList->AddCircleFilled(grab_pos, 9.0f, ImGui::GetColorU32(ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.25f)));
    }
    window->DrawList->AddCircleFilled(grab_pos, 5.0f, IM_COL32(255, 255, 255, 255));

    // Tooltip
    if (g.ActiveId == id || g.HoveredId == id) {
        char buf[64]; sprintf_s(buf, format, *v);
        ImVec2 txt_sz = ImGui::CalcTextSize(buf);
        ImVec2 tip_pos = ImVec2(grab_pos.x, frame_bb.Min.y - 15);

        window->DrawList->AddRectFilled(
            ImVec2(tip_pos.x - txt_sz.x / 2 - 4, tip_pos.y - 2),
            ImVec2(tip_pos.x + txt_sz.x / 2 + 4, tip_pos.y + txt_sz.y + 2),
            IM_COL32(20, 20, 20, 240), 4.0f
        );
        window->DrawList->AddText(ImVec2(tip_pos.x - txt_sz.x / 2, tip_pos.y), IM_COL32(255, 255, 255, 255), buf);
    }

    ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y), label);
    return value_changed;
}

// --- Custom Button (Gradient + Hover Effect) ---
bool CustomButton(const char* label, const ImVec2& size_arg) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // --- Animation ---
    // Click shrinkage effect
    float clickAnim = 0.0f;
    if (held) clickAnim = 2.0f; // Shrink by 2 pixels

    ImRect draw_bb = bb;
    draw_bb.Min.x += clickAnim; draw_bb.Min.y += clickAnim;
    draw_bb.Max.x -= clickAnim; draw_bb.Max.y -= clickAnim;

    // Colors
    ImU32 col_top, col_bot;
    if (held) {
        col_top = ImGui::GetColorU32(ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.4f));
        col_bot = ImGui::GetColorU32(ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.2f));
    }
    else if (hovered) {
        col_top = ImGui::GetColorU32(ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.3f));
        col_bot = ImGui::GetColorU32(ImVec4(themeAccent.x, themeAccent.y, themeAccent.z, 0.1f));
    }
    else {
        col_top = IM_COL32(40, 40, 45, 150);
        col_bot = IM_COL32(20, 20, 25, 150);
    }

    // Draw
    window->DrawList->AddRectFilledMultiColor(draw_bb.Min, draw_bb.Max, col_top, col_top, col_bot, col_bot);

    ImU32 border_col = hovered ? ImGui::GetColorU32(themeAccent) : IM_COL32(60, 60, 70, 100);
    window->DrawList->AddRect(draw_bb.Min, draw_bb.Max, border_col, 4.0f, 0, 1.0f);

    ImVec2 text_pos = ImVec2(draw_bb.Min.x + (draw_bb.GetWidth() - label_size.x) / 2, draw_bb.Min.y + (draw_bb.GetHeight() - label_size.y) / 2);
    ImGui::RenderText(text_pos, label);

    return pressed;
}

bool CustomCollapsingHeader(const char* label, bool default_open) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    // FIX: Use a separate ID for animation to prevent collision with the boolean state
    const ImGuiID anim_id = id + 1;
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    float height = ImGui::GetFrameHeight() + 4.0f;
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, height);
    ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool* p_open = ImGui::GetStateStorage()->GetBoolRef(id, default_open);
    bool hovered, held;
    // Use simple ButtonBehavior
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    if (pressed) {
        *p_open = !*p_open;
    }

    // FIX: Use anim_id instead of id for GetFloat/SetFloat to avoid overwriting the p_open boolean
    float t = *p_open ? 1.0f : 0.0f;
    float anim = ImGui::GetStateStorage()->GetFloat(anim_id, t);
    if (g.LastActiveId == id) {
        float t_anim = ImGui::GetIO().DeltaTime * 12.0f;
        if (anim < t) anim += t_anim;
        if (anim > t) anim -= t_anim;
    }
    else {
        anim = t;
    }
    ImGui::GetStateStorage()->SetFloat(anim_id, anim);
    anim = ImClamp(anim, 0.0f, 1.0f);

    ImU32 bg_col;
    if (hovered) bg_col = ImGui::GetColorU32(ImVec4(themePanelBg.x + 0.05f, themePanelBg.y + 0.05f, themePanelBg.z + 0.05f, themePanelBg.w));
    else bg_col = ImGui::GetColorU32(themePanelBg);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col, themeRounding);

    if (anim > 0.01f) {
        ImU32 accent = ImGui::GetColorU32(themeAccent);
        window->DrawList->AddRectFilled(
            ImVec2(bb.Min.x, bb.Min.y + (height * 0.2f)),
            ImVec2(bb.Min.x + 3.0f, bb.Max.y - (height * 0.2f)),
            accent,
            2.0f
        );
    }

    ImU32 text_col = (*p_open || hovered) ? ImGui::GetColorU32(themeText) : ImGui::GetColorU32(themeTextDisabled);
    ImGui::PushStyleColor(ImGuiCol_Text, text_col);
    ImGui::RenderText(ImVec2(bb.Min.x + 30.0f, bb.Min.y + (height - label_size.y) / 2), label);
    ImGui::PopStyleColor();

    // Arrow
    window->DrawList->PathClear();
    float arrow_sz = 5.0f;
    ImVec2 arrow_center = ImVec2(bb.Max.x - 20, bb.Min.y + height / 2);
    float rot = 1.57f * anim; // Rotate 90 degrees when open

    ImVec2 p1 = ImVec2(-arrow_sz / 2, -arrow_sz);
    ImVec2 p2 = ImVec2(arrow_sz / 2, 0);
    ImVec2 p3 = ImVec2(-arrow_sz / 2, arrow_sz);

    auto Rotate = [&](ImVec2 v) {
        return ImVec2(v.x * cos(rot) - v.y * sin(rot), v.x * sin(rot) + v.y * cos(rot));
        };

    p1 = Rotate(p1); p2 = Rotate(p2); p3 = Rotate(p3);

    window->DrawList->AddLine(ImVec2(arrow_center.x + p1.x, arrow_center.y + p1.y), ImVec2(arrow_center.x + p2.x, arrow_center.y + p2.y), text_col, 2.0f);
    window->DrawList->AddLine(ImVec2(arrow_center.x + p2.x, arrow_center.y + p2.y), ImVec2(arrow_center.x + p3.x, arrow_center.y + p3.y), text_col, 2.0f);

    return *p_open;
}

// Enhanced notification system with animations
void AddNotification(const std::string& msg, float duration, ImVec4 color) {
    notifications.push_back(Notification(msg, ImGui::GetTime(), duration, color));
}

void RenderNotifications() {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImGuiIO& io = ImGui::GetIO();
    float yOffset = io.DisplaySize.y - 60.0f;
    float notificationWidth = 300.0f;

    for (auto it = notifications.begin(); it != notifications.end(); ) {
        float elapsed = ImGui::GetTime() - it->startTime;
        if (elapsed > it->duration) {
            it = notifications.erase(it);
            continue;
        }

        float alpha = 1.0f;
        if (elapsed < 0.3f) {
            alpha = SmoothStep(elapsed / 0.3f);
        }
        else if (elapsed > it->duration - 0.3f) {
            alpha = SmoothStep((it->duration - elapsed) / 0.3f);
        }

        ImU32 textColor = ImGui::ColorConvertFloat4ToU32(ImVec4(it->color.x, it->color.y, it->color.z, it->color.w * alpha));
        ImU32 bgColor = IM_COL32(30, 30, 30, static_cast<int>(200 * alpha));
        ImU32 borderColor = IM_COL32(100, 100, 100, static_cast<int>(255 * alpha));

        ImVec2 textSize = ImGui::CalcTextSize(it->message.c_str());
        float xOffset = io.DisplaySize.x - notificationWidth - 20.0f;
        ImVec2 pos(xOffset, yOffset);
        ImVec2 size(notificationWidth, textSize.y + 20.0f);

        if (elapsed < 0.3f) {
            pos.x = io.DisplaySize.x - (notificationWidth + 20.0f) * SmoothStep(elapsed / 0.3f);
        }

        drawList->AddRectFilled(ImVec2(pos.x + 2, pos.y + 2), ImVec2(pos.x + size.x + 2, pos.y + size.y + 2), IM_COL32(0, 0, 0, static_cast<int>(100 * alpha)), 8.0f);
        drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, 8.0f);
        drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), borderColor, 8.0f, ImDrawFlags_RoundCornersAll, 1.5f);

        ImVec2 textPos(pos.x + (size.x - textSize.x) / 2.0f, pos.y + (size.y - textSize.y) / 2.0f);
        drawList->AddText(textPos, textColor, it->message.c_str());

        yOffset -= size.y + 10.0f;
        ++it;
    }
}



std::string GetConfigPath(const std::string& filename) {
    // Ensure the filename is safe and ends with .txt
    std::string safe_filename = filename;
    if (safe_filename.length() < 4 || safe_filename.substr(safe_filename.length() - 4) != ".txt") {
        safe_filename += ".txt";
    }
    return CONFIG_FOLDER + safe_filename;
}

// Helper to create the config directory if it doesn't exist
void EnsureConfigDirectoryExists() {
    try {
        if (!std::filesystem::exists(CONFIG_FOLDER)) {
            std::filesystem::create_directories(CONFIG_FOLDER);
        }
    }
    catch (const std::exception& e) {
        // Simple error handling for directory creation
        AddNotification("Error creating config directory.", 3.0f, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
    }
}

// Populates the global list of config files (by name, without extension)
void LoadConfigList() {
    g_configFiles.clear();
    EnsureConfigDirectoryExists();

    try {
        for (const auto& entry : std::filesystem::directory_iterator(CONFIG_FOLDER)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                g_configFiles.push_back(entry.path().stem().string());
            }
        }
    }
    catch (...) {
        // Ignore errors during listing
    }

    // Ensure "default" is always a loadable option
    if (g_configFiles.empty()) {
        g_configFiles.push_back("default");
    }
}

// Deletes a config file
void DeleteConfig(const std::string& filename) {
    if (filename == "default") {
        AddNotification("? Cannot delete 'default' config.", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        return;
    }

    std::string fullPath = GetConfigPath(filename);
    try {
        if (std::filesystem::remove(fullPath)) {
            AddNotification("? Configuration '" + filename + "' Deleted.", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
            LoadConfigList(); // Reload the list
            if (g_currentConfigName == filename) {
                g_currentConfigName = "default"; // Revert selection
            }
        }
        else {
            AddNotification("? Config File not found.", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        }
    }
    catch (...) {
        AddNotification("? Error deleting config.", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    }
}



// Save and Load configuration
std::string format_color(const ImVec4& color) {
    std::stringstream ss;
    ss << color.x << "," << color.y << "," << color.z << "," << color.w;
    return ss.str();
}

void SaveConfig(const std::string& filename) {
    EnsureConfigDirectoryExists();
    std::string fullPath = GetConfigPath(filename);

    std::ofstream file(fullPath);
    if (file.is_open()) {

        // --- VISUALS (ESP & GENERAL) ---
        file << "[VISUALS]\n";
        file << "espEnabled=" << espEnabled << "\n";
        file << "showTeammates=" << showTeammates << "\n";
        file << "glowThroughWalls=" << glowThroughWalls << "\n";
        file << "nameESPEnabled=" << nameESPEnabled << "\n";
        file << "armorBarEnabled=" << armorBarEnabled << "\n";
        file << "distanceTextEnabled=" << distanceTextEnabled << "\n";
        file << "soundESPEnabled=" << soundESPEnabled << "\n";
        file << "backtrackESPEnabled=" << backtrackESPEnabled << "\n";
        file << "velocityESPEnabled=" << velocityESPEnabled << "\n";
        file << "advancedFlagsEnabled=" << advancedFlagsEnabled << "\n";
        file << "espGradientHealthBar=" << espGradientHealthBar << "\n";
        file << "espGlowEffect=" << espGlowEffect << "\n";
        file << "espTextShadow=" << espTextShadow << "\n";
        file << "espDistanceFade=" << espDistanceFade << "\n";
        file << "espOutlineGlow=" << espOutlineGlow << "\n";
        file << "outlineEnabled=" << outlineEnabled << "\n";
        file << "healthBarEnabled=" << healthBarEnabled << "\n";
        file << "healthBarStyle=" << healthBarStyle << "\n";
        file << "oofArrowsEnabled=" << oofArrowsEnabled << "\n";
        file << "oofArrowsDistanceColor=" << oofArrowsDistanceColor << "\n";

        // Geometry
        file << "box2DEnabled=" << box2DEnabled << "\n";
        file << "box3DEnabled=" << box3DEnabled << "\n";
        file << "skeletonEnabled=" << boneESPEnabled << "\n";
        file << "skeletonStyle=" << skeletonStyle << "\n";
        file << "wireframeBoneESPEnabled=" << wireframeBoneESPEnabled << "\n";
        file << "headCircleEnabled=" << headCircleEnabled << "\n";
        file << "snaplinesEnabled=" << snaplinesEnabled << "\n";
        file << "tracersEnabled=" << tracersEnabled << "\n";
        file << "glowESPEnabled=" << glowESPEnabled << "\n";
        file << "fovCircleEnabled=" << fovCircleEnabled << "\n";

        // Visuals Floats/Ints
        file << "espMaxDistance=" << espMaxDistance << "\n";
        file << "espTextScale=" << espTextScale << "\n";
        file << "espIconScale=" << espIconScale << "\n";
        file << "espGlowIntensity=" << espGlowIntensity << "\n";
        file << "glowMaxDistance=" << glowMaxDistance << "\n";
        file << "glowOpacity=" << glowOpacity << "\n";
        file << "boxThickness=" << boxThickness << "\n";
        file << "boxRounding=" << boxRounding << "\n";
        file << "skeletonThickness=" << skeletonThickness << "\n";
        file << "snaplineThickness=" << snaplineThickness << "\n";
        file << "tracerThickness=" << tracerThickness << "\n";
        file << "headCircleThickness=" << headCircleThickness << "\n";
        file << "wireframeNumSides=" << wireframeNumSides << "\n";
        file << "wireframeNumRings=" << wireframeNumRings << "\n";
        file << "espBoxStyle=" << espBoxStyle << "\n";
        file << "oofArrowsRadius=" << oofArrowsRadius << "\n";
        file << "oofArrowsSize=" << oofArrowsSize << "\n";
        file << "oofMarginDegrees=" << oofMarginDegrees << "\n";

        // Wireframe Bone Radii
        file << "pelvisWireRadius=" << pelvisWireRadius << "\n";
        file << "spine0WireRadius=" << spine0WireRadius << "\n";
        file << "spine1WireRadius=" << spine1WireRadius << "\n";
        file << "spine2WireRadius=" << spine2WireRadius << "\n";
        file << "spine3WireRadius=" << spine3WireRadius << "\n";
        file << "neckWireRadius=" << neckWireRadius << "\n";
        file << "headWireRadius=" << headWireRadius << "\n";
        file << "clavicleLWireRadius=" << clavicleLWireRadius << "\n";
        file << "upperarmLWireRadius=" << upperarmLWireRadius << "\n";
        file << "forearmLWireRadius=" << forearmLWireRadius << "\n";
        file << "handLWireRadius=" << handLWireRadius << "\n";
        file << "clavicleRWireRadius=" << clavicleRWireRadius << "\n";
        file << "upperarmRWireRadius=" << upperarmRWireRadius << "\n";
        file << "forearmRWireRadius=" << forearmRWireRadius << "\n";
        file << "handRWireRadius=" << handRWireRadius << "\n";
        file << "thighLWireRadius=" << thighLWireRadius << "\n";
        file << "calfLWireRadius=" << calfLWireRadius << "\n";
        file << "footLWireRadius=" << footLWireRadius << "\n";
        file << "thighRWireRadius=" << thighRWireRadius << "\n";
        file << "calfRWireRadius=" << calfRWireRadius << "\n";
        file << "footRWireRadius=" << footRWireRadius << "\n";

        // Visuals Colors
        file << "espColor=" << format_color(espColor) << "\n";
        file << "teammateColor=" << format_color(teammateColor) << "\n";
        file << "visibleColor=" << format_color(visibleColor) << "\n";
        file << "skeletonColor=" << format_color(skeletonColor) << "\n";
        file << "snaplineColor=" << format_color(snaplineColor) << "\n";
        file << "tracerColor=" << format_color(tracerColor) << "\n";
        file << "headCircleColor=" << format_color(headCircleColor) << "\n";
        file << "glowColor=" << format_color(glowColor) << "\n";
        file << "fovCircleColor=" << format_color(fovCircleColor) << "\n";
        file << "wireframeVisibleColor=" << format_color(wireframeVisibleColor) << "\n";
        file << "wireframeInvisibleColor=" << format_color(wireframeInvisibleColor) << "\n";
        file << "backtrackESPColor=" << format_color(backtrackESPColor) << "\n";
        file << "soundESPColor=" << format_color(soundESPColor) << "\n";
        file << "espTextColor=" << format_color(espTextColor) << "\n";
        file << "espIconColor=" << format_color(espIconColor) << "\n";
        file << "espDistanceColor=" << format_color(espDistanceColor) << "\n";
        file << "skeletonStartColor=" << format_color(skeletonStartColor) << "\n";
        file << "skeletonEndColor=" << format_color(skeletonEndColor) << "\n";
        file << "espBoxFillColor=" << format_color(espBoxFillColor) << "\n";

        // --- AIMBOT ---
        file << "[AIMBOT]\n";
        file << "aimbotEnabled=" << aimbotEnabled << "\n";
        file << "externalAimbotEnabled=" << externalAimbotEnabled << "\n";
        file << "externalAimbotUseHead=" << externalAimbotUseHead << "\n";
        file << "externalAimbotAimlock=" << externalAimbotAimlock << "\n";
        file << "externalAimbotPredictMovement=" << externalAimbotPredictMovement << "\n";
        file << "externalAimbotAutoSwitch=" << externalAimbotAutoSwitch << "\n";
        file << "aimOnVisibleOnly=" << aimOnVisibleOnly << "\n";
        file << "aimTeammates=" << aimTeammates << "\n";
        file << "aimbotFOV=" << aimbotFOV << "\n";
        file << "aimbotSmoothing=" << aimbotSmoothing << "\n";
        file << "externalAimbotMaxLockDistance=" << externalAimbotMaxLockDistance << "\n";
        file << "externalAimbotFOV=" << externalAimbotFOV << "\n";
        file << "externalAimbotSmoothness=" << externalAimbotSmoothness << "\n";
        file << "aimKey=" << aimKey << "\n";
        file << "externalAimbotKey=" << externalAimbotKey << "\n";
        file << "externalAimbotHitgroup=" << externalAimbotHitgroup << "\n";
        file << "externalAimbotTargetSelection=" << externalAimbotTargetSelection << "\n";

        // --- TRIGGERBOT (FIXED) ---
        file << "[TRIGGERBOT]\n";
        file << "triggerbotEnabled=" << triggerbotEnabled << "\n";
        file << "triggerbotDelay=" << triggerbotDelay << "\n";
        file << "triggerbotTeammates=" << triggerbotTeammates << "\n";
        file << "triggerbotVisibleOnly=" << triggerbotVisibleOnly << "\n";
        file << "triggerbotKey=" << triggerbotKey << "\n";
        file << "triggerbotRequireStill=" << triggerbotRequireStill << "\n";
        file << "triggerbotStillThreshold=" << triggerbotStillThreshold << "\n";
        file << "triggerbotFirstShotDelay=" << triggerbotFirstShotDelay << "\n";

        // --- MISC FEATURES ---
        file << "[MISC]\n";
        file << "noRecoilEnabled=" << noRecoilEnabled << "\n";
        file << "noFlashEnabled=" << noFlashEnabled << "\n";
        file << "revealAllEnabled=" << revealAllEnabled << "\n";
        file << "infiniteArmorEnabled=" << infiniteArmorEnabled << "\n";
        file << "hitsoundEnabled=" << hitsoundEnabled << "\n";
        file << "bulletRedirectionEnabled=" << bulletRedirectionEnabled << "\n";
        file << "bulletRedirectionYaw=" << bulletRedirectionYaw << "\n";
        file << "bulletRedirectionPitch=" << bulletRedirectionPitch << "\n";
        file << "bulletRedirectionKey=" << bulletRedirectionKey << "\n";
        file << "fovChangerEnabled=" << fovChangerEnabled << "\n";
        file << "fovChangerValue=" << fovChangerValue << "\n";
        file << "worldModEnabled=" << worldModEnabled << "\n";
        file << "worldBrightness=" << worldBrightness << "\n";
        file << "healthAnimationSpeed=" << healthAnimationSpeed << "\n";
        file << "hitmarkerSpeed=" << hitmarkerSpeed << "\n";
        file << "hitmarkerFadeDuration=" << hitmarkerFadeDuration << "\n";

        // --- PARTICLES (FIXED) ---
        file << "[PARTICLES]\n";
        file << "particleModEnabled=" << particleModEnabled << "\n";
        file << "globalParticleColor=" << format_color(globalParticleColor) << "\n";
        file << "rainbowEnabled=" << rainbowEnabled << "\n";
        file << "rainbowSpeed=" << rainbowSpeed << "\n";
        file << "lightModEnabled=" << lightModEnabled << "\n";
        file << "lightModColor=" << format_color(lightModColor) << "\n";

        // Save Particle Groups
        for (const auto& group : particleGroups) {
            std::string sanitizedName = sanitize_key(group.name);
            file << "part_grp_" << sanitizedName << "_enabled=" << group.enabled << "\n";
            file << "part_grp_" << sanitizedName << "_color=" << format_color(group.color) << "\n";
        }


        // --- HUD / OVERLAY ---
        file << "[HUD]\n";
        file << "watermarkEnabled=" << watermarkEnabled << "\n";
        file << "crosshairEnabled=" << crosshairEnabled << "\n";
        file << "customCrosshairEnabled=" << customCrosshairEnabled << "\n";
        file << "showFPS=" << showFPS << "\n";
        file << "showPosition=" << showPosition << "\n";
        file << "showVelocity=" << showVelocity << "\n";
        file << "showSpectators=" << showSpectators << "\n";
        file << "showGameTime=" << showGameTime << "\n";
        file << "showKeysPressed=" << showKeysPressed << "\n";
        file << "showLocalPlayerInfo=" << showLocalPlayerInfo << "\n";
        file << "showEntityCount=" << showEntityCount << "\n";
        file << "showSystemInfo=" << showSystemInfo << "\n";
        file << "showNetworkInfo=" << showNetworkInfo << "\n";
        file << "showPlayerInfo=" << showPlayerInfo << "\n";
        file << "showMatchInfo=" << showMatchInfo << "\n";
        file << "showAngleLines=" << showAngleLines << "\n";
        file << "showTargetInfo=" << showTargetInfo << "\n";
        file << "showMinimap=" << showMinimap << "\n";
        file << "showRadarHud=" << showRadarHud << "\n";
        file << "showGrenadeWarning=" << showGrenadeWarning << "\n";
        file << "showBombTimer=" << showBombTimer << "\n";
        file << "showDefuseTimer=" << showDefuseTimer << "\n";
        file << "showKillFeed=" << showKillFeed << "\n";
        file << "showDamageIndicator=" << showDamageIndicator << "\n";
        file << "showRecoilCrosshair=" << showRecoilCrosshair << "\n";
        file << "showSpreadCircle=" << showSpreadCircle << "\n";
        file << "showAmmoWarning=" << showAmmoWarning << "\n";
        file << "showMoneyDisplay=" << showMoneyDisplay << "\n";
        file << "showRoundTimer=" << showRoundTimer << "\n";
        file << "showBombIcon=" << showBombIcon << "\n";
        file << "showGrenadeTrajectory=" << showGrenadeTrajectory << "\n";
        file << "showSmokeWarning=" << showSmokeWarning << "\n";
        file << "showFireWarning=" << showFireWarning << "\n";
        file << "showFlashWarning=" << showFlashWarning << "\n";
        file << "showScopeLines=" << showScopeLines << "\n";
        file << "showFovChangerIndicator=" << showFovChangerIndicator << "\n";
        file << "showThirdPersonIndicator=" << showThirdPersonIndicator << "\n";
        file << "showSpectatorCount=" << showSpectatorCount << "\n";
        file << "showTeamInfo=" << showTeamInfo << "\n";
        file << "showClantag=" << showClantag << "\n";
        file << "showRank=" << showRank << "\n";
        file << "showWinProbability=" << showWinProbability << "\n";
        file << "showEconomyInfo=" << showEconomyInfo << "\n";
        file << "showBuyMenuHelper=" << showBuyMenuHelper << "\n";
        file << "showGrenadeHelper=" << showGrenadeHelper << "\n";
        file << "showJumpStats=" << showJumpStats << "\n";
        file << "showStaminaBar=" << showStaminaBar << "\n";
        file << "showKeybinds=" << showKeybinds << "\n";
        file << "showMenuKeybind=" << showMenuKeybind << "\n";
        file << "showSpectatorList=" << showSpectatorList << "\n";
        file << "crosshairSize=" << crosshairSize << "\n";
        file << "crosshairThickness=" << crosshairThickness << "\n";
        file << "crosshairColor=" << format_color(crosshairColor) << "\n";

        // --- THEME / MENU ---
        file << "[THEME]\n";
        file << "currentTheme=" << currentTheme << "\n";
        file << "currentCheckboxStyle=" << currentCheckboxStyle << "\n";
        file << "themeRounding=" << themeRounding << "\n";
        file << "themeRoundedIcons=" << themeRoundedIcons << "\n";
        file << "themeAnimatedElements=" << themeAnimatedElements << "\n";
        file << "themeAnimationSpeed=" << themeAnimationSpeed << "\n";
        file << "useGradientButtons=" << useGradientButtons << "\n";
        file << "useOutlineButtons=" << useOutlineButtons << "\n";
        file << "useGlassButtons=" << useGlassButtons << "\n";
        file << "useModernButtons=" << useModernButtons << "\n";
        file << "themeCustomButtons=" << themeCustomButtons << "\n";
        file << "checkboxRippleEffect=" << checkboxRippleEffect << "\n";
        file << "checkboxAnimationSpeed=" << checkboxAnimationSpeed << "\n";
        file << "hoverColor=" << format_color(hoverColor) << "\n";
        file << "activeColor=" << format_color(activeColor) << "\n";
        file << "themeAccent=" << format_color(themeAccent) << "\n";
        file << "themeBackground=" << format_color(themeBackground) << "\n";
        file << "themePanelBg=" << format_color(themePanelBg) << "\n";
        file << "themeText=" << format_color(themeText) << "\n";
        file << "themeTextDisabled=" << format_color(themeTextDisabled) << "\n";
        file << "checkboxActiveColor=" << format_color(checkboxActiveColor) << "\n";
        file << "checkboxInactiveColor=" << format_color(checkboxInactiveColor) << "\n";

        file.close();

        g_currentConfigName = filename;
        LoadConfigList();

        AddNotification("? Configuration '" + filename + "' Saved Successfully", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
    }
    else {
        AddNotification("? Error Saving Config to: " + fullPath, 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    }
    // Re-apply theme after saving to ensure colors are set globally (for consistency)
    ApplyTheme(currentTheme);
}

void LoadConfig(const std::string& filename) {

    std::string fullPath = GetConfigPath(filename);
    std::ifstream file(fullPath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '[' || line[0] == ';') continue;

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) continue;

            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Helper macro for parsing floats/ints/bools
#define PARSE_VAR(v, type) if (key == #v) v = std::stoi(value)
#define PARSE_FLOAT(v) if (key == #v) v = std::stof(value)
#define PARSE_COLOR(v) if (key == #v) v = parse_color(value)

// --- VISUALS (ESP & GENERAL) ---
            PARSE_VAR(espEnabled, bool);
            PARSE_VAR(showTeammates, bool);
            PARSE_VAR(glowThroughWalls, bool);
            PARSE_VAR(nameESPEnabled, bool);
            PARSE_VAR(armorBarEnabled, bool);
            PARSE_VAR(distanceTextEnabled, bool);
            PARSE_VAR(soundESPEnabled, bool);
            PARSE_VAR(backtrackESPEnabled, bool);
            PARSE_VAR(velocityESPEnabled, bool);
            PARSE_VAR(advancedFlagsEnabled, bool);
            PARSE_VAR(espGradientHealthBar, bool);
            PARSE_VAR(espGlowEffect, bool);
            PARSE_VAR(espTextShadow, bool);
            PARSE_VAR(espDistanceFade, bool);
            PARSE_VAR(espOutlineGlow, bool);
            PARSE_VAR(outlineEnabled, bool);
            PARSE_VAR(healthBarEnabled, bool);
            PARSE_VAR(healthBarStyle, int);
            PARSE_VAR(oofArrowsEnabled, bool);
            PARSE_VAR(oofArrowsDistanceColor, bool);

            // Geometry
            PARSE_VAR(box2DEnabled, bool);
            PARSE_VAR(box3DEnabled, bool);
            PARSE_VAR(boneESPEnabled, bool);
            PARSE_VAR(skeletonStyle, int);
            PARSE_VAR(wireframeBoneESPEnabled, bool);
            PARSE_VAR(headCircleEnabled, bool);
            PARSE_VAR(snaplinesEnabled, bool);
            PARSE_VAR(tracersEnabled, bool);
            PARSE_VAR(glowESPEnabled, bool);
            PARSE_VAR(fovCircleEnabled, bool);

            // Visuals Floats/Ints
            PARSE_FLOAT(espMaxDistance);
            PARSE_FLOAT(espTextScale);
            PARSE_FLOAT(espIconScale);
            PARSE_FLOAT(espGlowIntensity);
            PARSE_FLOAT(glowMaxDistance);
            PARSE_FLOAT(glowOpacity);
            PARSE_FLOAT(boxThickness);
            PARSE_FLOAT(boxRounding);
            PARSE_FLOAT(skeletonThickness);
            PARSE_FLOAT(snaplineThickness);
            PARSE_FLOAT(tracerThickness);
            PARSE_FLOAT(headCircleThickness);
            PARSE_VAR(wireframeNumSides, int);
            PARSE_VAR(wireframeNumRings, int);
            PARSE_VAR(espBoxStyle, int);
            PARSE_FLOAT(oofArrowsRadius);
            PARSE_FLOAT(oofArrowsSize);
            PARSE_FLOAT(oofMarginDegrees);

            // Wireframe Bone Radii
            PARSE_FLOAT(pelvisWireRadius);
            PARSE_FLOAT(spine0WireRadius);
            PARSE_FLOAT(spine1WireRadius);
            PARSE_FLOAT(spine2WireRadius);
            PARSE_FLOAT(spine3WireRadius);
            PARSE_FLOAT(neckWireRadius);
            PARSE_FLOAT(headWireRadius);
            PARSE_FLOAT(clavicleLWireRadius);
            PARSE_FLOAT(upperarmLWireRadius);
            PARSE_FLOAT(forearmLWireRadius);
            PARSE_FLOAT(handLWireRadius);
            PARSE_FLOAT(clavicleRWireRadius);
            PARSE_FLOAT(upperarmRWireRadius);
            PARSE_FLOAT(forearmRWireRadius);
            PARSE_FLOAT(handRWireRadius);
            PARSE_FLOAT(thighLWireRadius);
            PARSE_FLOAT(calfLWireRadius);
            PARSE_FLOAT(footLWireRadius);
            PARSE_FLOAT(thighRWireRadius);
            PARSE_FLOAT(calfRWireRadius);
            PARSE_FLOAT(footRWireRadius);

            // Visuals Colors
            PARSE_COLOR(espColor);
            PARSE_COLOR(teammateColor);
            PARSE_COLOR(visibleColor);
            PARSE_COLOR(skeletonColor);
            PARSE_COLOR(snaplineColor);
            PARSE_COLOR(tracerColor);
            PARSE_COLOR(headCircleColor);
            PARSE_COLOR(glowColor);
            PARSE_COLOR(fovCircleColor);
            PARSE_COLOR(wireframeVisibleColor);
            PARSE_COLOR(wireframeInvisibleColor);
            PARSE_COLOR(backtrackESPColor);
            PARSE_COLOR(soundESPColor);
            PARSE_COLOR(espTextColor);
            PARSE_COLOR(espIconColor);
            PARSE_COLOR(espDistanceColor);
            PARSE_COLOR(skeletonStartColor);
            PARSE_COLOR(skeletonEndColor);
            PARSE_COLOR(espBoxFillColor);

            // --- AIMBOT ---
            PARSE_VAR(aimbotEnabled, bool);
            PARSE_VAR(externalAimbotEnabled, bool);
            PARSE_VAR(externalAimbotUseHead, bool);
            PARSE_VAR(externalAimbotAimlock, bool);
            PARSE_VAR(externalAimbotPredictMovement, bool);
            PARSE_VAR(externalAimbotAutoSwitch, bool);
            PARSE_VAR(aimOnVisibleOnly, bool);
            PARSE_VAR(aimTeammates, bool);
            PARSE_FLOAT(aimbotFOV);
            PARSE_FLOAT(aimbotSmoothing);
            PARSE_FLOAT(externalAimbotMaxLockDistance);
            PARSE_FLOAT(externalAimbotFOV);
            PARSE_FLOAT(externalAimbotSmoothness);
            PARSE_VAR(aimKey, int);
            PARSE_VAR(externalAimbotKey, int);
            PARSE_VAR(externalAimbotHitgroup, int);
            PARSE_VAR(externalAimbotTargetSelection, int);

            // --- TRIGGERBOT (FIXED) ---
            PARSE_VAR(triggerbotEnabled, bool);
            PARSE_FLOAT(triggerbotDelay);
            PARSE_VAR(triggerbotTeammates, bool);
            PARSE_VAR(triggerbotVisibleOnly, bool);
            PARSE_VAR(triggerbotKey, int);
            PARSE_VAR(triggerbotRequireStill, bool);
            PARSE_FLOAT(triggerbotStillThreshold);
            PARSE_FLOAT(triggerbotFirstShotDelay);

            // --- MISC FEATURES ---
            PARSE_VAR(noRecoilEnabled, bool);
            PARSE_VAR(noFlashEnabled, bool);
            PARSE_VAR(revealAllEnabled, bool);
            PARSE_VAR(infiniteArmorEnabled, bool);
            PARSE_VAR(hitsoundEnabled, bool);
            PARSE_VAR(bulletRedirectionEnabled, bool);
            PARSE_FLOAT(bulletRedirectionYaw);
            PARSE_FLOAT(bulletRedirectionPitch);
            PARSE_VAR(bulletRedirectionKey, int);
            PARSE_VAR(fovChangerEnabled, bool);
            PARSE_FLOAT(fovChangerValue);
            PARSE_VAR(worldModEnabled, bool);
            PARSE_FLOAT(worldBrightness);
            PARSE_FLOAT(healthAnimationSpeed);
            PARSE_FLOAT(hitmarkerSpeed);
            PARSE_FLOAT(hitmarkerFadeDuration);

            // --- PARTICLES (FIXED) ---
            PARSE_VAR(particleModEnabled, bool);
            PARSE_COLOR(globalParticleColor);
            PARSE_VAR(rainbowEnabled, bool);
            PARSE_FLOAT(rainbowSpeed);
            PARSE_VAR(lightModEnabled, bool);
            PARSE_COLOR(lightModColor);

            // Dynamic Particle Group Parsing
            if (key.rfind("part_grp_", 0) == 0) {
                std::string sub = key.substr(9); // remove "part_grp_"
                bool isColor = false;
                std::string name;

                if (sub.length() > 6 && sub.substr(sub.length() - 6) == "_color") {
                    name = sub.substr(0, sub.length() - 6);
                    isColor = true;
                }
                else if (sub.length() > 8 && sub.substr(sub.length() - 8) == "_enabled") {
                    name = sub.substr(0, sub.length() - 8);
                    isColor = false;
                }

                // Match with existing groups
                for (auto& group : particleGroups) {
                    if (sanitize_key(group.name) == name) {
                        if (isColor) {
                            group.color = parse_color(value);
                            // Apply to indices immediately upon load
                            for (int idx : group.indices) {
                                particleColors[idx] = group.color;
                            }
                        }
                        else {
                            group.enabled = (bool)std::stoi(value);
                            // Apply to indices immediately upon load
                            for (int idx : group.indices) {
                                particleTargetsEnabled[idx] = group.enabled;
                            }
                        }
                        break;
                    }
                }
            }

            // --- HUD / OVERLAY ---
            PARSE_VAR(watermarkEnabled, bool);
            PARSE_VAR(crosshairEnabled, bool);
            PARSE_VAR(customCrosshairEnabled, bool);
            PARSE_VAR(showFPS, bool);
            PARSE_VAR(showPosition, bool);
            PARSE_VAR(showVelocity, bool);
            PARSE_VAR(showSpectators, bool);
            PARSE_VAR(showGameTime, bool);
            PARSE_VAR(showKeysPressed, bool);
            PARSE_VAR(showLocalPlayerInfo, bool);
            PARSE_VAR(showEntityCount, bool);
            PARSE_VAR(showSystemInfo, bool);
            PARSE_VAR(showNetworkInfo, bool);
            PARSE_VAR(showPlayerInfo, bool);
            PARSE_VAR(showMatchInfo, bool);
            PARSE_VAR(showAngleLines, bool);
            PARSE_VAR(showTargetInfo, bool);
            PARSE_VAR(showMinimap, bool);
            PARSE_VAR(showRadarHud, bool);
            PARSE_VAR(showGrenadeWarning, bool);
            PARSE_VAR(showBombTimer, bool);
            PARSE_VAR(showDefuseTimer, bool);
            PARSE_VAR(showKillFeed, bool);
            PARSE_VAR(showDamageIndicator, bool);
            PARSE_VAR(showRecoilCrosshair, bool);
            PARSE_VAR(showSpreadCircle, bool);
            PARSE_VAR(showAmmoWarning, bool);
            PARSE_VAR(showMoneyDisplay, bool);
            PARSE_VAR(showRoundTimer, bool);
            PARSE_VAR(showBombIcon, bool);
            PARSE_VAR(showGrenadeTrajectory, bool);
            PARSE_VAR(showSmokeWarning, bool);
            PARSE_VAR(showFireWarning, bool);
            PARSE_VAR(showFlashWarning, bool);
            PARSE_VAR(showScopeLines, bool);
            PARSE_VAR(showFovChangerIndicator, bool);
            PARSE_VAR(showThirdPersonIndicator, bool);
            PARSE_VAR(showSpectatorCount, bool);
            PARSE_VAR(showTeamInfo, bool);
            PARSE_VAR(showClantag, bool);
            PARSE_VAR(showRank, bool);
            PARSE_VAR(showWinProbability, bool);
            PARSE_VAR(showEconomyInfo, bool);
            PARSE_VAR(showBuyMenuHelper, bool);
            PARSE_VAR(showGrenadeHelper, bool);
            PARSE_VAR(showJumpStats, bool);
            PARSE_VAR(showStaminaBar, bool);
            PARSE_VAR(showKeybinds, bool);
            PARSE_VAR(showMenuKeybind, bool);
            PARSE_VAR(showSpectatorList, bool);
            PARSE_FLOAT(crosshairSize);
            PARSE_FLOAT(crosshairThickness);
            PARSE_COLOR(crosshairColor);

            // --- THEME / MENU ---
            PARSE_VAR(currentTheme, int);
            PARSE_VAR(currentCheckboxStyle, int);
            PARSE_FLOAT(themeRounding);
            PARSE_VAR(themeRoundedIcons, bool);
            PARSE_VAR(themeAnimatedElements, bool);
            PARSE_FLOAT(themeAnimationSpeed);
            PARSE_VAR(useGradientButtons, bool);
            PARSE_VAR(useOutlineButtons, bool);
            PARSE_VAR(useGlassButtons, bool);
            PARSE_VAR(useModernButtons, bool);
            PARSE_VAR(themeCustomButtons, bool);
            PARSE_VAR(checkboxRippleEffect, bool);
            PARSE_FLOAT(checkboxAnimationSpeed);
            PARSE_COLOR(hoverColor);
            PARSE_COLOR(activeColor);
            PARSE_COLOR(themeAccent);
            PARSE_COLOR(themeBackground);
            PARSE_COLOR(themePanelBg);
            PARSE_COLOR(themeText);
            PARSE_COLOR(themeTextDisabled);
            PARSE_COLOR(checkboxActiveColor);
            PARSE_COLOR(checkboxInactiveColor);

#undef PARSE_VAR
#undef PARSE_FLOAT
#undef PARSE_COLOR
        }
        file.close();

        g_currentConfigName = filename;

        AddNotification("? Configuration '" + filename + "' Loaded Successfully", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));

        // IMPORTANT: Re-apply the loaded theme to update ImGui style colors
        ApplyTheme(currentTheme);
    }
    else {
        AddNotification("? Config File '" + filename + "' Not Found. Loading Defaults.", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    }
}