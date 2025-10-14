#define NOMINMAX
#include "cheat.h"

bool IsValidPtr(void* ptr) {
    MEMORY_BASIC_INFORMATION mbi;
    if (!ptr) return false;
    if (VirtualQuery(ptr, &mbi, sizeof(mbi)) == 0) return false;
    return (mbi.State == MEM_COMMIT) && ((mbi.Protect & PAGE_NOACCESS) == 0);
}

ImFont* defaultFont = nullptr;
ImFont* navFont = nullptr;
ImFont* iconFont = nullptr; // New: For icons
bool g_unloadRequested = false;
bool espEnabled = true;
bool aimbotEnabled = true;
bool menuOpen = false;
bool healthBarEnabled = true;
bool snaplinesEnabled = true;
bool distanceEnabled = true;
bool boneESPEnabled = true;
bool headCircleEnabled = true;
bool showTeammates = false;
bool box3DEnabled = false;
bool glowESPEnabled = true;
bool ammoEnabled = true;
bool radarEnabled = true;
bool healthTextEnabled = true;
bool armorEnabled = true;
bool fovCircleEnabled = true;
bool flagsEnabled = true;
bool tracersEnabled = true;
bool noFlashEnabled = false;
bool revealAllEnabled = false;
bool infiniteArmorEnabled = false;
bool hitsoundEnabled = true; // New: Hitsound toggle
bool outlineEnabled = true; // New: Global outline toggle for ESP features
bool noRecoilEnabled = false;
bool watermarkEnabled = true;
bool crosshairEnabled = false;
bool customCrosshairEnabled = true;
bool showFPS = true;
bool showPosition = false;
bool showVelocity = false;
bool showWeaponInfo = true;
bool showSpectators = true;
bool showGameTime = false;
bool showKeysPressed = false;
bool showLocalPlayerInfo = true;
bool showEntityCount = true;
bool showSystemInfo = false;
bool showNetworkInfo = false;
bool showPlayerInfo = false;
bool showMatchInfo = false;
bool showAngleLines = false;
bool showTargetInfo = false;
bool showMinimap = false;
bool showRadarHud = true;
bool showGrenadeWarning = true;
bool showBombTimer = true;
bool showDefuseTimer = true;
bool showKillFeed = true;
bool showDamageIndicator = true;
bool showRecoilCrosshair = false;
bool showSpreadCircle = false;
bool showAmmoWarning = true;
bool showMoneyDisplay = false;
bool showRoundTimer = true;
bool showBombIcon = true;
bool showGrenadeTrajectory = false;
bool showSmokeWarning = true;
bool showFireWarning = true;
bool showFlashWarning = true;
bool showScopeLines = false;
bool showFovChangerIndicator = true;
bool showThirdPersonIndicator = true;
bool showSpectatorCount = true;
bool showTeamInfo = false;
bool showClantag = false;
bool showRank = false;
bool showWinProbability = false;
bool showEconomyInfo = false;
bool showBuyMenuHelper = false;
bool showGrenadeHelper = false;
bool showJumpStats = false;
bool showStaminaBar = false;
bool showKeybinds = false;
bool showMenuKeybind = true;
bool bunnyhopEnabled = false;
bool triggerbotEnabled = false;
bool showSpectatorList = false; // NEW: Default off
bool externalAimbotEnabled = false;
bool externalAimbotUseHead = true;
bool recordingExternalAimKey = false;
bool fovChangerEnabled = false;
bool glowThroughWalls = true;
bool chamsEnabled = true;
bool chamsThroughWalls = true;
bool chamsVisibleOnly = true;
float glowMaxDistance = 5000.0f;
float fovChangerValue = 90.0f;
float triggerbotDelay = 20.0f;
float crosshairSize = 8.0f;
float crosshairThickness = 1.5f;
float externalAimbotFOV = 5.0f;
float externalAimbotSmoothness = 2.0f;
float boxThickness = 1.5f;
float skeletonThickness = 1.2f;
float snaplineThickness = 1.0f;
float tracerThickness = 0.8f;
float headCircleThickness = 1.3f;
float glowOpacity = 0.5f;
float aimbotFOV = 10.0f;
float aimbotSmoothing = 5.0f; // New
bool aimOnVisibleOnly = true; // New
bool aimTeammates = false; // New
int aimKey = VK_LBUTTON; // New
int externalAimbotKey = VK_XBUTTON1; // Mouse side button by default
int externalAimbotHitgroup = 0; // 0=head, 1=neck, 2=chest, 3=pelvis, 4=random
int hitgroupBones[] = { bone_head, bone_neck, bone_spine_3, bone_pelvis, bone_head };
LONG volatile g_inPresent = 0;
float boxRounding = 4.0f; // New: Rounding for box corners
ImVec4 espColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
ImVec4 teammateColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 visibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
ImVec4 skeletonColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
ImVec4 snaplineColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
ImVec4 tracerColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
ImVec4 headCircleColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
ImVec4 glowColor = ImVec4(0.5f, 0.0f, 0.5f, 0.5f);
ImVec4 fovCircleColor = ImVec4(1.0f, 1.0f, 1.0f, 0.4f);
ImVec4 crosshairColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 chamsEnemyColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
ImVec4 chamsTeammateColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 chamsVisibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
ImVec4 weaponColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
HWND g_hwnd = NULL;
std::string debugLog = "";
bool showDebugConsole = false;
bool recordingAimKey = false;
int recordedAimKey = VK_LBUTTON;
bool keyRecorded = false;
const char* hitgroupNames[] = { "Head", "Neck", "Chest", "Pelvis", "Random" };
GlowSettings enemyGlowSettings;
GlowSettings teammateGlowSettings;






bool weaponESPEnabled = true;
float weaponMaxDistance = 2000.0f;

bool soundESPEnabled = true;
float soundMaxTime = 5.0f;
std::map<Vector3, float> soundLocations;

bool showAdvancedFlags = true;
bool velocityESPEnabled = true;

bool backtrackESPEnabled = false;
float backtrackDuration = 0.2f;
std::map<uintptr_t, std::vector<std::pair<Vector3, float>>> playerHistory;

bool autoStrafeEnabled = false;
float strafeSpeed = 1.0f;




// Notifications global
std::vector<Notification> notifications;

// For health animation and hitmarkers
std::map<uintptr_t, float> displayedHealth; // For smooth health bar animation
std::vector<Hitmarker> hitmarkers; // List of active hitmarkers
float healthAnimationSpeed = 200.0f; // HP per second decrease rate
float hitmarkerSpeed = 50.0f; // Pixels per second upward movement
float hitmarkerFadeDuration = 1.0f; // Seconds to fade out

// For lingering ESP on dead entities
std::map<uintptr_t, float> deathTimes; // pawn -> death timestamp

// Bone indices
const int bone_pelvis = 0;
const int bone_spine_0 = 1;
const int bone_spine_1 = 2;
const int bone_spine_2 = 3;
const int bone_spine_3 = 4;
const int bone_neck = 5;
const int bone_head = 6;
const int bone_clavicle_L = 7;
const int bone_upperarm_L = 8;
const int bone_forearm_L = 9;
const int bone_hand_L = 10;
const int bone_clavicle_R = 12;
const int bone_upperarm_R = 13;
const int bone_forearm_R = 14;
const int bone_hand_R = 15;
const int bone_thigh_L = 22;
const int bone_calf_L = 23;
const int bone_foot_L = 24;
const int bone_thigh_R = 25;
const int bone_calf_R = 26;
const int bone_foot_R = 27;

std::vector<std::pair<int, int>> boneConnections = {
    {bone_head, bone_neck},
    {bone_neck, bone_spine_3},
    {bone_spine_3, bone_spine_2},
    {bone_spine_2, bone_spine_1},
    {bone_spine_1, bone_spine_0},
    {bone_spine_0, bone_pelvis},
    {bone_spine_3, bone_clavicle_L},
    {bone_clavicle_L, bone_upperarm_L},
    {bone_upperarm_L, bone_forearm_L},
    {bone_forearm_L, bone_hand_L},
    {bone_spine_3, bone_clavicle_R},
    {bone_clavicle_R, bone_upperarm_R},
    {bone_upperarm_R, bone_forearm_R},
    {bone_forearm_R, bone_hand_R},
    {bone_pelvis, bone_thigh_L},
    {bone_thigh_L, bone_calf_L},
    {bone_calf_L, bone_foot_L},
    {bone_pelvis, bone_thigh_R},
    {bone_thigh_R, bone_calf_R},
    {bone_calf_R, bone_foot_R}
};

