#define NOMINMAX
#include "cheat.h"
#include <algorithm>

void RenderStandardBox(ImDrawList* drawList, ImVec2 min, ImVec2 max, ImU32 color) {
    if (outlineEnabled) {
        drawList->AddRect(
            ImVec2(min.x - 1, min.y - 1),
            ImVec2(max.x + 1, max.y + 1),
            IM_COL32(0, 0, 0, 150), boxRounding, ImDrawFlags_RoundCornersAll, boxThickness + 1.0f
        );
    }

    if (espOutlineGlow) {
        // Glow effect
        for (int i = 0; i < 3; i++) {
            float offset = i * 1.5f;
            drawList->AddRect(
                ImVec2(min.x - offset, min.y - offset),
                ImVec2(max.x + offset, max.y + offset),
                IM_COL32(0, 0, 0, (int)(50 * espGlowIntensity * (3 - i) / 3.0f)),
                boxRounding, ImDrawFlags_RoundCornersAll, 1.0f
            );
        }
    }

    drawList->AddRect(min, max, color, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
}



void RenderCorneredBox(ImDrawList* drawList, ImVec2 min, ImVec2 max, ImU32 color) {
    float cornerLength = (max.x - min.x) / 4.0f;
    float cornerHeight = (max.y - min.y) / 4.0f;

    // Top left
    drawList->AddLine(ImVec2(min.x, min.y), ImVec2(min.x + cornerLength, min.y), color, boxThickness);
    drawList->AddLine(ImVec2(min.x, min.y), ImVec2(min.x, min.y + cornerHeight), color, boxThickness);

    // Top right
    drawList->AddLine(ImVec2(max.x, min.y), ImVec2(max.x - cornerLength, min.y), color, boxThickness);
    drawList->AddLine(ImVec2(max.x, min.y), ImVec2(max.x, min.y + cornerHeight), color, boxThickness);

    // Bottom left
    drawList->AddLine(ImVec2(min.x, max.y), ImVec2(min.x + cornerLength, max.y), color, boxThickness);
    drawList->AddLine(ImVec2(min.x, max.y), ImVec2(min.x, max.y - cornerHeight), color, boxThickness);

    // Bottom right
    drawList->AddLine(ImVec2(max.x, max.y), ImVec2(max.x - cornerLength, max.y), color, boxThickness);
    drawList->AddLine(ImVec2(max.x, max.y), ImVec2(max.x, max.y - cornerHeight), color, boxThickness);
}

void RenderFilledBox(ImDrawList* drawList, ImVec2 min, ImVec2 max, ImU32 color) {
    ImU32 fillColor = IM_COL32(
        (int)(espBoxFillColor.x * 255),
        (int)(espBoxFillColor.y * 255),
        (int)(espBoxFillColor.z * 255),
        (int)(espBoxFillColor.w * 100) // Semi-transparent
    );

    drawList->AddRectFilled(min, max, fillColor, boxRounding);
    drawList->AddRect(min, max, color, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
}

void RenderGradientBox(ImDrawList* drawList, ImVec2 min, ImVec2 max, ImU32 color) {
    ImU32 topColor = IM_COL32(
        (int)(espBoxGradientTop.x * 255),
        (int)(espBoxGradientTop.y * 255),
        (int)(espBoxGradientTop.z * 255),
        (int)(espBoxGradientTop.w * 255)
    );

    ImU32 bottomColor = IM_COL32(
        (int)(espBoxGradientBottom.x * 255),
        (int)(espBoxGradientBottom.y * 255),
        (int)(espBoxGradientBottom.z * 255),
        (int)(espBoxGradientBottom.w * 255)
    );

    drawList->AddRectFilledMultiColor(min, max, topColor, topColor, bottomColor, bottomColor);
    drawList->AddRect(min, max, color, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
}

void RenderOutlineBox(ImDrawList* drawList, ImVec2 min, ImVec2 max, ImU32 color) {
    drawList->AddRect(min, max, color, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
}

void DrawEnhancedText(ImDrawList* drawList, ImVec2 pos, ImU32 color, const char* text, float alpha) {
    // Apply text scale
    ImVec2 scaledPos = ImVec2(pos.x * espTextScale, pos.y * espTextScale);

    ImU32 finalColor = IM_COL32(
        (int)(((color >> IM_COL32_R_SHIFT) & 0xFF) * alpha),
        (int)(((color >> IM_COL32_G_SHIFT) & 0xFF) * alpha),
        (int)(((color >> IM_COL32_B_SHIFT) & 0xFF) * alpha),
        (int)(((color >> IM_COL32_A_SHIFT) & 0xFF) * alpha)
    );

    if (espTextShadow) {
        drawList->AddText(ImVec2(scaledPos.x + 1, scaledPos.y + 1), IM_COL32(0, 0, 0, (int)(150 * alpha)), text);
    }
    drawList->AddText(scaledPos, finalColor, text);
}

void RenderEnhancedHealthBar(Entity& e, ImVec2 boxMin, ImVec2 boxMax, ImDrawList* drawList, float alpha) {
    if (!healthBarEnabled) return;

    float healthFrac = e.health / 100.0f;
    healthFrac = std::max(0.0f, std::min(1.0f, healthFrac)); // Clamp between 0-1

    switch (healthBarStyle) {
    case HEALTH_BAR_VERTICAL:
    case HEALTH_BAR_GRADIENT: {
        // Vertical health bar (left side)
        float barWidth = 4.0f;
        float barHeight = (boxMax.y - boxMin.y) * healthFrac;

        ImVec2 barPos(boxMin.x - barWidth - 3, boxMin.y + (boxMax.y - boxMin.y) - barHeight);
        ImVec2 barSize(boxMin.x - 3, boxMax.y);

        // Background
        drawList->AddRectFilled(
            ImVec2(boxMin.x - barWidth - 4, boxMin.y - 1),
            ImVec2(boxMin.x - 2, boxMax.y + 1),
            IM_COL32(0, 0, 0, (int)(150 * alpha)), 2.0f
        );

        ImU32 healthColor;
        if (healthBarStyle == HEALTH_BAR_GRADIENT || espGradientHealthBar) {
            // Gradient health bar (green -> yellow -> red)
            float r = (1.0f - healthFrac) * 2.0f;
            float g = healthFrac * 2.0f;
            r = std::min(1.0f, r);
            g = std::min(1.0f, g);

            healthColor = IM_COL32(
                (int)(255 * r * alpha),
                (int)(255 * g * alpha),
                0,
                (int)(255 * alpha)
            );
        }
        else {
            // Standard health bar
            healthColor = IM_COL32(
                (int)(255 * (1.0f - healthFrac) * alpha),
                (int)(255 * healthFrac * alpha),
                0,
                (int)(255 * alpha)
            );
        }

        drawList->AddRectFilled(barPos, barSize, healthColor, 2.0f);
        break;
    }

    case HEALTH_BAR_HORIZONTAL: {
        // Horizontal health bar (top of box)
        float barHeight = 3.0f;
        float barWidth = (boxMax.x - boxMin.x) * healthFrac;

        ImVec2 barPos(boxMin.x, boxMin.y - barHeight - 2);
        ImVec2 barSize(boxMin.x + barWidth, boxMin.y - 2);

        // Background
        drawList->AddRectFilled(
            ImVec2(boxMin.x - 1, boxMin.y - barHeight - 3),
            ImVec2(boxMax.x + 1, boxMin.y - 1),
            IM_COL32(0, 0, 0, (int)(150 * alpha)), 1.0f
        );

        ImU32 healthColor = IM_COL32(
            (int)(255 * (1.0f - healthFrac) * alpha),
            (int)(255 * healthFrac * alpha),
            0,
            (int)(255 * alpha)
        );

        drawList->AddRectFilled(barPos, barSize, healthColor, 1.0f);
        break;
    }

    case HEALTH_BAR_TEXT_ONLY:
        // Health text will be handled in RenderEnhancedInfo
        break;
    }

    // Health text (if enabled)
    if (healthTextEnabled && healthBarStyle != HEALTH_BAR_TEXT_ONLY) {
        char healthText[16];
        sprintf_s(healthText, "%d", e.health);

        ImVec2 textSize = ImGui::CalcTextSize(healthText);
        ImVec2 textPos(boxMin.x - textSize.x - 5, boxMin.y + (boxMax.y - boxMin.y) / 2 - textSize.y / 2);

        DrawEnhancedText(drawList, textPos, IM_COL32(255, 255, 255, (int)(255 * alpha)), healthText, alpha);
    }
}

void RenderEnhancedInfo(Entity& e, ImVec2 boxMin, ImVec2 boxMax, ImDrawList* drawList, ImU32 color, float alpha) {
    std::vector<std::string> infoLines;

    // Name
    if (nameESPEnabled) {
        infoLines.push_back(e.name);
    }


    // Distance
    if (distanceTextEnabled) {
        float dist = sqrtf(powf(GetLocalPlayer().origin.x - e.origin.x, 2) +
            powf(GetLocalPlayer().origin.y - e.origin.y, 2)) / 100.0f;
        char distText[32];
        sprintf_s(distText, "%.0fm", dist);
        infoLines.push_back(distText);
    }

    // Render all info lines
    float yOffset = boxMin.y - (infoLines.size() * (12 * espTextScale));
    for (const auto& line : infoLines) {
        DrawEnhancedText(drawList, ImVec2(boxMin.x, yOffset), color, line.c_str(), alpha);
        yOffset += 12 * espTextScale;
    }
}


void RenderEnhancedESPBox(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight,
    ImDrawList* drawList, ImU32 color) {
    Vector3 footPos = e.origin;
    Vector3 headPos = GetBonePosition(e.boneMatrix, bone_head);
    headPos.z += 5.0f;

    Vector3 screenFoot, screenHead;
    if (!WorldToScreen(footPos, screenFoot, viewMatrix, screenWidth, screenHeight) ||
        !WorldToScreen(headPos, screenHead, viewMatrix, screenWidth, screenHeight)) {
        return;
    }

    float height = screenFoot.y - screenHead.y;
    float width = height / 2.2f;
    float x = screenHead.x - width / 2.0f;
    float y = screenHead.y;

    if (height < 10.0f || height > 500.0f || width < 5.0f || width > 250.0f) {
        return;
    }

    ImVec2 boxMin(x, y);
    ImVec2 boxMax(x + width, y + height);

    // Distance-based alpha fade
    float distance = sqrtf(powf(GetLocalPlayer().origin.x - e.origin.x, 2) +
        powf(GetLocalPlayer().origin.y - e.origin.y, 2)) / 100.0f;
    float alphaMultiplier = espDistanceFade ? (1.0f - (distance / espMaxDistance)) : 1.0f;
    alphaMultiplier = std::max(0.3f, std::min(1.0f, alphaMultiplier));

    ImU32 finalColor = IM_COL32(
        (int)(((color >> IM_COL32_R_SHIFT) & 0xFF) * alphaMultiplier),
        (int)(((color >> IM_COL32_G_SHIFT) & 0xFF) * alphaMultiplier),
        (int)(((color >> IM_COL32_B_SHIFT) & 0xFF) * alphaMultiplier),
        (int)(((color >> IM_COL32_A_SHIFT) & 0xFF) * alphaMultiplier)
    );

    switch (espBoxStyle) {
    case ESP_BOX_STANDARD:
        RenderStandardBox(drawList, boxMin, boxMax, finalColor);
        break;
    case ESP_BOX_CORNERED:
        RenderCorneredBox(drawList, boxMin, boxMax, finalColor);
        break;
    case ESP_BOX_FILLED:
        RenderFilledBox(drawList, boxMin, boxMax, finalColor);
        break;
    case ESP_BOX_GRADIENT:
        RenderGradientBox(drawList, boxMin, boxMax, finalColor);
        break;
    case ESP_BOX_OUTLINE_ONLY:
        RenderOutlineBox(drawList, boxMin, boxMax, finalColor);
        break;
    }

    RenderEnhancedHealthBar(e, boxMin, boxMax, drawList, alphaMultiplier);
    RenderEnhancedInfo(e, boxMin, boxMax, drawList, finalColor, alphaMultiplier);
}


void RenderStandardSkeleton(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight,
    ImDrawList* drawList, ImU32 color, float alpha) {
    for (const auto& conn : boneConnections) {
        Vector3 boneFrom = GetBonePosition(e.boneMatrix, conn.first);
        Vector3 boneTo = GetBonePosition(e.boneMatrix, conn.second);
        Vector3 screenFrom, screenTo;

        if (WorldToScreen(boneFrom, screenFrom, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(boneTo, screenTo, viewMatrix, screenWidth, screenHeight)) {
            if (outlineEnabled) {
                drawList->AddLine(
                    ImVec2(screenFrom.x, screenFrom.y),
                    ImVec2(screenTo.x, screenTo.y),
                    IM_COL32(0, 0, 0, 255), skeletonThickness + 1.0f
                );
            }
            drawList->AddLine(
                ImVec2(screenFrom.x, screenFrom.y),
                ImVec2(screenTo.x, screenTo.y),
                color, skeletonThickness
            );
        }
    }
}

void RenderDottedSkeleton(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight,
    ImDrawList* drawList, ImU32 color, float alpha) {
    for (const auto& conn : boneConnections) {
        Vector3 boneFrom = GetBonePosition(e.boneMatrix, conn.first);
        Vector3 boneTo = GetBonePosition(e.boneMatrix, conn.second);
        Vector3 screenFrom, screenTo;

        if (WorldToScreen(boneFrom, screenFrom, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(boneTo, screenTo, viewMatrix, screenWidth, screenHeight)) {

            // Create dotted line effect
            float segmentLength = 8.0f;
            ImVec2 start(screenFrom.x, screenFrom.y);
            ImVec2 end(screenTo.x, screenTo.y);
            ImVec2 dir = ImVec2(end.x - start.x, end.y - start.y);
            float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
            dir = ImVec2(dir.x / length, dir.y / length);

            for (float t = 0; t < length; t += segmentLength * 2) {
                ImVec2 segStart(start.x + dir.x * t, start.y + dir.y * t);
                ImVec2 segEnd(start.x + dir.x * (t + segmentLength), start.y + dir.y * (t + segmentLength));

                if (segEnd.x > end.x && dir.x > 0) segEnd = end;
                if (segEnd.x < end.x && dir.x < 0) segEnd = end;
                if (segEnd.y > end.y && dir.y > 0) segEnd = end;
                if (segEnd.y < end.y && dir.y < 0) segEnd = end;

                drawList->AddLine(segStart, segEnd, color, skeletonThickness);
            }
        }
    }
}

void RenderGradientSkeleton(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight,
    ImDrawList* drawList, ImU32 color, float alpha) {
    if (!boneESPEnabled || !e.boneMatrix) return;

    Vector3 headPos = GetBonePosition(e.boneMatrix, bone_head);
    const float maxDistance = 72.0f; // approximate distance from head to foot

    for (const auto& conn : boneConnections) {
        Vector3 boneFrom = GetBonePosition(e.boneMatrix, conn.first);
        Vector3 boneTo = GetBonePosition(e.boneMatrix, conn.second);
        Vector3 screenFrom, screenTo;

        if (WorldToScreen(boneFrom, screenFrom, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(boneTo, screenTo, viewMatrix, screenWidth, screenHeight)) {

            // Calculate distance from head for gradient effect
            float distFrom = (boneFrom - headPos).Length();
            float distTo = (boneTo - headPos).Length();

            // Normalize distances (0.0 = head, 1.0 = feet)
            float tFrom = std::min(1.0f, distFrom / maxDistance);
            float tTo = std::min(1.0f, distTo / maxDistance);

            // Interpolate colors
            ImU32 colorFrom = IM_COL32(
                (int)(skeletonStartColor.x * 255 + (skeletonEndColor.x - skeletonStartColor.x) * 255 * tFrom),
                (int)(skeletonStartColor.y * 255 + (skeletonEndColor.y - skeletonStartColor.y) * 255 * tFrom),
                (int)(skeletonStartColor.z * 255 + (skeletonEndColor.z - skeletonStartColor.z) * 255 * tFrom),
                (int)(skeletonStartColor.w * 255 * alpha)
            );

            ImU32 colorTo = IM_COL32(
                (int)(skeletonStartColor.x * 255 + (skeletonEndColor.x - skeletonStartColor.x) * 255 * tTo),
                (int)(skeletonStartColor.y * 255 + (skeletonEndColor.y - skeletonStartColor.y) * 255 * tTo),
                (int)(skeletonStartColor.z * 255 + (skeletonEndColor.z - skeletonStartColor.z) * 255 * tTo),
                (int)(skeletonStartColor.w * 255 * alpha)
            );

            // Since ImDrawList doesn't support gradient lines directly, we'll use the average color
            ImU32 avgColor = IM_COL32(
                ((colorFrom >> IM_COL32_R_SHIFT) & 0xFF + (colorTo >> IM_COL32_R_SHIFT) & 0xFF) / 2,
                ((colorFrom >> IM_COL32_G_SHIFT) & 0xFF + (colorTo >> IM_COL32_G_SHIFT) & 0xFF) / 2,
                ((colorFrom >> IM_COL32_B_SHIFT) & 0xFF + (colorTo >> IM_COL32_B_SHIFT) & 0xFF) / 2,
                ((colorFrom >> IM_COL32_A_SHIFT) & 0xFF + (colorTo >> IM_COL32_A_SHIFT) & 0xFF) / 2
            );

            if (outlineEnabled) {
                drawList->AddLine(
                    ImVec2(screenFrom.x, screenFrom.y),
                    ImVec2(screenTo.x, screenTo.y),
                    IM_COL32(0, 0, 0, (int)(255 * alpha)), skeletonThickness + 1.0f
                );
            }

            drawList->AddLine(
                ImVec2(screenFrom.x, screenFrom.y),
                ImVec2(screenTo.x, screenTo.y),
                avgColor, skeletonThickness
            );
        }
    }
}

void RenderEnhancedSkeleton(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight,
    ImDrawList* drawList, ImU32 color, float alpha) {
    if (!boneESPEnabled || !e.boneMatrix) return;

    switch (skeletonStyle) {
    case SKELETON_STANDARD:
        RenderStandardSkeleton(e, viewMatrix, screenWidth, screenHeight, drawList, color, alpha);
        break;
    case SKELETON_DOTTED:
        RenderDottedSkeleton(e, viewMatrix, screenWidth, screenHeight, drawList, color, alpha);
        break;
    case SKELETON_GRADIENT:
        RenderGradientSkeleton(e, viewMatrix, screenWidth, screenHeight, drawList, color, alpha);
        break;
    }
}

void Render3DBox(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList, ImU32 color) {
    Vector3 boxPoints[8] = {
    { e.origin.x - 15, e.origin.y - 15, e.origin.z },
    { e.origin.x + 15, e.origin.y - 15, e.origin.z },
    { e.origin.x + 15, e.origin.y + 15, e.origin.z },
    { e.origin.x - 15, e.origin.y + 15, e.origin.z },
    { e.origin.x - 15, e.origin.y - 15, e.origin.z + 72 },
    { e.origin.x + 15, e.origin.y - 15, e.origin.z + 72 },
    { e.origin.x + 15, e.origin.y + 15, e.origin.z + 72 },
    { e.origin.x - 15, e.origin.y + 15, e.origin.z + 72 }
    };


    Vector3 screenPoints[8];
    bool visible = false;
    for (int i = 0; i < 8; i++) {
        if (WorldToScreen(boxPoints[i], screenPoints[i], viewMatrix, screenWidth, screenHeight)) {
            visible = true;
        }
    }
    if (!visible) return;

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
    float outlineThickness = boxThickness + 2.0f;

    if (outlineEnabled) {
        for (int i = 0; i < 12; i++) {
            drawList->AddLine(
                ImVec2(screenPoints[edges[i][0]].x, screenPoints[edges[i][0]].y),
                ImVec2(screenPoints[edges[i][1]].x, screenPoints[edges[i][1]].y),
                outlineColor, outlineThickness);
        }
    }

    for (int i = 0; i < 12; i++) {
        drawList->AddLine(
            ImVec2(screenPoints[edges[i][0]].x, screenPoints[edges[i][0]].y),
            ImVec2(screenPoints[edges[i][1]].x, screenPoints[edges[i][1]].y),
            color, boxThickness);
    }
}

void RenderRadar(const LocalPlayer& lp, const std::vector<Entity>& entities, ImDrawList* drawList, float screenWidth, float screenHeight) {
    float radarSize = 150.0f;
    float radarX = screenWidth - radarSize - 10.0f;
    float radarY = 10.0f;
    float radarCenterX = radarX + radarSize / 2.0f;
    float radarCenterY = radarY + radarSize / 2.0f;
    float maxDist = 2000.0f;

    drawList->AddRectFilled(ImVec2(radarX, radarY), ImVec2(radarX + radarSize, radarY + radarSize), IM_COL32(0, 0, 0, 180));
    drawList->AddRect(ImVec2(radarX - 1, radarY - 1), ImVec2(radarX + radarSize + 1, radarY + radarSize + 1), IM_COL32(255, 255, 255, 200), 4.0f, ImDrawFlags_RoundCornersAll, 2.0f);

    drawList->AddCircleFilled(ImVec2(radarCenterX, radarCenterY), 5.0f, IM_COL32(0, 255, 0, 255), 12);

    float currentTime = ImGui::GetTime();

    // Get the view matrix to extract proper camera angles
    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);

    // Extract yaw from view matrix - this is the actual direction you're looking
    // The view matrix contains the camera's orientation in world space
    float viewYaw = atan2f(viewMatrix.m[3][0], viewMatrix.m[0][0]);
    float yawRad = -viewYaw;  // Restored negative sign to match the double axis flip
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);

    for (const auto& e : entities) {
        if (e.pawn == lp.pawn) continue;

        bool isDead = e.health <= 0;
        bool shouldRender = !isDead || (deathTimes.count(e.pawn) && currentTime - deathTimes[e.pawn] < 1.0f);
        if (!shouldRender || (!showTeammates && e.team == lp.team)) continue;

        float dx = e.origin.x - lp.origin.x;
        float dy = e.origin.y - lp.origin.y;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist > maxDist) continue;

        // Rotate based on actual camera direction
        float rotated_dx = dx * cosYaw - dy * sinYaw;
        float rotated_dy = dx * sinYaw + dy * cosYaw;

        float radarPosX = radarCenterX - (rotated_dx / maxDist) * (radarSize / 2.0f);  // Kept the flip for correct positions
        float radarPosY = radarCenterY - (rotated_dy / maxDist) * (radarSize / 2.0f);  // Flipped sign here to correct front-back orientation

        // Clamp to radar bounds
        radarPosX = std::max(radarX + 3.0f, std::min(radarX + radarSize - 3.0f, radarPosX));
        radarPosY = std::max(radarY + 3.0f, std::min(radarY + radarSize - 3.0f, radarPosY));

        ImU32 color = (e.team == lp.team) ? IM_COL32(0, 0, 255, 255) : IM_COL32(255, 0, 0, 255);

        if (outlineEnabled) {
            drawList->AddCircleFilled(ImVec2(radarPosX, radarPosY), 5.0f, IM_COL32(0, 0, 0, 255), 12);
        }
        drawList->AddCircleFilled(ImVec2(radarPosX, radarPosY), 4.0f, color, 12);
    }

    // Draw direction indicator (small line showing where you're looking)
    // Since we're already using the view matrix rotation, forward is always "up" in the radar
    float indicatorLength = 20.0f;
    float indicatorX = radarCenterX;
    float indicatorY = radarCenterY - indicatorLength;

    drawList->AddLine(
        ImVec2(radarCenterX, radarCenterY),
        ImVec2(indicatorX, indicatorY),
        IM_COL32(255, 255, 0, 255), 3.0f
    );

    // Draw a small triangle at the end of the indicator
    drawList->AddTriangleFilled(
        ImVec2(indicatorX, indicatorY),
        ImVec2(indicatorX - 3.0f, indicatorY + 6.0f),
        ImVec2(indicatorX + 3.0f, indicatorY + 6.0f),
        IM_COL32(255, 255, 0, 255)
    );
}



void RenderSoundESP(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList) {
    if (!soundESPEnabled) return;


    float currentTime = ImGui::GetTime();

    for (const auto& sound : soundLocations) {
        float alpha = 1.0f - ((currentTime - sound.second) / soundMaxTime);
        if (alpha <= 0.0f) continue;

        Vector3 screenPos;
        if (WorldToScreen(sound.first, screenPos, viewMatrix, screenWidth, screenHeight)) {
            ImU32 color = IM_COL32(
                soundESPColor.x * 255,
                soundESPColor.y * 255,
                soundESPColor.z * 255,
                (int)(alpha * 255)
            );

            float radius = 10.0f + sinf(currentTime * 5.0f) * 2.0f;
            drawList->AddCircle(
                ImVec2(screenPos.x, screenPos.y),
                radius,
                color,
                12,
                2.0f
            );

            drawList->AddText(
                ImVec2(screenPos.x + 8, screenPos.y - 8),
                color,
                "!"
            );
        }
    }
}

void RenderBacktrackESP(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList) {
    if (!backtrackESPEnabled) return;


    float currentTime = ImGui::GetTime();

    for (const auto& player : playerHistory) {
        for (const auto& history : player.second) {
            float alpha = 1.0f - ((currentTime - history.second) / backtrackDuration);

            Vector3 screenPos;
            if (WorldToScreen(history.first, screenPos, viewMatrix, screenWidth, screenHeight)) {
                drawList->AddCircle(
                    ImVec2(screenPos.x, screenPos.y),
                    3.0f,
                    IM_COL32(
                        backtrackESPColor.x * 255,
                        backtrackESPColor.y * 255,
                        backtrackESPColor.z * 255,
                        (int)(alpha * 255)
                    ),
                    8,
                    1.5f
                );
            }
        }
    }
}



void RenderVelocityIndicator(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color) {
    if (!velocityESPEnabled) return;


    Vector3 velocity = ReadMemory<Vector3>(e.pawn + 0x430);
    float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

    char speedText[32];
    sprintf_s(speedText, "%.0f u/s", speed);

    ImU32 speedColor;
    if (speed < 100.0f) speedColor = IM_COL32(0, 255, 0, 255);
    else if (speed < 200.0f) speedColor = IM_COL32(255, 255, 0, 255);
    else speedColor = IM_COL32(255, 0, 0, 255);

    DrawTextWithShadow(drawList, ImVec2(boxPos.x, boxPos.y - 35), speedColor, speedText);

    if (speed > 50.0f) {
        float angle = atan2f(velocity.y, velocity.x);
        float arrowLength = 20.0f;
        ImVec2 arrowEnd = ImVec2(
            boxPos.x + cosf(angle) * arrowLength,
            boxPos.y + sinf(angle) * arrowLength
        );

        drawList->AddLine(
            ImVec2(boxPos.x, boxPos.y),
            arrowEnd,
            speedColor,
            2.0f
        );

        drawList->AddTriangleFilled(
            arrowEnd,
            ImVec2(arrowEnd.x - 5.0f * cosf(angle + 0.5f), arrowEnd.y - 5.0f * sinf(angle + 0.5f)),
            ImVec2(arrowEnd.x - 5.0f * cosf(angle - 0.5f), arrowEnd.y - 5.0f * sinf(angle - 0.5f)),
            speedColor
        );
    }
}

void DrawMinimalCrosshair(ImDrawList* drawList, float screenWidth, float screenHeight) {
    if (!customCrosshairEnabled) return;


    ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
    ImU32 color = ImGui::ColorConvertFloat4ToU32(crosshairColor);

    float gap = crosshairSize * 0.3f;
    float length = crosshairSize;

    drawList->AddLine(
        ImVec2(center.x - length, center.y),
        ImVec2(center.x - gap, center.y),
        color, crosshairThickness
    );
    drawList->AddLine(
        ImVec2(center.x + gap, center.y),
        ImVec2(center.x + length, center.y),
        color, crosshairThickness
    );

    drawList->AddLine(
        ImVec2(center.x, center.y - length),
        ImVec2(center.x, center.y - gap),
        color, crosshairThickness
    );
    drawList->AddLine(
        ImVec2(center.x, center.y + gap),
        ImVec2(center.x, center.y + length),
        color, crosshairThickness
    );

    drawList->AddCircleFilled(center, 1.5f, color, 8);
}



void DrawCleanWatermark(ImDrawList* drawList, float screenWidth) {
    if (!watermarkEnabled) return;


    static float startTime = ImGui::GetTime();
    float currentTime = ImGui::GetTime();
    float uptime = currentTime - startTime;

    char watermarkText[256];
    sprintf_s(watermarkText, "CS2 WoW | %.1f FPS | %d Entities | %.1fs",
        ImGui::GetIO().Framerate, GetEntities().size(), uptime);

    ImVec2 textSize = ImGui::CalcTextSize(watermarkText);
    ImVec2 pos(screenWidth - textSize.x - 15.0f, 15.0f);

    drawList->AddRectFilled(
        ImVec2(pos.x - 10, pos.y - 5),
        ImVec2(pos.x + textSize.x + 10, pos.y + textSize.y + 5),
        IM_COL32(0, 0, 0, 180), 6.0f
    );

    drawList->AddRect(
        ImVec2(pos.x - 10, pos.y - 5),
        ImVec2(pos.x + textSize.x + 10, pos.y + textSize.y + 5),
        IM_COL32(255, 255, 255, 30), 6.0f, ImDrawFlags_RoundCornersAll, 1.0f
    );

    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32(0, 0, 0, 100), watermarkText);
    drawList->AddText(pos, IM_COL32(220, 220, 220, 255), watermarkText);
}

void DrawGameInfoHUD(ImDrawList* drawList, const LocalPlayer& lp, const std::vector<Entity>& entities) {
    float yOffset = 60.0f;
    float screenWidth = ImGui::GetIO().DisplaySize.x;


    if (showLocalPlayerInfo && lp.pawn) {
        char playerInfo[256];
        sprintf_s(playerInfo, "HP: %d | Armor: %d | $: ? | K/D: ?/?", lp.health, lp.armor);

        ImVec2 textSize = ImGui::CalcTextSize(playerInfo);
        ImVec2 pos(10.0f, yOffset);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(255, 255, 255, 255), playerInfo);

        yOffset += textSize.y + 10.0f;
    }

    if (showEntityCount) {
        char entityInfo[64];
        sprintf_s(entityInfo, "Entities: %d", (int)entities.size());

        ImVec2 textSize = ImGui::CalcTextSize(entityInfo);
        ImVec2 pos(10.0f, yOffset);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(200, 200, 255, 255), entityInfo);

        yOffset += textSize.y + 10.0f;
    }


    if (showPosition && lp.pawn) {
        char posInfo[128];
        sprintf_s(posInfo, "Pos: %.1f, %.1f, %.1f", lp.origin.x, lp.origin.y, lp.origin.z);

        ImVec2 textSize = ImGui::CalcTextSize(posInfo);
        ImVec2 pos(screenWidth - textSize.x - 10.0f, 40.0f);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(200, 255, 200, 255), posInfo);
    }
}

