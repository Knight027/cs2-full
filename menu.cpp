#include "cheat.h"

void AddNotification(const std::string& msg, float duration, ImVec4 color) {
    notifications.push_back(Notification(msg, ImGui::GetTime(), duration, color));
}

void RenderNotifications() {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImGuiIO& io = ImGui::GetIO();
    float yOffset = io.DisplaySize.y - 60.0f; // Start from bottom, stack upwards
    float notificationWidth = 300.0f; // Fixed width for notifications

    for (auto it = notifications.begin(); it != notifications.end(); ) {
        float elapsed = ImGui::GetTime() - it->startTime;
        if (elapsed > it->duration) {
            it = notifications.erase(it);
            continue;
        }

        float alpha = 1.0f - (elapsed / it->duration);
        ImU32 textColor = ImGui::ColorConvertFloat4ToU32(ImVec4(it->color.x, it->color.y, it->color.z, it->color.w * alpha));
        ImU32 bgColor = IM_COL32(30, 30, 30, static_cast<int>(200 * alpha)); // Semi-transparent dark background
        ImU32 borderColor = IM_COL32(100, 100, 100, static_cast<int>(255 * alpha)); // Border

        ImVec2 textSize = ImGui::CalcTextSize(it->message.c_str());
        float xOffset = io.DisplaySize.x - notificationWidth - 20.0f;
        ImVec2 pos(xOffset, yOffset);
        ImVec2 size(notificationWidth, textSize.y + 20.0f);

        // Draw background rect with rounding
        drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, 8.0f);
        drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), borderColor, 8.0f, ImDrawFlags_RoundCornersAll, 1.5f);

        // Center text vertically and horizontally
        ImVec2 textPos(pos.x + (size.x - textSize.x) / 2.0f, pos.y + (size.y - textSize.y) / 2.0f);
        drawList->AddText(textPos, textColor, it->message.c_str());

        yOffset -= size.y + 10.0f; // Stack upwards with spacing
        ++it;
    }
}