void RecordAimKey() {
    if (recordingAimKey) {
        for (int i = 1; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                // Avoid capturing menu toggle keys
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

uintptr_t GetPawnByIndex(int index) {
    if (index < 1 || index > 64) return 0;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return 0;

    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((index & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return 0;

    uintptr_t controller = ReadMemory<uintptr_t>(listEntry + 0x78 * (index & 0x1FF));
    if (!controller) return 0;

    uint32_t pawnHandle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
    if (!pawnHandle) return 0;

    uintptr_t pawnListEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);
    if (!pawnListEntry) return 0;

    uintptr_t pawn = ReadMemory<uintptr_t>(pawnListEntry + 0x78 * (pawnHandle & 0x1FF));
    return pawn;
}

__int64 __fastcall HK_CreateMove(void* thisptr, int a2, int a3, float a4, bool a5) {
    // Call original first
    __int64 result = oCreateMove(thisptr, a2, a3, a4, a5);

    // Apply FOV changer if enabled
    if (fovChangerEnabled) {
        LocalPlayer lp = GetLocalPlayer();
        if (lp.pawn && lp.health > 0) {
            // Read current FOV first to make sure we're writing to the right place
            float currentFov = ReadMemory<float>(lp.pawn + fovOffset);
            if (currentFov != fovChangerValue) {
                WriteMemory<float>(lp.pawn + fovOffset, fovChangerValue);
            }
        }
    }

    return result;
}

std::string hitSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\sounds\\hitsound.mp3";
std::string headSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\sounds\\headsound.mp3";
std::string killSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\sounds\\killsound.mp3";
float soundVolume = 1000.0f; // MCI volume (0-1000)

void PlayMP3(const std::string& filePath) {
    static int soundCounter = 0;
    std::string alias = "mp3sound" + std::to_string(soundCounter++);
    std::string openCmd = "open \"" + filePath + "\" type mpegvideo alias " + alias;
    mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
    std::string volumeCmd = "setaudio " + alias + " volume to " + std::to_string(static_cast<int>(soundVolume));
    mciSendStringA(volumeCmd.c_str(), NULL, 0, NULL);
    std::string playCmd = "play " + alias;
    mciSendStringA(playCmd.c_str(), NULL, 0, NULL);
    // No close - async play completes even if entity dies; minor leak acceptable for cheat
}

float ToDegrees(float radians) { return radians * 180.0f / 3.1415926535f; }
float ToRadians(float degrees) { return degrees * 3.1415926535f / 180.0f; }

Vector3 CalculateAngle(Vector3 src, Vector3 dst) {
    Vector3 angle;
    Vector3 delta = { dst.x - src.x, dst.y - src.y, dst.z - src.z };

    float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
    angle.x = ToDegrees(atan2f(-delta.z, hyp));
    angle.y = ToDegrees(atan2f(delta.y, delta.x));
    angle.z = 0.0f;

    // Clamp pitch
    if (angle.x > 89.0f) angle.x = 89.0f;
    if (angle.x < -89.0f) angle.x = -89.0f;

    // Normalize yaw
    if (angle.y > 180.0f) angle.y -= 360.0f;
    if (angle.y < -180.0f) angle.y += 360.0f;

    return angle;
}

uintptr_t GetControllerByIndex(int index) {
    if (index < 1 || index > 64) return 0;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return 0;

    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((index & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return 0;

    uintptr_t controller = ReadMemory<uintptr_t>(listEntry + 0x78 * (index & 0x1FF));
    return controller;
}

Vector3 SmoothAngle(Vector3 current, Vector3 target, float smoothing) {
    Vector3 delta = {
        target.x - current.x,
        target.y - current.y,
        0.0f
    };

    // Normalize yaw difference
    while (delta.y > 180.0f) delta.y -= 360.0f;
    while (delta.y < -180.0f) delta.y += 360.0f;

    Vector3 smoothed = {
        current.x + delta.x / smoothing,
        current.y + delta.y / smoothing,
        0.0f
    };

    return smoothed;
}





void ApplyChams(uintptr_t entity, bool isTeammate, bool isVisible) {
    if (!chamsEnabled) return;

    // This is a simplified implementation - actual chams requires material hooks
    // For now, we'll enhance the glow system to simulate chams
    if (glowESPEnabled) {
        // We're already using glow, so we'll just adjust glow colors based on chams settings
        // In a full implementation, you'd hook material rendering here
    }
}

void RenderWeaponESP(const std::vector<WeaponEntity>& weapons, Matrix4x4 viewMatrix,
    float screenWidth, float screenHeight, ImDrawList* drawList) {
    if (!weaponESPEnabled) return;

    LocalPlayer lp = GetLocalPlayer();

    for (const auto& weapon : weapons) {
        float distance = sqrtf(
            powf(lp.origin.x - weapon.origin.x, 2) +
            powf(lp.origin.y - weapon.origin.y, 2) +
            powf(lp.origin.z - weapon.origin.z, 2)
        );

        if (distance > weaponMaxDistance) continue;

        Vector3 screenPos;
        if (WorldToScreen(weapon.origin, screenPos, viewMatrix, screenWidth, screenHeight)) {
            const char* weaponName = GetWeaponName(weapon.defIndex);
            char weaponText[64];
            sprintf_s(weaponText, "%s (%.0fm)", weaponName, distance / 100.0f);

            DrawTextWithShadow(drawList, ImVec2(screenPos.x, screenPos.y),
                IM_COL32(weaponColor.x * 255, weaponColor.y * 255,
                    weaponColor.z * 255, 255), weaponText);

            // Draw small box/icon
            drawList->AddRect(
                ImVec2(screenPos.x - 5, screenPos.y - 5),
                ImVec2(screenPos.x + 5, screenPos.y + 5),
                IM_COL32(weaponColor.x * 255, weaponColor.y * 255, weaponColor.z * 255, 255),
                2.0f
            );
        }
    }
}

void ProcessSounds() {
    if (!soundESPEnabled) return;

    float currentTime = ImGui::GetTime();

    // Clean old sounds - FIXED: Use iterator-based removal
    auto it = soundLocations.begin();
    while (it != soundLocations.end()) {
        if (currentTime - it->second > soundMaxTime) {
            it = soundLocations.erase(it);
        }
        else {
            ++it;
        }
    }

    // In a real implementation, you'd hook sound functions or read sound entity list
    // For now, we'll simulate by detecting enemy movements
    auto entities = GetEntities();
    LocalPlayer lp = GetLocalPlayer();

    for (const auto& e : entities) {
        if (e.health <= 0 || e.team == lp.team) continue;

        // If enemy is moving and on ground, create sound
        // Note: You'll need to find the correct velocity offset for CS2
        // This is a placeholder - you'll need to update this offset
        Vector3 velocity = ReadMemory<Vector3>(e.pawn + 0x430); // Uncomment when you have the correct offset

        float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

        if (speed > 50.0f) {
            soundLocations[e.origin] = currentTime;
        }
    }
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

            // Draw pulsing circle
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

void RenderAdvancedFlags(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color) {
    if (!showAdvancedFlags) return;

    std::vector<std::string> flags;

    // Movement states
    int flagsValue = ReadMemory<int>(e.pawn + m_fFlagsOffset);
    if (flagsValue & (1 << 0)) flags.push_back("ON GROUND");
    if (flagsValue & (1 << 1)) flags.push_back("DUCKING");
    if (flagsValue & (1 << 2)) flags.push_back("JUMPING");

    // Weapon states - simplified since we don't have the exact offsets
    uintptr_t activeWeapon = ReadMemory<uintptr_t>(e.pawn + activeWeaponOffset);
    if (activeWeapon) {
        // Check if reloading by comparing ammo
        int currentAmmo = ReadMemory<int>(activeWeapon + ammoOffset);
        int reserveAmmo = 0; // You'd need to find reserve ammo offset

        // Simple reload detection - if ammo changed recently
        static std::map<uintptr_t, int> previousAmmo;
        auto it = previousAmmo.find(activeWeapon);
        if (it != previousAmmo.end() && it->second < currentAmmo) {
            flags.push_back("RELOADING");
        }
        previousAmmo[activeWeapon] = currentAmmo;
    }

    // Render flags
    float yOffset = 0.0f;
    for (const auto& flag : flags) {
        DrawTextWithShadow(drawList, ImVec2(boxPos.x, boxPos.y + yOffset), color, flag.c_str());
        yOffset += 12.0f;
    }
}

void RenderVelocityIndicator(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color) {
    if (!velocityESPEnabled) return;

    // Placeholder - you need to find the correct velocity offset for CS2
    Vector3 velocity = ReadMemory<Vector3>(e.pawn + 0x430); // Uncomment when you have the correct offset

    float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

    char speedText[32];
    sprintf_s(speedText, "%.0f u/s", speed);

    // Color based on speed (green = slow, yellow = medium, red = fast)
    ImU32 speedColor;
    if (speed < 100.0f) speedColor = IM_COL32(0, 255, 0, 255);
    else if (speed < 200.0f) speedColor = IM_COL32(255, 255, 0, 255);
    else speedColor = IM_COL32(255, 0, 0, 255);

    DrawTextWithShadow(drawList, ImVec2(boxPos.x, boxPos.y - 35), speedColor, speedText);

    // Direction indicator
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

        // Arrow head
        drawList->AddTriangleFilled(
            arrowEnd,
            ImVec2(arrowEnd.x - 5.0f * cosf(angle + 0.5f), arrowEnd.y - 5.0f * sinf(angle + 0.5f)),
            ImVec2(arrowEnd.x - 5.0f * cosf(angle - 0.5f), arrowEnd.y - 5.0f * sinf(angle - 0.5f)),
            speedColor
        );
    }
}

void UpdateBacktrackData(const std::vector<Entity>& entities) {
    if (!backtrackESPEnabled) return;

    float currentTime = ImGui::GetTime();

    for (const auto& e : entities) {
        if (e.health <= 0) continue;

        playerHistory[e.pawn].push_back(std::make_pair(e.origin, currentTime));

        // Remove old entries - FIXED: Use iterator-based removal
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

void RunAutoStrafe(const LocalPlayer& lp) {
    if (!autoStrafeEnabled || !lp.pawn || lp.health <= 0) return;

    int flags = ReadMemory<int>(lp.pawn + m_fFlagsOffset);
    if (flags & 1) return; // On ground

    // Placeholder - you need to find the correct velocity offset for CS2
     Vector3 velocity = ReadMemory<Vector3>(lp.pawn + 0x430); // Uncomment when you have the correct offset

    float currentSpeed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);

    if (currentSpeed < 10.0f) return;

    // Simple auto strafe - adjust view angles for optimal movement
    Vector3 viewAngles = lp.viewAngles;

    // Alternate between left and right based on frame count
    static bool strafeDir = false;
    static int frameCount = 0;

    if (frameCount++ % 60 == 0) {
        strafeDir = !strafeDir;
    }

    if (strafeDir) {
        viewAngles.y += strafeSpeed;
    }
    else {
        viewAngles.y -= strafeSpeed;
    }

    // Normalize angle
    if (viewAngles.y > 180.0f) viewAngles.y -= 360.0f;
    if (viewAngles.y < -180.0f) viewAngles.y += 360.0f;

    WriteMemory<Vector3>(lp.pawn + viewAnglesOffset, viewAngles);
}





void DrawMinimalCrosshair(ImDrawList* drawList, float screenWidth, float screenHeight) {
    if (!customCrosshairEnabled) return;

    ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
    ImU32 color = ImGui::ColorConvertFloat4ToU32(crosshairColor);

    // Simple crosshair - minimal design
    float gap = crosshairSize * 0.3f;
    float length = crosshairSize;

    // Horizontal line
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

    // Vertical line
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

    // Center dot
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

    // Clean background with subtle shadow
    drawList->AddRectFilled(
        ImVec2(pos.x - 10, pos.y - 5),
        ImVec2(pos.x + textSize.x + 10, pos.y + textSize.y + 5),
        IM_COL32(0, 0, 0, 180), 6.0f
    );

    // Subtle border
    drawList->AddRect(
        ImVec2(pos.x - 10, pos.y - 5),
        ImVec2(pos.x + textSize.x + 10, pos.y + textSize.y + 5),
        IM_COL32(255, 255, 255, 30), 6.0f, ImDrawFlags_RoundCornersAll, 1.0f
    );

    // Text with slight glow
    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32(0, 0, 0, 100), watermarkText);
    drawList->AddText(pos, IM_COL32(220, 220, 220, 255), watermarkText);
}

void DrawGameInfoHUD(ImDrawList* drawList, const LocalPlayer& lp, const std::vector<Entity>& entities) {
    float yOffset = 60.0f;
    float screenWidth = ImGui::GetIO().DisplaySize.x;

    // Player info box
    if (showLocalPlayerInfo && lp.pawn) {
        char playerInfo[256];
        sprintf_s(playerInfo, "HP: %d | Armor: %d | $: ? | K/D: ?/?", lp.health, lp.armor);

        ImVec2 textSize = ImGui::CalcTextSize(playerInfo);
        ImVec2 pos(10.0f, yOffset);

        // Background
        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(255, 255, 255, 255), playerInfo);

        yOffset += textSize.y + 10.0f;
    }

    // Weapon info
    if (showWeaponInfo && lp.activeWeapon) {
        char weaponInfo[128];
        sprintf_s(weaponInfo, "Weapon: %s | Ammo: %d/%d", GetWeaponName(lp.weaponDefIndex), lp.ammo, 0); // Reserve ammo not available

        ImVec2 textSize = ImGui::CalcTextSize(weaponInfo);
        ImVec2 pos(10.0f, yOffset);

        // Background
        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);

        // Color based on ammo
        ImU32 color = lp.ammo > 5 ? IM_COL32(255, 255, 255, 255) : IM_COL32(255, 50, 50, 255);
        drawList->AddText(pos, color, weaponInfo);

        yOffset += textSize.y + 10.0f;
    }

    // Entity count
    if (showEntityCount) {
        char entityInfo[64];
        sprintf_s(entityInfo, "Entities: %d", (int)entities.size());

        ImVec2 textSize = ImGui::CalcTextSize(entityInfo);
        ImVec2 pos(10.0f, yOffset);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(200, 200, 255, 255), entityInfo);

        yOffset += textSize.y + 10.0f;
    }

    // Spectator count
    if (showSpectatorCount) {
        auto spectators = GetSpectators();
        if (!spectators.empty()) {
            char specInfo[64];
            sprintf_s(specInfo, "Spectators: %d", (int)spectators.size());

            ImVec2 textSize = ImGui::CalcTextSize(specInfo);
            ImVec2 pos(10.0f, yOffset);

            drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
            drawList->AddText(pos, IM_COL32(255, 200, 50, 255), specInfo);
        }
    }

    // Position info (if enabled)
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

    // FOV Changer indicator
    if (showFovChangerIndicator && fovChangerEnabled) {
        char fovText[32];
        sprintf_s(fovText, "FOV: %.0f", fovChangerValue);

        ImVec2 textSize = ImGui::CalcTextSize(fovText);
        ImVec2 pos(xOffset, yOffset);

        drawList->AddRectFilled(ImVec2(pos.x - 5, pos.y - 2), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 2), IM_COL32(0, 0, 0, 150), 4.0f);
        drawList->AddText(pos, IM_COL32(100, 255, 100, 255), fovText);

        yOffset -= textSize.y + 5.0f;
    }


    // Aimbot status
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

    // Background for all keybinds
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

    // Title
    drawList->AddText(ImVec2(xOffset, yOffset), IM_COL32(255, 255, 100, 255), "Keybinds:");
    yOffset += 20;

    // Keybind list
    for (const char* bind : keybinds) {
        drawList->AddText(ImVec2(xOffset, yOffset), IM_COL32(255, 255, 255, 255), bind);
        yOffset += 20;
    }
}

void glow(uintptr_t current) {
    if (!IsValidPtr((void*)current)) return;

    uintptr_t glowBase = current + 0xCC0;
    if (!IsValidPtr((void*)glowBase)) return;

    uintptr_t glowColorOverride = glowBase + 0x40;
    uintptr_t glowEnable = glowBase + 0x51;

    if (!IsValidPtr((void*)glowColorOverride) || !IsValidPtr((void*)glowEnable)) return;

    // red color yaya
    *(uint64_t*)(glowColorOverride) = 0x800000FF;
    *(bool*)(glowEnable) = true;
}

bool WorldToScreen(Vector3 pos, Vector3& screen, Matrix4x4 viewMatrix, float screenWidth, float screenHeight) {
    float w = viewMatrix.m[3][0] * pos.x + viewMatrix.m[3][1] * pos.y + viewMatrix.m[3][2] * pos.z + viewMatrix.m[3][3];
    if (w < 0.01f) return false;
    float x = viewMatrix.m[0][0] * pos.x + viewMatrix.m[0][1] * pos.y + viewMatrix.m[0][2] * pos.z + viewMatrix.m[0][3];
    float y = viewMatrix.m[1][0] * pos.x + viewMatrix.m[1][1] * pos.y + viewMatrix.m[1][2] * pos.z + viewMatrix.m[1][3];
    screen.x = (screenWidth / 2.0f) + (screenWidth / 2.0f) * x / w;
    screen.y = (screenHeight / 2.0f) - (screenHeight / 2.0f) * y / w;
    return true;
}

Vector3 GetBonePosition(uintptr_t boneMatrix, int boneIndex) {
    Vector3 pos;
    uintptr_t boneOffset = boneMatrix + boneIndex * 0x20;
    pos.x = ReadMemory<float>(boneOffset + 0x00);
    pos.y = ReadMemory<float>(boneOffset + 0x04);
    pos.z = ReadMemory<float>(boneOffset + 0x08);
    return pos;
}

std::vector<std::string> GetSpectators() {
    std::vector<std::string> spectators;
    uintptr_t localController = ReadMemory<uintptr_t>(clientBase + localPlayerControllerOffset);
    if (!localController) return spectators;

    uint32_t localPawnHandle = ReadMemory<uint32_t>(localController + pawnHandleOffset);
    if (localPawnHandle == 0xFFFFFFFF) return spectators; // Invalid

    LocalPlayer lp = GetLocalPlayer(); // Assume this gets local index or use 0 for local
    int localIndex = lp.index; // If not available, find local index via playerSlotOffset +1

    for (int i = 1; i < 65; i++) {
        if (i == localIndex) continue; // Skip local

        uintptr_t controller = GetControllerByIndex(i);
        if (!controller) continue;

        uint32_t pawnHandle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
        if (pawnHandle == 0xFFFFFFFF) continue;

        uintptr_t pawn = GetPawnByIndex(i); // Reuses handle logic internally
        if (!pawn) continue;

        uintptr_t obsServices = ReadMemory<uintptr_t>(pawn + observerServicesOffset);
        if (!obsServices) continue;

        uint8_t obsMode = ReadMemory<uint8_t>(obsServices + observerModeOffset);
        if (obsMode == 0) continue; // Not observing

        uint32_t obsTarget = ReadMemory<uint32_t>(obsServices + observerTargetOffset);
        if (obsTarget == 0xFFFFFFFF) continue;

        if (obsTarget == localPawnHandle) {
            char name[64];
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(controller + playerNameOffset), name, sizeof(name), nullptr);
            name[63] = '\0';
            spectators.push_back(std::string(name));
        }
    }
    return spectators;
}

LocalPlayer GetLocalPlayer() {
    LocalPlayer lp = {};

    // Check if client base is valid first
    if (!clientBase) {
        debugLog += "[ERROR] GetLocalPlayer: Invalid client base\n";
        return lp;
    }

    lp.pawn = ReadMemory<uintptr_t>(clientBase + localPlayerOffset);

    // If pawn is 0, we're probably in menu/not in game
    if (!lp.pawn) {
        debugLog += "[INFO] GetLocalPlayer: No local player pawn (probably in menu)\n";
        return lp;
    }

    // Read scene node for origin and view angles
    uintptr_t sceneNode = ReadMemory<uintptr_t>(lp.pawn + sceneNodeOffset);
    if (!sceneNode) {
        std::stringstream ss;
        ss << "[INFO] GetLocalPlayer: Invalid scene node for pawn 0x" << std::hex << lp.pawn << " (probably in menu)\n";
        debugLog += ss.str();
        return lp;
    }

    // Try to read origin - if this fails, we're likely in menu
    Vector3 testOrigin = ReadMemory<Vector3>(sceneNode + originOffset);
    if (testOrigin.x == 0.0f && testOrigin.y == 0.0f && testOrigin.z == 0.0f) {
        debugLog += "[INFO] GetLocalPlayer: Zero origin (probably in menu)\n";
        return lp;
    }

    lp.origin = testOrigin;
    lp.viewAngles = ReadMemory<Vector3>(lp.pawn + viewAnglesOffset);
    lp.team = ReadMemory<int>(lp.pawn + teamOffset);
    lp.health = ReadMemory<int>(lp.pawn + healthOffset);

    // Read active weapon and ammo (these might be 0 in menu)
    lp.activeWeapon = ReadMemory<uintptr_t>(lp.pawn + activeWeaponOffset);
    lp.ammo = lp.activeWeapon ? ReadMemory<int>(lp.activeWeapon + ammoOffset) : 0;

    // Read local controller for player index
    uintptr_t localController = ReadMemory<uintptr_t>(clientBase + localPlayerControllerOffset);
    if (localController) {
        lp.index = ReadMemory<int>(localController + playerSlotOffset);
    }
    else {
        lp.index = -1;
        debugLog += "[INFO] GetLocalPlayer: No local controller (probably in menu)\n";
    }
    return lp;
}

std::vector<Entity> GetEntities() {
    std::vector<Entity> entities;

    if (!clientBase) {
        return entities;
    }

    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) {
        return entities;
    }

    LocalPlayer lp = GetLocalPlayer();

    for (int i = 1; i < 64; i++) {
        uintptr_t list_entry = ReadMemory<uintptr_t>(entityList + 0x8 * (i >> 9) + 0x10);
        if (!list_entry) continue;

        uintptr_t controller = ReadMemory<uintptr_t>(list_entry + 0x78 * (i & 0x1FF));
        if (!controller) continue;

        uint32_t pawn_handle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
        if (!pawn_handle || pawn_handle == 0xFFFFFFFF) continue;

        uintptr_t p_index = pawn_handle & 0x7FFF;
        uintptr_t list_entry2 = ReadMemory<uintptr_t>(entityList + 0x8 * (p_index >> 9) + 0x10);
        if (!list_entry2) continue;

        uintptr_t entity = ReadMemory<uintptr_t>(list_entry2 + 0x78 * (p_index & 0x1FF));
        if (!entity) continue;

        // Check if this entity is valid by reading health
        int health = ReadMemory<int>(entity + healthOffset);
        if (health < 0 || health > 100) continue; // Skip invalid entities

        Entity e;
        e.health = health;

        uintptr_t sceneNode = ReadMemory<uintptr_t>(entity + sceneNodeOffset);
        if (!sceneNode) continue;

        e.origin = ReadMemory<Vector3>(sceneNode + originOffset);
        e.team = ReadMemory<int>(entity + teamOffset);
        e.boneMatrix = ReadMemory<uintptr_t>(sceneNode + boneMatrixOffset);

        // Read name safely
        uintptr_t namePtr = ReadMemory<uintptr_t>(controller + playerNameOffset);
        if (namePtr) {
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)namePtr, e.name, 64, nullptr);
            e.name[63] = '\0';
        }
        else {
            strcpy_s(e.name, "Unknown");
        }

        uintptr_t activeWeapon = ReadMemory<uintptr_t>(entity + activeWeaponOffset);
        if (activeWeapon) {
            e.weaponDefIndex = ReadMemory<int>(activeWeapon + itemDefIndexOffset);
            e.ammo = ReadMemory<int>(activeWeapon + ammoOffset);
        }
        else {
            e.weaponDefIndex = 0;
            e.ammo = 0;
        }

        e.armor = ReadMemory<int>(entity + armorOffset);
        e.hasDefuser = ReadMemory<bool>(entity + hasDefuserOffset);
        e.hasHelmet = ReadMemory<bool>(entity + hasHelmetOffset);
        e.flashDuration = ReadMemory<float>(entity + flashDurationOffset);
        e.scoped = ReadMemory<bool>(entity + scopedOffset);
        e.defusing = ReadMemory<bool>(entity + isDefusingOffset);

        // Visibility check - only if we have a valid local player index
        if (lp.index >= 0 && lp.index < 64) {
            uintptr_t spottedAddr = entity + entitySpottedStateOffset + 0x8 + (lp.index * sizeof(bool));
            e.visible = ReadMemory<bool>(spottedAddr);
        }
        else {
            e.visible = false;
        }

        e.pawn = entity;
        e.lastHitGroup = ReadMemory<int>(entity + lastHitGroupOffset);

        entities.push_back(e);
    }
    return entities;
}

