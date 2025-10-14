#include "cheat.h"

ImVec4 soundESPColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
ImVec4 backtrackESPColor = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);
ImVec4 velocityIndicatorColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
ImVec4 advancedFlagsColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

void AddNotification(const std::string& msg, float duration, ImVec4 color) {
    notifications.push_back(Notification(msg, ImGui::GetTime(), duration, color));
}

std::string GetKeyName(int key) {
    switch (key) {
    case VK_LBUTTON: return "Left Mouse";
    case VK_RBUTTON: return "Right Mouse";
    case VK_MBUTTON: return "Middle Mouse";
    case VK_XBUTTON1: return "Mouse 4";
    case VK_XBUTTON2: return "Mouse 5";
    case VK_SHIFT: return "Shift";
    case VK_CONTROL: return "Ctrl";
    case VK_MENU: return "Alt";
    case VK_SPACE: return "Space";
    case VK_CAPITAL: return "Caps Lock";
    case VK_TAB: return "Tab";
    default: return "Key " + std::to_string(key);
    }
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

bool ColorPicker(const char* label, ImVec4& color) {
    bool changed = false;

    // Display current color as a colored button that opens the picker
    ImGui::PushID(label);
    ImGui::ColorButton("##ColorBtn", color, ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("%s", label);

    // Simple color picker that always shows
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    changed = ImGui::ColorEdit4("##Picker", (float*)&color,
        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);

    ImGui::PopID();
    return changed;
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
        file << "hitsoundEnabled=" << hitsoundEnabled << "\n";
        file << "outlineEnabled=" << outlineEnabled << "\n";
        file << "boxRounding=" << boxRounding << "\n";
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
        file << "showSpectatorList=" << showSpectatorList << "\n";

        // New color variables
        file << "weaponColor=" << weaponColor.x << "," << weaponColor.y << "," << weaponColor.z << "," << weaponColor.w << "\n";
        file << "soundESPColor=" << soundESPColor.x << "," << soundESPColor.y << "," << soundESPColor.z << "," << soundESPColor.w << "\n";
        file << "backtrackESPColor=" << backtrackESPColor.x << "," << backtrackESPColor.y << "," << backtrackESPColor.z << "," << backtrackESPColor.w << "\n";
        file << "velocityIndicatorColor=" << velocityIndicatorColor.x << "," << velocityIndicatorColor.y << "," << velocityIndicatorColor.z << "," << velocityIndicatorColor.w << "\n";
        file << "advancedFlagsColor=" << advancedFlagsColor.x << "," << advancedFlagsColor.y << "," << advancedFlagsColor.z << "," << advancedFlagsColor.w << "\n";
        file << "chamsEnemyColor=" << chamsEnemyColor.x << "," << chamsEnemyColor.y << "," << chamsEnemyColor.z << "," << chamsEnemyColor.w << "\n";
        file << "chamsTeammateColor=" << chamsTeammateColor.x << "," << chamsTeammateColor.y << "," << chamsTeammateColor.z << "," << chamsTeammateColor.w << "\n";
        file << "chamsVisibleColor=" << chamsVisibleColor.x << "," << chamsVisibleColor.y << "," << chamsVisibleColor.z << "," << chamsVisibleColor.w << "\n";

        file.close();
        debugLog += "[INFO] Configuration saved\n";
        AddNotification("Configuration Saved", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
    }
    else {
        debugLog += "[ERROR] Failed to save config\n";
        AddNotification("Failed to Save Config", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    }
}

void LoadConfig() {
    std::ifstream file("C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\config.txt");
    if (!file.is_open()) {
        debugLog += "[ERROR] Failed to open config.txt\n";
        AddNotification("Failed to Load Config", 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
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
            else if (key == "hitsoundEnabled") hitsoundEnabled = std::stoi(value) != 0;
            else if (key == "outlineEnabled") outlineEnabled = std::stoi(value) != 0;
            else if (key == "boxRounding") boxRounding = std::stof(value);
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
            else if (key == "soundVolume") soundVolume = std::stof(value);
            else if (key == "noRecoilEnabled") noRecoilEnabled = std::stoi(value);
            else if (key == "bunnyhopEnabled") bunnyhopEnabled = std::stoi(value);
            else if (key == "triggerbotEnabled") triggerbotEnabled = std::stoi(value);
            else if (key == "triggerbotDelay") triggerbotDelay = std::stof(value);
            else if (key == "showSpectatorList") showSpectatorList = std::stoi(value);

            // New color variables
            else if (key == "weaponColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &weaponColor.x, &weaponColor.y, &weaponColor.z, &weaponColor.w);
            }
            else if (key == "soundESPColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &soundESPColor.x, &soundESPColor.y, &soundESPColor.z, &soundESPColor.w);
            }
            else if (key == "backtrackESPColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &backtrackESPColor.x, &backtrackESPColor.y, &backtrackESPColor.z, &backtrackESPColor.w);
            }
            else if (key == "velocityIndicatorColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &velocityIndicatorColor.x, &velocityIndicatorColor.y, &velocityIndicatorColor.z, &velocityIndicatorColor.w);
            }
            else if (key == "advancedFlagsColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &advancedFlagsColor.x, &advancedFlagsColor.y, &advancedFlagsColor.z, &advancedFlagsColor.w);
            }
            else if (key == "chamsEnemyColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &chamsEnemyColor.x, &chamsEnemyColor.y, &chamsEnemyColor.z, &chamsEnemyColor.w);
            }
            else if (key == "chamsTeammateColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &chamsTeammateColor.x, &chamsTeammateColor.y, &chamsTeammateColor.z, &chamsTeammateColor.w);
            }
            else if (key == "chamsVisibleColor") {
                sscanf_s(value.c_str(), "%f,%f,%f,%f", &chamsVisibleColor.x, &chamsVisibleColor.y, &chamsVisibleColor.z, &chamsVisibleColor.w);
            }
        }
        catch (const std::exception& e) {
            debugLog += "[ERROR] Failed to parse config for " + key + ": " + e.what() + "\n";
            AddNotification("Config Parse Error: " + key, 3.0f, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        }
    }

    file.close();
    debugLog += "[INFO] Configuration loaded\n";
    AddNotification("Configuration Loaded", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
}

void SetupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Modern, clean style with better spacing
    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;

    // Clean borders
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;

    // Subtle rounding
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;

    // Modern color scheme
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.60f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.15f, 0.70f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.22f, 0.70f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.27f, 0.70f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.85f, 0.45f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.95f, 0.55f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.25f, 0.70f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.30f, 0.90f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.22f, 0.70f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.22f, 0.27f, 0.90f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.25f, 0.70f);
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
}

void RenderVisualsTab() {
    ImGui::Columns(2, "VisualsColumns", true);

    // Left column - ESP Features
    ImGui::BeginChild("ESP Features", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "ESP Features");
    ImGui::Separator();

    // Basic ESP
    ImGui::Checkbox("Enable ESP", &espEnabled);
    ImGui::Checkbox("Show Teammates", &showTeammates);
    ImGui::Checkbox("Enable Outlines", &outlineEnabled);

    ImGui::Spacing();

    // Box ESP with inline customization
    ImGui::Checkbox("Box ESP", &box3DEnabled);
    if (ImGui::CollapsingHeader("Box Settings##BoxESP")) {
        ImGui::Indent();
        ImGui::SliderFloat("Thickness##Box", &boxThickness, 0.5f, 5.0f, "%.1f");
        ImGui::SliderFloat("Rounding##Box", &boxRounding, 0.0f, 10.0f, "%.1f");
        ColorPicker("Box Color##Box", espColor);
        ImGui::Unindent();
    }

    // Skeleton with inline customization
    ImGui::Checkbox("Skeleton", &boneESPEnabled);
    if (ImGui::CollapsingHeader("Skeleton Settings##Skeleton")) {
        ImGui::Indent();
        ImGui::SliderFloat("Thickness##Skeleton", &skeletonThickness, 0.5f, 5.0f, "%.1f");
        ColorPicker("Skeleton Color##Skeleton", skeletonColor);
        ImGui::Unindent();
    }

    // Head Circle with inline customization
    ImGui::Checkbox("Head Circle", &headCircleEnabled);
    if (ImGui::CollapsingHeader("Head Circle Settings##HeadCircle")) {
        ImGui::Indent();
        ImGui::SliderFloat("Thickness##HeadCircle", &headCircleThickness, 0.5f, 5.0f, "%.1f");
        ColorPicker("Head Circle Color##HeadCircle", headCircleColor);
        ImGui::Unindent();
    }

    // Snaplines with inline customization
    ImGui::Checkbox("Snaplines", &snaplinesEnabled);
    if (ImGui::CollapsingHeader("Snaplines Settings##Snaplines")) {
        ImGui::Indent();
        ImGui::SliderFloat("Thickness##Snaplines", &snaplineThickness, 0.5f, 5.0f, "%.1f");
        ColorPicker("Snapline Color##Snaplines", snaplineColor);
        ImGui::Unindent();
    }

    // Tracers with inline customization
    ImGui::Checkbox("Tracers", &tracersEnabled);
    if (ImGui::CollapsingHeader("Tracers Settings##Tracers")) {
        ImGui::Indent();
        ImGui::SliderFloat("Thickness##Tracers", &tracerThickness, 0.5f, 5.0f, "%.1f");
        ColorPicker("Tracer Color##Tracers", tracerColor);
        ImGui::Unindent();
    }

    ImGui::Spacing();
    ImGui::Checkbox("Health Bar", &healthBarEnabled);
    ImGui::Checkbox("Armor Bar", &armorEnabled);
    ImGui::Checkbox("Distance", &distanceEnabled);
    ImGui::Checkbox("Weapon Info", &ammoEnabled);
    ImGui::Checkbox("Player Flags", &flagsEnabled);

    // NEW FEATURES with inline customization
    ImGui::Spacing();

    // Weapon ESP with inline customization
    ImGui::Checkbox("Weapon ESP", &weaponESPEnabled);
    if (ImGui::CollapsingHeader("Weapon ESP Settings##WeaponESP")) {
        ImGui::Indent();
        ImGui::SliderFloat("Max Distance##Weapon", &weaponMaxDistance, 500.0f, 5000.0f, "%.0f units");
        ColorPicker("Weapon Color##Weapon", weaponColor);
        ImGui::Unindent();
    }

    // Sound ESP with inline customization
    ImGui::Checkbox("Sound ESP", &soundESPEnabled);
    if (ImGui::CollapsingHeader("Sound ESP Settings##SoundESP")) {
        ImGui::Indent();
        ImGui::SliderFloat("Display Time##Sound", &soundMaxTime, 1.0f, 10.0f, "%.1f s");
        ColorPicker("Sound Color##Sound", soundESPColor);
        ImGui::Unindent();
    }

    // Backtrack ESP with inline customization
    ImGui::Checkbox("Backtrack ESP", &backtrackESPEnabled);
    if (ImGui::CollapsingHeader("Backtrack ESP Settings##BacktrackESP")) {
        ImGui::Indent();
        ImGui::SliderFloat("Duration##Backtrack", &backtrackDuration, 0.1f, 1.0f, "%.1f s");
        ColorPicker("Backtrack Color##Backtrack", backtrackESPColor);
        ImGui::Unindent();
    }

    // Velocity ESP with inline customization
    ImGui::Checkbox("Velocity ESP", &velocityESPEnabled);
    if (ImGui::CollapsingHeader("Velocity ESP Settings##VelocityESP")) {
        ImGui::Indent();
        ColorPicker("Base Color##Velocity", velocityIndicatorColor);
        ImGui::TextDisabled("Colors change based on speed");
        ImGui::TextDisabled("Green = Slow, Yellow = Medium, Red = Fast");
        ImGui::Unindent();
    }

    // Advanced Flags with inline customization
    ImGui::Checkbox("Advanced Flags", &showAdvancedFlags);
    if (ImGui::CollapsingHeader("Advanced Flags Settings##AdvancedFlags")) {
        ImGui::Indent();
        ColorPicker("Flags Color##AdvancedFlags", advancedFlagsColor);
        ImGui::Unindent();
    }

    // Additional features with inline customization
    ImGui::Spacing();

    // Glow ESP with inline customization
    ImGui::Checkbox("Glow ESP", &glowESPEnabled);
    if (ImGui::CollapsingHeader("Glow ESP Settings##GlowESP")) {
        ImGui::Indent();
        ImGui::SliderFloat("Max Distance##Glow", &glowMaxDistance, 500.0f, 10000.0f, "%.0f units");
        ImGui::SliderFloat("Opacity##Glow", &glowOpacity, 0.1f, 1.0f, "%.1f");
        ImGui::Checkbox("Glow Through Walls##Glow", &glowThroughWalls);
        ColorPicker("Glow Color##Glow", glowColor);
        ImGui::Unindent();
    }

    // Radar with inline customization
    ImGui::Checkbox("Radar", &radarEnabled);
    if (ImGui::CollapsingHeader("Radar Settings##Radar")) {
        ImGui::Indent();
        ImGui::SliderFloat("Radar Size##Radar", &glowMaxDistance, 100.0f, 300.0f, "%.0f");
        ColorPicker("Enemy Color##Radar", espColor);
        ColorPicker("Teammate Color##Radar", teammateColor);
        ImGui::Unindent();
    }

    // Crosshair with inline customization
    ImGui::Checkbox("Custom Crosshair", &customCrosshairEnabled);
    if (ImGui::CollapsingHeader("Crosshair Settings##Crosshair")) {
        ImGui::Indent();
        ImGui::SliderFloat("Size##Crosshair", &crosshairSize, 4.0f, 20.0f, "%.1f");
        ImGui::SliderFloat("Thickness##Crosshair", &crosshairThickness, 1.0f, 5.0f, "%.1f");
        ColorPicker("Crosshair Color##Crosshair", crosshairColor);
        ImGui::Unindent();
    }

    // FOV Circle with inline customization
    ImGui::Checkbox("FOV Circle", &fovCircleEnabled);
    if (ImGui::CollapsingHeader("FOV Circle Settings##FOVCircle")) {
        ImGui::Indent();
        ImGui::SliderFloat("Circle Thickness##FOV", &boxThickness, 0.5f, 5.0f, "%.1f");
        ColorPicker("FOV Circle Color##FOV", fovCircleColor);
        ImGui::Unindent();
    }

    // Chams with inline customization
    ImGui::Checkbox("Chams", &chamsEnabled);
    if (ImGui::CollapsingHeader("Chams Settings##Chams")) {
        ImGui::Indent();
        ImGui::Checkbox("Chams Through Walls##Chams", &chamsThroughWalls);
        ImGui::Checkbox("Visible Only##Chams", &chamsVisibleOnly);
        ColorPicker("Enemy Chams Color##Chams", chamsEnemyColor);
        ColorPicker("Teammate Chams Color##Chams", chamsTeammateColor);
        ColorPicker("Visible Chams Color##Chams", chamsVisibleColor);
        ImGui::Unindent();
    }

    ImGui::EndChild();

    ImGui::NextColumn();

    // Right column - Colors & Settings
    ImGui::BeginChild("Visual Settings", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "Visual Settings");
    ImGui::Separator();

    // Global color settings
    if (ImGui::CollapsingHeader("Global Colors", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        ColorPicker("Enemy Color", espColor);
        ColorPicker("Visible Color", visibleColor);
        ColorPicker("Teammate Color", teammateColor);
        ImGui::Unindent();
    }

    // Global ESP settings
    if (ImGui::CollapsingHeader("Global ESP Settings")) {
        ImGui::Indent();
        ImGui::SliderFloat("Max Render Distance", &glowMaxDistance, 500.0f, 10000.0f, "%.0f units");
        ImGui::TextDisabled("Note: Some features may have individual distance settings");
        ImGui::Unindent();
    }

    // Quick presets
    if (ImGui::CollapsingHeader("Quick Presets")) {
        ImGui::Indent();
        if (ImGui::Button("Red Theme", ImVec2(-1, 0))) {
            espColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            visibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            teammateColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
        }
        if (ImGui::Button("Blue Theme", ImVec2(-1, 0))) {
            espColor = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
            visibleColor = ImVec4(0.0f, 1.0f, 0.5f, 1.0f);
            teammateColor = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);
        }
        if (ImGui::Button("Green Theme", ImVec2(-1, 0))) {
            espColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            visibleColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            teammateColor = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
        }
        ImGui::Unindent();
    }

    ImGui::EndChild();

    ImGui::Columns(1);
}

void RenderAimbotTab() {
    ImGui::Columns(2, "AimbotColumns", true);

    // Left column - Main Settings
    ImGui::BeginChild("Aimbot Settings", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "Aimbot Settings");
    ImGui::Separator();

    ImGui::Checkbox("Enable Aimbot", &aimbotEnabled);

    ImGui::Spacing();
    ImGui::SliderFloat("FOV", &aimbotFOV, 1.0f, 90.0f, "%.1f°");
    ImGui::SliderFloat("Smoothness", &aimbotSmoothing, 1.0f, 20.0f, "%.1f");

    ImGui::Spacing();
    ImGui::Checkbox("Visible Only", &aimOnVisibleOnly);
    ImGui::Checkbox("Target Teammates", &aimTeammates);
    ImGui::Checkbox("Show FOV Circle", &fovCircleEnabled);

    // Key binding
    ImGui::Spacing();
    ImGui::Text("Aim Key:");
    if (!recordingAimKey) {
        if (ImGui::Button("Bind Key##Aim", ImVec2(80, 0))) {
            recordingAimKey = true;
        }
    }
    else {
        ImGui::Button("Press Key...##Aim", ImVec2(80, 0));
    }
    ImGui::SameLine();
    ImGui::Text("%s", GetKeyName(aimKey).c_str());

    ImGui::EndChild();

    ImGui::NextColumn();

    // Right column - External Aimbot
    ImGui::BeginChild("External Aimbot", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.85f, 0.65f, 0.25f, 1.0f), "External Aimbot");
    ImGui::Separator();

    ImGui::Checkbox("Enable External", &externalAimbotEnabled);

    ImGui::Spacing();
    ImGui::SliderFloat("External FOV", &externalAimbotFOV, 1.0f, 30.0f, "%.1f°");
    ImGui::SliderFloat("External Smooth", &externalAimbotSmoothness, 0.0f, 10.0f, "%.1f");

    // Hitgroup selection
    ImGui::Spacing();
    ImGui::Text("Target Bone:");
    if (ImGui::BeginCombo("##Hitgroup", hitgroupNames[externalAimbotHitgroup])) {
        for (int i = 0; i < 5; i++) {
            bool isSelected = (externalAimbotHitgroup == i);
            if (ImGui::Selectable(hitgroupNames[i], isSelected)) {
                externalAimbotHitgroup = i;
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // External key binding
    ImGui::Spacing();
    ImGui::Text("External Key:");
    if (!recordingExternalAimKey) {
        if (ImGui::Button("Bind Key##Ext", ImVec2(80, 0))) {
            recordingExternalAimKey = true;
        }
    }
    else {
        ImGui::Button("Press Key...##Ext", ImVec2(80, 0));
    }
    ImGui::SameLine();
    ImGui::Text("%s", GetKeyName(externalAimbotKey).c_str());

    ImGui::EndChild();

    ImGui::Columns(1);
}

void RenderMiscTab() {
    ImGui::Columns(2, "MiscColumns", true);

    // Left column - Game Mods
    ImGui::BeginChild("Game Modifications", ImVec2(0, 0), true);
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "Game Modifications");
    ImGui::Separator();

    ImGui::Checkbox("No Flash", &noFlashEnabled);
    ImGui::Checkbox("No Recoil", &noRecoilEnabled);
    ImGui::Checkbox("Reveal All", &revealAllEnabled);
    ImGui::Checkbox("Infinite Armor", &infiniteArmorEnabled);

    ImGui::Spacing();
    ImGui::Checkbox("Bunnyhop", &bunnyhopEnabled);
    ImGui::Checkbox("Triggerbot", &triggerbotEnabled);
    if (triggerbotEnabled) {
        ImGui::SliderFloat("Trigger Delay", &triggerbotDelay, 0.0f, 100.0f, "%.0f ms");
    }

    // NEW FEATURES
    ImGui::Spacing();
    ImGui::Checkbox("Auto Strafe", &autoStrafeEnabled);
    if (autoStrafeEnabled) {
        ImGui::SliderFloat("Strafe Speed", &strafeSpeed, 0.5f, 3.0f, "%.1f");
    }

    ImGui::Spacing();
    ImGui::Checkbox("Hit Sound", &hitsoundEnabled);
    ImGui::Checkbox("Spectator List", &showSpectatorList);

    ImGui::EndChild();

    ImGui::NextColumn();

    // Right column - FOV & Config
    ImGui::BeginChild("FOV & Configuration", ImVec2(0, 0), true);

    // FOV Changer
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "FOV Changer");
    ImGui::Separator();

    ImGui::Checkbox("Enable FOV Changer", &fovChangerEnabled);
    ImGui::SliderFloat("FOV Value", &fovChangerValue, 60.0f, 120.0f, "%.0f");

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "Configuration");
    ImGui::Separator();

    if (ImGui::Button("Save Config", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, 0))) {
        SaveConfig();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Config", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        LoadConfig();
    }

    ImGui::Spacing();
    if (ImGui::Button("Reset to Defaults", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        // Reset logic here
    }

    ImGui::EndChild();

    ImGui::Columns(1);
}

void RenderConfigTab() {
    ImGui::BeginChild("Config Management", ImVec2(0, 0), true);

    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "Configuration Management");
    ImGui::Separator();

    // Config file info
    ImGui::Text("Config Location:");
    ImGui::TextDisabled("C:\\Users\\dionr\\Downloads\\cs2-full-main\\config.txt");

    ImGui::Spacing();

    // Action buttons
    if (ImGui::Button("Save Configuration", ImVec2(150, 40))) {
        SaveConfig();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Configuration", ImVec2(150, 40))) {
        LoadConfig();
    }

    ImGui::Spacing();
    if (ImGui::Button("Reset to Defaults", ImVec2(150, 40))) {
        // Reset implementation
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Debug console
    ImGui::Checkbox("Show Debug Console", &showDebugConsole);
    if (showDebugConsole) {
        ImGui::BeginChild("DebugConsole", ImVec2(0, 200), true);
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
}

void RenderCleanMenu() {
    if (!menuOpen) return;

    static bool styleSet = false;
    if (!styleSet) {
        SetupImGuiStyle();
        styleSet = true;
    }

    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("CS2 WoW Cheat - Professional Edition", &menuOpen,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Header with gradient effect
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();

    // Header background
    draw_list->AddRectFilled(
        window_pos,
        ImVec2(window_pos.x + window_size.x, window_pos.y + 40),
        ImGui::GetColorU32(ImVec4(0.12f, 0.12f, 0.14f, 1.00f))
    );

    // Title
    ImGui::SetCursorPos(ImVec2(15, 10));
    ImGui::TextColored(ImVec4(0.20f, 0.85f, 0.45f, 1.0f), "CS2 WoW Cheat");
    ImGui::SameLine();
    ImGui::TextDisabled("v1.0 Professional");

    // Quick stats on header
    ImGui::SetCursorPos(ImVec2(window_size.x - 200, 12));
    ImGui::Text("FPS: %.1f | Entities: %d", ImGui::GetIO().Framerate, GetEntities().size());

    ImGui::SetCursorPosY(45);

    // Main content area
    static int selectedTab = 0;
    const char* tabNames[] = { "VISUALS", "AIMBOT", "MISC", "CONFIG" };

    // Tabs
    ImGui::BeginChild("Tabs", ImVec2(0, 40), false);
    float tabWidth = ImGui::GetWindowWidth() / 4.0f;

    for (int i = 0; i < 4; i++) {
        if (i > 0) ImGui::SameLine();

        bool isSelected = (selectedTab == i);
        ImVec2 cursorPos = ImGui::GetCursorPos();

        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.85f, 0.45f, 0.30f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.85f, 0.45f, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.85f, 0.45f, 0.50f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.20f, 0.85f, 0.45f, 1.00f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.18f, 0.70f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.18f, 0.22f, 0.70f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.25f, 0.70f));
        }

        if (ImGui::Button(tabNames[i], ImVec2(tabWidth - 5, 35))) {
            selectedTab = i;
        }

        ImGui::PopStyleColor(isSelected ? 4 : 3);
    }
    ImGui::EndChild();

    ImGui::Spacing();

    // Tab content
    ImGui::BeginChild("Content", ImVec2(0, 0), true);

    switch (selectedTab) {
    case 0: RenderVisualsTab(); break;
    case 1: RenderAimbotTab(); break;
    case 2: RenderMiscTab(); break;
    case 3: RenderConfigTab(); break;
    }

    ImGui::EndChild();
    ImGui::End();
}