void SaveConfig() {
    std::ofstream file("C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\config.txt");
    if (file.is_open()) {
        file << "espEnabled=" << espEnabled << "\n";
        file << "aimbotEnabled=" << aimbotEnabled << "\n";
        file << "healthBarEnabled=" << healthBarEnabled << "\n";
        file << "snaplinesEnabled=" << snaplinesEnabled << "\n";
        file << "distanceEnabled=" << distanceEnabled << "\n";
        file << "boneESPEnabled=" << boneESPEnabled << "\n";
        file << "headCircleEnabled=" << headCircleEnabled << "\n";
        file << "showTeammates=" << showTeammates << "\n";
        file << "box3DEnabled=" << box3DEnabled << "\n";
        file << "glowESPEnabled=" << glowESPEnabled << "\n";
        file << "ammoEnabled=" << ammoEnabled << "\n";
        file << "radarEnabled=" << radarEnabled << "\n";
        file << "healthTextEnabled=" << healthTextEnabled << "\n";
        file << "armorEnabled=" << armorEnabled << "\n";
        file << "fovCircleEnabled=" << fovCircleEnabled << "\n";
        file << "flagsEnabled=" << flagsEnabled << "\n";
        file << "tracersEnabled=" << tracersEnabled << "\n";
        file << "noFlashEnabled=" << noFlashEnabled << "\n";
        file << "revealAllEnabled=" << revealAllEnabled << "\n";
        file << "infiniteArmorEnabled=" << infiniteArmorEnabled << "\n";
        file << "hitsoundEnabled=" << hitsoundEnabled << "\n"; // New
        file << "outlineEnabled=" << outlineEnabled << "\n"; // New
        file << "boxRounding=" << boxRounding << "\n"; // New
        file << "boxThickness=" << boxThickness << "\n";
        file << "skeletonThickness=" << skeletonThickness << "\n";
        file << "snaplineThickness=" << snaplineThickness << "\n";
        file << "tracerThickness=" << tracerThickness << "\n";
        file << "headCircleThickness=" << headCircleThickness << "\n";
        file << "glowOpacity=" << glowOpacity << "\n";
        file << "aimbotFOV=" << aimbotFOV << "\n";
        file << "aimbotSmoothing=" << aimbotSmoothing << "\n";
        file << "aimOnVisibleOnly=" << aimOnVisibleOnly << "\n";
        file << "aimTeammates=" << aimTeammates << "\n";
        file << "aimKey=" << aimKey << "\n";
        file << "espColor=" << espColor.x << "," << espColor.y << "," << espColor.z << "," << espColor.w << "\n";
        file << "teammateColor=" << teammateColor.x << "," << teammateColor.y << "," << teammateColor.z << "," << teammateColor.w << "\n";
        file << "visibleColor=" << visibleColor.x << "," << visibleColor.y << "," << visibleColor.z << "," << visibleColor.w << "\n";
        file << "skeletonColor=" << skeletonColor.x << "," << skeletonColor.y << "," << skeletonColor.z << "," << skeletonColor.w << "\n";
        file << "snaplineColor=" << snaplineColor.x << "," << snaplineColor.y << "," << snaplineColor.z << "," << snaplineColor.w << "\n";
        file << "tracerColor=" << tracerColor.x << "," << tracerColor.y << "," << tracerColor.z << "," << tracerColor.w << "\n";
        file << "headCircleColor=" << headCircleColor.x << "," << headCircleColor.y << "," << headCircleColor.z << "," << headCircleColor.w << "\n";
        file << "glowColor=" << glowColor.x << "," << glowColor.y << "," << glowColor.z << "," << glowColor.w << "\n";
        file << "fovCircleColor=" << fovCircleColor.x << "," << fovCircleColor.y << "," << fovCircleColor.z << "," << fovCircleColor.w << "\n";
        file << "hitSoundPath=" << hitSoundPath << "\n";
        file << "headSoundPath=" << headSoundPath << "\n";
        file << "killSoundPath=" << killSoundPath << "\n";
        file << "soundVolume=" << soundVolume << "\n";
        file << "noRecoilEnabled=" << noRecoilEnabled << "\n";
        file << "bunnyhopEnabled=" << bunnyhopEnabled << "\n";
        file << "triggerbotEnabled=" << triggerbotEnabled << "\n";
        file << "triggerbotDelay=" << triggerbotDelay << "\n";
        file << "showSpectatorList=" << showSpectatorList << "\n"; // NEW
        file.close();
        debugLog += "[INFO] Configuration saved\n";
        AddNotification("Configuration Saved", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // New: Notification on save
    }
    else {
        debugLog += "[ERROR] Failed to save config\n";
        AddNotification("Failed to Save Config", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // New: Error notification
    }
}

void LoadConfig() {
    std::ifstream file("C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\config.txt");
    if (!file.is_open()) {
        debugLog += "[ERROR] Failed to open config.txt\n";
        AddNotification("Failed to Load Config", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // New: Error notification
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("=");
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        try {
            if (key == "espEnabled") espEnabled = std::stoi(value) != 0;
            else if (key == "aimbotEnabled") aimbotEnabled = std::stoi(value) != 0;
            else if (key == "healthBarEnabled") healthBarEnabled = std::stoi(value) != 0;
            else if (key == "snaplinesEnabled") snaplinesEnabled = std::stoi(value) != 0;
            else if (key == "distanceEnabled") distanceEnabled = std::stoi(value) != 0;
            else if (key == "boneESPEnabled") boneESPEnabled = std::stoi(value) != 0;
            else if (key == "headCircleEnabled") headCircleEnabled = std::stoi(value) != 0;
            else if (key == "showTeammates") showTeammates = std::stoi(value) != 0;
            else if (key == "box3DEnabled") box3DEnabled = std::stoi(value) != 0;
            else if (key == "glowESPEnabled") glowESPEnabled = std::stoi(value) != 0;
            else if (key == "ammoEnabled") ammoEnabled = std::stoi(value) != 0;
            else if (key == "radarEnabled") radarEnabled = std::stoi(value) != 0;
            else if (key == "healthTextEnabled") healthTextEnabled = std::stoi(value) != 0;
            else if (key == "armorEnabled") armorEnabled = std::stoi(value) != 0;
            else if (key == "fovCircleEnabled") fovCircleEnabled = std::stoi(value) != 0;
            else if (key == "flagsEnabled") flagsEnabled = std::stoi(value) != 0;
            else if (key == "tracersEnabled") tracersEnabled = std::stoi(value) != 0;
            else if (key == "noFlashEnabled") noFlashEnabled = std::stoi(value) != 0;
            else if (key == "revealAllEnabled") revealAllEnabled = std::stoi(value) != 0;
            else if (key == "infiniteArmorEnabled") infiniteArmorEnabled = std::stoi(value) != 0;
            else if (key == "hitsoundEnabled") hitsoundEnabled = std::stoi(value) != 0; // New
            else if (key == "outlineEnabled") outlineEnabled = std::stoi(value) != 0; // New
            else if (key == "boxRounding") boxRounding = std::stof(value); // New
            else if (key == "boxThickness") boxThickness = std::stof(value);
            else if (key == "skeletonThickness") skeletonThickness = std::stof(value);
            else if (key == "snaplineThickness") snaplineThickness = std::stof(value);
            else if (key == "tracerThickness") tracerThickness = std::stof(value);
            else if (key == "headCircleThickness") headCircleThickness = std::stof(value);
            else if (key == "glowOpacity") glowOpacity = std::stof(value);
            else if (key == "aimbotFOV") aimbotFOV = std::stof(value);
            else if (key == "aimbotSmoothing") aimbotSmoothing = std::stof(value);
            else if (key == "aimOnVisibleOnly") aimOnVisibleOnly = std::stoi(value) != 0;
            else if (key == "aimTeammates") aimTeammates = std::stoi(value) != 0;
            else if (key == "aimKey") aimKey = std::stoi(value);
            else if (key == "espColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &espColor.x, &espColor.y, &espColor.z, &espColor.w);
            }
            else if (key == "teammateColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &teammateColor.x, &teammateColor.y, &teammateColor.z, &teammateColor.w);
            }
            else if (key == "visibleColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &visibleColor.x, &visibleColor.y, &visibleColor.z, &visibleColor.w);
            }
            else if (key == "skeletonColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &skeletonColor.x, &skeletonColor.y, &skeletonColor.z, &skeletonColor.w);
            }
            else if (key == "snaplineColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &snaplineColor.x, &snaplineColor.y, &snaplineColor.z, &snaplineColor.w);
            }
            else if (key == "tracerColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &tracerColor.x, &tracerColor.y, &tracerColor.z, &tracerColor.w);
            }
            else if (key == "headCircleColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &headCircleColor.x, &headCircleColor.y, &headCircleColor.z, &headCircleColor.w);
            }
            else if (key == "glowColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &glowColor.x, &glowColor.y, &glowColor.z, &glowColor.w);
            }
            else if (key == "fovCircleColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &fovCircleColor.x, &fovCircleColor.y, &fovCircleColor.z, &fovCircleColor.w);
            }
            else if (key == "hitSoundPath") hitSoundPath = value;
            else if (key == "headSoundPath") headSoundPath = value;
            else if (key == "killSoundPath") killSoundPath = value;
            else if (key == "soundVolume=") soundVolume = std::stof(value);
            if (key == "noRecoilEnabled") noRecoilEnabled = std::stoi(value);
            else if (key == "bunnyhopEnabled") bunnyhopEnabled = std::stoi(value);
            else if (key == "triggerbotEnabled") triggerbotEnabled = std::stoi(value);
            else if (key == "triggerbotDelay") triggerbotDelay = std::stof(value);
            else if (key == "showSpectatorList") showSpectatorList = std::stoi(value); // Assuming bool as 0/1
        }
        catch (const std::exception& e) {
            debugLog += "[ERROR] Failed to parse config for " + key + ": " + e.what() + "\n";
            AddNotification("Config Parse Error: " + key, 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // New: Error notification
        }
    }

    file.close();
    debugLog += "[INFO] Configuration loaded\n";
    AddNotification("Configuration Loaded", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // New: Notification on load
}

void SetupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(15, 15); // Increased padding for better spacing
    style.FramePadding = ImVec2(12, 8); // Larger frame padding
    style.ItemSpacing = ImVec2(12, 10); // More space between items
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 16.0f;
    style.GrabMinSize = 14.0f;
    style.WindowBorderSize = 1.5f; // Thicker border
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.WindowRounding = 12.0f; // Softer rounding
    style.FrameRounding = 10.0f;
    style.PopupRounding = 10.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabRounding = 10.0f;
    style.TabRounding = 10.0f;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // Center title
    style.SeparatorTextBorderSize = 4.0f;
    style.SeparatorTextPadding = ImVec2(25.0f, 10.0f);
    style.DisplaySafeAreaPadding = ImVec2(5, 5);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.98f); // Darker background
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.32f, 0.70f); // Softer border
    colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.17f, 0.70f); // Lighter frames
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.27f, 0.60f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.32f, 0.80f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.12f, 0.14f, 0.80f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.60f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.27f, 0.60f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.37f, 0.90f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.42f, 1.0f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.90f, 0.40f, 1.0f); // Brighter checkmark
    colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.90f, 0.40f, 1.0f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 1.0f, 0.50f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 0.60f); // Darker buttons
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.32f, 0.90f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.37f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.22f, 0.50f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.32f, 0.90f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.37f, 1.0f);
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.32f, 0.70f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.42f, 0.90f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.45f, 0.45f, 0.47f, 1.0f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.90f, 0.40f, 0.30f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 1.0f, 0.50f, 0.80f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.30f, 1.0f, 0.55f, 1.0f);
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.17f, 0.90f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.32f, 0.95f);
    colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.27f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.14f, 0.98f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.0f); // Brighter text
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.45f, 0.37f, 1.0f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.92f, 0.72f, 0.0f, 1.0f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.62f, 0.0f, 1.0f);
}