std::vector<WeaponEntity> GetDroppedWeapons() {
    std::vector<WeaponEntity> weapons;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    for (int i = 64; i < 1024; i++) {
        uintptr_t list_entry = ReadMemory<uintptr_t>(entityList + 0x8 * (i >> 9) + 0x10);
        if (!list_entry) continue;
        uintptr_t entity = ReadMemory<uintptr_t>(list_entry + 0x78 * (i & 0x1FF));
        if (entity) {
            uint32_t owner = ReadMemory<uint32_t>(entity + ownerEntityOffset);
            if (owner == 0xFFFFFFFF) {
                uintptr_t sceneNode = ReadMemory<uintptr_t>(entity + sceneNodeOffset);
                Vector3 origin = ReadMemory<Vector3>(sceneNode + originOffset);
                int defIndex = ReadMemory<int>(entity + itemDefIndexOffset);
                if (defIndex > 0) {
                    WeaponEntity w;
                    w.origin = origin;
                    w.defIndex = defIndex;
                    weapons.push_back(w);
                }
            }
        }
    }
    return weapons;
}

const char* GetWeaponName(int defIndex) {
    switch (defIndex) {
    case 1: return "Deagle";
    case 2: return "Dual Berettas";
    case 3: return "Five-SeveN";
    case 4: return "Glock-18";
    case 7: return "AK-47";
    case 8: return "AUG";
    case 9: return "AWP";
    case 10: return "FAMAS";
    case 11: return "G3SG1";
    case 13: return "Galil AR";
    case 14: return "M249";
    case 16: return "M4A4";
    case 17: return "MAC-10";
    case 19: return "P90";
    case 23: return "MP5-SD";
    case 24: return "UMP-45";
    case 25: return "XM1014";
    case 26: return "PP-Bizon";
    case 27: return "MAG-7";
    case 28: return "Negev";
    case 29: return "Sawed-Off";
    case 30: return "Tec-9";
    case 31: return "Zeus x27";
    case 32: return "P2000";
    case 33: return "MP7";
    case 34: return "MP9";
    case 35: return "Nova";
    case 36: return "P250";
    case 38: return "SCAR-20";
    case 39: return "SG 553";
    case 40: return "SSG 08";
    case 42: return "Knife";
    case 43: return "Flashbang";
    case 44: return "HE Grenade";
    case 45: return "Smoke Grenade";
    case 46: return "Molotov";
    case 47: return "Decoy Grenade";
    case 48: return "Incendiary Grenade";
    case 49: return "C4";
    case 60: return "M4A1-S";
    case 61: return "USP-S";
    case 63: return "CZ75-Auto";
    case 64: return "R8 Revolver";
    default: return "Unknown";
    }
}

