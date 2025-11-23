#include "menu.h"
#include "fontawesome.h"

void RenderMiscTab() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
    ImGui::Columns(2, "MiscCols", true);

    // Column 1: Movement & Game Mods
    ImGui::PushStyleColor(ImGuiCol_ChildBg, themePanelBg);
    if (ImGui::BeginChild("MovementMods", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {

        ImGui::TextColored(themeAccent, ICON_FA_RUNNING "  Movement");
        ImGui::Separator();

        CustomCheckbox(ICON_FA_SHOE_PRINTS " Bunnyhop", &bunnyhopEnabled);
        CustomCheckbox("Auto Strafe", &autoStrafeEnabled);
        CustomCheckbox(ICON_FA_BUG " Edge Jump", &edgeJumpEnabled);

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::TextColored(themeAccent, ICON_FA_WRENCH "  Game Modifications");
        ImGui::Separator();

        // Features restored from the original snippet
        CustomCheckbox(ICON_FA_COMMENT_DOTS " Client Chat", &clientChatEnabled);
        CustomCheckbox(ICON_FA_EYE_SLASH " No Flash", &noFlashEnabled);
        CustomCheckbox(ICON_FA_DOT_CIRCLE " No Spread", &noSpreadEnabled);
        CustomCheckbox(ICON_FA_USER_NINJA " No Legs", &noLegsEnabled);
        CustomCheckbox(ICON_FA_HANDSHAKE " Auto Accept Match", &autoAcceptEnabled);
        CustomCheckbox(ICON_FA_PERSON_SIGN " Spectator List", &showSpectatorList);


        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TextColored(themeAccent, ICON_FA_USER_NINJA "  Anti-Aim");
        ImGui::Separator();

        CustomCheckbox("Enable Anti-Aim", &antiAimEnabled);
        if (antiAimEnabled) {
            ImGui::Indent();

            // Keybind (Optional, but good practice)
            RenderKeybindState("Anti-Aim Key:", antiAimKey, recordingAntiAimKey);

            // AA Type Selector
            const char* aaTypes[] = { "Static Backward", "Jitter", "Spinbot" };
            ImGui::Text("Type:");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1.0f);
            ImGui::Combo("##AAType", &antiAimType, aaTypes, IM_ARRAYSIZE(aaTypes));
            ImGui::PopItemWidth();

            // Type-specific settings
            if (antiAimType == 1) { // Jitter
                CustomSliderFloat("Jitter Range", &antiAimJitterRange, 5.0f, 60.0f, "%.0f°");
            }
            else if (antiAimType == 2) { // Spinbot
                CustomSliderFloat("Spin Speed", &antiAimSpinSpeed, 0.1f, 5.0f, "%.1f rps"); // rps = rotations per second
            }

            // Common setting: Pitch
            // We will hardcode Pitch Down (89 deg) in the core logic, as it's the standard AA pitch.
            ImGui::Unindent();
        }

        

    } ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::NextColumn();

    // Column 2: View & Sound
    ImGui::PushStyleColor(ImGuiCol_ChildBg, themePanelBg);
    if (ImGui::BeginChild("ViewSound", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {

        ImGui::TextColored(themeAccent, ICON_FA_COMPASS "  Out of View");
        ImGui::Separator();

        CustomCheckbox("Enable OOF Arrows", &oofArrowsEnabled);
        if (oofArrowsEnabled) {
            CustomSliderFloat("Radius", &oofArrowsRadius, 50.0f, 400.0f, "%.0f px");
            CustomSliderFloat("Size", &oofArrowsSize, 5.0f, 30.0f, "%.0f px");
            CustomSliderFloat("Trigger Margin", &oofMarginDegrees, 0.0f, 45.0f, "%.0f deg");
        }

        ImGui::TextColored(themeAccent, ICON_FA_CAMERA "  View & Camera");
        ImGui::Separator();

        CustomCheckbox(ICON_FA_COMPRESS_ARROWS_ALT " FOV Changer", &fovChangerEnabled);
        if (fovChangerEnabled) {
            CustomSliderFloat("Viewmodel FOV", &fovChangerValue, 60.f, 140.f, "%.0f");
        }

        CustomCheckbox(ICON_FA_USER_ASTRONAUT " Thirdperson", &thirdPersonEnabled);
        if (thirdPersonEnabled) {
            CustomSliderFloat("Distance", &thirdPersonDistance, 50.f, 300.f);
            // Ensure you have RenderKeybindState defined in menu.h/menu_aimbot.cpp
            RenderKeybindState("TP Key:", thirdPersonKey, recordingThirdPersonKey);
        }

        ImGui::Spacing();
        ImGui::Separator();

        // Sound Settings
        ImGui::TextColored(themeAccent, ICON_FA_VOLUME_UP "  Sound & Effects");
        ImGui::Separator();

        CustomCheckbox("Hit Sound", &hitsoundEnabled);
        if (hitsoundEnabled) {
            ImGui::Indent();
            ImGui::TextDisabled("Sound file selector placeholder...");
            ImGui::Unindent();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextColored(themeAccent, "Particle Modulation");
        ImGui::Separator();

        CustomCheckbox(ICON_FA_CLOUD_RAIN " Enable Particle Modulation (Global)", &particleModEnabled);

        if (CustomButton("Configure Particles", ImVec2(ImGui::GetContentRegionAvail().x, 30.0f))) {
            particleConfigPopupOpen = true;
        }

        // Popup: All targets visible at once with individual toggles & colors
        if (particleConfigPopupOpen) {
            ImGui::OpenPopup("ParticleConfig");
        }

        if (ImGui::BeginPopupModal("ParticleConfig", &particleConfigPopupOpen, ImGuiWindowFlags_AlwaysAutoResize)) {


            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::TextColored(themeAccent, "Light Modulation");
            ImGui::Separator();

            CustomCheckbox("Enable Light Modulation", &lightModEnabled);
            if (lightModEnabled) {
                ImGui::ColorEdit3("Light Color", (float*)&lightModColor, ImGuiColorEditFlags_NoAlpha);

                if (CustomButton("Fullbright", ImVec2(ImGui::GetContentRegionAvail().x * 0.32f, 25.0f))) {
                    lightModColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                }
                ImGui::SameLine();
                if (CustomButton("Night Mode", ImVec2(ImGui::GetContentRegionAvail().x * 0.32f, 25.0f))) {
                    lightModColor = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
                }
                ImGui::SameLine();
                if (CustomButton("Red Tint", ImVec2(ImGui::GetContentRegionAvail().x * 0.32f, 25.0f))) {
                    lightModColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                }
            }


            ImGui::Text("Global Controls:");
            ImGui::ColorEdit3("Global Color", (float*)&globalParticleColor, ImGuiColorEditFlags_NoAlpha);

            ImGui::Separator();
            ImGui::Text("Rainbow Mode");
            CustomCheckbox("Enable Rainbow (Overrides Colors)", &rainbowEnabled);
            if (rainbowEnabled) {
                CustomSliderFloat("Rainbow Speed", &rainbowSpeed, 0.1f, 10.0f, "%.1f cycles/sec");
            }

            if (CustomButton("Apply Global Color")) {
                for (int i = 0; i < NUM_PARTICLE_TARGETS; ++i) {
                    particleColors[i] = globalParticleColor;
                }
                for (auto& group : particleGroups) {
                    group.color = globalParticleColor;
                }
            }

            ImGui::SameLine();
            if (CustomButton("Enable All")) {
                for (int i = 0; i < NUM_PARTICLE_TARGETS; ++i) {
                    particleTargetsEnabled[i] = true;
                }
                for (auto& group : particleGroups) {
                    group.enabled = true;
                }
            }

            ImGui::SameLine();
            if (CustomButton("Disable All")) {
                for (int i = 0; i < NUM_PARTICLE_TARGETS; ++i) {
                    particleTargetsEnabled[i] = false;
                }
                for (auto& group : particleGroups) {
                    group.enabled = false;
                }
            }

            ImGui::Separator();
            ImGui::Text("Groups:");
            for (auto& group : particleGroups) {
                ImGui::PushID(&group);
                ImGui::Text("%s", group.name.c_str());

                bool prevEnabled = group.enabled;
                CustomCheckbox("Enabled", &group.enabled);
                if (group.enabled != prevEnabled) {
                    for (int idx : group.indices) {
                        particleTargetsEnabled[idx] = group.enabled;
                    }
                }

                ImGui::SameLine();

                ImVec4 prevColor = group.color;
                ImGui::ColorEdit3("Color", (float*)&group.color, ImGuiColorEditFlags_NoAlpha);
                if (group.color.x != prevColor.x || group.color.y != prevColor.y || group.color.z != prevColor.z) {
                    for (int idx : group.indices) {
                        particleColors[idx] = group.color;
                    }
                }

                ImGui::PopID();
            }

            ImGui::Separator();
            ImGui::Text("Individual Particles by Group:");
            for (const auto& group : particleGroups) {  // Use const& to avoid modifying while rendering
                if (CustomCollapsingHeader(group.name.c_str())) {
                    for (int idx : group.indices) {
                        ImGui::PushID(idx);
                        char label[128];
                        snprintf(label, sizeof(label), "%s###%d", particleTargets[idx], idx);

                        bool enabled = particleTargetsEnabled[idx];
                        CustomCheckbox(label, &enabled);
                        particleTargetsEnabled[idx] = enabled;  // Live update

                        ImGui::SameLine();
                        ImGui::ColorEdit3("Color", (float*)&particleColors[idx], ImGuiColorEditFlags_NoAlpha);

                        ImGui::PopID();
                    }
                }
            }

            ImGui::Separator();
            if (CustomButton("Close", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f))) {
                particleConfigPopupOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextColored(themeAccent, "World Modulation");
        ImGui::Separator();

        CustomCheckbox("Enable World Modulation", &worldModEnabled);
        if (worldModEnabled) {
            CustomSliderFloat("World Brightness", &worldBrightness, 0.0f, 1.0f, "%.2f");
            // If you want to add tint, you can explore further with color correction
        }

        // Animation settings
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextColored(themeAccent, "Animation Settings");
        ImGui::Separator();

        CustomSliderFloat("Health Animation Speed", &healthAnimationSpeed, 50.0f, 500.0f, "%.0f HP/s");
        CustomSliderFloat("Hitmarker Speed", &hitmarkerSpeed, 10.0f, 100.0f, "%.0f px/s");
        CustomSliderFloat("Hitmarker Fade Duration", &hitmarkerFadeDuration, 0.5f, 3.0f, "%.1f s");

    } ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}