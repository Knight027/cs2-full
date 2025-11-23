#define NOMINMAX
#include "cheat.h"
#include <unordered_set>


void RecordAimKey() {
    if (recordingAimKey) {
        for (int i = 1; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                if (i != VK_INSERT && i != VK_END && i != VK_HOME) {
                    aimKey = i;
                    recordingAimKey = false;
                    keyRecorded = true;
                    debugLog += "[AIMBOT] Key recorded: " + std::to_string(i) + "\n";
                    AddNotification("Aim key set to: " + std::to_string(i), 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                    break;
                }
            }
        }
    }
}

void ApplyGlowV2(uintptr_t entity, bool isTeammate, bool isVisible) {
    if (!glowV2Enabled || !IsValidPtr((void*)entity)) return;

    uintptr_t glowBase = entity + 0xCB0; // From thread, adjust if needed
    if (!IsValidPtr((void*)glowBase)) return;

    // Set color at fixed offset (assume 0x40 for m_glowColorOverride)
    uint32_t color = (static_cast<uint8_t>(glowV2Color.w * 255) << 24) |
        (static_cast<uint8_t>(glowV2Color.x * 255) << 16) |
        (static_cast<uint8_t>(glowV2Color.y * 255) << 8) |
        static_cast<uint8_t>(glowV2Color.z * 255);
    WriteMemory<uint32_t>(glowBase + 0x40, color);

    // Set glowing using dynamic offset
    WriteMemory<bool>(glowBase + offsetToIsGlowing, true);

    // Optional: Set glow type if needed, assume 3 at 0x30
    WriteMemory<int>(glowBase + 0x30, 3);

    // Note: Scene offsets not used in this basic implementation
    // If needed, can add logic to access glow scene object via m_pGameSceneNode
}

void RemoveGlow(uintptr_t entity) {
    if (!IsValidPtr((void*)entity)) return;


    uintptr_t glowBase = entity + 0xCB0;
    if (!IsValidPtr((void*)glowBase)) return;

    uintptr_t glowEnable = glowBase + 0x51;
    if (IsValidPtr((void*)glowEnable)) {
        WriteMemory<bool>(glowEnable, false);
    }
}



// [Add Helper Function for Math]
void AngleVectors(const Vector3& angles, Vector3& forward) {
    float sp, sy, cp, cy;

    // CS2 view angles are in degrees
    float radPitch = ToRadians(angles.x);
    float radYaw = ToRadians(angles.y);

    sp = sin(radPitch);
    cp = cos(radPitch);
    sy = sin(radYaw);
    cy = cos(radYaw);

    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
}

void UpdateBulletTracers(const LocalPlayer& lp) {
    // Use your specific offset
    uintptr_t m_pBulletServicesOffset = 0x1678;

    if (!bulletTracersEnabled || !lp.pawn) return;

    // 1. Read Bullet Services
    uintptr_t bulletServices = ReadMemory<uintptr_t>(lp.pawn + m_pBulletServicesOffset);
    if (!bulletServices) return;

    // 2. Read Total Shots Fired (Offset 0x20)
    // We use a static variable to keep track of the previous count
    static int lastShotCount = -1;
    int currentShotCount = ReadMemory<int>(bulletServices + 0x20);

    // First run initialization: prevent spamming tracers on inject
    if (lastShotCount == -1) {
        lastShotCount = currentShotCount;
        return;
    }

    // 3. Detect Firing
    if (currentShotCount > lastShotCount) {
        Vector3 forward;
        AngleVectors(lp.viewAngles, forward);

        // 4. Calculate Start Position (Eye Position)
        // We add a Z-offset (approx 64 units) to the player origin so the beam starts at head level
        Vector3 startPos = lp.origin;

        // Try to read actual view offset if possible, otherwise hardcode standing height
        // m_vecViewOffset is usually around 0xC50 or 0xC58
        Vector3 viewOffset = ReadMemory<Vector3>(lp.pawn + 0xC58);
        if (viewOffset.z > 0.1f) {
            startPos.x += viewOffset.x;
            startPos.y += viewOffset.y;
            startPos.z += viewOffset.z;
        }
        else {
            startPos.z += 64.0f; // Fallback: Standing eye height
        }

        // 5. Calculate End Position (3500 units away)
        Vector3 endPos = {
            startPos.x + (forward.x * 3500.0f),
            startPos.y + (forward.y * 3500.0f),
            startPos.z + (forward.z * 3500.0f)
        };

        // 6. Add to vector
        BulletTracer tracer;
        tracer.start = startPos;
        tracer.end = endPos;
        tracer.timeCreated = ImGui::GetTime();

        localBulletTracers.push_back(tracer);

        // Update tracked count
        lastShotCount = currentShotCount;
    }
    else if (currentShotCount < lastShotCount) {
        // Reset if game restarts or player reconnects
        lastShotCount = currentShotCount;
    }
}