void RenderCleanESPBox(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight,
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
    float width = height / 2.2f; // More realistic aspect ratio

    float x = screenHead.x - width / 2.0f;
    float y = screenHead.y;

    // Skip if unrealistic size
    if (height < 10.0f || height > 500.0f || width < 5.0f || width > 250.0f) {
        return;
    }

    // Clean box with subtle outline
    ImVec2 boxMin(x, y);
    ImVec2 boxMax(x + width, y + height);

    if (outlineEnabled) {
        // Subtle outline
        drawList->AddRect(
            ImVec2(boxMin.x - 1, boxMin.y - 1),
            ImVec2(boxMax.x + 1, boxMax.y + 1),
            IM_COL32(0, 0, 0, 150), boxRounding, ImDrawFlags_RoundCornersAll, 1.0f
        );
    }

    // Main box
    drawList->AddRect(boxMin, boxMax, color, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);

    // Clean health bar
    if (healthBarEnabled) {
        float healthFrac = e.health / 100.0f;
        float barWidth = 4.0f;
        float barHeight = height * healthFrac;

        ImVec2 barPos(x - barWidth - 3, y + height - barHeight);
        ImVec2 barSize(x - 3, y + height);

        // Health bar background
        drawList->AddRectFilled(
            ImVec2(x - barWidth - 4, y - 1),
            ImVec2(x - 2, y + height + 1),
            IM_COL32(0, 0, 0, 150), 2.0f
        );

        // Health fill with gradient
        ImU32 healthColor = IM_COL32(
            255 * (1.0f - healthFrac),  // More red when low health
            255 * healthFrac,           // More green when high health
            0, 255
        );

        drawList->AddRectFilled(barPos, barSize, healthColor, 2.0f);
    }

    // Clean info text
    if (distanceEnabled || ammoEnabled) {
        std::string infoText;
        if (distanceEnabled) {
            float dist = sqrtf(powf(GetLocalPlayer().origin.x - e.origin.x, 2) +
                powf(GetLocalPlayer().origin.y - e.origin.y, 2)) / 100.0f;
            infoText += std::to_string((int)dist) + "m";
        }
        if (ammoEnabled && e.ammo > 0) {
            if (!infoText.empty()) infoText += " | ";
            infoText += std::to_string(e.ammo);
        }

        if (!infoText.empty()) {
            ImVec2 textSize = ImGui::CalcTextSize(infoText.c_str());
            ImVec2 textPos(x + (width - textSize.x) / 2.0f, boxMax.y + 2);
            DrawTextWithShadow(drawList, textPos, color, infoText.c_str());
        }
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

    ImU32 outlineColor = IM_COL32(0, 0, 0, 255); // Black outline
    float outlineThickness = boxThickness + 2.0f; // Slightly thicker for outline

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

    // Cleaner background with slight transparency and border
    drawList->AddRectFilled(ImVec2(radarX, radarY), ImVec2(radarX + radarSize, radarY + radarSize), IM_COL32(0, 0, 0, 180));
    drawList->AddRect(ImVec2(radarX - 1, radarY - 1), ImVec2(radarX + radarSize + 1, radarY + radarSize + 1), IM_COL32(255, 255, 255, 200), 4.0f, ImDrawFlags_RoundCornersAll, 2.0f);

    // Local player indicator (green at center)
    drawList->AddCircleFilled(ImVec2(radarCenterX, radarCenterY), 5.0f, IM_COL32(0, 255, 0, 255), 12);

    float currentTime = ImGui::GetTime();
    float yawRad = ToRadians(lp.viewAngles.y);
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);

    for (const auto& e : entities) {
        if (e.pawn == lp.pawn) continue;  // Skip local player

        bool isDead = e.health <= 0;
        bool shouldRender = !isDead || (deathTimes.count(e.pawn) && currentTime - deathTimes[e.pawn] < 1.0f);
        if (!shouldRender || (!showTeammates && e.team == lp.team)) continue;
        float dx = e.origin.x - lp.origin.x;
        float dy = e.origin.y - lp.origin.y;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist > maxDist) continue;

        // Rotate based on view angle
        float rotated_dx = dx * cosYaw + dy * sinYaw;
        float rotated_dy = -dx * sinYaw + dy * cosYaw;  // Adjust for forward up

        float radarPosX = radarCenterX + (rotated_dx / maxDist) * (radarSize / 2.0f);
        float radarPosY = radarCenterY + (rotated_dy / maxDist) * (radarSize / 2.0f);  // + for forward up (assuming y increases down)

        ImU32 color = (e.team == lp.team) ? IM_COL32(0, 0, 255, 255)  // Blue for teammates
            : IM_COL32(255, 0, 0, 255); // Red for enemies

        // Outline for radar dots
        if (outlineEnabled) {
            drawList->AddCircleFilled(ImVec2(radarPosX, radarPosY), 5.0f, IM_COL32(0, 0, 0, 255), 12);
        }
        drawList->AddCircleFilled(ImVec2(radarPosX, radarPosY), 4.0f, color, 12);
    }
}

