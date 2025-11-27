#include "menu.h"
#include "fontawesome.h"

// --- Theme Variables (Managed by menu_theme.cpp) ---
int currentTheme = THEME_DEFAULT;
int currentCheckboxStyle = CHECKBOX_DEFAULT;
bool themeInitialized = false;

// Default colors (Will be overwritten by ApplyTheme)
ImVec4 themeAccent = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
ImVec4 themeBackground = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
ImVec4 themeText = ImVec4(0.95f, 0.95f, 0.97f, 1.00f);
ImVec4 themeChildBg = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
ImVec4 themePanelBg = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
float themeRounding = 12.0f;
bool themeRoundedIcons = true;
bool themeAnimatedElements = true;
float themeAnimationSpeed = 1.0f;
ImVec4 hoverColor = ImVec4(0.25f, 0.90f, 0.50f, 1.00f);
ImVec4 activeColor = ImVec4(0.30f, 0.95f, 0.55f, 1.00f);
ImVec4 themeHeaderColor = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
ImVec4 themeBorderColor = ImVec4(0.20f, 0.20f, 0.25f, 0.60f);
ImVec4 themeFrameBg = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
ImVec4 themeTextDisabled = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
bool themeCustomButtons = false;
bool themeRoundedTabs = true;
float themeWindowRounding = 12.0f;
bool useGradientButtons = false;
bool useOutlineButtons = false;
bool useGlassButtons = false;
bool useModernButtons = false;
bool checkboxRippleEffect = true;
float checkboxAnimationSpeed = 1.0f;
ImVec4 checkboxActiveColor = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
ImVec4 checkboxInactiveColor = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);

// Remove all DirectX Globals
ImTextureID g_GameTexture = NULL;

// Helper: Draw a shadow/glow behind the menu
void DrawMenuShadow(ImVec2 pos, ImVec2 size, ImU32 color, float intensity) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    for (int i = 0; i < 8; i++) {
        float off = (float)i * 1.5f; // Tighter shadow
        float alpha = (1.0f - ((float)i / 8.0f)) * intensity;
        ImU32 col = (color & 0x00FFFFFF) | ((int)(alpha * 255) << 24);

        dl->AddRect(
            ImVec2(pos.x - off, pos.y - off),
            ImVec2(pos.x + size.x + off, pos.y + size.y + off),
            col,
            themeWindowRounding + off,
            0,
            2.0f
        );
    }
}

void UpdateTabAnimations() {
    float deltaTime = ImGui::GetIO().DeltaTime;
    float speed = 12.0f * themeAnimationSpeed;

    // 1. Fade In Logic
    if (g_MenuAnimation.tabAlpha < 1.0f) {
        g_MenuAnimation.tabAlpha += deltaTime * speed * 0.5f;
        if (g_MenuAnimation.tabAlpha > 1.0f) g_MenuAnimation.tabAlpha = 1.0f;
    }

    // 2. Slide Up Logic (Offset goes from 20 -> 0)
    if (g_MenuAnimation.tabOffset > 0.0f) {
        g_MenuAnimation.tabOffset -= deltaTime * speed * 10.0f;
        if (g_MenuAnimation.tabOffset < 0.0f) g_MenuAnimation.tabOffset = 0.0f;
    }
}

