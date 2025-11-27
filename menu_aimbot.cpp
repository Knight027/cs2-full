#include "menu.h"
#include "fontawesome.h"

std::string GetKeyName2(int key) {
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

// Helper arrays for External Aimbot options
// (Defining them here to ensure they exist, as they were referenced in your original code)
const char* targetSelectionNames[] = { "Crosshair", "Distance", "Health", "Cycle" };
const char* hitgroupNames[] = { "Head", "Neck", "Chest", "Stomach", "Pelvis" };

// Helper to display a keybind status elegantly
void RenderKeybindState(const char* label, int key, bool& recordingMode) {
    ImGui::Text("%s", label);
    ImGui::SameLine();

    std::string btnLabel;
    if (recordingMode) {
        btnLabel = ICON_FA_KEYBOARD " Press Key...";
        // Visual cue that we are recording
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
    }
    else {
        btnLabel = std::string(ICON_FA_KEY " ") + GetKeyName2(key);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    if (CustomButton(btnLabel.c_str(), ImVec2(120, 24))) {
        recordingMode = !recordingMode;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    // Active status indicator
    if (key > 0 && (GetAsyncKeyState(key) & 0x8000)) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.2f, 1.0f), ICON_FA_CHECK_CIRCLE " Active");
    }
}

void RenderAimbotTab() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
    ImGui::Columns(2, "AimbotCols", true);

    // --- LEFT COLUMN: Main Settings ---
    if (ImGui::BeginChild("MainAimbot", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {
        
        ImGui::TextColored(themeAccent, ICON_FA_MAGIC "  Silent Aim");
        ImGui::Separator();
        CustomCheckbox("Enable pSilent", &pSilentEnabled);
        if (pSilentEnabled) {
            ImGui::Indent();
            CustomSliderFloat("Silent FOV", &pSilentFOV, 0.1f, 10.0f, "%.1f°");
            ImGui::TextDisabled("Note: Hides aim from screen.");
            ImGui::Unindent();
        }
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::TextColored(themeAccent, ICON_FA_CROSSHAIRS "  Aimbot Configuration");
        ImGui::Separator();

        // Main Toggle using External Variable
        CustomCheckbox("Enable Aimbot", &externalAimbotEnabled);

        ImGui::Spacing();

        if (!externalAimbotEnabled) {
            ImGui::TextDisabled("Enable Aimbot to see settings...");
        }
        else {
            // Target Selection
            ImGui::Text(ICON_FA_BULLSEYE " Priority & Target");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Combo("##TargetMethod", &externalAimbotTargetSelection, targetSelectionNames, IM_ARRAYSIZE(targetSelectionNames));

            ImGui::Spacing();

            // Bone Selection
            ImGui::Text(ICON_FA_SKELETON " Aim Bone");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Combo("##Hitgroup", &externalAimbotHitgroup, hitgroupNames, IM_ARRAYSIZE(hitgroupNames));

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Keybind
            RenderKeybindState("Activation Key:", externalAimbotKey, recordingExternalAimKey);

            ImGui::Spacing();
            ImGui::Separator();

            // Logic Toggles
            ImGui::Text(ICON_FA_FILTER " Logic Checks");
            CustomCheckbox("Visibility Check", &aimOnVisibleOnly);
            CustomCheckbox("Predict Movement", &externalAimbotPredictMovement);
            CustomCheckbox(ICON_FA_LOCK " Aim Lock", &externalAimbotAimlock);

            if (externalAimbotAimlock) {
                ImGui::Indent();
                CustomCheckbox("Auto Switch", &externalAimbotAutoSwitch);
                ImGui::Unindent();
            }
        }

    } ImGui::EndChild();

    ImGui::NextColumn();

    // --- RIGHT COLUMN: Tuning & Misc ---
    if (ImGui::BeginChild("AimbotTuning", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {

        ImGui::TextColored(themeAccent, ICON_FA_SLIDERS_H "  Tuning & Visuals");
        ImGui::Separator();

        if (externalAimbotEnabled) {
            ImGui::Text("Field of View");
            CustomSliderFloat("##FOV", &externalAimbotFOV, 0.5f, 50.0f, "%.1f°");

            ImGui::Text("Smoothness");
            CustomSliderFloat("##Smooth", &externalAimbotSmoothness, 0.0f, 20.0f, "%.1f");

            ImGui::Spacing();
            CustomCheckbox("Draw FOV Circle", &fovCircleEnabled);
            if (fovCircleEnabled) {
                ImGui::SameLine();
                ColorPicker("##FovColor", fovCircleColor);
            }

            ImGui::Spacing();
            ImGui::Separator();
        }

        // Recoil Control (RCS)
        ImGui::Spacing();
        ImGui::TextColored(themeAccent, ICON_FA_GHOST "  Recoil Control");
        ImGui::Separator();
        CustomCheckbox("No Recoil (RCS)", &noRecoilEnabled);

        // Triggerbot Section
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TextColored(themeAccent, ICON_FA_MOUSE_POINTER "  Triggerbot");
        ImGui::Separator();

        CustomCheckbox("Enable Triggerbot", &triggerbotEnabled);
        if (triggerbotEnabled) {
            ImGui::Indent();
            CustomSliderFloat("Reaction Delay", &triggerbotDelay, 0.0f, 150.0f, "%.0f ms");
            CustomCheckbox("Teammates", &triggerbotTeammates);
            CustomCheckbox("Visible Only", &triggerbotVisibleOnly);

            ImGui::Spacing();
            RenderKeybindState("Trigger Key:", triggerbotKey, recordingTriggerKey);
            ImGui::Unindent();
        }

        // Exploits (Moved here from the old Misc/Aimbot split)
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TextColored(themeAccent, ICON_FA_RANDOM "  Bullet Redirection");
        ImGui::Separator();
        CustomCheckbox("Enable Redirection", &bulletRedirectionEnabled);
        if (bulletRedirectionEnabled) {
            CustomSliderFloat("Yaw Offset", &bulletRedirectionYaw, -180.0f, 180.0f, "%.0f°");
            CustomSliderFloat("Pitch Offset", &bulletRedirectionPitch, -89.0f, 89.0f, "%.0f°");
            RenderKeybindState("Override Key:", bulletRedirectionKey, recordingBulletRedirectionKey);
        }

    } ImGui::EndChild();

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}