void RemoveGlow(uintptr_t entity) {
    if (!IsValidPtr((void*)entity)) return;

    uintptr_t glowBase = entity + 0xCC0;
    if (!IsValidPtr((void*)glowBase)) return;

    uintptr_t glowEnable = glowBase + 0x51;
    if (IsValidPtr((void*)glowEnable)) {
        WriteMemory<bool>(glowEnable, false);
    }
}

void ApplyGlow(uintptr_t entity, GlowSettings* settings, bool isVisible) {
    if (!IsValidPtr((void*)entity) || !settings->enabled) {
        RemoveGlow(entity);
        return;
    }

    uintptr_t glowBase = entity + 0xCC0;
    if (!IsValidPtr((void*)glowBase)) return;

    // Calculate final color with brightness
    ImVec4 finalColor = settings->color;
    finalColor.x *= settings->brightness; // R
    finalColor.y *= settings->brightness; // G  
    finalColor.z *= settings->brightness; // B
    finalColor.w *= settings->brightness; // A

    // Clamp values
    finalColor.x = std::min(std::max(finalColor.x, 0.0f), 1.0f);
    finalColor.y = std::min(std::max(finalColor.y, 0.0f), 1.0f);
    finalColor.z = std::min(std::max(finalColor.z, 0.0f), 1.0f);
    finalColor.w = std::min(std::max(finalColor.w, 0.0f), 1.0f);

    // Convert to ARGB (Alpha, Red, Green, Blue)
    uint8_t alpha = static_cast<uint8_t>(finalColor.w * 255);
    uint8_t red = static_cast<uint8_t>(finalColor.x * 255);
    uint8_t green = static_cast<uint8_t>(finalColor.y * 255);
    uint8_t blue = static_cast<uint8_t>(finalColor.z * 255);

    uint32_t glowColor = (alpha << 24) | (red << 16) | (green << 8) | blue;

    uintptr_t glowColorOverride = glowBase + 0x40;
    uintptr_t glowEnable = glowBase + 0x51;

    if (!IsValidPtr((void*)glowColorOverride) || !IsValidPtr((void*)glowEnable)) return;

    // Apply glow
    WriteMemory<uint32_t>(glowColorOverride, glowColor);
    WriteMemory<bool>(glowEnable, true);

    // Optional: Apply outline settings if available
    uintptr_t glowThroughWallsAddr = glowBase + 0x50; // Common offset for through-walls
    if (IsValidPtr((void*)glowThroughWallsAddr)) {
        WriteMemory<bool>(glowThroughWallsAddr, glowThroughWalls);
    }

    // Optional: Apply outline width if available
    uintptr_t outlineWidthAddr = glowBase + 0x48; // Common offset for outline width
    if (IsValidPtr((void*)outlineWidthAddr) && settings->outline) {
        WriteMemory<float>(outlineWidthAddr, settings->outlineWidth);
    }
}

