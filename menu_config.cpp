#include "menu.h"
#include "fontawesome.h"
#include <algorithm> // Required for std::remove_if
#include <cctype>    // Required for ::isspace

// Buffer to hold the name for a new config
// It's best practice to declare this static if it's only used within this file
static std::string configNameBuffer = "";

void RenderConfigTab() {
    // Ensure the list is populated when the tab is opened (or if a config was saved/deleted)
    static bool initialLoad = true;
    if (initialLoad) {
        LoadConfigList();
        initialLoad = false;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));

    // Center the panel slightly
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    if (ImGui::BeginChild("ConfigMgr", ImVec2(ImGui::GetContentRegionAvail().x - 40, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {

        ImGui::TextColored(themeAccent, ICON_FA_CUBE " Configuration Manager");
        ImGui::Separator();
        ImGui::Spacing();

        // --- Config Selection ---
        ImGui::TextColored(themeAccent, ICON_FA_FOLDER_OPEN " Current Configuration: ");
        ImGui::SameLine();
        ImGui::Text(g_currentConfigName.c_str());

        // Combo box to select existing config
        if (ImGui::BeginCombo("##ConfigSelector", g_currentConfigName.c_str())) {
            for (const std::string& config : g_configFiles) {
                bool is_selected = (g_currentConfigName == config);
                if (ImGui::Selectable(config.c_str(), is_selected)) {
                    g_currentConfigName = config; // Set the name to be loaded/overwritten
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();


        // --- Config Actions (Load/Overwrite/Delete) ---
        ImGui::TextColored(themeAccent, ICON_FA_WRENCH " Actions");
        ImGui::Separator();
        ImGui::Columns(3, "ConfigActions", false);

        // Load Button (loads the currently selected config name)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.3f, 0.6f, 0.8f));
        if (CustomButton(ICON_FA_UPLOAD " Load", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
            LoadConfig(g_currentConfigName);
        }
        ImGui::PopStyleColor();
        ImGui::TextDisabled("Load selected config");

        ImGui::NextColumn();

        // Save/Overwrite Button (Saves to the currently selected config name)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 0.1f, 0.8f));
        if (CustomButton(ICON_FA_SAVE " Overwrite", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
            SaveConfig(g_currentConfigName);
        }
        ImGui::PopStyleColor();
        ImGui::TextDisabled("Overwrite selected config");

        ImGui::NextColumn();

        // Delete Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 0.8f));
        if (CustomButton(ICON_FA_TRASH_ALT " Delete", ImVec2(ImGui::GetContentRegionAvail().x, 35))) {
            DeleteConfig(g_currentConfigName);
        }
        ImGui::PopStyleColor();
        ImGui::TextDisabled("Delete selected config");

        ImGui::Columns(1);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();


        // --- Save New Config ---
        ImGui::TextColored(themeAccent, ICON_FA_PLUS_CIRCLE " Save New Configuration");
        ImGui::Separator();

        // Input field for the new config name
        ImGui::InputTextWithHint("##NewConfigName", "Enter new config name (e.g. 'Legit')", &configNameBuffer, ImGuiInputTextFlags_None);

        // Save New Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));
        if (CustomButton(ICON_FA_PLUS " Save New", ImVec2(200, 35))) {
            if (!configNameBuffer.empty()) {
                // Sanitize name: remove leading/trailing spaces, remove .txt if present
                std::string sanitizedName = configNameBuffer;

                // Remove trailing whitespace
                sanitizedName.erase(std::find_if(sanitizedName.rbegin(), sanitizedName.rend(), [](int ch) {
                    return !std::isspace(ch);
                    }).base(), sanitizedName.end());

                // Remove leading whitespace
                sanitizedName.erase(sanitizedName.begin(), std::find_if(sanitizedName.begin(), sanitizedName.end(), [](int ch) {
                    return !std::isspace(ch);
                    }));

                // Remove .txt suffix if present
                if (sanitizedName.length() > 4 && sanitizedName.substr(sanitizedName.length() - 4) == ".txt") {
                    sanitizedName = sanitizedName.substr(0, sanitizedName.length() - 4);
                }

                if (!sanitizedName.empty()) {
                    SaveConfig(sanitizedName);
                    configNameBuffer.clear(); // Clear buffer after saving
                }
                else {
                    AddNotification("Config name cannot be empty.", 3.0f, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                }
            }
            else {
                AddNotification("Please enter a name for the new config.", 3.0f, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
            }
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled("Saves current settings to a new file.");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Reset
        if (CustomButton(ICON_FA_REDO " Reset All to Defaults", ImVec2(250, 30))) {
            // NOTE: You need to implement a ResetDefaults() function in cheat.cpp or menu_utils.cpp
            // ResetDefaults(); 
            // AddNotification("? All settings reset to default.", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
        }

        ImGui::Spacing();

        // Console / Debug
        if (CustomCollapsingHeader(ICON_FA_TERMINAL " Debug Information")) {
            ImGui::Text("Config Directory: C:\\CheatConfigs\\");
            ImGui::Text("File Count: %zu", g_configFiles.size());
        }

    } ImGui::EndChild();

    ImGui::PopStyleVar();
}