void RenderBulletTracers(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList) {
    if (localBulletTracers.empty()) return;

    float currentTime = ImGui::GetTime();

    // Iterate through all tracers
    for (auto it = localBulletTracers.begin(); it != localBulletTracers.end(); ) {

        // Calculate how long this specific tracer has been alive
        float lifeTime = currentTime - it->timeCreated;

        // If it has existed longer than your custom setting, remove it
        if (lifeTime > bulletTracerDuration) {
            it = localBulletTracers.erase(it);
            continue;
        }

        // Calculate Alpha (Fade out effect)
        // It stays solid for 80% of the time, then fades out in the last 20%
        float alpha = 1.0f;
        if (lifeTime > (bulletTracerDuration * 0.8f)) {
            alpha = 1.0f - ((lifeTime - (bulletTracerDuration * 0.8f)) / (bulletTracerDuration * 0.2f));
        }

        Vector3 screenStart, screenEnd;
        // Only draw if both points are valid on screen (or at least start is)
        if (WorldToScreen(it->start, screenStart, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(it->end, screenEnd, viewMatrix, screenWidth, screenHeight)) {

            ImU32 color = IM_COL32(
                (int)(bulletTracerColor.x * 255),
                (int)(bulletTracerColor.y * 255),
                (int)(bulletTracerColor.z * 255),
                (int)(bulletTracerColor.w * 255 * alpha) // Apply fade
            );

            drawList->AddLine(
                ImVec2(screenStart.x, screenStart.y),
                ImVec2(screenEnd.x, screenEnd.y),
                color,
                bulletTracerThickness
            );
        }
        ++it;
    }
}




void RunAutoStrafe(const LocalPlayer& lp) {
    if (!autoStrafeEnabled || !bunnyhopEnabled || lp.health <= 0 || !lp.pawn) return;
    int flags = ReadMemory<int>(lp.pawn + m_fFlagsOffset);
    if (flags & 1) return; // Ground only
    // Velocity (updated to 0x450 = m_vecVelocity)
    Vector3 vel = ReadMemory<Vector3>(lp.pawn + 0x450);
    float speed2d = sqrtf(vel.x * vel.x + vel.y * vel.y);
    if (speed2d < 10.0f || speed2d > 1000.0f) return; // Sanity
    // Vel yaw vs view yaw
    float velYaw = ToDegrees(atan2f(vel.y, vel.x));
    float viewYaw = lp.viewAngles.y;
    float delta = NormalizeYaw(viewYaw - velYaw); // -180 to 180
    // PERFECT GAIN: Strafe perp to align view->vel
    float maxStrafe = strafeSpeed; // 0.7x-1.3x slider, but for digital, ignore or use for threshold
    int left_val = 256, right_val = 256;
    if (fabsf(delta) > 1.0f) {
        if (delta > 0.0f) {
            right_val = 65537; // Press right
        }
        else {
            left_val = 65537; // Press left
        }
    }
    WriteMemory<int>(clientBase + 0x1BE64C0, left_val); // left
    WriteMemory<int>(clientBase + 0x1BE6550, right_val); // right
    // Debug (remove later)
    static float lastSpeed = 0;
    if (speed2d > lastSpeed + 5.0f) {
        debugLog += "[STRAFE] Gained to " + std::to_string(speed2d) + " UPS | Delta: " + std::to_string(delta) + "\n";
    }
    lastSpeed = speed2d;
}

DWORD WINAPI RunBhopThread(LPVOID param) {
    static bool prevOnGround = false;
    static float prevVelZ = 0.0f;
    static bool justPressed = false;  // Pulse control

    while (bhopThreadRunning) {
        LocalPlayer lp = GetLocalPlayer();  // Get fresh each tick
        if (!lp.pawn || lp.health <= 0) {
            Sleep(10);
            continue;
        }

        bool holdingJump = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
        bool holdingEdge = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

        int flags = ReadMemory<int>(lp.pawn + m_fFlagsOffset);
        bool onGround = (flags & 1) != 0;

        Vector3 velocity = ReadMemory<Vector3>(lp.pawn + 0x430);
        float velZ = velocity.z;

        bool landedFlags = !prevOnGround && onGround;
        bool landedVel = (prevVelZ < -2.0f) && (velZ > -2.0f && velZ < 2.0f);  // Tighten for land
        bool justLanded = landedFlags || landedVel;

        if (bunnyhopEnabled && holdingJump) {
            if (justLanded && !justPressed) {
                WriteMemory<int>(clientBase + forceJumpOffset, 65537);  // +jump
                justPressed = true;  // Set to release next
                debugLog += "[BHOP] Jump pressed on land! Flags: " + std::to_string(landedFlags) + " Vel: " + std::to_string(landedVel) + "\n";
            }
            else if (justPressed || !onGround) {
                WriteMemory<int>(clientBase + forceJumpOffset, 256);  // -jump immediate
                justPressed = false;  // Reset
            }
        }
        else {
            WriteMemory<int>(clientBase + forceJumpOffset, 256);
            justPressed = false;
        }

        if (edgeJumpEnabled && holdingEdge && !onGround) {
            WriteMemory<int>(clientBase + forceJumpOffset, 256);  // Force -jump mid-air
        }

        prevOnGround = onGround;
        prevVelZ = velZ;

        Sleep(1);  // ~1000hz, precise
    }
    return 0;
}

void RunJumpBug(const LocalPlayer& lp) {
    if (!jumpBugEnabled || !lp.pawn || lp.health <= 0) return;

    int flags = ReadMemory<int>(lp.pawn + m_fFlagsOffset);
    if (flags & 1) return;  // On ground, no need

    Vector3 velocity = ReadMemory<Vector3>(lp.pawn + 0x430);  // m_vecVelocity offset (verify if needed)
    if (velocity.z > -2.0f && velocity.z < 0.0f) {  // Detect near-landing frame
        WriteMemory<int>(clientBase + forceJumpOffset, 65537);  // Force jump input
        debugLog += "[JUMPBUG] Performed jump bug\n";
    }
}

void UpdateBacktrackData(const std::vector<Entity>& entities) {
    if (!backtrackESPEnabled) return;


    float currentTime = ImGui::GetTime();
    for (const auto& e : entities) {
        if (e.health <= 0) continue;

        playerHistory[e.pawn].push_back(std::make_pair(e.origin, currentTime));
        auto& history = playerHistory[e.pawn];
        auto it = history.begin();
        while (it != history.end()) {
            if (currentTime - it->second > backtrackDuration) {
                it = history.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void ProcessSounds() {
    if (!soundESPEnabled) return;


    float currentTime = ImGui::GetTime();
    auto it = soundLocations.begin();
    while (it != soundLocations.end()) {
        if (currentTime - it->second > soundMaxTime) {
            it = soundLocations.erase(it);
        }
        else {
            ++it;
        }
    }

    auto entities = GetEntities();
    LocalPlayer lp = GetLocalPlayer();

    for (const auto& e : entities) {
        if (e.health <= 0 || e.team == lp.team) continue;
        Vector3 velocity = ReadMemory<Vector3>(e.pawn + 0x430);
        float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

        if (speed > 50.0f) {
            soundLocations[e.origin] = currentTime;
        }
    }
}

// Typedefs already in cheat.h
float __fastcall HK_GetInaccuracy(void* thisptr) {
    if (noSpreadEnabled && IsValidPtr(thisptr)) return 0.0f;
    return oGetInaccuracy(thisptr);
}

float __fastcall HK_GetSpread(void* thisptr) {
    if (noSpreadEnabled && IsValidPtr(thisptr)) return 0.0f;
    return oGetSpread(thisptr);
}

void __fastcall HK_UpdateAccuracyPenalty(void* thisptr) {
    if (noSpreadEnabled) return;  // NOP: Prevent penalty buildup (visual only)
    oUpdateAccuracyPenalty(thisptr);
}

void ApplyGlow(uintptr_t entity, GlowSettings* settings, bool isVisible) {
    if (!IsValidPtr((void*)entity) || !settings->enabled) {
        RemoveGlow(entity);
        return;
    }


    uintptr_t glowBase = entity + 0xCB0;
    if (!IsValidPtr((void*)glowBase)) return;

    ImVec4 finalColor = settings->color;
    finalColor.x *= settings->brightness;
    finalColor.y *= settings->brightness;
    finalColor.z *= settings->brightness;
    finalColor.w *= settings->brightness;

    finalColor.x = std::min(std::max(finalColor.x, 0.0f), 1.0f);
    finalColor.y = std::min(std::max(finalColor.y, 0.0f), 1.0f);
    finalColor.z = std::min(std::max(finalColor.z, 0.0f), 1.0f);
    finalColor.w = std::min(std::max(finalColor.w, 0.0f), 1.0f);

    uint8_t alpha = static_cast<uint8_t>(finalColor.w * 255);
    uint8_t red = static_cast<uint8_t>(finalColor.x * 255);
    uint8_t green = static_cast<uint8_t>(finalColor.y * 255);
    uint8_t blue = static_cast<uint8_t>(finalColor.z * 255);

    uint32_t glowColor = (alpha << 24) | (red << 16) | (green << 8) | blue;

    uintptr_t glowColorOverride = glowBase + 0x40;
    uintptr_t glowEnable = glowBase + 0x51;

    if (!IsValidPtr((void*)glowColorOverride) || !IsValidPtr((void*)glowEnable)) return;

    WriteMemory<uint32_t>(glowColorOverride, glowColor);
    WriteMemory<bool>(glowEnable, true);

    uintptr_t glowThroughWallsAddr = glowBase + 0x50;
    if (IsValidPtr((void*)glowThroughWallsAddr)) {
        WriteMemory<bool>(glowThroughWallsAddr, glowThroughWalls);
    }

    uintptr_t outlineWidthAddr = glowBase + 0x48;
    if (IsValidPtr((void*)outlineWidthAddr) && settings->outline) {
        WriteMemory<float>(outlineWidthAddr, settings->outlineWidth);
    }
}

void NewRenderParticle(CParticleCollection* collection, void* a2, void* a3, void* a4) {
    if (particleModEnabled && collection && collection->data && collection->data->color) {
        char* name = *collection->data->particleName;
        if (name) {
            // Find index or group (existing logic to check if enabled)
            for (int i = 0; i < NUM_PARTICLE_TARGETS; ++i) {
                if (strstr(name, particleTargets[i]) && particleTargetsEnabled[i]) {
                    ImVec4 col;
                    if (rainbowEnabled) {
                        // Compute rainbow color
                        float time = ImGui::GetTime();  // Requires ImGui included, or use clock/time
                        float hue = fmod(time * rainbowSpeed, 1.0f);
                        col = ImColor::HSV(hue, 1.0f, 1.0f);  // Full saturation/value for vibrant rainbow
                    }
                    else {
                        col = particleColors[i];  // Static color
                    }
                    collection->data->color->r = col.x;
                    collection->data->color->g = col.y;
                    collection->data->color->b = col.z;
                    break;  // Assume one match
                }
            }
            // Also handle groups if needed (loop through groups and apply similarly)
            for (const auto& group : particleGroups) {
                if (group.enabled) {
                    for (int idx : group.indices) {
                        if (strstr(name, particleTargets[idx])) {
                            ImVec4 col;
                            if (rainbowEnabled) {
                                float time = ImGui::GetTime();
                                float hue = fmod(time * rainbowSpeed, 1.0f);
                                col = ImColor::HSV(hue, 1.0f, 1.0f);
                            }
                            else {
                                col = group.color;
                            }
                            collection->data->color->r = col.x;
                            collection->data->color->g = col.y;
                            collection->data->color->b = col.z;
                            break;
                        }
                    }
                }
            }
        }
    }
    oRenderParticle(collection, a2, a3, a4);  // Call original
}



void __fastcall HK_UpdateSkybox(void* thisptr, void* skyentity) {
    // Call original FIRST
    oUpdateSkybox(thisptr, skyentity);

    if (!skyModEnabled || !IsValidPtr(skyentity)) return;

    ImVec4 tint = skyColor;
    tint.w = 1.0f;  // Full alpha

    if (skyNoSky) {
        tint = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else if (skyNightMode) {
        // Darken for night mode (multiplicative)
        tint.x *= 0.2f;
        tint.y *= 0.2f;
        tint.z *= 0.4f;
    }

    float brightnessVal = skyNoSky ? 0.0f : skyBrightness;

    uintptr_t base = (uintptr_t)skyentity;

    // Write main tint (float4)
    WriteMemory<float>(base + skyTintColorOffset + 0x0, tint.x);
    WriteMemory<float>(base + skyTintColorOffset + 0x4, tint.y);
    WriteMemory<float>(base + skyTintColorOffset + 0x8, tint.z);
    WriteMemory<float>(base + skyTintColorOffset + 0xC, tint.w);

    // Write lighting-only tint (float4) - SAME values
    WriteMemory<float>(base + skyTintLightingOffset + 0x0, tint.x);
    WriteMemory<float>(base + skyTintLightingOffset + 0x4, tint.y);
    WriteMemory<float>(base + skyTintLightingOffset + 0x8, tint.z);
    WriteMemory<float>(base + skyTintLightingOffset + 0xC, tint.w);

    // Brightness scale
    WriteMemory<float>(base + skyBrightnessOffset, brightnessVal);

    // Debug log (remove later)
    debugLog += "[SKY] Applied tint {" + std::to_string(tint.x) + "," + std::to_string(tint.y) + "," + std::to_string(tint.z) + "} brightness " + std::to_string(brightnessVal) + "\n";
}






float __fastcall HK_GetFOV(void* thisptr) {
    // We don't need to call the original if we are overriding it, 
    // but calling it ensures any internal game logic updates happen.
    float original = oGetFOV(thisptr);

    if (fovChangerEnabled) {
        // Check if we are scoped to avoid breaking sniper scopes
        LocalPlayer lp = GetLocalPlayer();
        if (lp.pawn) {
            bool isScoped = ReadMemory<bool>(lp.pawn + scopedOffset);
            if (isScoped) {
                return original; // Return original FOV if scoped
            }
        }

        return static_cast<float>(fovChangerValue);
    }
    return original;
}

// Advanced version - modify the shot in the user command
void AdvancedBulletRedirection() {
    if (!bulletRedirectionEnabled) return;

    bool isKeyPressed = (GetAsyncKeyState(bulletRedirectionKey) & 0x8000) != 0;
    if (!isKeyPressed) return;

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn || lp.health <= 0) return;

    // Get current command
    uintptr_t input = ReadMemory<uintptr_t>(clientBase + inputOffset);
    if (!input) return;

    int commandNumber = ReadMemory<int>(input + 0x64);
    uintptr_t userCmd = ReadMemory<uintptr_t>(input + 0x100 + (commandNumber % 150) * 0x68);
    if (!userCmd) return;

    // Check if attacking in this command
    int buttons = ReadMemory<int>(userCmd + 0x20); // Button state
    if (buttons & (1 << 0)) { // IN_ATTACK flag
        // Modify view angles in the command
        Vector3 cmdAngles = ReadMemory<Vector3>(userCmd + 0xC);
        cmdAngles.y += bulletRedirectionYaw;
        cmdAngles.x += bulletRedirectionPitch;
        cmdAngles = ClampAngle(cmdAngles);
        WriteMemory<Vector3>(userCmd + 0xC, cmdAngles);
    }
}


void __fastcall HK_OverrideView(void* thisptr, CViewSetup* setup) {
    oOverrideView(thisptr, setup);  // Call original

    if (thirdPersonEnabled) {
        // Compute forward vector from view angles for proper behind-camera offset
        float pitch = ToRadians(setup->angView.x);
        float yaw = ToRadians(setup->angView.y);

        Vector_t forward = {
            cos(pitch) * cos(yaw),
            cos(pitch) * sin(yaw),
            -sin(pitch)
        };

        // Offset camera position back along the view direction
        setup->vecOrigin.x -= thirdPersonDistance * forward.x;
        setup->vecOrigin.y -= thirdPersonDistance * forward.y;
        setup->vecOrigin.z -= thirdPersonDistance * forward.z;
    }
}





void RecordAntiAimKey() {
    if (recordingAntiAimKey) {
        for (int i = 1; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                if (i != VK_INSERT && i != VK_END && i != VK_HOME) {
                    antiAimKey = i;
                    recordingAntiAimKey = false;
                    // debugLog logic is optional
                    AddNotification("Anti-Aim key set.", 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                    break;
                }
            }
        }
    }
}

Vector3 ClampAngle(Vector3 angle) {
    // Normalize Yaw
    if (angle.y > 180.0f) angle.y -= 360.0f;
    if (angle.y < -180.0f) angle.y += 360.0f;

    // Clamp Pitch
    if (angle.x > 89.0f) angle.x = 89.0f;
    if (angle.x < -89.0f) angle.x = -89.0f;

    angle.z = 0.0f; // Roll is usually 0
    return angle;
}

void RunAntiAim() {
    if (!antiAimEnabled) return;

    if (antiAimKey != 0 && !(GetAsyncKeyState(antiAimKey) & 0x8000)) {
        return;
    }
    
    uintptr_t input = ReadMemory<uintptr_t>(clientBase + inputOffset);
    if (!input) return;
    int commandNumber = ReadMemory<int>(input + 0x64);

    
    uintptr_t userCmd = ReadMemory<uintptr_t>(input + 0x100 + (commandNumber % 150) * 0x68);
    if (!userCmd) return;

    
    int buttons = ReadMemory<int>(userCmd + 0x20);
    if (buttons & (1 << 0)) {
        return;
    }

    Vector3 currentAngles = ReadMemory<Vector3>(userCmd + 0xC);
    float newYaw = currentAngles.y;

    switch (antiAimType) {
        case 0: 
            newYaw += 180.0f;
            break;

        case 1: { 
            newYaw += 180.0f;
            static bool flip = false;
            if (flip) {
                newYaw += antiAimJitterRange;
            }
            else {
                newYaw -= antiAimJitterRange;
            }
            flip = !flip;
            break;
        }

        case 2: { 
        
            newYaw = fmod(ImGui::GetTime() * antiAimSpinSpeed * 360.0f, 360.0f);
            break;
        }
    }

    Vector3 newAngles = { 89.0f, newYaw, 0.0f };

    newAngles = ClampAngle(newAngles);

    WriteMemory<Vector3>(userCmd + 0xC, newAngles);
}



void RunAimbot() {

    if (!aimbotEnabled) return;

    bool aimKeyDown = (GetAsyncKeyState(aimKey) & 0x8000) != 0;
    if (!aimKeyDown) return;

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn || lp.health <= 0) {
        return;
    }

    std::vector<Entity> entities = GetEntities();
    if (entities.empty()) {
        return;
    }

    Entity* bestTarget = nullptr;
    float bestFov = aimbotFOV;
    Vector3 bestAngle = { 0, 0, 0 };

    for (auto& entity : entities) {
        // Skip invalid entities
        if (entity.health <= 0 || entity.health > 100) continue;
        if (entity.team == lp.team && !aimTeammates) continue;
        if (aimOnVisibleOnly && !entity.visible) continue;
        if (!entity.boneMatrix) continue;

        // Get head position
        Vector3 headPos = GetBonePosition(entity.boneMatrix, bone_head);
        if (headPos.x == 0 && headPos.y == 0 && headPos.z == 0) continue;

        // Calculate angle to target
        Vector3 angleTo = CalculateAngle(lp.origin, headPos);
        Vector3 currentAngles = lp.viewAngles;

        // Normalize angles
        angleTo = ClampAngle(angleTo);
        currentAngles = ClampAngle(currentAngles);

        // Calculate FOV difference
        float deltaX = angleTo.x - currentAngles.x;
        float deltaY = angleTo.y - currentAngles.y;

        // Normalize Y difference
        while (deltaY > 180.0f) deltaY -= 360.0f;
        while (deltaY < -180.0f) deltaY += 360.0f;

        float fov = sqrtf(deltaX * deltaX + deltaY * deltaY);

        // Check if this is the best target
        if (fov < bestFov) {
            bestFov = fov;
            bestTarget = &entity;
            bestAngle = angleTo;
        }
    }

    // Aim at the best target
    if (bestTarget != nullptr) {
        // Apply smoothing
        Vector3 currentAngles = ReadMemory<Vector3>(lp.pawn + viewAnglesOffset);
        Vector3 smoothedAngle = SmoothAngle(currentAngles, bestAngle, aimbotSmoothing);

        // Clamp the smoothed angle
        smoothedAngle = ClampAngle(smoothedAngle);

        // Write the new angles
        WriteMemory<Vector3>(lp.pawn + viewAnglesOffset, smoothedAngle);

        // Debug logging
        debugLog += "[AIMBOT] Targeting: " + std::string(bestTarget->name) +
            " | FOV: " + std::to_string(bestFov) +
            " | Health: " + std::to_string(bestTarget->health) +
            " | Team: " + std::to_string(bestTarget->team) + "\n";
    }
}

Entity* GetBestExternalAimbotTarget(const std::vector<Entity>& entities, const LocalPlayer& lp,
    Matrix4x4 viewMatrix, float screenWidth, float screenHeight) {
    Entity* bestTarget = nullptr;
    float bestScore = FLT_MAX;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    float fovPixels = (externalAimbotFOV / 90.0f) * (screenWidth / 2.0f);

    for (auto& entity : entities) {
        if (entity.health <= 0 || entity.health > 100) continue;
        if (entity.team == lp.team) continue;
        if (aimOnVisibleOnly && !entity.visible) continue;
        if (!entity.boneMatrix) continue;

        // Distance check
        float distance = sqrtf(
            powf(lp.origin.x - entity.origin.x, 2) +
            powf(lp.origin.y - entity.origin.y, 2) +
            powf(lp.origin.z - entity.origin.z, 2)
        );

        if (distance > externalAimbotMaxLockDistance) continue;

        Vector3 targetPos;
        int boneIndex = hitgroupBones[externalAimbotHitgroup];
        if (externalAimbotHitgroup == 4) {
            boneIndex = hitgroupBones[rand() % 4];
        }
        targetPos = GetBonePosition(entity.boneMatrix, boneIndex);

        Vector3 screenPos;
        if (!WorldToScreen(targetPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
            continue;
        }

        // Calculate different scoring methods
        float score = FLT_MAX;

        switch (externalAimbotTargetSelection) {
        case TARGET_CLOSEST:
            score = distance;
            break;

        case TARGET_CLOSEST_TO_CROSSHAIR: {
            float dx = screenPos.x - centerX;
            float dy = screenPos.y - centerY;
            score = sqrtf(dx * dx + dy * dy);
            // Only consider targets within FOV
            if (score > fovPixels) continue;
            break;
        }

        case TARGET_LOWEST_HEALTH:
            score = entity.health; // Lower health = better score
            break;

        case TARGET_HIGHEST_THREAT:
            // Combine distance and visibility for threat calculation
            score = distance / (entity.visible ? 2.0f : 1.0f);
            break;
        }

        if (score < bestScore) {
            bestScore = score;
            bestTarget = const_cast<Entity*>(&entity);
        }
    }

    return bestTarget;
}



bool SetupPopupAcceptMatchHook() {
    if (!popupAcceptMatchFoundAddr) {
        debugLog += "[ERROR] PopupAcceptMatchFound address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)popupAcceptMatchFoundAddr, &HK_PopupAcceptMatchFound, (LPVOID*)&oPopupAcceptMatchFound) != MH_OK) {
        debugLog += "[ERROR] Failed to create PopupAcceptMatchFound hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)popupAcceptMatchFoundAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable PopupAcceptMatchFound hook\n";
        return false;
    }

    debugLog += "[DEBUG] PopupAcceptMatchFound hook set up successfully\n";
    return true;
}

void __fastcall HK_PopupAcceptMatchFound(void* thisptr) {
    oPopupAcceptMatchFound(thisptr);
    if (autoAcceptEnabled && setLocalPlayerReadyAddr) {
        ((SetLocalPlayerReadyFn)setLocalPlayerReadyAddr)("accept");
        debugLog += "[AUTOACCEPT] Match found - auto accepted\n";
    }
}



void RunExternalAimbot() {
    if (!externalAimbotEnabled) return;

    static bool wasKeyPressed = false;
    bool isKeyPressed = (GetAsyncKeyState(externalAimbotKey) & 0x8000) != 0;

    // Handle key recording
    if (recordingExternalAimKey) {
        for (int i = 1; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                if (i != VK_INSERT && i != VK_END && i != VK_HOME) {
                    externalAimbotKey = i;
                    recordingExternalAimKey = false;
                    debugLog += "[EXTERNAL AIMBOT] Key recorded: " + std::to_string(i) + "\n";
                    AddNotification("External aim key set to: " + std::to_string(i), 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                    break;
                }
            }
        }
        return;
    }

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn || lp.health <= 0) {
        externalAimbotLockedTarget = 0;
        return;
    }

    std::vector<Entity> entities = GetEntities();
    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;

    // Handle aimlock logic
    if (externalAimbotAimlock) {
        if (isKeyPressed && !wasKeyPressed) {
            // Key just pressed - find new target
            Entity* newTarget = GetBestExternalAimbotTarget(entities, lp, viewMatrix, screenWidth, screenHeight);
            if (newTarget) {
                externalAimbotLockedTarget = newTarget->pawn;
                debugLog += "[EXTERNAL AIMBOT] Locked onto target: " + std::string(newTarget->name) + "\n";
            }
        }
        else if (!isKeyPressed && wasKeyPressed) {
            // Key released - clear lock
            externalAimbotLockedTarget = 0;
        }
        else if (isKeyPressed && externalAimbotLockedTarget != 0) {
            // Key held - maintain lock or find new target if needed
            bool targetValid = false;
            Entity* lockedEntity = nullptr;

            // Check if current locked target is still valid
            for (auto& entity : entities) {
                if (entity.pawn == externalAimbotLockedTarget) {
                    if (entity.health > 0 && entity.health <= 100) {
                        targetValid = true;
                        lockedEntity = &entity;
                        break;
                    }
                }
            }

            if (!targetValid && externalAimbotAutoSwitch) {
                // Auto-switch to new target
                Entity* newTarget = GetBestExternalAimbotTarget(entities, lp, viewMatrix, screenWidth, screenHeight);
                if (newTarget) {
                    externalAimbotLockedTarget = newTarget->pawn;
                    debugLog += "[EXTERNAL AIMBOT] Auto-switched to new target: " + std::string(newTarget->name) + "\n";
                    lockedEntity = newTarget;
                }
                else {
                    externalAimbotLockedTarget = 0;
                }
            }

            // Aim at locked target
            if (lockedEntity && lockedEntity->boneMatrix) {
                Vector3 targetPos;
                int boneIndex = hitgroupBones[externalAimbotHitgroup];
                if (externalAimbotHitgroup == 4) {
                    boneIndex = hitgroupBones[rand() % 4];
                }
                targetPos = GetBonePosition(lockedEntity->boneMatrix, boneIndex);

                // Movement prediction (simple linear prediction)
                if (externalAimbotPredictMovement) {
                    Vector3 velocity = ReadMemory<Vector3>(lockedEntity->pawn + 0x430);
                    // Predict position 100ms ahead
                    targetPos.x += velocity.x * 0.1f;
                    targetPos.y += velocity.y * 0.1f;
                    targetPos.z += velocity.z * 0.1f;
                }

                Vector3 screenPos;
                if (WorldToScreen(targetPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
                    float centerX = screenWidth / 2.0f;
                    float centerY = screenHeight / 2.0f;

                    float deltaX = screenPos.x - centerX;
                    float deltaY = screenPos.y - centerY;

                    if (externalAimbotSmoothness > 0) {
                        deltaX /= externalAimbotSmoothness;
                        deltaY /= externalAimbotSmoothness;
                    }

                    mouse_event(MOUSEEVENTF_MOVE, (int)deltaX, (int)deltaY, 0, 0);

                    const char* hitgroupName = hitgroupNames[externalAimbotHitgroup];
                    if (externalAimbotHitgroup == 4) {
                        hitgroupName = "Random";
                    }

                    debugLog += "[EXTERNAL AIMBOT] Aimlock on: " + std::string(lockedEntity->name) +
                        " | Hitgroup: " + hitgroupName + " | Locked: YES\n";
                }
            }
        }
    }
    else {
        // Original behavior without aimlock
        if (isKeyPressed) {
            Entity* closestTarget = GetBestExternalAimbotTarget(entities, lp, viewMatrix, screenWidth, screenHeight);
            if (closestTarget && closestTarget->boneMatrix) {
                Vector3 targetPos;
                int boneIndex = hitgroupBones[externalAimbotHitgroup];
                if (externalAimbotHitgroup == 4) {
                    boneIndex = hitgroupBones[rand() % 4];
                }
                targetPos = GetBonePosition(closestTarget->boneMatrix, boneIndex);

                Vector3 screenPos;
                if (WorldToScreen(targetPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
                    float centerX = screenWidth / 2.0f;
                    float centerY = screenHeight / 2.0f;

                    float deltaX = screenPos.x - centerX;
                    float deltaY = screenPos.y - centerY;

                    if (externalAimbotSmoothness > 0) {
                        deltaX /= externalAimbotSmoothness;
                        deltaY /= externalAimbotSmoothness;
                    }

                    mouse_event(MOUSEEVENTF_MOVE, (int)deltaX, (int)deltaY, 0, 0);

                    const char* hitgroupName = hitgroupNames[externalAimbotHitgroup];
                    if (externalAimbotHitgroup == 4) {
                        hitgroupName = "Random";
                    }

                    debugLog += "[EXTERNAL AIMBOT] Aiming at: " + std::string(closestTarget->name) +
                        " | Hitgroup: " + hitgroupName + " | Locked: NO\n";
                }
            }
        }
    }

    wasKeyPressed = isKeyPressed;
}



uintptr_t GetEntityFromSerialIndex(int index) {
    if (index < 1 || index > 2047) return 0;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return 0;
    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((index & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return 0;
    return ReadMemory<uintptr_t>(listEntry + 0x70 * (index & 0x1FF));
}

void RecordTriggerKey() {
    if (recordingTriggerKey) {
        for (int i = 1; i < 256; ++i) {
            if (GetAsyncKeyState(i) & 0x8000) {
                if (i != VK_INSERT && i != VK_END && i != VK_HOME) {
                    triggerbotKey = i;
                    recordingTriggerKey = false;
                    triggerKeyRecorded = true;
                    debugLog += "[TRIGGERBOT] Key recorded: " + std::to_string(i) + "\n";
                    AddNotification("Trigger key set: " + std::to_string(i), 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                    break;
                }
            }
        }
    }
}

void RunTriggerbot(const LocalPlayer& lp, const std::vector<Entity>& /*unused*/) {
    if (!triggerbotEnabled || !lp.pawn || lp.health <= 0) return;

    // Key check
    if (triggerbotKey != 0 && !(GetAsyncKeyState(triggerbotKey) & 0x8000)) return;

    if (triggerbotRequireStill) {
        Vector3 velocity = ReadMemory<Vector3>(lp.pawn + 0x430);  // m_vecVelocity
        float speed = velocity.Length();
        if (speed > triggerbotStillThreshold) {
            return;  // Moving too fast - don't shoot
        }
    }

    // Read crosshair ID (serial index, usually 1000-2000 range for players)
    int crosshairID = ReadMemory<int>(lp.pawn + crosshairIDOffset);
    if (crosshairID <= 0 || crosshairID > 2047) {
        triggerbotLastID = 0;
        return;
    }

    // Resolve to pawn pointer using entity list
    uintptr_t targetPawn = GetEntityFromSerialIndex(crosshairID);
    if (!targetPawn || targetPawn == lp.pawn) {
        triggerbotLastID = 0;
        return;
    }

    // Sanity: alive enemy
    int targetHealth = ReadMemory<int>(targetPawn + healthOffset);
    if (targetHealth < 1 || targetHealth > 100) {
        triggerbotLastID = 0;
        return;
    }

    int targetTeam = ReadMemory<int>(targetPawn + teamOffset);
    if (targetTeam == lp.team && !triggerbotTeammates) {
        triggerbotLastID = 0;
        return;
    }

    // Visibility check (optional)
    bool visible = true;
    if (triggerbotVisibleOnly) {
        if (lp.index >= 0 && lp.index < 64) {
            if (entitySpottedByMaskOffset) {
                uint64_t spottedByMask = ReadMemory<uint64_t>(targetPawn + entitySpottedStateOffset + entitySpottedByMaskOffset);
                visible = (spottedByMask & (1ULL << lp.index)) != 0;
            }
            else {
                uintptr_t spottedAddr = targetPawn + entitySpottedStateOffset + 0x8 + (lp.index * 1);  // bool size 1
                visible = ReadMemory<bool>(spottedAddr);
            }
        }
        if (!visible) {
            triggerbotLastID = 0;
            return;
        }
    }

    ULONGLONG now = GetTickCount64();

    // Target acquisition logic
    if (crosshairID != triggerbotLastID) {
        triggerbotAcquireTime = now;
        triggerbotLastID = crosshairID;
    }

    // Check first shot delay
    if (now - triggerbotAcquireTime < static_cast<ULONGLONG>(triggerbotDelay)) return;

    // Check between-shot delay (use a small value or another variable if needed)
    if (now - triggerbotLastShotTime < 5) return;  // Minimal delay between shots to prevent too rapid fire

    // SIMULATE NORMAL MOUSE CLICK
    INPUT inputs[2] = { 0 };
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, &inputs[0], sizeof(INPUT));  // Down
    Sleep(5);  // Tiny hold
    SendInput(1, &inputs[1], sizeof(INPUT));  // Up

    triggerbotLastShotTime = now;
}


void UpdatePostProcessingExposure(const LocalPlayer& lp) {
    if (!worldModEnabled || !lp.pawn) return;

    uintptr_t cameraServices = ReadMemory<uintptr_t>(lp.pawn + m_pCameraServicesOffset);
    if (!cameraServices) return;

    uint32_t activePPHandle = ReadMemory<uint32_t>(cameraServices + m_hActivePostProcessingVolumeOffset);
    if (!activePPHandle || activePPHandle == 0xFFFFFFFF) return;

    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return;

    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((activePPHandle & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return;

    uintptr_t ppEntity = ReadMemory<uintptr_t>(listEntry + 0x70 * (activePPHandle & 0x1FF));
    if (!ppEntity) return;

    WriteMemory<bool>(ppEntity + m_bExposureControlOffset, true);

    // Map worldBrightness (0.0-1.0) to exposure value, e.g., 0.1f + (worldBrightness * 0.9f) for night-like
    float exposureValue = 0.1f + (worldBrightness * 0.9f);  // Adjust range as needed

    WriteMemory<float>(ppEntity + m_flMinExposureOffset, exposureValue);
    WriteMemory<float>(ppEntity + m_flMaxExposureOffset, exposureValue);
}


void ApplyFeatures(const LocalPlayer& lp, const std::vector<Entity>& entities) {
    if (!lp.pawn) {
        debugLog += "[FEATURES] No local player pawn\n";
        return;
    }


    static std::map<uintptr_t, int> previousHealth;
    static int prevLocalAmmo = -1;
    static float lastFireTime = 0.0f;

    RunAutoStrafe(lp);
    RunJumpBug(lp);
    UpdateBacktrackData(entities);
    ProcessSounds();
    RecordTriggerKey();
    UpdatePostProcessingExposure(lp);
    UpdateBulletTracers(lp);
    RecordAntiAimKey();

    if (lp.pawn && lp.health > 0) {
        RunAntiAim(); // <-- ADD THIS CALL
    }

    if (lp.ammo < prevLocalAmmo && prevLocalAmmo != -1) {
        lastFireTime = ImGui::GetTime();
        std::stringstream ss;
        ss << "[DEBUG] Local player fired at time " << lastFireTime << "\n";
        debugLog += ss.str();
    }
    prevLocalAmmo = lp.ammo;

    for (const auto& e : entities) {
        if (e.team == lp.team && !showTeammates) continue;
        if (e.health <= 0 || e.health > 100) continue;


        auto it = previousHealth.find(e.pawn);
        int prevHP = (it != previousHealth.end()) ? it->second : e.health;
        int healthDelta = e.health - prevHP;

        if (healthDelta < 0) {
            int absDelta = -healthDelta;
            float currentTime = static_cast<float>(ImGui::GetTime());
            if (hitsoundEnabled && (currentTime - lastFireTime < 0.2f)) {
                if (e.health <= 0) {
                    PlayMP3(killSoundPath);
                    AddNotification("Kill Confirmed!", 2.0f, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                }
                else if (e.lastHitGroup == 1) {
                    PlayMP3(headSoundPath);
                }
                else {
                    PlayMP3(hitSoundPath);
                }
                debugLog += "[DEBUG] Hitsound triggered for local hit (delta: " + std::to_string(absDelta) + ")\n";
                lastFireTime = 0.0f;
            }
            hitmarkers.push_back({ GetBonePosition(e.boneMatrix, bone_head), absDelta, static_cast<float>(ImGui::GetTime()), hitmarkerFadeDuration });
            if (e.health <= 0) {
                deathTimes[e.pawn] = static_cast<float>(ImGui::GetTime());
            }
        }

        previousHealth[e.pawn] = e.health;
    }

    if (noFlashEnabled && lp.pawn) {
        WriteMemory<float>(lp.pawn + flashDurationOffset, 0.0f);
    }

    if (infiniteArmorEnabled && lp.pawn) {
        WriteMemory<int>(lp.pawn + armorOffset, 100);
    }

    if (glowV2Enabled) {
        for (const auto& e : entities) {
            if (e.health <= 0) continue;
            bool isTeammate = (e.team == lp.team);
            ApplyGlowV2(e.pawn, isTeammate, e.visible);
        }
    }
    if (triggerbotEnabled) {
        LocalPlayer lp = GetLocalPlayer();
        std::vector<Entity> entities = GetEntities();  // Even if unused, for consistency
        RunTriggerbot(lp, entities);
    }

    if (noSpreadEnabled && lp.activeWeapon) {
        uintptr_t weapon = lp.activeWeapon;
        // Recent offsets (Jul 2025): m_iShotsFired=0x23FC (player-side?), but weapon-side too
        WriteMemory<float>(weapon + 0x1A00, 0.0f);  // m_flSpreadCone (common, verify dump)
        WriteMemory<float>(weapon + 0x19F8, 0.0f);  // m_fAccuracyPenalty (approx)
        WriteMemory<int>(lp.pawn + shotsFiredOffset, 0);      // m_iShotsFired (resets pattern)
    }

    if (skyModEnabled) {
        static std::unordered_set<uintptr_t> skyEntitiesPatched;  // Patch once per entity

        uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
        if (!entityList) return;

        for (int i = 1; i < 2048; ++i) {  // Scan world entities (sky usually <1024)
            uintptr_t list_entry = ReadMemory<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
            if (!list_entry || !IsValidPtr((void*)list_entry)) continue;

            uintptr_t entity = ReadMemory<uintptr_t>(list_entry + 0x70 * (i & 0x1FF));
            if (!entity || !IsValidPtr((void*)entity)) continue;

            uintptr_t ident = ReadMemory<uintptr_t>(entity + entityIdentityOffset);
            if (!ident || !IsValidPtr((void*)ident)) continue;

            char className[128] = { 0 };
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(ident + designerNameOffset), className, sizeof(className) - 1, nullptr);

            if (strstr(className, "C_EnvSky") != nullptr) {  // Exact match for sky entities
                if (skyEntitiesPatched.find(entity) != skyEntitiesPatched.end()) continue;  // Already patched

                // Compute colors (0-255 uint8)
                uint8_t r = clamp<uint8_t>((uint8_t)(skyColor.x * 255.0f), 0, 255);
                uint8_t g = clamp<uint8_t>((uint8_t)(skyColor.y * 255.0f), 0, 255);
                uint8_t b = clamp<uint8_t>((uint8_t)(skyColor.z * 255.0f), 0, 255);
                uint8_t a = 255;

                if (skyNoSky) {
                    r = g = b = 0;
                }
                else if (skyNightMode) {
                    r = clamp<uint8_t>((uint8_t)(r * 0.2f), 0, 255);
                    g = clamp<uint8_t>((uint8_t)(g * 0.2f), 0, 255);
                    b = clamp<uint8_t>((uint8_t)(b * 0.4f), 0, 255);
                }

                uintptr_t base = entity;

                // Write main tint (Color_t: bytes r,g,b,a)
                WriteMemory<uint8_t>(base + skyTintColorOffset + 0, r);
                WriteMemory<uint8_t>(base + skyTintColorOffset + 1, g);
                WriteMemory<uint8_t>(base + skyTintColorOffset + 2, b);
                WriteMemory<uint8_t>(base + skyTintColorOffset + 3, a);

                // Write lighting tint (same)
                WriteMemory<uint8_t>(base + skyTintLightingOffset + 0, r);
                WriteMemory<uint8_t>(base + skyTintLightingOffset + 1, g);
                WriteMemory<uint8_t>(base + skyTintLightingOffset + 2, b);
                WriteMemory<uint8_t>(base + skyTintLightingOffset + 3, a);

                // Brightness
                float bright = skyNoSky ? 0.0f : skyBrightness;
                WriteMemory<float>(base + skyBrightnessOffset, bright);

                skyEntitiesPatched.insert(entity);
                debugLog += "[SKY] Patched C_EnvSky: " + std::string(className) + " | Color: (" +
                    std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ") bright: " +
                    std::to_string(bright) + "\n";
            }
        }

        // Optional: Clear patched set on map change (e.g., detect via localplayer health/team change)
        static int lastMapSeed = 0;
        LocalPlayer lp = GetLocalPlayer();
        int currentMapSeed = ReadMemory<int>(clientBase + 0x12345678);  // Add dwMapname or similar offset if needed
        if (lastMapSeed != currentMapSeed) {
            skyEntitiesPatched.clear();
            lastMapSeed = currentMapSeed;
            debugLog += "[SKY] Cleared patched entities (map change)\n";
        }
    }

    if (glowESPEnabled) {
        static std::map<uintptr_t, bool> glowApplied;

        for (const auto& e : entities) {
            if (e.health <= 0 || e.health > 100) continue;
            if (!e.pawn) continue;

            float distance = sqrtf(
                powf(lp.origin.x - e.origin.x, 2) +
                powf(lp.origin.y - e.origin.y, 2) +
                powf(lp.origin.z - e.origin.z, 2)
            );

            if (distance > glowMaxDistance) {
                if (glowApplied[e.pawn]) {
                    RemoveGlow(e.pawn);
                    glowApplied[e.pawn] = false;
                }
                continue;
            }

            if (e.team == lp.team && !showTeammates) {
                if (glowApplied[e.pawn]) {
                    RemoveGlow(e.pawn);
                    glowApplied[e.pawn] = false;
                }
                continue;
            }

            GlowSettings* settings = (e.team == lp.team) ? &teammateGlowSettings : &enemyGlowSettings;

            if (!glowApplied[e.pawn] || settings->enabled) {
                ApplyGlow(e.pawn, settings, e.visible);
                glowApplied[e.pawn] = true;
            }
        }

        for (auto it = glowApplied.begin(); it != glowApplied.end(); ) {
            bool found = false;
            for (const auto& e : entities) {
                if (e.pawn == it->first) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                RemoveGlow(it->first);
                it = glowApplied.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    if (revealAllEnabled) {
        uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
        for (int i = 1; i < 64; i++) {
            uintptr_t list_entry = ReadMemory<uintptr_t>(entityList + 0x8 * (i >> 9) + 0x10);
            if (!list_entry) continue;
            uintptr_t controller = ReadMemory<uintptr_t>(list_entry + 0x70 * (i & 0x1FF));
            if (!controller) continue;
            uint32_t pawn_handle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
            if (!pawn_handle) continue;
            uintptr_t p_index = pawn_handle & 0x7FFF;
            uintptr_t list_entry2 = ReadMemory<uintptr_t>(entityList + 0x8 * (p_index >> 9) + 0x10);
            if (!list_entry2) continue;
            uintptr_t entity = ReadMemory<uintptr_t>(list_entry2 + 0x70 * (p_index & 0x1FF));
            if (entity && entity != lp.pawn) {
                int team = ReadMemory<int>(entity + teamOffset);
                if (team != lp.team) {
                    uintptr_t spottedAddr = entity + entitySpottedStateOffset + 0x8 + (lp.index * sizeof(bool));
                    WriteMemory<bool>(spottedAddr, true);
                }
            }
        }
    }

}



void __fastcall HK_FrameStageNotify(void* thisptr, int stage) {
    // Context variables
    Vector3 oldPunch = { 0.f, 0.f, 0.f };
    bool shouldRestore = false;
    LocalPlayer lp = GetLocalPlayer();

    // --- VISUAL NO RECOIL ---
    // We run this in RENDER_START to stop the screen from shaking
    if (stage == FRAME_RENDER_START && noRecoilEnabled && lp.pawn && lp.health > 0) {
        // Verify aimPunchOffset is not 0 (Check offsets.txt if this fails)
        // Usually aimPunchAngle is at client.dll + offset or pawn + offset
        if (aimPunchOffset != 0) {
            // 1. Save the current punch
            oldPunch = ReadMemory<Vector3>(lp.pawn + aimPunchOffset);

            // 2. Check if we actually have recoil to remove
            if (oldPunch.x != 0.f || oldPunch.y != 0.f) {
                // 3. Set punch to zero (visuals only)
                WriteMemory<Vector3>(lp.pawn + aimPunchOffset, { 0.f, 0.f, 0.f });
                shouldRestore = true;
            }
        }
    }


    // --- CALL ORIGINAL GAME FUNCTION ---
    oFrameStageNotify(thisptr, stage);

    // --- RESTORE RECOIL ---
    // We must put the recoil back, or the server will reject our shots (bullets will fly wildly)
    if (shouldRestore) {
        WriteMemory<Vector3>(lp.pawn + aimPunchOffset, oldPunch);
    }
}



__int64 __fastcall HK_CreateMove(void* thisptr, int a2, int a3, float a4, bool a5) {
    __int64 result = oCreateMove(thisptr, a2, a3, a4, a5);

    return result;
}

void __fastcall HK_RenderLegs(__int64* a1, __int64* a2, __int64 a3, __int64 a4, __int64 a5) {
    if (!noLegsEnabled) {
        oRenderLegs(a1, a2, a3, a4, a5);
    }
    // If enabled, we skip calling original -> no legs rendered
}

void RunFOVChanger() {
    while (true) {
        if (fovChangerEnabled) {
            LocalPlayer lp = GetLocalPlayer();
            if (lp.pawn && lp.health > 0) {
                uintptr_t camera_services = ReadMemory<uintptr_t>(lp.pawn + m_pCameraServicesOffset);
                if (IsValidPtr((void*)camera_services)) {
                    bool is_scoped = ReadMemory<bool>(lp.pawn + scopedOffset);
                    if (!is_scoped) {
                        WriteMemory<uint32_t>(camera_services + m_iFOVOffset, static_cast<uint32_t>(fovChangerValue));
                        WriteMemory<uint32_t>(camera_services + m_iFOVStartOffset, static_cast<uint32_t>(fovChangerValue));
                        WriteMemory<float>(camera_services + m_flFOVRateOffset, 0.0f);
                        WriteMemory<float>(camera_services + m_flFOVTimeOffset, -1.0f);
                        debugLog += "[FOV] Thread locked to " + std::to_string(fovChangerValue) + "\n";
                    }
                }
            }
        }
        Sleep(1);  // 1ms interval for frequent updates
    }
}