void RunExternalAimbot() {
    if (!externalAimbotEnabled) return;

    // Record key if needed
    static bool recording = false;
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

    // Check if key is pressed
    if (!GetAsyncKeyState(externalAimbotKey)) return;

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn || lp.health <= 0) return;

    std::vector<Entity> entities = GetEntities();
    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;

    Entity* closestTarget = nullptr;
    float closestDistance = FLT_MAX;
    Vector2 targetScreenPos;

    // Find closest target to crosshair
    for (auto& entity : entities) {
        if (entity.health <= 0 || entity.health > 100) continue;
        if (entity.team == lp.team) continue; // Don't aim at teammates
        if (!entity.boneMatrix) continue;

        // Get target position based on selected hitgroup
        Vector3 targetPos;
        int boneIndex = hitgroupBones[externalAimbotHitgroup];

        if (externalAimbotHitgroup == 4) { // Random hitgroup
            boneIndex = hitgroupBones[rand() % 4]; // Random between 0-3 (head, neck, chest, pelvis)
        }

        targetPos = GetBonePosition(entity.boneMatrix, boneIndex);

        Vector3 screenPos;
        if (WorldToScreen(targetPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
            // Calculate distance from crosshair
            float centerX = screenWidth / 2.0f;
            float centerY = screenHeight / 2.0f;
            float dx = screenPos.x - centerX;
            float dy = screenPos.y - centerY;
            float distance = sqrtf(dx * dx + dy * dy);

            // Convert FOV to pixels (approximate)
            float fovPixels = (externalAimbotFOV / 90.0f) * (screenWidth / 2.0f);

            if (distance < fovPixels && distance < closestDistance) {
                closestDistance = distance;
                closestTarget = &entity;
                targetScreenPos = { screenPos.x, screenPos.y };
            }
        }
    }

    if (closestTarget != nullptr) {
        float centerX = screenWidth / 2.0f;
        float centerY = screenHeight / 2.0f;

        // Calculate mouse movement
        float deltaX = targetScreenPos.x - centerX;
        float deltaY = targetScreenPos.y - centerY;

        // Apply smoothing (0 = instant)
        if (externalAimbotSmoothness > 0) {
            deltaX /= externalAimbotSmoothness;
            deltaY /= externalAimbotSmoothness;
        }

        // Move mouse
        mouse_event(MOUSEEVENTF_MOVE, (int)deltaX, (int)deltaY, 0, 0);

        const char* hitgroupName = hitgroupNames[externalAimbotHitgroup];
        if (externalAimbotHitgroup == 4) {
            hitgroupName = "Random";
        }

        debugLog += "[EXTERNAL AIMBOT] Aiming at: " + std::string(closestTarget->name) +
            " | Hitgroup: " + hitgroupName +
            " | Distance: " + std::to_string(closestDistance) + " pixels\n";
    }
}

bool IsGameInFocus() {
    HWND foregroundWindow = GetForegroundWindow();
    return (foregroundWindow == g_hwnd);
}

void RenderExternalAimbotFOV(ImDrawList* drawList, float screenWidth, float screenHeight) {
    if (!externalAimbotEnabled) return;

    // Don't show FOV circle if game is not in focus
    if (!IsGameInFocus()) return;

    // Convert FOV to pixels
    float fovPixels = (externalAimbotFOV / 90.0f) * (screenWidth / 2.0f);

    // Choose color based on whether aimbot is active
    ImU32 circleColor;
    if (GetAsyncKeyState(externalAimbotKey) && !menuOpen) {
        circleColor = IM_COL32(255, 0, 0, 100); // Red when active
    }
    else {
        circleColor = IM_COL32(0, 255, 0, 80);  // Green when inactive
    }

    ImU32 outlineColor = IM_COL32(0, 0, 0, 150);

    // Draw FOV circle
    ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);

    // Outline
    drawList->AddCircle(center, fovPixels + 1.0f, outlineColor, 0, 2.0f);
    // Main circle
    drawList->AddCircle(center, fovPixels, circleColor, 0, 1.5f);

    // Add crosshair lines to show FOV circle is for aimbot
    float crossSize = 8.0f;
    drawList->AddLine(ImVec2(center.x - crossSize, center.y), ImVec2(center.x + crossSize, center.y), circleColor, 1.0f);
    drawList->AddLine(ImVec2(center.x, center.y - crossSize), ImVec2(center.x, center.y + crossSize), circleColor, 1.0f);
}

void RunAimbot() {
    // Record key if needed
    RecordAimKey();

    // Check if aimbot should run
    if (!aimbotEnabled) {
        return;
    }

    bool aimKeyDown = (GetAsyncKeyState(aimKey) & 0x8000) != 0;
    if (!aimKeyDown) {
        return;
    }

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn || lp.health <= 0) {
        debugLog += "[AIMBOT] No local player or dead\n";
        return;
    }

    std::vector<Entity> entities = GetEntities();
    if (entities.empty()) {
        debugLog += "[AIMBOT] No entities found\n";
        return;
    }

    Entity* bestTarget = nullptr;
    float bestFov = aimbotFOV;
    Vector3 bestAngle = { 0, 0, 0 };

    for (auto& entity : entities) {
        // Skip invalid targets
        if (entity.health <= 0 || entity.health > 100) continue;
        if (entity.team == lp.team && !aimTeammates) continue;
        if (aimOnVisibleOnly && !entity.visible) continue;
        if (!entity.boneMatrix) continue;

        // Get head position
        Vector3 headPos = GetBonePosition(entity.boneMatrix, bone_head);

        // Calculate angle to target
        Vector3 angleTo = CalculateAngle(lp.origin, headPos);

        // Get current view angles
        Vector3 currentAngles = lp.viewAngles;

        // Calculate FOV difference
        float deltaX = angleTo.x - currentAngles.x;
        float deltaY = angleTo.y - currentAngles.y;

        // Normalize angles
        while (deltaY > 180.0f) deltaY -= 360.0f;
        while (deltaY < -180.0f) deltaY += 360.0f;

        float fov = sqrtf(deltaX * deltaX + deltaY * deltaY);

        if (fov < bestFov) {
            bestFov = fov;
            bestTarget = &entity;
            bestAngle = angleTo;
        }
    }

    if (bestTarget != nullptr) {
        // Clamp angles
        if (bestAngle.x > 89.0f) bestAngle.x = 89.0f;
        if (bestAngle.x < -89.0f) bestAngle.x = -89.0f;

        // Apply smoothing
        Vector3 currentAngles = ReadMemory<Vector3>(lp.pawn + viewAnglesOffset);
        Vector3 smoothedAngle = SmoothAngle(currentAngles, bestAngle, aimbotSmoothing);

        // Write new angles
        WriteMemory<Vector3>(lp.pawn + viewAnglesOffset, smoothedAngle);

        // Debug logging
        debugLog += "[AIMBOT] Targeting: " + std::string(bestTarget->name) +
            " | FOV: " + std::to_string(bestFov) +
            " | Health: " + std::to_string(bestTarget->health) +
            " | Team: " + std::to_string(bestTarget->team) + "\n";
    }
    else {
        debugLog += "[AIMBOT] No valid target found in FOV\n";
    }
}