void RenderCleanMenu() {
    if (!menuOpen && g_MenuAnimation.alpha <= 0.0f) return;

    UpdateMenuAnimations();
    UpdateTabAnimations();

    float alpha = g_MenuAnimation.alpha;
    if (alpha <= 0.01f && !menuOpen) return;

    // Set styling based on alpha
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

    // Scale animation (Pop in effect)
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    float scale = Lerp(0.95f, 1.0f, EaseOutBack(alpha));
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    // Flags: NoDecoration removes the OS title bar and default ImGui background borders
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

    if (!g_MenuAnimation.styleSet) {
        ApplyTheme(currentTheme);
        g_MenuAnimation.styleSet = true;
    }

    // 1. Draw "Dimmer" 
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        ImVec2(0, 0),
        ImGui::GetIO().DisplaySize,
        IM_COL32(0, 0, 0, 120)
    );

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, themeWindowRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::SetNextWindowSize(ImVec2(920, 780), ImGuiCond_FirstUseEver);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    if (ImGui::Begin("##CS2WoW", &menuOpen, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar)) {
        ImVec2 p = ImGui::GetWindowPos();
        ImVec2 s = ImGui::GetWindowSize();
        ImDrawList* dl = ImGui::GetWindowDrawList();

        // 2. Shadow Glow (Uses Theme Accent)
        ImU32 shadowCol = ImGui::GetColorU32(themeAccent);
        DrawMenuShadow(p, s, shadowCol, 0.35f);

        // 3. Main Glass Background (Uses Theme Background)
        // We convert the themeBackground (ImVec4) to U32, but enforce some transparency for the glass effect
        ImU32 bgColTop = ImGui::GetColorU32(ImVec4(themeBackground.x, themeBackground.y, themeBackground.z, 0.92f));
        ImU32 bgColBot = ImGui::GetColorU32(ImVec4(themeBackground.x * 0.8f, themeBackground.y * 0.8f, themeBackground.z * 0.8f, 0.95f));

        dl->AddRectFilledMultiColor(p, ImVec2(p.x + s.x, p.y + s.y), bgColTop, bgColTop, bgColBot, bgColBot);

        // 4. Header Area
        float headerH = 70.0f;
        ImU32 headerCol = ImGui::GetColorU32(ImVec4(themeBackground.x * 1.2f, themeBackground.y * 1.2f, themeBackground.z * 1.2f, 1.0f));
        dl->AddRectFilled(p, ImVec2(p.x + s.x, p.y + headerH), headerCol, themeWindowRounding, ImDrawFlags_RoundCornersTop);

        // Accent Line
        dl->AddRectFilled(ImVec2(p.x, p.y + headerH), ImVec2(p.x + s.x, p.y + headerH + 1), ImGui::GetColorU32(themeAccent));

        // Border
        ImU32 borderCol = ImGui::GetColorU32(themeBorderColor);
        dl->AddRect(p, ImVec2(p.x + s.x, p.y + s.y), borderCol, themeWindowRounding, 0, 1.0f);

        // --- HEADER CONTENT ---
        ImGui::SetCursorPos(ImVec2(25, 20));
        ImGui::PushFont(defaultFont);
        ImGui::TextColored(themeAccent, ICON_FA_DRAGON " CS2 WoW");
        ImGui::SameLine();
        ImGui::TextColored(themeTextDisabled, " | Internal");
        ImGui::PopFont();

        // Tabs
        const char* tabs[] = { ICON_FA_EYE " Visuals", ICON_FA_CROSSHAIRS " Aimbot", ICON_FA_COGS " Misc", ICON_FA_FILE_CODE " Config", ICON_FA_PAINT_BRUSH " Theme" };
        float tabStart = 220.0f;
        ImGui::SetCursorPos(ImVec2(tabStart, 18));

        for (int i = 0; i < 5; i++) {
            bool selected = (g_MenuAnimation.activeTab == i);
            ImU32 txtCol = selected ? ImGui::GetColorU32(themeAccent) : ImGui::GetColorU32(themeTextDisabled);

            ImGui::PushStyleColor(ImGuiCol_Text, txtCol);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.05f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.1f));

            if (ImGui::Button(tabs[i], ImVec2(110, 35))) {
                if (g_MenuAnimation.activeTab != i) {
                    // Trigger Animation
                    g_MenuAnimation.activeTab = i;
                    g_MenuAnimation.tabAlpha = 0.0f; // Reset opacity
                    g_MenuAnimation.tabOffset = 15.0f; // Reset slide offset (15px down)
                }
            }

            ImGui::PopStyleColor(4);

            // Animated Underline
            // You can add a sliding underline here if you want, but the text color change is clean.
            if (selected) {
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                dl->AddRectFilled(ImVec2(min.x + 15, max.y - 2), ImVec2(max.x - 15, max.y), ImGui::GetColorU32(themeAccent), 2.0f);
            }
            ImGui::SameLine();
        }

        // --- PAGE CONTENT ---
        ImGui::SetCursorPos(ImVec2(20, headerH + 20));
        ImVec2 contentSize = ImVec2(s.x - 40, s.y - headerH - 40);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, g_MenuAnimation.tabAlpha * g_MenuAnimation.alpha);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
        ImGui::BeginChild("PageContent", contentSize, false, ImGuiWindowFlags_NoScrollbar);

        switch (g_MenuAnimation.activeTab) {
        case 0: RenderVisualsTab(); break;
        case 1: RenderAimbotTab(); break;
        case 2: RenderMiscTab(); break;
        case 3: RenderConfigTab(); break;
        case 4: RenderThemeTab(); break;
        }
        
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        
    }
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleVar();
}