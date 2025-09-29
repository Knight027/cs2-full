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
bool bunnyhopEnabled = false;
bool triggerbotEnabled = false;
bool showSpectatorList = false; // NEW: Default off
float triggerbotDelay = 20.0f;
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
HWND g_hwnd = NULL;
std::string debugLog = "";
bool showDebugConsole = false;

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

std::string hitSoundPath = "C:\\Users\\dionr\\Downloads\\r6\\cs2WoW\\launch\\hitsound.mp3";
std::string headSoundPath = "C:\\Users\\dionr\\Downloads\\r6\\cs2WoW\\launch\\headshot.mp3";
std::string killSoundPath = "C:\\Users\\dionr\\Downloads\\r6\\cs2WoW\\launch\\killsound.mp3";
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
    Vector3 smoothed;
    smoothed.x = current.x + (target.x - current.x) / smoothing;
    smoothed.y = current.y + (target.y - current.y) / smoothing;
    smoothed.z = 0.0f;
    return smoothed;
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
    lp.pawn = ReadMemory<uintptr_t>(clientBase + localPlayerOffset);

    if (!lp.pawn) {
        debugLog += "[ERROR] GetLocalPlayer: Invalid local player pawn\n";
        return lp;
    }

    // Read scene node for origin and view angles
    uintptr_t sceneNode = ReadMemory<uintptr_t>(lp.pawn + sceneNodeOffset);
    if (!sceneNode) {
        std::stringstream ss;
        ss << "[ERROR] GetLocalPlayer: Invalid scene node for pawn 0x" << std::hex << lp.pawn << "\n";
        debugLog += ss.str();
        return lp;
    }
    uint64_t client_base = (uint64_t)GetModuleHandleA("client.dll");
    uint64_t local = *(uint64_t*)(client_base + 0x1BDFD10);
    lp.origin = ReadMemory<Vector3>(sceneNode + originOffset);
    lp.viewAngles = ReadMemory<Vector3>(lp.pawn + viewAnglesOffset); // Assuming this offset exists
    lp.team = ReadMemory<int>(local + teamOffset);
    lp.health = ReadMemory<int>(lp.pawn + healthOffset); // Added: Read local health

    // New: Read active weapon and ammo
    lp.activeWeapon = ReadMemory<uintptr_t>(lp.pawn + activeWeaponOffset);
    lp.ammo = lp.activeWeapon ? ReadMemory<int>(lp.activeWeapon + ammoOffset) : 0;

    // Validate team ID (assuming CS2 uses 2 for T, 3 for CT, 0/1 for invalid/spectator)
    if (lp.team != 2 && lp.team != 3) {
        debugLog += "[WARNING] GetLocalPlayer: Invalid team ID " +
            std::to_string(lp.team) + " for pawn 0x" + std::to_string(lp.pawn) + "\n";
        lp.team = 0; // Set to invalid to prevent incorrect team-based rendering
    }

    // Validate view angles (basic sanity check: pitch -90 to 90, yaw -180 to 180)
    if (lp.viewAngles.x < -90.0f || lp.viewAngles.x > 90.0f ||
        lp.viewAngles.y < -180.0f || lp.viewAngles.y > 180.0f) {
        debugLog += "[WARNING] GetLocalPlayer: Invalid view angles (" +
            std::to_string(lp.viewAngles.x) + "," + std::to_string(lp.viewAngles.y) +
            ") for pawn 0x" + std::to_string(lp.pawn) + "\n";
        lp.viewAngles = { 0.0f, 0.0f, 0.0f }; // Reset to prevent radar rotation issues
    }

    // Read local controller for player index
    uintptr_t localController = ReadMemory<uintptr_t>(clientBase + localPlayerControllerOffset);
    if (!localController) {
        debugLog += "[ERROR] GetLocalPlayer: Invalid local controller\n";
        return lp;
    }

    lp.index = ReadMemory<int>(localController + playerSlotOffset);

    // Validate player index (assuming CS2 player slots are 0-63 for valid players)
    if (lp.index < 0 || lp.index >= 64) {
        debugLog += "[WARNING] GetLocalPlayer: Invalid player index " +
            std::to_string(lp.index) + " for controller 0x" +
            std::to_string(localController) + "\n";
        lp.index = -1; // Set to invalid to prevent incorrect visibility checks
    }

    // Log successful retrieval
    debugLog += "[DEBUG] GetLocalPlayer: origin=(" +
        std::to_string(lp.origin.x) + "," +
        std::to_string(lp.origin.y) + "," +
        std::to_string(lp.origin.z) +
        "), team=" + std::to_string(lp.team) +
        ", index=" + std::to_string(lp.index) +
        ", viewAngles=(" + std::to_string(lp.viewAngles.x) + "," +
        std::to_string(lp.viewAngles.y) + ")\n";

    return lp;
}