void DrawIndicators(ImDrawList* drawList, float screenWidth, float screenHeight) {
    float yOffset = screenHeight - 100.0f;
    float xOffset = 10.0f;


    if (showFovChangerIndicator && fovChangerEnabled) {
        char fovText[32];
        sprintf_s(fovText, "FOV: %.0f", fovChangerValue);

        ImVec2 textSize = ImGui::CalcTextSize(fovText);
        ImVec2 pos(xOffset, yOffset);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(100, 255, 100, 255), fovText);

        yOffset -= textSize.y + 5.0f;
    }

    if (aimbotEnabled) {
        char aimText[32] = "Aimbot: READY";
        ImVec2 textSize = ImGui::CalcTextSize(aimText);
        ImVec2 pos(xOffset, yOffset);

        ImU32 color = (GetAsyncKeyState(aimKey) & 0x8000) ? IM_COL32(255, 50, 50, 255) : IM_COL32(50, 255, 50, 255);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, color, aimText);
    }
}

void DrawKeybinds(ImDrawList* drawList, float screenWidth, float screenHeight) {
    if (!showKeybinds) return;


    float yOffset = screenHeight - 150.0f;
    float xOffset = screenWidth - 200.0f;

    const char* keybinds[] = {
        "INSERT - Menu",
        "LMB - Aimbot",
        "Mouse4 - Ext Aim",
        "SPACE - Bunnyhop"
    };

    float maxWidth = 0;
    for (const char* bind : keybinds) {
        ImVec2 textSize = ImGui::CalcTextSize(bind);
        if (textSize.x > maxWidth) maxWidth = textSize.x;
    }

    drawList->AddRectFilled(
        ImVec2(xOffset - 10, yOffset - 5),
        ImVec2(xOffset + maxWidth + 10, yOffset + (20 * 4) + 5),
        IM_COL32(0, 0, 0, 180),
        6.0f
    );

    drawList->AddRect(
        ImVec2(xOffset - 10, yOffset - 5),
        ImVec2(xOffset + maxWidth + 10, yOffset + (20 * 4) + 5),
        IM_COL32(255, 255, 255, 100),
        6.0f,
        ImDrawFlags_RoundCornersAll,
        1.0f
    );

    drawList->AddText(ImVec2(xOffset, yOffset), IM_COL32(255, 255, 100, 255), "Keybinds:");
    yOffset += 20;

    for (const char* bind : keybinds) {
        drawList->AddText(ImVec2(xOffset, yOffset), IM_COL32(255, 255, 255, 255), bind);
        yOffset += 20;
    }
}



