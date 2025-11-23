#include "menu.h"
#include "fontawesome.h"

void RenderVisualsTab() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));

    ImGui::Columns(2, "VisualCols", false);

    // --- LEFT COLUMN ---

    // 1. Player ESP
    ImGui::PushStyleColor(ImGuiCol_ChildBg, themePanelBg);
    if (ImGui::BeginChild("PlayerESP", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {
        ImGui::TextColored(themeAccent, ICON_FA_USER_TAG "  Player ESP");
        ImGui::Separator();

        CustomCheckbox("Master Switch", &espEnabled);
        CustomCheckbox("Teammates", &showTeammates);

        ImGui::Spacing();

        // Group: Geometry
        ImGui::TextDisabled(ICON_FA_SHAPES " Geometry");
        ImGui::Indent();
        CustomCheckbox("Box 2D", &box2DEnabled);
        CustomCheckbox("Box 3D", &box3DEnabled);
        CustomCheckbox("Skeleton", &boneESPEnabled);
        CustomCheckbox("Wireframe Bones", &wireframeBoneESPEnabled);
        CustomCheckbox("Head Circle", &headCircleEnabled);
        ImGui::Unindent();

        ImGui::Spacing();

        // Group: Information
        ImGui::TextDisabled(ICON_FA_INFO_CIRCLE " Info");
        ImGui::Indent();
        CustomCheckbox("Name", &nameESPEnabled);
        CustomCheckbox("Health Bar", &healthBarEnabled);
        CustomCheckbox("Armor Bar", &armorEnabled);
        CustomCheckbox("Weapon", &weaponESPEnabled);
        CustomCheckbox("Flags", &flagsEnabled);
        ImGui::Unindent();

        ImGui::Spacing();
        ImGui::Separator();

        // Settings Slider Group
        if (CustomCollapsingHeader(ICON_FA_SLIDERS_H " Settings & Colors")) {
            CustomSliderFloat("Max Distance", &espMaxDistance, 100.f, 10000.f, "%.0f m");
            CustomSliderFloat("Box Thickness", &boxThickness, 1.f, 3.f);

            ImGui::Spacing();
            ImGui::Text("Colors:");
            ImGui::Columns(2, "EspColors", false);
            ColorPicker("Box", espColor);
            ColorPicker("Visible", visibleColor);
            ImGui::NextColumn();
            ColorPicker("Skeleton", skeletonColor);
            ColorPicker("Wireframe Bones Visible", wireframeVisibleColor);
            ColorPicker("Wireframe Bones Invisible", wireframeInvisibleColor);
            ColorPicker("Head", headCircleColor);
            ImGui::Columns(1);
        }

    } ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::NextColumn();

    // --- RIGHT COLUMN ---

    // 2. Glow & Chams
    ImGui::PushStyleColor(ImGuiCol_ChildBg, themePanelBg);
    if (ImGui::BeginChild("GlowChams", ImVec2(0, ImGui::GetContentRegionAvail().y * 0.5f), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {
        ImGui::TextColored(themeAccent, ICON_FA_GHOST "  Glow & Chams");
        ImGui::Separator();


        // Glow
        CustomCheckbox(ICON_FA_LIGHTBULB " Enable Glow", &glowESPEnabled);
        if (glowESPEnabled) {
            ImGui::Indent();
            CustomCheckbox("Through Walls", &glowThroughWalls);
            ColorPicker("Glow Color", glowColor);
            CustomSliderFloat("Thickness", &glowV2Brightness, 0.1f, 10.f);
            ImGui::Unindent();
        }


    } ImGui::EndChild();
    ImGui::PopStyleColor();

    // 3. World & Misc
    ImGui::PushStyleColor(ImGuiCol_ChildBg, themePanelBg);
    if (ImGui::BeginChild("WorldVisuals", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {
        ImGui::TextColored(themeAccent, ICON_FA_GLOBE_EUROPE "  World & Tracers");
        ImGui::Separator();

        CustomCheckbox(ICON_FA_CROSSHAIRS " Sniper Crosshair", &customCrosshairEnabled);
        CustomCheckbox("Recoil Crosshair", &showRecoilCrosshair);

        ImGui::Spacing();

        CustomCheckbox(ICON_FA_BEZIER_CURVE " Bullet Tracers", &bulletTracersEnabled);
        if (bulletTracersEnabled) {
            ColorPicker("##TracerCol", bulletTracerColor); ImGui::SameLine();
            CustomSliderFloat("Duration", &bulletTracerDuration, 1.f, 5.f, "%.1fs");
        }

        ImGui::Spacing();
        CustomCheckbox(ICON_FA_BOMB " Bomb Timer", &showBombTimer);
        CustomCheckbox(ICON_FA_SMOG " Smoke/Flash Timer", &showSmokeWarning);
        CustomCheckbox(ICON_FA_MOON " Night Mode", &skyModEnabled);

    } ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Columns(1);
    ImGui::PopStyleVar(2);
}