std::vector<Entity> GetEntities() {
    std::vector<Entity> entities;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    LocalPlayer lp = GetLocalPlayer();
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
        if (entity) {
            Entity e;
            e.health = ReadMemory<int>(entity + healthOffset);
            uintptr_t sceneNode = ReadMemory<uintptr_t>(entity + sceneNodeOffset);
            e.origin = ReadMemory<Vector3>(sceneNode + originOffset);
            e.team = ReadMemory<int>(entity + teamOffset);
            e.boneMatrix = ReadMemory<uintptr_t>(sceneNode + boneMatrixOffset);
            // Fixed name reading: m_sSanitizedPlayerName is a pointer to the string
            uintptr_t namePtr = ReadMemory<uintptr_t>(controller + playerNameOffset);
            if (namePtr) {
                ReadProcessMemory(GetCurrentProcess(), (LPCVOID)namePtr, e.name, 64, nullptr);
                e.name[63] = '\0'; // Ensure null-termination
            }
            else {
                strcpy_s(e.name, "Unknown"); // Fallback if pointer is invalid
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
            uintptr_t spottedAddr = entity + entitySpottedStateOffset + 0x8 + (lp.index * sizeof(bool));
            e.visible = ReadMemory<bool>(spottedAddr);
            e.pawn = entity; // Added for hitsound
            e.lastHitGroup = ReadMemory<int>(entity + lastHitGroupOffset);
            e.visible = ReadMemory<bool>(entity + entitySpottedStateOffset); // Placeholder
            if (e.health >= 0 && e.health <= 100) {  // Include dead entities (health == 0)
                entities.push_back(e);
                debugLog += "[DEBUG] Entity " + std::to_string(i) + ": health=" + std::to_string(e.health) +
                    ", armor=" + std::to_string(e.armor) + ", ammo=" + std::to_string(e.ammo) +
                    ", origin=(" + std::to_string(e.origin.x) + "," + std::to_string(e.origin.y) + "," + std::to_string(e.origin.z) +
                    "), team=" + std::to_string(e.team) + ", name=" + std::string(e.name) + "\n";
            }
        }
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

void RenderGlow(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList) {
    Vector3 footPos = e.origin;
    Vector3 headPos = e.origin;
    headPos.z += 72.0f;
    Vector3 screenFoot, screenHead;
    if (WorldToScreen(footPos, screenFoot, viewMatrix, screenWidth, screenHeight) &&
        WorldToScreen(headPos, screenHead, viewMatrix, screenWidth, screenHeight)) {
        float height = screenFoot.y - screenHead.y;
        float width = height / 2.0f;
        float x = screenHead.x - width / 2.0f;
        ImU32 glowCol = IM_COL32(glowColor.x * 255, glowColor.y * 255, glowColor.z * 255, glowOpacity * 255);
        ImU32 outlineCol = IM_COL32(0, 0, 0, 255); // Black outline for glow

        // Draw outline slightly larger
        if (outlineEnabled) {
            drawList->AddRect(ImVec2(x - 4, screenHead.y - 4), ImVec2(x + width + 4, screenFoot.y + 4),
                outlineCol, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness + 1.0f);
        }

        // Draw glow rect with rounding for cleaner look
        drawList->AddRect(ImVec2(x - 3, screenHead.y - 3), ImVec2(x + width + 3, screenFoot.y + 3),
            glowCol, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness + 1.5f);
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

void RunAimbot() {
    if (!aimbotEnabled || !GetAsyncKeyState(aimKey)) return;

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn) return;

    std::vector<Entity> entities = GetEntities();
    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;

    Entity* target = nullptr;
    float closestAngle = aimbotFOV;
    Vector3 targetAngle;

    float currentTime = ImGui::GetTime();
    for (auto& e : entities) {
        bool isDead = e.health <= 0;
        bool shouldTarget = !isDead || (deathTimes.count(e.pawn) && currentTime - deathTimes[e.pawn] < 1.0f);
        if (!shouldTarget || (!aimTeammates && e.team == lp.team)) continue;
        if (aimOnVisibleOnly && !e.visible) continue;
        if (e.health <= 0) continue;

        Vector3 headPos = GetBonePosition(e.boneMatrix, bone_head);
        Vector3 screenPos;
        if (!WorldToScreen(headPos, screenPos, viewMatrix, screenWidth, screenHeight)) continue;

        float dx = screenPos.x - (screenWidth / 2.0f);
        float dy = screenPos.y - (screenHeight / 2.0f);
        float dist = sqrtf(dx * dx + dy * dy);
        float fovPixels = (aimbotFOV / 90.0f) * (screenWidth / 2.0f);
        if (dist > fovPixels) continue;

        Vector3 angle = CalculateAngle(lp.origin, headPos);
        float angleDist = sqrtf(powf(angle.x - lp.viewAngles.x, 2) + powf(angle.y - lp.viewAngles.y, 2));
        if (angleDist < closestAngle) {
            closestAngle = angleDist;
            target = &e;
            targetAngle = angle;
        }
    }

    if (target) {
        Vector3 smoothedAngle = SmoothAngle(lp.viewAngles, targetAngle, aimbotSmoothing);
        WriteMemory<Vector3>(lp.pawn + viewAnglesOffset, smoothedAngle);
        debugLog += "[DEBUG] Aimbot targeting " + std::string(target->name) +
            ", angles=(" + std::to_string(smoothedAngle.x) + "," + std::to_string(smoothedAngle.y) + ")\n";
    }
}

void ApplyFeatures(const LocalPlayer& lp, const std::vector<Entity>& entities) {
    static std::map<uintptr_t, int> previousHealth; // Assuming this already exists for health tracking
    static int prevLocalAmmo = -1; // New: Track local player's previous ammo
    static float lastFireTime = 0.0f; // New: Timestamp of last local fire

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
        for (const auto& e : entities) {
            uint8_t currentTeamNum = *(uint8_t*)(e.pawn + 0x3EB);
            uint8_t teamNum = *(uint8_t*)(lp.pawn + 0x3EB);
            if (currentTeamNum == teamNum) continue;  // Skip teammates and self (matches dllmain logic)

            if (!IsValidPtr((void*)e.pawn)) continue;

            uint64_t glowBase = e.pawn + 0xCC0;
            if (!IsValidPtr((void*)glowBase)) continue;

            uint64_t glowColorOverride = glowBase + 0x40;
            uint64_t glowEnable = glowBase + 0x51;

            if (!IsValidPtr((void*)glowColorOverride) || !IsValidPtr((void*)glowEnable)) continue;

            *(uint64_t*)(glowColorOverride) = 0x800000FF;  // Fixed red from dllmain
            *(bool*)(glowEnable) = true;
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
    LocalPlayer lp = GetLocalPlayer();
    Matrix4x4 viewMatrix = ReadMemory<Matrix4x4>(clientBase + viewMatrixOffset);
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = io.DisplaySize.x;
    float screenHeight = io.DisplaySize.y;
    float deltaTime = io.DeltaTime;

    auto drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(200, 50),
        IM_COL32(0, 0, 0, 200), IM_COL32(0, 0, 0, 200), IM_COL32(20, 20, 20, 200), IM_COL32(20, 20, 20, 200));
    drawList->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "CS2 WoW Cheat v1.0");
    char fpsText[32];
    sprintf_s(fpsText, "FPS: %.1f | Entities: %d", ImGui::GetIO().Framerate, GetEntities().size());
    drawList->AddText(ImVec2(10, 30), IM_COL32(200, 200, 200, 255), fpsText);

    auto entities = GetEntities();
    ApplyFeatures(lp, entities);
    RunAimbot(); // Run aimbot here
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
        Vector3 headPos = e.origin;
        headPos.z += 72.0f;
        Vector3 screenFoot, screenHead;
        if (WorldToScreen(footPos, screenFoot, viewMatrix, screenWidth, screenHeight) &&
            WorldToScreen(headPos, screenHead, viewMatrix, screenWidth, screenHeight)) {
            float height = screenFoot.y - screenHead.y;
            float width = height / 2.0f;
            float x = screenHead.x - width / 2.0f;

            if (glowESPEnabled) {
                RenderGlow(e, viewMatrix, screenWidth, screenHeight, drawList);
            }

            ImU32 outlineCol = IM_COL32(0, 0, 0, 255); // Black outline
            ImVec2 boxMin(x, screenHead.y);
            ImVec2 boxMax(x + width, screenFoot.y);

            if (box3DEnabled) {
                Render3DBox(e, viewMatrix, screenWidth, screenHeight, drawList, boxColor);
            }
            else {
                // Rounded box with optional outline
                if (outlineEnabled) {
                    drawList->AddRect(ImVec2(x - 1, screenHead.y - 1), ImVec2(x + width + 1, screenFoot.y + 1),
                        outlineCol, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness + 1.0f);
                }
                drawList->AddRect(boxMin, boxMax, boxColor, boxRounding, ImDrawFlags_RoundCornersAll, boxThickness);
            }

            // Name with shadow
            DrawTextWithShadow(drawList, ImVec2(x, screenHead.y - 20), boxColor, e.name);

            // Weapon text with shadow
            char weaponText[64];
            if (ammoEnabled && e.ammo > 0) {
                sprintf_s(weaponText, "%s [%d]", GetWeaponName(e.weaponDefIndex), e.ammo);
            }
            else {
                sprintf_s(weaponText, "%s", GetWeaponName(e.weaponDefIndex));
            }
            DrawTextWithShadow(drawList, ImVec2(x, screenHead.y - 35), boxColor, weaponText);

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
                ImVec2 healthBgMin(x - barWidth - 3, screenHead.y);
                ImVec2 healthBgMax(x - 3, screenFoot.y);
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
                ImVec2 armorBgMin(x + width + 3, screenHead.y);
                ImVec2 armorBgMax(x + width + barWidth + 3, screenFoot.y);
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
                // Outline for snaplines (thicker black line first)
                if (outlineEnabled) {
                    drawList->AddLine(from, ImVec2(screenFoot.x, screenFoot.y), outlineCol, snaplineThickness + 1.0f);
                }
                drawList->AddLine(from, ImVec2(screenFoot.x, screenFoot.y), snapColor, snaplineThickness);
            }

            if (distanceEnabled) {
                float dist = sqrtf(powf(lp.origin.x - e.origin.x, 2) + powf(lp.origin.y - e.origin.y, 2) + powf(lp.origin.z - e.origin.z, 2)) / 100.0f;
                char distText[32];
                sprintf_s(distText, "%.0fm", dist);
                DrawTextWithShadow(drawList, ImVec2(x, screenFoot.y + 10), boxColor, distText);
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
                    DrawTextWithShadow(drawList, ImVec2(x, screenFoot.y + 25), boxColor, flagsStr.c_str());
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
        }
    }

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