void RenderOOFArrow(const Vector3& targetPos, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImU32 color, ImDrawList* drawList) {
    // 1. Manual WorldToScreen projection (we need 'w' even if it's negative)
    float w = viewMatrix.m[3][0] * targetPos.x + viewMatrix.m[3][1] * targetPos.y + viewMatrix.m[3][2] * targetPos.z + viewMatrix.m[3][3];
    float x = viewMatrix.m[0][0] * targetPos.x + viewMatrix.m[0][1] * targetPos.y + viewMatrix.m[0][2] * targetPos.z + viewMatrix.m[0][3];
    float y = viewMatrix.m[1][0] * targetPos.x + viewMatrix.m[1][1] * targetPos.y + viewMatrix.m[1][2] * targetPos.z + viewMatrix.m[1][3];

    // Prevent division by zero
    float wVal = (w < 0.001f && w > -0.001f) ? 0.001f : w;

    float screenX = (screenWidth / 2.0f) + (screenWidth / 2.0f) * x / wVal;
    float screenY = (screenHeight / 2.0f) - (screenHeight / 2.0f) * y / wVal;

    // 2. Calculate Direction Vector from Center
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    ImVec2 dir;
    dir.x = screenX - centerX;
    dir.y = screenY - centerY;

    // 3. Handle "Behind" targets (w < 0)
    // If w is negative, the math inverts the position. We fix this by flipping the direction.
    if (w < 0.0f) {
        dir.x *= -1.0f;
        dir.y *= -1.0f;
    }

    // 4. Normalize Direction
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length < 1.0f) return; // Too close to center to draw an arrow

    dir.x /= length;
    dir.y /= length;

    // 5. Calculate Arrow Position on Radius
    ImVec2 arrowPos = ImVec2(
        centerX + dir.x * oofArrowsRadius,
        centerY + dir.y * oofArrowsRadius
    );

    // 6. Draw Triangle (Pointing Outwards)
    float angle = atan2f(dir.y, dir.x); // Angle of the direction vector
    float size = oofArrowsSize;

    // Tip of the arrow (furthest out)
    ImVec2 p1 = ImVec2(
        arrowPos.x + size * cosf(angle),
        arrowPos.y + size * sinf(angle)
    );

    // Back Left
    ImVec2 p2 = ImVec2(
        arrowPos.x + (size * 0.6f) * cosf(angle + 2.6f), // ~150 degrees offset
        arrowPos.y + (size * 0.6f) * sinf(angle + 2.6f)
    );

    // Back Right
    ImVec2 p3 = ImVec2(
        arrowPos.x + (size * 0.6f) * cosf(angle - 2.6f), // ~-150 degrees offset
        arrowPos.y + (size * 0.6f) * sinf(angle - 2.6f)
    );

    if (outlineEnabled) {
        drawList->AddTriangle(p1, p2, p3, IM_COL32(0, 0, 0, 255), 2.0f);
    }
    drawList->AddTriangleFilled(p1, p2, p3, color);
}