void ApplyFeatures(const LocalPlayer& lp, const std::vector<Entity>& entities) {
    if (!lp.pawn) {
        debugLog += "[FEATURES] No local player pawn\n";
        return;
    }
    static std::map<uintptr_t, int> previousHealth; // Assuming this already exists for health tracking
    static int prevLocalAmmo = -1; // New: Track local player's previous ammo
    static float lastFireTime = 0.0f; // New: Timestamp of last local fire

    RunAutoStrafe(lp);
    UpdateBacktrackData(entities);
    ProcessSounds();

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

        ApplyChams(e.pawn, e.team == lp.team, e.visible);

        // Get previous health
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

    if (glowESPEnabled) {
        static std::map<uintptr_t, bool> glowApplied; // Track which entities have glow applied

        for (const auto& e : entities) {
            if (e.health <= 0 || e.health > 100) continue;
            if (!e.pawn) continue;

            // Distance check
            float distance = sqrtf(
                powf(lp.origin.x - e.origin.x, 2) +
                powf(lp.origin.y - e.origin.y, 2) +
                powf(lp.origin.z - e.origin.z, 2)
            );

            if (distance > glowMaxDistance) {
                // Remove glow if too far
                if (glowApplied[e.pawn]) {
                    RemoveGlow(e.pawn);
                    glowApplied[e.pawn] = false;
                }
                continue;
            }

            // Team check
            if (e.team == lp.team && !showTeammates) {
                if (glowApplied[e.pawn]) {
                    RemoveGlow(e.pawn);
                    glowApplied[e.pawn] = false;
                }
                continue;
            }

            // Apply or update glow
            GlowSettings* settings = (e.team == lp.team) ? &teammateGlowSettings : &enemyGlowSettings;

            if (!glowApplied[e.pawn] || settings->enabled) {
                ApplyGlow(e.pawn, settings, e.visible);
                glowApplied[e.pawn] = true;
            }
        }

        // Clean up glowApplied map for entities that no longer exist
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
            uintptr_t controller = ReadMemory<uintptr_t>(list_entry + 0x78 * (i & 0x1FF));
            if (!controller) continue;
            uint32_t pawn_handle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
            if (!pawn_handle) continue;
            uintptr_t p_index = pawn_handle & 0x7FFF;
            uintptr_t list_entry2 = ReadMemory<uintptr_t>(entityList + 0x8 * (p_index >> 9) + 0x10);
            if (!list_entry2) continue;
            uintptr_t entity = ReadMemory<uintptr_t>(list_entry2 + 0x78 * (p_index & 0x1FF));
            if (entity && entity != lp.pawn) {
                int team = ReadMemory<int>(entity + teamOffset);
                if (team != lp.team) {
                    uintptr_t spottedAddr = entity + entitySpottedStateOffset + 0x8 + (lp.index * sizeof(bool));
                    WriteMemory<bool>(spottedAddr, true);
                }
            }
        }
    }
    if (noRecoilEnabled) {
        Vector3 zeroPunch = { 0.0f, 0.0f, 0.0f };
        WriteMemory<Vector3>(lp.pawn + aimPunchOffset, zeroPunch);
    }

    // New: Bunnyhop
    if (bunnyhopEnabled && (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
        int flags = ReadMemory<int>(lp.pawn + m_fFlagsOffset);
        if (flags & 1) {  // On ground
            WriteMemory<int>(clientBase + forceJumpOffset, 65537);  // Press
        }
        else {
            WriteMemory<int>(clientBase + forceJumpOffset, 256);  // Release
        }

        // Fallback if forceJump doesn't work (simulate space press/release)
        if (flags & 1) {
            keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);  // Release first for perfect timing
            Sleep(1);
            keybd_event(VK_SPACE, 0, 0, 0);  // Press
        }
    }
}