void RenderMenu() {
    if (!menuOpen) return;

    static bool styleSet = false;
    if (!styleSet) {
        SetupImGuiStyle();
        styleSet = true;
    }

    ImGui::SetNextWindowSize(ImVec2(650, 650), ImGuiCond_FirstUseEver); // Slightly larger default size
    ImGui::Begin("CS2 WoW Cheat - v1.0", &menuOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    // Enhanced Menu Bar with more icons and gradient effect (simulated with colors)
    if (ImGui::BeginMenuBar()) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.12f, 0.12f, 0.14f, 0.95f));
        if (ImGui::BeginMenu("\uf15b File")) { // FontAwesome icon
            if (ImGui::MenuItem("\uf0c7 Save Config", "Ctrl+S")) { // Icon for save
                SaveConfig();
            }
            if (ImGui::MenuItem("\uf019 Load Config", "Ctrl+L")) { // Icon for load
                LoadConfig();
            }
            if (ImGui::MenuItem("\uf2f5 Unload Cheat", "Ctrl+U")) { // Icon for unload
                FreeLibraryAndExitThread((HMODULE)GetModuleHandle(nullptr), 0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("\uf06e View")) {
            if (ImGui::MenuItem("\uf188 Toggle Debug Console", "Ctrl+D", &showDebugConsole)) { // Icon for debug
                // Toggle debug console
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("\uf059 Help")) {
            ImGui::MenuItem("About: CS2 WoW Cheat v1.0 - Enhanced Edition");
            ImGui::EndMenu();
        }
        ImGui::PopStyleColor();
        ImGui::EndMenuBar();
    }

    // Sidebar with icons and better spacing
    ImGui::BeginChild("Sidebar", ImVec2(200, 0), true, ImGuiWindowFlags_NoScrollbar); // Wider sidebar

    if (navFont) ImGui::PushFont(navFont);

    ImGui::TextColored(ImVec4(0.20f, 0.90f, 0.40f, 1.0f), "\uf013 Navigation"); // Icon for navigation
    ImGui::Separator();
    static int selectedTab = 0;
    if (ImGui::Selectable("\uf06e Visuals", selectedTab == 0)) selectedTab = 0; // Icon
    if (ImGui::Selectable("\uf05b Aimbot", selectedTab == 1)) selectedTab = 1; // Icon
    if (ImGui::Selectable("\uf085 Misc", selectedTab == 2)) selectedTab = 2; // Icon
    if (ImGui::Selectable("\uf188 Debug", selectedTab == 3)) selectedTab = 3; // Icon
    
    ImGui::PopFont();

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("MainContent", ImVec2(0, -40), true); // Adjusted for larger status bar

    if (selectedTab == 0) { // Visuals
        ImGui::TextColored(ImVec4(0.20f, 0.90f, 0.40f, 1.0f), "\uf06e Visuals Configuration");
        ImGui::SeparatorText("General Settings");
        ImGui::Checkbox("Enable ESP", &espEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle all ESP features.");
        ImGui::Checkbox("Show Teammates", &showTeammates);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Display ESP for teammates.");
        ImGui::Checkbox("Enable Outlines", &outlineEnabled); // New
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add black outlines to all ESP features for better visibility.");

        if (ImGui::CollapsingHeader("Box ESP")) {
            ImGui::Checkbox("3D Box", &box3DEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Use 3D bounding boxes instead of 2D.");
            ImGui::SliderFloat("Box Rounding", &boxRounding, 0.0f, 12.0f, "%.1f"); // New
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust corner rounding for 2D boxes (0 for sharp corners).");
            ImGui::SliderFloat("Box Thickness", &boxThickness, 0.5f, 6.0f, "%.1f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust thickness of box ESP lines.");
            ImGui::ColorEdit4("Enemy Box Color", (float*)&espColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for enemy boxes (non-visible).");
            ImGui::ColorEdit4("Visible Enemy Color", (float*)&visibleColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for visible enemies.");
            ImGui::ColorEdit4("Teammate Box Color", (float*)&teammateColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for teammate boxes.");
        }

        if (ImGui::CollapsingHeader("Glow ESP")) {
            ImGui::Checkbox("Enable Glow", &glowESPEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add a glow effect around entities.");
            ImGui::SliderFloat("Glow Opacity", &glowOpacity, 0.1f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust opacity of glow effect.");
            ImGui::ColorEdit4("Glow Color", (float*)&glowColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color and opacity for glow effect.");
        }

        if (ImGui::CollapsingHeader("Skeleton ESP")) {
            ImGui::Checkbox("Enable Skeleton", &boneESPEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Draw player skeletons.");
            ImGui::SliderFloat("Skeleton Thickness", &skeletonThickness, 0.5f, 6.0f, "%.1f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust thickness of skeleton lines.");
            ImGui::ColorEdit4("Skeleton Color", (float*)&skeletonColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for skeleton ESP.");
        }

        if (ImGui::CollapsingHeader("Head Circle")) {
            ImGui::Checkbox("Enable Head Circle", &headCircleEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Draw circles around player heads.");
            ImGui::SliderFloat("Head Circle Thickness", &headCircleThickness, 0.5f, 6.0f, "%.1f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust thickness of head circles.");
            ImGui::ColorEdit4("Head Circle Color", (float*)&headCircleColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for head circle ESP.");
        }

        if (ImGui::CollapsingHeader("Snaplines")) {
            ImGui::Checkbox("Enable Snaplines", &snaplinesEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Draw lines from screen bottom to entities.");
            ImGui::SliderFloat("Snapline Thickness", &snaplineThickness, 0.5f, 6.0f, "%.1f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust thickness of snaplines.");
            ImGui::ColorEdit4("Snapline Color", (float*)&snaplineColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for snaplines.");
        }

        if (ImGui::CollapsingHeader("Tracers")) {
            ImGui::Checkbox("Enable Tracers", &tracersEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Draw tracers to enemy heads from crosshair.");
            ImGui::SliderFloat("Tracer Thickness", &tracerThickness, 0.5f, 6.0f, "%.1f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adjust thickness of tracers.");
            ImGui::ColorEdit4("Tracer Color", (float*)&tracerColor, ImGuiColorEditFlags_AlphaPreview);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for tracers.");
        }

        if (ImGui::CollapsingHeader("Info Display")) {
            ImGui::Checkbox("Show Distance", &distanceEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Display distance to entities.");
            ImGui::Checkbox("Health Bar", &healthBarEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show health bars next to entities.");
            ImGui::Checkbox("Health Text", &healthTextEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show health as text.");
            ImGui::Checkbox("Armor Display", &armorEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show armor bars next to entities.");
            ImGui::Checkbox("Weapon & Ammo", &ammoEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Display weapon and ammo count.");
            ImGui::Checkbox("Player Flags", &flagsEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show flags like helmet, kit, etc.");
        }

        if (ImGui::CollapsingHeader("Radar")) {
            ImGui::Checkbox("Enable Radar", &radarEnabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show mini-radar in top-right corner.");
        }
    }
    else if (selectedTab == 1) { // Aimbot
        ImGui::TextColored(ImVec4(0.20f, 0.90f, 0.40f, 1.0f), "\uf05b Aimbot Configuration");
        ImGui::SeparatorText("Aimbot Settings");
        ImGui::Checkbox("Enable Aimbot", &aimbotEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle aimbot functionality.");
        ImGui::SliderFloat("Aimbot FOV (degrees)", &aimbotFOV, 1.0f, 90.0f, "%.1f");
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Set the aimbot field of view.");
        ImGui::SliderFloat("Aimbot Smoothing", &aimbotSmoothing, 1.0f, 20.0f, "%.1f");
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Higher values make aimbot smoother (less snappy).");
        ImGui::Checkbox("Aim on Visible Only", &aimOnVisibleOnly);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Only aim at visible enemies.");
        ImGui::Checkbox("Aim Teammates", &aimTeammates);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Allow aimbot to target teammates.");
        ImGui::Checkbox("Show FOV Circle", &fovCircleEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Display aimbot FOV circle on screen.");
        ImGui::ColorEdit4("FOV Circle Color", (float*)&fovCircleColor, ImGuiColorEditFlags_AlphaPreview);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Color for aimbot FOV circle.");
    }
    else if (selectedTab == 2) { // Misc
        ImGui::TextColored(ImVec4(0.20f, 0.90f, 0.40f, 1.0f), "\uf085 Miscellaneous Features");
        ImGui::SeparatorText("Misc Options");
        ImGui::Checkbox("No Flash", &noFlashEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Remove flashbang effects.");
        ImGui::Checkbox("Reveal All Enemies", &revealAllEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Force all enemies to be spotted on radar.");
        ImGui::Checkbox("Infinite Armor", &infiniteArmorEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Set local player armor to maximum every frame.");
        ImGui::Checkbox("Hitsound", &hitsoundEnabled); // New
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Play a sound when hitting an enemy.");
        static char hitBuf[256] = { 0 };
        static char headBuf[256] = { 0 };
        static char killBuf[256] = { 0 };
        static bool initBufs = true;
        if (initBufs) {
            strcpy_s(hitBuf, hitSoundPath.c_str());
            strcpy_s(headBuf, headSoundPath.c_str());
            strcpy_s(killBuf, killSoundPath.c_str());
            initBufs = false;
        }
        ImGui::InputText("Hit Sound Path", hitBuf, sizeof(hitBuf));
        hitSoundPath = hitBuf;
        ImGui::InputText("Headshot Sound Path", headBuf, sizeof(headBuf));
        headSoundPath = headBuf;
        ImGui::InputText("Kill Sound Path", killBuf, sizeof(killBuf));
        killSoundPath = killBuf;
        ImGui::SliderFloat("Sound Volume", &soundVolume, 0.0f, 1000.0f, "%.0f");

        ImGui::SeparatorText("Config Management");
        if (ImGui::Button("Save Configuration", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, 0))) {
            SaveConfig();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Configuration", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            LoadConfig();
        }

        ImGui::SeparatorText("New Features");
        ImGui::Checkbox("No Recoil", &noRecoilEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Removes visual recoil.");

        ImGui::Checkbox("Bunnyhop", &bunnyhopEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Auto-jump when holding space.");

        ImGui::Checkbox("Triggerbot", &triggerbotEnabled);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Auto-shoot when crosshair on enemy.");
        ImGui::SliderFloat("Trigger Delay (ms)", &triggerbotDelay, 0.0f, 100.0f, "%.0f");
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Random delay for humanization.");
        ImGui::Checkbox("Show Spectator List", &showSpectatorList); // NEW: Toggle for separate window

        ImGui::Spacing();
        if (ImGui::Button("Reset to Defaults", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            espEnabled = true;
            aimbotEnabled = true;
            healthBarEnabled = true;
            snaplinesEnabled = true;
            distanceEnabled = true;
            boneESPEnabled = true;
            headCircleEnabled = true;
            showTeammates = false;
            box3DEnabled = false;
            glowESPEnabled = true;
            ammoEnabled = true;
            radarEnabled = true;
            healthTextEnabled = true;
            armorEnabled = true;
            fovCircleEnabled = true;
            flagsEnabled = true;
            tracersEnabled = true;
            noFlashEnabled = false;
            revealAllEnabled = false;
            infiniteArmorEnabled = false;
            hitsoundEnabled = true;
            outlineEnabled = true; // New
            noRecoilEnabled = false;
            bunnyhopEnabled = false;
            triggerbotEnabled = false;
            triggerbotDelay = 20.0f;
            boxRounding = 4.0f; // New
            boxThickness = 1.5f;
            skeletonThickness = 1.2f;
            snaplineThickness = 1.0f;
            tracerThickness = 0.8f;
            headCircleThickness = 1.3f;
            glowOpacity = 0.5f;
            aimbotFOV = 10.0f;
            espColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            teammateColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
            visibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            skeletonColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            snaplineColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
            tracerColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
            headCircleColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
            glowColor = ImVec4(0.5f, 0.0f, 0.5f, 0.5f);
            fovCircleColor = ImVec4(1.0f, 1.0f, 1.0f, 0.4f);
            hitSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\hitsound.mp3";
            headSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\headsound.mp3";
            killSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\killsound.mp3";
            soundVolume = 1000.0f;
            debugLog += "[INFO] Configuration reset to defaults\n";
            AddNotification("Reset to Defaults", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // New: Notification
        }

        
    }
    else if (selectedTab == 3) { // Debug
        ImGui::TextColored(ImVec4(0.20f, 0.90f, 0.40f, 1.0f), "\uf188 Debug Information");
        ImGui::SeparatorText("Debug Console");
        ImGui::BeginChild("DebugConsole", ImVec2(0, 200), true); // Larger console
        ImGui::TextWrapped("%s", debugLog.c_str());
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        if (ImGui::Button("Clear Console")) {
            debugLog = "";
        }
    }

    

    ImGui::EndChild();

    // Enhanced Status bar with icons
    ImGui::BeginChild("StatusBar", ImVec2(0, 40), true);
    char statusText[128];
    sprintf_s(statusText, "\uf017 FPS: %.1f | \uf007 Entities: %d | \uf017 Time: %s", ImGui::GetIO().Framerate, GetEntities().size(), __TIME__); // Icons
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "%s", statusText);
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    if (ImGui::Button("\uf2f5 Unload", ImVec2(80, 0))) { // Icon for unload
        FreeLibraryAndExitThread((HMODULE)GetModuleHandle(nullptr), 0);
    }
    ImGui::EndChild();

    ImGui::End();
    debugLog += "[DEBUG] RenderMenu called\n";
    std::cout << "[DEBUG] RenderMenu called" << std::endl;
}