void RenderHitmarkers(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList, float deltaTime) {
    for (auto it = hitmarkers.begin(); it != hitmarkers.end(); ) {
        float elapsed = static_cast<float>(ImGui::GetTime()) - it->startTime;
        if (elapsed > it->duration) {
            it = hitmarkers.erase(it);
            continue;
        }


        float alpha = 1.0f - (elapsed / it->duration);
        ImU32 color = IM_COL32(255, 255, 255, static_cast<int>(255 * alpha));

        Vector3 screenPos;
        if (WorldToScreen(it->worldPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
            float offsetY = -hitmarkerSpeed * elapsed;
            ImVec2 textPos(screenPos.x, screenPos.y + offsetY);
            char damageText[16];
            sprintf_s(damageText, "-%d", it->damage);
            DrawTextWithShadow(drawList, textPos, color, damageText);
        }

        ++it;
    }
}

void RenderExternalAimbotFOV(ImDrawList* drawList, float screenWidth, float screenHeight) {
    if (!externalAimbotEnabled) return;


    if (!IsGameInFocus()) return;

    float fovPixels = (externalAimbotFOV / 90.0f) * (screenWidth / 2.0f);

    ImU32 circleColor;
    if (GetAsyncKeyState(externalAimbotKey) && !menuOpen) {
        circleColor = IM_COL32(255, 0, 0, 100);
    }
    else {
        circleColor = IM_COL32(0, 255, 0, 80);
    }

    ImU32 outlineColor = IM_COL32(0, 0, 0, 150);

    ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);

    drawList->AddCircle(center, fovPixels + 1.0f, outlineColor, 0, 2.0f);
    drawList->AddCircle(center, fovPixels, circleColor, 0, 1.5f);

    float crossSize = 8.0f;
    drawList->AddLine(ImVec2(center.x - crossSize, center.y), ImVec2(center.x + crossSize, center.y), circleColor, 1.0f);
    drawList->AddLine(ImVec2(center.x, center.y - crossSize), ImVec2(center.x, center.y + crossSize), circleColor, 1.0f);
}

