#pragma once
#include "cheat.h"
#include "dependencies/ImGui/imgui_internal.h"

#define CONFIG_FOLDER "C:\\CheatConfigs\\"

// Menu animation system
struct MenuAnimation {
    float alpha = 0.0f;
    float backgroundDim = 0.0f;
    float tabAnimationProgress = 0.0f;
    int currentTab = 0;
    int previousTab = 0;
    // Tab Animation State
    int activeTab = 0;          // The currently displayed tab
    int targetTab = 0;          // The tab we want to go to
    float tabAlpha = 1.0f;      // Opacity for fade effect
    float tabOffset = 0.0f;     // Y-offset for slide effect

    bool styleSet = false;
};

// Theme system
enum ThemeType {
    THEME_DEFAULT = 0,
    THEME_DARK_PRO,
    THEME_LIGHT,
    THEME_PURPLE_NEON,
    THEME_RETRO,
    THEME_MIDNIGHT,
    THEME_COUNT
};

enum CheckboxStyle {
    CHECKBOX_DEFAULT = 0,
    CHECKBOX_ROUNDED,
    CHECKBOX_SWITCH,
    CHECKBOX_FILLED,
    CHECKBOX_OUTLINE,
    CHECKBOX_ANIMATED,
    CHECKBOX_STYLE_COUNT
};

// Global menu variables
extern MenuAnimation g_MenuAnimation;
extern int currentTheme;
extern int currentCheckboxStyle;
extern bool themeInitialized;
extern ImVec4 themeAccent;
extern ImVec4 themeBackground;
extern ImVec4 themeText;
extern ImVec4 themeChildBg;
extern float themeRounding;
extern bool themeRoundedIcons;
extern bool themeAnimatedElements;
extern float themeAnimationSpeed;
extern ImVec4 hoverColor;
extern ImVec4 activeColor;
extern ImVec4 themeHeaderColor;
extern ImVec4 themeBorderColor;
extern ImVec4 themeFrameBg;
extern ImVec4 themeTextDisabled;
extern bool themeCustomButtons;
extern bool themeRoundedTabs;
extern float themeWindowRounding;
extern bool useGradientButtons;
extern bool useOutlineButtons;
extern bool useGlassButtons;
extern bool useModernButtons;
extern bool checkboxRippleEffect;
extern float checkboxAnimationSpeed;
extern ImVec4 checkboxActiveColor;
extern ImVec4 checkboxInactiveColor;
extern ImVec4 themePanelBg;
extern MenuAnimation g_MenuAnimation;

// Animation functions
float SmoothStep(float t);
float EaseInOutCubic(float t);
float EaseOutBack(float t);
float EaseInElastic(float t);
float Lerp(float a, float b, float t);
ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t);

// UI Components
bool ThemedCheckbox(const char* label, bool* v, const char* tooltip = nullptr);
bool ThemedCheckbox(const char* label, bool* v, float size, const char* tooltip = nullptr);
bool ThemedButton(const char* label, const ImVec2& size = ImVec2(0, 0), bool special = false, const char* tooltip = nullptr);
bool AnimatedTabButton(const char* label, bool selected, const ImVec2& size = ImVec2(0, 0));
bool AnimatedButtonEx(const char* label, const ImVec2& size, int effectType = 0);
bool ColorPicker(const char* label, ImVec4& color);
bool ToggleButton(const char* label, bool* state, const ImVec2& size = ImVec2(0, 0));
bool BadgeButton(const char* label, int count, const ImVec2& size = ImVec2(0, 0));
bool IconButton(const char* icon, const char* label, const ImVec2& size = ImVec2(0, 0));
bool ThemeSelectionCard(const char* label, const char* description, ImVec4 accent, bool selected);

void ApplyTheme(int theme);
void ThemePreviewWidget();
void SetupImGuiStyle();
void UpdateMenuAnimations();
void RenderPageTransition(ImDrawList* draw_list, const ImVec2& pos, const ImVec2& size);

// Tab rendering functions
void RenderVisualsTab();
void RenderAimbotTab();
void RenderMiscTab();
void RenderConfigTab();
void RenderThemeTab();

// Main menu function
void RenderCleanMenu();

// ImVec2 helper functions
namespace ImVec2Helpers {
    inline ImVec2 Add(const ImVec2& a, const ImVec2& b);
    inline ImVec2 Subtract(const ImVec2& a, const ImVec2& b);
    inline ImVec2 Multiply(const ImVec2& a, float b);
    inline ImVec2 Multiply(float a, const ImVec2& b);
    inline ImVec2 Divide(const ImVec2& a, float b);
}


extern std::string g_currentConfigName;
extern std::vector<std::string> g_configFiles;

// Updated function declarations
void SaveConfig(const std::string& filename);
void LoadConfig(const std::string& filename);
void LoadConfigList(); // New function to populate the config file list
void DeleteConfig(const std::string& filename);


extern ImTextureID g_GameTexture;
bool CustomCheckbox(const char* label, bool* v);
bool CustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.2f");
bool CustomButton(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
void SetupWindowBlur(HWND hwnd); // Function to enable DWM Blur
bool CustomCollapsingHeader(const char* label, bool default_open = false);