void RunTriggerbot(const LocalPlayer& lp, const std::vector<Entity>& entities) {
    if (!triggerbotEnabled) return;

    int crosshairID = ReadMemory<int>(lp.pawn + crosshairIDOffset);
    if (crosshairID < 1 || crosshairID > 64) return;

    uintptr_t targetPawn = GetPawnByIndex(crosshairID);
    if (!targetPawn) return;

    int team = ReadMemory<int>(targetPawn + teamOffset);
    int health = ReadMemory<int>(targetPawn + healthOffset);

    // Optional visibility (add if you have vis check offset/function, else assume crosshair means visible)
    bool visible = true;  // Placeholder; implement if needed

    if (health > 0 && health <= 100 && team != lp.team && visible) {
        int delay = static_cast<int>(triggerbotDelay * (rand() % 100 / 100.0f));
        Sleep(delay);

        // Shoot
        WriteMemory<int>(clientBase + forceAttackOffset, 5);  // Press
        Sleep(10);
        WriteMemory<int>(clientBase + forceAttackOffset, 4);  // Release

        // Fallback if forceAttack doesn't work (simulate mouse click)
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        Sleep(10);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
}

void DrawTextWithShadow(ImDrawList* drawList, ImVec2 pos, ImU32 color, const char* text) {
    // Shadow
    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32(0, 0, 0, 255), text);
    // Main text
    drawList->AddText(pos, color, text);
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

void RenderESP() {
    if (!clientBase) {
        return;
    }
    LocalPlayer lp = GetLocalPlayer();

    if (!lp.pawn) {
        // Just render the overlay but skip entity rendering
        ImGuiIO& io = ImGui::GetIO();
        auto drawList = ImGui::GetBackgroundDrawList();
        drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(200, 50),
            IM_COL32(0, 0, 0, 200), IM_COL32(0, 0, 0, 200), IM_COL32(20, 20, 20, 200), IM_COL32(20, 20, 20, 200));
        drawList->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "CS2 WoW Cheat v1.0");
        drawList->AddText(ImVec2(10, 30), IM_COL32(200, 200, 200, 255), "In Menu - Waiting for Game");
        return;
    }

    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;
    float deltaTime = io.DeltaTime;

    auto drawList = ImGui::GetBackgroundDrawList();

    RenderExternalAimbotFOV(drawList, screenWidth, screenHeight);

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
    }
    RunTriggerbot(lp, entities);  // New

    // Rate-limited console update to prevent flooding
    if (showDebugConsole) {
        static float lastUpdate = 0.0f;
        float now = ImGui::GetTime();
        if (now - lastUpdate > 0.5f) {  // Update every 0.5 seconds
            system("cls");  // Clear console
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

    RenderWeaponESP(GetDroppedWeapons(), viewMatrix, screenWidth, screenHeight, drawList);
    RenderSoundESP(viewMatrix, screenWidth, screenHeight, drawList);
    RenderBacktrackESP(viewMatrix, screenWidth, screenHeight, drawList);

    if (showSpectatorList) {
        ImGuiIO& io = ImGui::GetIO();
        float screenWidth = io.DisplaySize.x;
        float screenHeight = io.DisplaySize.y;

        ImGui::SetNextWindowPos(ImVec2(screenWidth - 220.0f, 10.0f));
        ImGui::SetNextWindowSize(ImVec2(200.0f, 300.0f));

        ImGui::Begin("Spectator List", &showSpectatorList, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        auto spectators = GetSpectators();
        if (spectators.empty()) {
            ImGui::Text("None");
        }
        else {
            for (const auto& name : spectators) {
                ImGui::Text("%s", name.c_str());
            }
        }
        ImGui::End();
    }

    static std::map<uintptr_t, int> previousHealth;
    static std::map<uintptr_t, int> previousHitGroup;

    float currentTime = ImGui::GetTime();

    // Clean up deathTimes for non-existent entities
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

    for (auto& e : entities) {
        if (hitsoundEnabled && e.team != lp.team) {
            auto healthIt = previousHealth.find(e.pawn);
            auto hitGroupIt = previousHitGroup.find(e.pawn);
            int prevHealth = (healthIt != previousHealth.end()) ? healthIt->second : 100;
            int prevHitGroup = (hitGroupIt != previousHitGroup.end()) ? hitGroupIt->second : 0;

            if (e.health != prevHealth || e.lastHitGroup != prevHitGroup) {
                if (e.health <= 0 && prevHealth > 0) {
                    PlayMP3(killSoundPath);
                    debugLog += "[DEBUG] Killsound triggered for entity " + std::string(e.name) + "\n";
                    AddNotification("Enemy Killed: " + std::string(e.name), 3.0f, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                    deathTimes[e.pawn] = static_cast<float>(ImGui::GetTime());
                }
                else if (e.health > 0 && e.health < prevHealth) {
                    int damage = prevHealth - e.health;
                    if (e.lastHitGroup == 8) { // HITGROUP_HEAD = 8
                        PlayMP3(headSoundPath);
                        debugLog += "[DEBUG] Headshot sound triggered for entity " + std::string(e.name) + "\n";
                        AddNotification("Headshot: -" + std::to_string(damage), 2.0f, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
                    }
                    else {
                        PlayMP3(hitSoundPath);
                        debugLog += "[DEBUG] Hitsound triggered for entity " + std::string(e.name) + "\n";
                        AddNotification("Hit: -" + std::to_string(damage), 2.0f, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                    }
                    hitmarkers.push_back({ GetBonePosition(e.boneMatrix, bone_head), damage, static_cast<float>(ImGui::GetTime()), hitmarkerFadeDuration });
                }
            }
            previousHealth[e.pawn] = e.health;
            previousHitGroup[e.pawn] = e.lastHitGroup;
        }

        bool isDead = e.health <= 0;
        bool shouldRender = !isDead || (deathTimes.count(e.pawn) && currentTime - deathTimes[e.pawn] < 1.0f);
        if (!shouldRender || (!showTeammates && e.team == lp.team) || !espEnabled) continue;
        ImU32 boxColor = (e.team == lp.team) ? IM_COL32(teammateColor.x * 255, teammateColor.y * 255, teammateColor.z * 255, 255)
            : (e.visible ? IM_COL32(visibleColor.x * 255, visibleColor.y * 255, visibleColor.z * 255, 255)
                : IM_COL32(espColor.x * 255, espColor.y * 255, espColor.z * 255, 255));
        Vector3 footPos = e.origin;
        Vector3 headPos = GetBonePosition(e.boneMatrix, bone_head); // Use actual head bone position
        headPos.z += 5.0f;
        Vector3 screenFoot, screenHead;
        if (WorldToScreen(footPos, screenFoot, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(headPos, screenHead, viewMatrix, screenWidth, screenHeight)) {

            // Calculate the actual height from feet to head
            float height = screenFoot.y - screenHead.y;
            float width = height / 2.0f; // Maintain aspect ratio

            float centerX = (screenHead.x + screenFoot.x) / 2.0f;

            // Position the box so bottom is exactly at feet
            float x = centerX - width / 2.0f;
            float boxTop = screenHead.y; // Calculate top from bottom position
            float boxBottom = screenFoot.y; // Bottom is exactly at feet

            if (height < 10.0f || height > 500.0f) continue; // Skip if height is unrealistic
            if (width < 5.0f || width > 250.0f) continue;   // Skip if width is unrealistic

            const float screenMargin = 90.0f;
            if (x < -screenMargin || x > screenWidth + screenMargin ||
                boxTop < -screenMargin || boxBottom > screenHeight + screenMargin) {
                continue; // Skip rendering if too far off-screen
            }

            ImU32 outlineCol = IM_COL32(0, 0, 0, 155); // Black outline
            // Now use boxTop and boxBottom for drawing
            ImVec2 boxMin(x, boxTop);
            ImVec2 boxMax(x + width, boxBottom);

            if (box3DEnabled) {
                Render3DBox(e, viewMatrix, screenWidth, screenHeight, drawList, boxColor);
            }
            else {
                // Rounded box with optional outline
                if (outlineEnabled) {
                    drawList->AddRect(ImVec2(x - 1, boxTop - 1), ImVec2(x + width + 1, boxBottom + 1),
                        outlineCol, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness + 1.0f);
                }
                drawList->AddRect(boxMin, boxMax, boxColor, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
            }

            // Name with shadow
            DrawTextWithShadow(drawList, ImVec2(x, boxTop - 20), boxColor, e.name);

            // Weapon text with shadow
            char weaponText[64];
            if (ammoEnabled && e.ammo > 0) {
                sprintf_s(weaponText, "%s [%d]", GetWeaponName(e.weaponDefIndex), e.ammo);
            }
            else {
                sprintf_s(weaponText, "%s", GetWeaponName(e.weaponDefIndex));
            }
            DrawTextWithShadow(drawList, ImVec2(x, boxTop - 35), boxColor, weaponText);

            if (healthBarEnabled || healthTextEnabled) {
                // Health animation
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
                    // Health bar background with rounding and outline
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

            if (armorEnabled && e.armor > 0) {
                float barWidth = 6.0f;
                float armorFrac = static_cast<float>(e.armor) / 100.0f;
                ImU32 armorColor = IM_COL32(0, 100, 255, 255);
                ImVec2 armorBgMin(x + width + 3, boxTop);
                ImVec2 armorBgMax(x + width + barWidth + 3, boxBottom);
                ImVec2 armorFillMin(x + width + 3, screenHead.y + height * (1.0f - armorFrac));
                // Armor bar with rounding and outline
                if (outlineEnabled) {
                    drawList->AddRect(ImVec2(armorBgMin.x - 1, armorBgMin.y - 1), ImVec2(armorBgMax.x + 1, armorBgMax.y + 1),
                        outlineCol, 2.0f, ImDrawFlags_RoundCornersAll, 1.0f);
                }
                drawList->AddRectFilled(armorBgMin, armorBgMax, IM_COL32(30, 30, 30, 255), 2.0f, ImDrawFlags_RoundCornersAll);
                drawList->AddRectFilled(armorFillMin, armorBgMax, armorColor, 2.0f, ImDrawFlags_RoundCornersAll);
            }

            if (snaplinesEnabled) {
                ImU32 snapColor = IM_COL32(snaplineColor.x * 255, snaplineColor.y * 255, snaplineColor.z * 255, snaplineColor.w * 255);
                ImVec2 from = ImVec2(screenWidth / 2.0f, screenHeight);

                // Connect to the bottom center of the box (feet position)
                ImVec2 to = ImVec2(x + width / 2.0f, boxBottom);

                // Outline for snaplines (thicker black line first)
                if (outlineEnabled) {
                    drawList->AddLine(from, to, outlineCol, snaplineThickness + 1.0f);
                }
                drawList->AddLine(from, to, snapColor, snaplineThickness);
            }

            if (distanceEnabled) {
                float dist = sqrtf(powf(lp.origin.x - e.origin.x, 2) + powf(lp.origin.y - e.origin.y, 2) + powf(lp.origin.z - e.origin.z, 2)) / 100.0f;
                char distText[32];
                sprintf_s(distText, "%.0fm", dist);
                DrawTextWithShadow(drawList, ImVec2(x, boxBottom + 10), boxColor, distText);
            }

            if (boneESPEnabled && e.boneMatrix) {
                ImU32 skelColor = IM_COL32(skeletonColor.x * 255, skeletonColor.y * 255, skeletonColor.z * 255, skeletonColor.w * 255);
                ImU32 skelOutline = IM_COL32(0, 0, 0, 255);
                for (const auto& conn : boneConnections) {
                    Vector3 boneFrom = GetBonePosition(e.boneMatrix, conn.first);
                    Vector3 boneTo = GetBonePosition(e.boneMatrix, conn.second);
                    Vector3 screenFrom, screenTo;
                    if (WorldToScreen(boneFrom, screenFrom, viewMatrix, screenWidth, screenHeight) &&
                        WorldToScreen(boneTo, screenTo, viewMatrix, screenWidth, screenHeight)) {
                        if (outlineEnabled) {
                            drawList->AddLine(ImVec2(screenFrom.x, screenFrom.y), ImVec2(screenTo.x, screenTo.y), skelOutline, skeletonThickness + 1.0f);
                        }
                        drawList->AddLine(ImVec2(screenFrom.x, screenFrom.y), ImVec2(screenTo.x, screenTo.y), skelColor, skeletonThickness);
                    }
                }
            }

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
            RenderAdvancedFlags(e, ImVec2(x, boxBottom + 40), drawList, boxColor);
            RenderVelocityIndicator(e, ImVec2(x, boxBottom + 25), drawList, boxColor);
        }
    }

    DrawMinimalCrosshair(drawList, screenWidth, screenHeight);
    DrawCleanWatermark(drawList, screenWidth);
    DrawGameInfoHUD(drawList, lp, entities);
    DrawIndicators(drawList, screenWidth, screenHeight);
    DrawKeybinds(drawList, screenWidth, screenHeight);

    // Clean up previousHealth map for non-existent entities (optional, prevents memory growth)
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

    // Clean up displayedHealth similarly
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

    // Removed dropped weapons ESP to eliminate blue dots

    if (radarEnabled) {
        RenderRadar(lp, entities, drawList, screenWidth, screenHeight);
    }

    if (aimbotEnabled && fovCircleEnabled) {
        float fovPixels = (aimbotFOV / 90.0f) * (screenWidth / 2.0f);
        ImU32 fovCol = IM_COL32(fovCircleColor.x * 255, fovCircleColor.y * 255, fovCircleColor.z * 255, fovCircleColor.w * 255);
        ImU32 fovOutline = IM_COL32(0, 0, 0, 255);
        if (outlineEnabled) {
            drawList->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), fovPixels + 1.0f, fovOutline, 0, boxThickness + 1.0f);
        }
        drawList->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), fovPixels, fovCol, 0, boxThickness);
    }

    // Render hitmarkers last so they appear on top
    RenderHitmarkers(viewMatrix, screenWidth, screenHeight, drawList, deltaTime);
}