void DrawTextWithShadow(ImDrawList* drawList, ImVec2 pos, ImU32 color, const char* text) {
    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32(0, 0, 0, 255), text);
    drawList->AddText(pos, color, text);
}

void DrawWireframeBone(ImDrawList* drawList, const Vector3& start3D, const Vector3& end3D, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImU32 color, float radius, int numSides, int numRings) {
    Vector3 dir = end3D - start3D;
    float length = dir.Length();
    if (length < 0.001f) return;
    dir = dir.Normalize();

    // Find perpendicular vectors
    Vector3 perp1;
    if (std::fabs(dir.x) > 0.5f) {
        perp1 = Vector3(dir.y, -dir.x, 0).Normalize();
    }
    else {
        perp1 = Vector3(0, dir.z, -dir.y).Normalize();
    }
    Vector3 perp2 = dir.Cross(perp1);

    // Draw rings
    for (int r = 0; r <= numRings; ++r) {
        float t = static_cast<float>(r) / numRings;
        Vector3 ringPos = start3D + dir * (length * t);
        Vector3 prevPt;
        bool first = true;
        for (int s = 0; s <= numSides; ++s) {
            float angle = static_cast<float>(s) / numSides * 2.0f * 3.1415926535f;
            Vector3 offset = perp1 * (std::cos(angle) * radius) + perp2 * (std::sin(angle) * radius);
            Vector3 pt3D = ringPos + offset;
            Vector3 pt2D;
            if (!WorldToScreen(pt3D, pt2D, viewMatrix, screenWidth, screenHeight)) continue;
            if (!first) {
                drawList->AddLine(ImVec2(prevPt.x, prevPt.y), ImVec2(pt2D.x, pt2D.y), color, 1.0f);
            }
            prevPt = pt2D;
            first = false;
        }
    }

    // Draw longitudinal lines
    for (int s = 0; s < numSides; ++s) {
        float angle = static_cast<float>(s) / numSides * 2.0f * 3.1415926535f;
        Vector3 offset = perp1 * (std::cos(angle) * radius) + perp2 * (std::sin(angle) * radius);
        Vector3 startPt = start3D + offset;
        Vector3 endPt = end3D + offset;
        Vector3 screenStart, screenEnd;
        if (WorldToScreen(startPt, screenStart, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(endPt, screenEnd, viewMatrix, screenWidth, screenHeight)) {
            drawList->AddLine(ImVec2(screenStart.x, screenStart.y), ImVec2(screenEnd.x, screenEnd.y), color, 1.0f);
        }
    }
}

void DrawWireframeSphere(ImDrawList* drawList, const Vector3& center, float radius, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImU32 color, int numMeridians, int numParallels) {
    // Latitudes (parallels - horizontal circles)
    for (int p = 0; p <= numParallels; ++p) {
        float phi = static_cast<float>(p) / numParallels * 3.1415926535f;  // 0 to pi
        float r_sin_phi = radius * std::sin(phi);
        float z = radius * std::cos(phi);  // Z up (height)
        Vector3 prevPt;
        bool first = true;
        for (int m = 0; m <= numMeridians; ++m) {
            float theta = static_cast<float>(m) / numMeridians * 2.0f * 3.1415926535f;
            float x = r_sin_phi * std::cos(theta);
            float y = r_sin_phi * std::sin(theta);
            Vector3 pt3D = center + Vector3{ x, y, z };  // CS2: X side, Y forward, Z up
            Vector3 pt2D;
            if (!WorldToScreen(pt3D, pt2D, viewMatrix, screenWidth, screenHeight)) continue;
            if (!first) {
                drawList->AddLine(ImVec2(prevPt.x, prevPt.y), ImVec2(pt2D.x, pt2D.y), color, 1.0f);
            }
            prevPt = pt2D;
            first = false;
        }
    }

    // Longitudes (meridians - vertical arcs)
    for (int m = 0; m < numMeridians; ++m) {
        float theta = static_cast<float>(m) / numMeridians * 2.0f * 3.1415926535f;
        Vector3 prevPt;
        bool first = true;
        for (int p = 0; p <= numParallels; ++p) {
            float phi = static_cast<float>(p) / numParallels * 3.1415926535f;
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::sin(phi) * std::sin(theta);
            float z = radius * std::cos(phi);
            Vector3 pt3D = center + Vector3{ x, y, z };
            Vector3 pt2D;
            if (!WorldToScreen(pt3D, pt2D, viewMatrix, screenWidth, screenHeight)) continue;
            if (!first) {
                drawList->AddLine(ImVec2(prevPt.x, prevPt.y), ImVec2(pt2D.x, pt2D.y), color, 1.0f);
            }
            prevPt = pt2D;
            first = false;
        }
    }
}

void RenderESP() {
    if (!clientBase) {
        return;
    }

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn) {
        ImGuiIO& io = ImGui::GetIO();
        auto drawList = ImGui::GetBackgroundDrawList();
        drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(200, 50),
            IM_COL32(0, 0, 0, 200), IM_COL32(0, 0, 0, 200), IM_COL32(20, 20, 20, 200), IM_COL32(20, 20, 20, 200));
        drawList->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "CS2 WoW Cheat v1.0");
        drawList->AddText(ImVec2(10, 30), IM_COL32(200, 200, 200, 255), "In Menu - Waiting for Game");
        return;
    }

    static float lastShotTime = 0.0f;
    static int prevClipAmmo = 999;  // High initial to detect first shot
    


    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;
    float deltaTime = io.DeltaTime;

    auto drawList = ImGui::GetBackgroundDrawList();

    RenderExternalAimbotFOV(drawList, screenWidth, screenHeight);

    // Draw header info
    drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(200, 50),
        IM_COL32(0, 0, 0, 200), IM_COL32(0, 0, 0, 200), IM_COL32(20, 20, 20, 200), IM_COL32(20, 20, 20, 200));
    drawList->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "CS2 WoW Cheat v1.0");
    char fpsText[32];
    sprintf_s(fpsText, "FPS: %.1f | Entities: %d", ImGui::GetIO().Framerate, GetEntities().size());
    drawList->AddText(ImVec2(10, 30), IM_COL32(200, 200, 200, 255), fpsText);

    auto entities = GetEntities();
    ApplyFeatures(lp, entities);

    if (IsGameInFocus() && !menuOpen) {
        RunAimbot();
        RunExternalAimbot();
        AdvancedBulletRedirection();
    }

    RunTriggerbot(lp, entities);
    if (showSpectatorList) {
        RenderSpectatorListWindow();
    }

    if (showDebugConsole) {
        static float lastUpdate = 0.0f;
        float now = ImGui::GetTime();
        if (now - lastUpdate > 0.5f) {
            system("cls");
            std::cout << "[DEBUG] GetLocalPlayer: origin=("
                << lp.origin.x << "," << lp.origin.y << "," << lp.origin.z
                << "), team=" << lp.team
                << ", index=" << lp.index
                << ", health=" << lp.health
                << ", viewAngles=(" << lp.viewAngles.x << "," << lp.viewAngles.y << ")"
                << std::endl;
            for (size_t i = 0; i < entities.size(); ++i) {
                const auto& e = entities[i];
                std::cout << "[DEBUG] Entity " << i << ": health=" << e.health
                    << ", armor=" << e.armor
                    << ", ammo=" << e.ammo
                    << ", origin=(" << e.origin.x << "," << e.origin.y << "," << e.origin.z << ")"
                    << ", team=" << e.team
                    << ", name=" << e.name
                    << std::endl;
            }
            lastUpdate = now;
        }
    }

    RenderSoundESP(viewMatrix, screenWidth, screenHeight, drawList);
    RenderBacktrackESP(viewMatrix, screenWidth, screenHeight, drawList);



    static std::map<uintptr_t, int> previousHealth;
    static std::map<uintptr_t, int> previousHitGroup;

    float currentTime = ImGui::GetTime();

    // Clean up death times
    for (auto it = deathTimes.begin(); it != deathTimes.end(); ) {
        bool found = false;
        for (const auto& e : entities) {
            if (e.pawn == it->first) {
                found = true;
                break;
            }
        }
        if (!found) {
            it = deathTimes.erase(it);
        }
        else {
            ++it;
        }
    }

    ImU32 outlineCol = IM_COL32(0, 0, 0, 155);

    // SINGLE PASS entity rendering - removed duplicate loop
    for (auto& e : entities) {
        if (!espEnabled) continue;

        bool isDead = e.health <= 0;
        bool shouldRender = !isDead || (deathTimes.count(e.pawn) && currentTime - deathTimes[e.pawn] < 1.0f);
        if (!shouldRender || (!showTeammates && e.team == lp.team)) continue;

        ImU32 boxColor = (e.team == lp.team) ? IM_COL32(teammateColor.x * 255, teammateColor.y * 255, teammateColor.z * 255, 255)
            : (e.visible ? IM_COL32(visibleColor.x * 255, visibleColor.y * 255, visibleColor.z * 255, 255)
                : IM_COL32(espColor.x * 255, espColor.y * 255, espColor.z * 255, 255));

        Vector3 footPos = e.origin;
        Vector3 headPos = GetBonePosition(e.boneMatrix, bone_head);
        headPos.z += 5.0f;
        Vector3 screenFoot, screenHead;

        bool onScreenHead = WorldToScreen(headPos, screenHead, viewMatrix, screenWidth, screenHeight);
        bool onScreenFoot = WorldToScreen(footPos, screenFoot, viewMatrix, screenWidth, screenHeight);

        // --- OOF ARROW LOGIC STARTS HERE ---
        int currentFov = 90;
        if (lp.pawn) {
            uintptr_t cameraServices = ReadMemory<uintptr_t>(lp.pawn + m_pCameraServicesOffset);
            if (cameraServices) {
                int readFov = ReadMemory<int>(cameraServices + m_iFOVOffset);
                if (readFov > 0) currentFov = readFov;
            }
        }

        // 2. Calculate Margin in Pixels
        // We convert "10 degrees" into screen pixels based on your resolution and FOV.
        float pxPerDegree = screenWidth / static_cast<float>(currentFov);
        float marginPx = oofMarginDegrees * pxPerDegree;

        // 3. Determine if target is "Off Screen" OR "Within Margin"
        // We check if the head position is strictly off-screen, OR if it is too close to the edge.
        bool isOffScreen = !onScreenHead || !onScreenFoot;
        bool inMargin = false;

        if (!isOffScreen) { // Only check margin if they are technically on screen
            if (screenHead.x < marginPx || screenHead.x >(screenWidth - marginPx) ||
                screenHead.y < marginPx || screenHead.y >(screenHeight - marginPx)) {
                inMargin = true;
            }
        }

        // --- OOF ARROW LOGIC ---
        if (oofArrowsEnabled && (isOffScreen || inMargin)) {
            ImU32 arrowColor = boxColor;

            // Distance Fade
            if (oofArrowsDistanceColor) {
                float dist = (e.origin - lp.origin).Length();
                float alpha = 1.0f - std::clamp((dist - 500.0f) / 2000.0f, 0.0f, 0.6f);
                arrowColor = (arrowColor & 0x00FFFFFF) | ((int)(255 * alpha) << 24);
            }

            // Use Matrix-based arrow rendering (Perfect accuracy)
            Vector3 targetPos = e.origin;
            targetPos.z += 40.0f;

            RenderOOFArrow(targetPos, viewMatrix, screenWidth, screenHeight, arrowColor, drawList);

            // SKIP normal box ESP if we drew an arrow? 
            // Usually you want to skip drawing the box if the arrow is active to avoid clutter.
            // Uncomment the line below if you want the Box to disappear when the Arrow appears.
            // continue; 
        }

        if (onScreenFoot && onScreenHead) {

            float height = screenFoot.y - screenHead.y;
            float width = height / 2.0f;
            float centerX = (screenHead.x + screenFoot.x) / 2.0f;
            float x = centerX - width / 2.0f;
            float boxTop = screenHead.y;
            float boxBottom = screenFoot.y;

            if (height < 10.0f || height > 500.0f) continue;
            if (width < 5.0f || width > 250.0f) continue;

            // BOX ESP (2D or 3D)
            if (box2DEnabled || box3DEnabled) {
                if (box3DEnabled) {
                    Render3DBox(e, viewMatrix, screenWidth, screenHeight, drawList, boxColor);
                }
                else {
                    // 2D Box
                    if (outlineEnabled) {
                        drawList->AddRect(ImVec2(x - 1, boxTop - 1), ImVec2(x + width + 1, boxBottom + 1),
                            outlineCol, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness + 1.0f);
                    }
                    drawList->AddRect(ImVec2(x, boxTop), ImVec2(x + width, boxBottom), boxColor,
                        boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
                }
            }

            RenderBulletTracers(viewMatrix, screenWidth, screenHeight, drawList);

            RenderHitmarkers(viewMatrix, screenWidth, screenHeight, drawList, deltaTime);

            // NAME ESP
            if (nameESPEnabled) {
                DrawTextWithShadow(drawList, ImVec2(x, boxTop - 20), boxColor, e.name);
            }

            if (weaponESPEnabled && e.weaponName[0] != '\0') {
                // Calculate Y position: Start at bottom
                float textY = boxBottom + 5;

                // If distance text is enabled, move weapon text below it
                if (distanceTextEnabled) {
                    textY += 12;
                }

                // If flags are enabled, move weapon text further down (rough estimation)
                if (flagsEnabled) {
                    textY += 12;
                }

                DrawTextWithShadow(drawList, ImVec2(x, textY), boxColor, e.weaponName);
            }


            // HEALTH BAR & TEXT
            if (healthBarEnabled || healthTextEnabled) {
                auto dispIt = displayedHealth.find(e.pawn);
                if (dispIt == displayedHealth.end()) {
                    displayedHealth[e.pawn] = static_cast<float>(e.health);
                }
                else if (dispIt->second > e.health) {
                    dispIt->second -= healthAnimationSpeed * deltaTime;
                    if (dispIt->second < e.health) dispIt->second = static_cast<float>(e.health);
                }
                else {
                    dispIt->second = static_cast<float>(e.health);
                }
                float animatedHealth = dispIt->second;
                float healthFrac = animatedHealth / 100.0f;

                float barWidth = 6.0f;
                ImU32 healthColor = IM_COL32(255 * (1.0f - healthFrac), 255 * healthFrac, 0, 255);
                ImVec2 healthBgMin(x - barWidth - 3, boxTop);
                ImVec2 healthBgMax(x - 3, boxBottom);
                ImVec2 healthFillMin(x - barWidth - 3, screenHead.y + height * (1.0f - healthFrac));

                if (healthBarEnabled) {
                    if (outlineEnabled) {
                        drawList->AddRect(ImVec2(healthBgMin.x - 1, healthBgMin.y - 1), ImVec2(healthBgMax.x + 1, healthBgMax.y + 1),
                            outlineCol, 2.0f, ImDrawFlags_RoundCornersAll, 1.0f);
                    }
                    drawList->AddRectFilled(healthBgMin, healthBgMax, IM_COL32(30, 30, 30, 255), 2.0f, ImDrawFlags_RoundCornersAll);
                    drawList->AddRectFilled(healthFillMin, healthBgMax, healthColor, 2.0f, ImDrawFlags_RoundCornersAll);
                }

                if (healthTextEnabled) {
                    char healthText[16];
                    sprintf_s(healthText, "%d HP", e.health);
                    DrawTextWithShadow(drawList, ImVec2(x - barWidth - 35, screenHead.y), healthColor, healthText);
                }
            }

            // ARMOR BAR
            if (armorEnabled && e.armor > 0) {
                float barWidth = 6.0f;
                float armorFrac = static_cast<float>(e.armor) / 100.0f;
                ImU32 armorColor = IM_COL32(0, 100, 255, 255);
                ImVec2 armorBgMin(x + width + 3, boxTop);
                ImVec2 armorBgMax(x + width + barWidth + 3, boxBottom);
                ImVec2 armorFillMin(x + width + 3, screenHead.y + height * (1.0f - armorFrac));

                if (outlineEnabled) {
                    drawList->AddRect(ImVec2(armorBgMin.x - 1, armorBgMin.y - 1), ImVec2(armorBgMax.x + 1, armorBgMax.y + 1),
                        outlineCol, 2.0f, ImDrawFlags_RoundCornersAll, 1.0f);
                }
                drawList->AddRectFilled(armorBgMin, armorBgMax, IM_COL32(30, 30, 30, 255), 2.0f, ImDrawFlags_RoundCornersAll);
                drawList->AddRectFilled(armorFillMin, armorBgMax, armorColor, 2.0f, ImDrawFlags_RoundCornersAll);
            }

            // DISTANCE TEXT
            if (distanceTextEnabled) {
                float dist = sqrtf(powf(lp.origin.x - e.origin.x, 2) +
                    powf(lp.origin.y - e.origin.y, 2) +
                    powf(lp.origin.z - e.origin.z, 2)) / 100.0f;
                char distText[32];
                sprintf_s(distText, "%.0fm", dist);
                DrawTextWithShadow(drawList, ImVec2(x, boxBottom + 10), boxColor, distText);
            }

            // SNAPLINES
            if (snaplinesEnabled) {
                ImU32 snapColor = IM_COL32(snaplineColor.x * 255, snaplineColor.y * 255,
                    snaplineColor.z * 255, snaplineColor.w * 255);
                ImVec2 from = ImVec2(screenWidth / 2.0f, screenHeight);
                ImVec2 to = ImVec2(x + width / 2.0f, boxBottom);

                if (outlineEnabled) {
                    drawList->AddLine(from, to, outlineCol, snaplineThickness + 1.0f);
                }
                drawList->AddLine(from, to, snapColor, snaplineThickness);
            }

            // HEAD CIRCLE
            if (headCircleEnabled && e.boneMatrix) {
                Vector3 headBonePos = GetBonePosition(e.boneMatrix, bone_head);
                Vector3 screenHeadBone;
                if (WorldToScreen(headBonePos, screenHeadBone, viewMatrix, screenWidth, screenHeight)) {
                    ImU32 headCol = IM_COL32(headCircleColor.x * 255, headCircleColor.y * 255, headCircleColor.z * 255, headCircleColor.w * 255);
                    ImU32 headOutline = IM_COL32(0, 0, 0, 255);
                    float radius = width / 6;
                    if (outlineEnabled) {
                        drawList->AddCircle(ImVec2(screenHeadBone.x, screenHeadBone.y), radius + 1.0f, headOutline, 0, headCircleThickness + 1.0f);
                    }
                    drawList->AddCircle(ImVec2(screenHeadBone.x, screenHeadBone.y), radius, headCol, 0, headCircleThickness);
                }
            }

            // FLAGS
            if (flagsEnabled) {
                std::string flagsStr;
                if (e.hasHelmet) flagsStr += "H ";
                if (e.armor > 0) flagsStr += "K ";
                if (e.hasDefuser) flagsStr += "KIT ";
                if (e.scoped) flagsStr += "SCOPED ";
                if (e.flashDuration > 0) flagsStr += "FLASHED ";
                if (e.defusing) flagsStr += "DEFUSING ";
                if (!flagsStr.empty()) {
                    DrawTextWithShadow(drawList, ImVec2(x, boxBottom + 25), boxColor, flagsStr.c_str());
                }
            }

            // TRACERS
            if (tracersEnabled && e.boneMatrix) {
                Vector3 headBonePos = GetBonePosition(e.boneMatrix, bone_head);
                Vector3 screenHeadBone;
                if (WorldToScreen(headBonePos, screenHeadBone, viewMatrix, screenWidth, screenHeight)) {
                    ImU32 traceCol = IM_COL32(tracerColor.x * 255, tracerColor.y * 255, tracerColor.z * 255, tracerColor.w * 255);
                    ImU32 traceOutline = IM_COL32(0, 0, 0, 255);
                    ImVec2 from = ImVec2(screenWidth / 2.0f, screenHeight / 2.0f);
                    if (outlineEnabled) {
                        drawList->AddLine(from, ImVec2(screenHeadBone.x, screenHeadBone.y), traceOutline, tracerThickness + 1.0f);
                    }
                    drawList->AddLine(from, ImVec2(screenHeadBone.x, screenHeadBone.y), traceCol, tracerThickness);
                }
            }

            // ADVANCED FLAGS & VELOCITY INDICATOR
            RenderVelocityIndicator(e, ImVec2(x, boxBottom + 25), drawList, boxColor);

            // BONE ESP
            if (boneESPEnabled && e.boneMatrix) {
                if (wireframeBoneESPEnabled) {
                    ImU32 wireColor = ImGui::ColorConvertFloat4ToU32(e.visible ? wireframeVisibleColor : wireframeInvisibleColor);
                    std::map<int, float> boneRadii = {
                        {bone_pelvis, pelvisWireRadius},
                        {bone_spine_0, spine0WireRadius},
                        {bone_spine_1, spine1WireRadius},
                        {bone_spine_2, spine2WireRadius},
                        {bone_spine_3, spine3WireRadius},
                        {bone_neck, neckWireRadius},
                        {bone_head, headWireRadius},
                        {bone_clavicle_L, clavicleLWireRadius},
                        {bone_upperarm_L, upperarmLWireRadius},
                        {bone_forearm_L, forearmLWireRadius},
                        {bone_hand_L, handLWireRadius},
                        {bone_clavicle_R, clavicleRWireRadius},
                        {bone_upperarm_R, upperarmRWireRadius},
                        {bone_forearm_R, forearmRWireRadius},
                        {bone_hand_R, handRWireRadius},
                        {bone_thigh_L, thighLWireRadius},
                        {bone_calf_L, calfLWireRadius},
                        {bone_foot_L, footLWireRadius},
                        {bone_thigh_R, thighRWireRadius},
                        {bone_calf_R, calfRWireRadius},
                        {bone_foot_R, footRWireRadius}
                    };

                    for (const auto& conn : boneConnections) {
                        Vector3 boneA = GetBonePosition(e.boneMatrix, conn.first);
                        Vector3 boneB = GetBonePosition(e.boneMatrix, conn.second);
                        float radA = boneRadii.count(conn.first) ? boneRadii[conn.first] : 5.0f;
                        float radB = boneRadii.count(conn.second) ? boneRadii[conn.second] : 5.0f;
                        float rad = (radA + radB) / 2.0f;
                        DrawWireframeBone(drawList, boneA, boneB, viewMatrix, screenWidth, screenHeight, wireColor, rad, wireframeNumSides, wireframeNumRings);
                    }

                    Vector3 headPos = GetBonePosition(e.boneMatrix, bone_head);
                    float headRad = boneRadii[bone_head];
                    DrawWireframeSphere(drawList, headPos, headRad, viewMatrix, screenWidth, screenHeight, wireColor, wireframeNumSides, wireframeNumRings * 2);
                }

                ImU32 skelOutline = IM_COL32(0, 0, 0, 255);

                // Calculate distance for fade if enabled
                float distAlpha = 1.0f;
                if (espDistanceFade) {
                    float dist = sqrtf(powf(lp.origin.x - e.origin.x, 2) + powf(lp.origin.y - e.origin.y, 2) + powf(lp.origin.z - e.origin.z, 2));
                    distAlpha = 1.0f - std::clamp((dist - 100.0f) / espMaxDistance, 0.0f, 1.0f);
                }

                for (const auto& conn : boneConnections) {
                    Vector3 boneFrom = GetBonePosition(e.boneMatrix, conn.first);
                    Vector3 boneTo = GetBonePosition(e.boneMatrix, conn.second);
                    Vector3 screenFrom, screenTo;
                    if (WorldToScreen(boneFrom, screenFrom, viewMatrix, screenWidth, screenHeight) &&
                        WorldToScreen(boneTo, screenTo, viewMatrix, screenWidth, screenHeight)) {

                        ImVec2 from(screenFrom.x, screenFrom.y);
                        ImVec2 to(screenTo.x, screenTo.y);

                        switch (skeletonStyle) {
                        case SKELETON_STANDARD: {
                            ImU32 skelColor = IM_COL32(skeletonColor.x * 255 * distAlpha, skeletonColor.y * 255 * distAlpha, skeletonColor.z * 255 * distAlpha, skeletonColor.w * 255 * distAlpha);
                            if (outlineEnabled) {
                                drawList->AddLine(from, to, skelOutline, skeletonThickness + 1.0f);
                            }
                            drawList->AddLine(from, to, skelColor, skeletonThickness);
                            break;
                        }

                        case SKELETON_DOTTED: {
                            ImU32 skelColor = IM_COL32(skeletonColor.x * 255 * distAlpha, skeletonColor.y * 255 * distAlpha, skeletonColor.z * 255 * distAlpha, skeletonColor.w * 255 * distAlpha);
                            // Draw dotted line: alternate segments
                            ImVec2 dir = ImVec2(to.x - from.x, to.y - from.y);
                            float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
                            ImVec2 norm = ImVec2(dir.x / len, dir.y / len);
                            float dotSize = 2.0f; // Size of each dot
                            float gapSize = 2.0f; // Gap between dots
                            for (float d = 0; d < len; d += dotSize + gapSize) {
                                ImVec2 start = ImVec2(from.x + norm.x * d, from.y + norm.y * d);
                                ImVec2 end = ImVec2(start.x + norm.x * dotSize, start.y + norm.y * dotSize);
                                if (outlineEnabled) {
                                    drawList->AddLine(start, end, skelOutline, skeletonThickness + 1.0f);
                                }
                                drawList->AddLine(start, end, skelColor, skeletonThickness);
                            }
                            break;
                        }

                        case SKELETON_GRADIENT: {
                            // Interpolate color from start (top) to end (bottom) based on bone Y position
                            float fromY = boneFrom.y; // Use world Y for vertical gradient
                            float toY = boneTo.y;
                            float minY = std::min(fromY, toY);
                            float maxY = std::max(fromY, toY);
                            float rangeY = maxY - minY;
                            if (rangeY < 0.001f) rangeY = 1.0f; // Avoid division by zero

                            // Gradient from skeletonStartColor (higher Y) to skeletonEndColor (lower Y)
                            ImU32 startCol = IM_COL32(skeletonStartColor.x * 255 * distAlpha, skeletonStartColor.y * 255 * distAlpha, skeletonStartColor.z * 255 * distAlpha, skeletonStartColor.w * 255 * distAlpha);
                            ImU32 endCol = IM_COL32(skeletonEndColor.x * 255 * distAlpha, skeletonEndColor.y * 255 * distAlpha, skeletonEndColor.z * 255 * distAlpha, skeletonEndColor.w * 255 * distAlpha);

                            // For outline, use a simple black outline on the entire line (no gradient for outline)
                            if (outlineEnabled) {
                                drawList->AddLine(from, to, skelOutline, skeletonThickness + 1.0f);
                            }

                            // Draw gradient line using multi-color (approximate with AddLine for simplicity, or segment if needed)
                            // For true gradient, segment the line
                            int segments = 10; // Number of segments for smooth gradient
                            for (int i = 0; i < segments; ++i) {
                                float t1 = (float)i / segments;
                                float t2 = (float)(i + 1) / segments;
                                ImVec2 p1 = ImVec2(from.x + (to.x - from.x) * t1, from.y + (to.y - from.y) * t1);
                                ImVec2 p2 = ImVec2(from.x + (to.x - from.x) * t2, from.y + (to.y - from.y) * t2);

                                // Interpolate color based on average Y of segment
                                float avgY = (minY + rangeY * (t1 + t2) / 2.0f);
                                float interp = (maxY - avgY) / rangeY; // 1.0 at top, 0.0 at bottom
                                ImU32 segCol = IM_COL32(
                                    (int)((skeletonStartColor.x * interp + skeletonEndColor.x * (1.0f - interp)) * 255 * distAlpha),
                                    (int)((skeletonStartColor.y * interp + skeletonEndColor.y * (1.0f - interp)) * 255 * distAlpha),
                                    (int)((skeletonStartColor.z * interp + skeletonEndColor.z * (1.0f - interp)) * 255 * distAlpha),
                                    (int)((skeletonStartColor.w * interp + skeletonEndColor.w * (1.0f - interp)) * 255 * distAlpha)
                                );

                                drawList->AddLine(p1, p2, segCol, skeletonThickness);
                            }
                            break;
                        }
                        }
                    }
                }
            }

            // HITSOUNDS
            if (hitsoundEnabled && e.team != lp.team) {
                auto healthIt = previousHealth.find(e.pawn);
                if (healthIt == previousHealth.end()) {
                    previousHealth[e.pawn] = e.health;
                    previousHitGroup[e.pawn] = e.lastHitGroup;
                    continue;
                }
                int prevHealth = healthIt->second;
                int prevHitGroup = previousHitGroup[e.pawn];

                if (prevHealth > e.health) {  // Damage detected
                    float currentTime = ImGui::GetTime();
                    float timeSinceShot = currentTime - lastShotTime;
                    if (timeSinceShot < 0.8f) {  // Wider: 800ms (covers high ping)
                        // YOUR HIT! Trigger sounds/chat/etc.
                        int damage = prevHealth - e.health;
                        bool isHeadshot = (e.lastHitGroup == 1);  // Head = 1 in CS2
                        bool isKill = (e.health <= 0);

                        if (isKill) {
                            PlayMP3(killSoundPath);
                            debugLog += "[KILL] YOU killed " + std::string(e.name) + "\n";
                            AddNotification("Enemy Killed: " + std::string(e.name), 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                            deathTimes[e.pawn] = currentTime;
                        }
                        else {
                            if (isHeadshot) {
                                PlayMP3(headSoundPath);
                                debugLog += "[HEADSHOT] YOU headshot " + std::string(e.name) + " (-" + std::to_string(damage) + ")\n";
                                AddNotification("Headshot: -" + std::to_string(damage), 2.0f, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
                            }
                            else {
                                PlayMP3(hitSoundPath);
                                debugLog += "[HIT] YOU hit " + std::string(e.name) + " (-" + std::to_string(damage) + ")\n";
                                AddNotification("Hit: -" + std::to_string(damage), 2.0f, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                            }
                            hitmarkers.push_back({ GetBonePosition(e.boneMatrix, bone_head), damage, currentTime, hitmarkerFadeDuration });
                            if (clientChatEnabled) {
                                chat::builder msg;
                                ChatColor accent(0.2f, 0.8f, 1.0f);   // Cyan accent
                                ChatColor white(1.0f, 1.0f, 1.0f);
                                ChatColor gray(0.8f, 0.8f, 0.8f);
                                ChatColor green(0.6f, 1.0f, 0.68f);
                                ChatColor yellow(1.0f, 0.8f, 0.2f);
                                ChatColor red(1.0f, 0.5f, 0.5f);

                                msg << accent << lp.name << white << "]: ";
                                if (isKill) {
                                    msg << "Killed " << gray << e.name << white << ".";
                                }
                                else {
                                    msg << "Hit " << gray << e.name << white << " for "
                                        << (isHeadshot ? yellow : green) << damage << white << " hp";
                                }
                                chat::sendMessage(msg.get());
                            }
                        }
                    }
                    // Always update previous values (outside the if, so tracking continues)
                    previousHealth[e.pawn] = e.health;
                    previousHitGroup[e.pawn] = e.lastHitGroup;
                }
            }
        }
    }

    // Clean up previousHealth map
    for (auto it = previousHealth.begin(); it != previousHealth.end(); ) {
        bool found = false;
        for (const auto& e : entities) {
            if (e.pawn == it->first) {
                found = true;
                break;
            }
        }
        if (!found) {
            it = previousHealth.erase(it);
        }
        else {
            ++it;
        }
    }

    // Clean up displayedHealth
    for (auto it = displayedHealth.begin(); it != displayedHealth.end(); ) {
        bool found = false;
        for (const auto& e : entities) {
            if (e.pawn == it->first) {
                found = true;
                break;
            }
        }
        if (!found) {
            it = displayedHealth.erase(it);
        }
        else {
            ++it;
        }
    }

    // Render radar
    if (radarEnabled) {
        RenderRadar(lp, entities, drawList, screenWidth, screenHeight);
    }

    // FOV Circle
    if (aimbotEnabled && fovCircleEnabled) {
        float fovPixels = (aimbotFOV / 90.0f) * (screenWidth / 2.0f);
        ImU32 fovCol = IM_COL32(fovCircleColor.x * 255, fovCircleColor.y * 255, fovCircleColor.z * 255, fovCircleColor.w * 255);
        ImU32 fovOutline = IM_COL32(0, 0, 0, 255);
        if (outlineEnabled) {
            drawList->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), fovPixels + 1.0f, fovOutline, 0, boxThickness + 1.0f);
        }
        drawList->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), fovPixels, fovCol, 0, boxThickness);
    }

    // UI Elements
    DrawMinimalCrosshair(drawList, screenWidth, screenHeight);
    DrawCleanWatermark(drawList, screenWidth);
    DrawGameInfoHUD(drawList, lp, entities);
    DrawIndicators(drawList, screenWidth, screenHeight);
    DrawKeybinds(drawList, screenWidth, screenHeight);

    // Render hitmarkers last
    RenderHitmarkers(viewMatrix, screenWidth, screenHeight, drawList, deltaTime);
}