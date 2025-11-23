#define NOMINMAX
#include "cheat.h"
#include <unordered_set>
#include <array>
#include "menu.h"

const char* GetWeaponName(int defIndex) {
    // Comprehensive weapon list
    switch (defIndex) {
        // Pistols
    case 1: return "Desert Eagle";
    case 2: return "Dual Berettas";
    case 3: return "Five-SeveN";
    case 4: return "Glock-18";
    case 32: return "P2000";
    case 36: return "P250";
    case 61: return "USP-S";
    case 63: return "CZ75-Auto";
    case 64: return "R8 Revolver";

        // SMGs
    case 17: return "MAC-10";
    case 19: return "P90";
    case 23: return "MP5-SD";
    case 24: return "UMP-45";
    case 26: return "PP-Bizon";
    case 33: return "MP7";
    case 34: return "MP9";

        // Rifles
    case 7: return "AK-47";
    case 8: return "AUG";
    case 9: return "AWP";
    case 10: return "FAMAS";
    case 11: return "G3SG1";
    case 13: return "Galil AR";
    case 14: return "M249";
    case 16: return "M4A4";
    case 28: return "Negev";
    case 35: return "Nova";
    case 38: return "SCAR-20";
    case 39: return "SG 553";
    case 40: return "SSG 08";
    case 60: return "M4A1-S";

        // Shotguns
    case 25: return "XM1014";
    case 27: return "MAG-7";
    case 29: return "Sawed-Off";

        // Equipment
    case 31: return "Zeus x27";
    case 42: return "Knife";
    case 43: return "Flashbang";
    case 44: return "HE Grenade";
    case 45: return "Smoke";
    case 46: return "Molotov";
    case 47: return "Decoy";
    case 48: return "Incendiary";
    case 49: return "C4";
    case 59: return "Knife";
    case 72: return "Tablet";

        // Default case - log unknown weapons
    default:
        // Don't log common invalid values to avoid spam
        if (defIndex > 0 && defIndex < 1000) {
            debugLog += "[WEAPON] Unknown weapon defIndex: " + std::to_string(defIndex) + "\n";
        }
        static char unknown[32];
        sprintf_s(unknown, "Weapon %d", defIndex);
        return unknown;
    }
}

void PlayMP3(const std::string& filePath) {
    static int soundCounter = 0;
    std::string alias = "mp3sound" + std::to_string(soundCounter++);
    std::string openCmd = "open "" + filePath + "" type mpegvideo alias " + alias;
    mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
    std::string volumeCmd = "setaudio " + alias + " volume to " + std::to_string(static_cast<int>(soundVolume));
    mciSendStringA(volumeCmd.c_str(), NULL, 0, NULL);
    std::string playCmd = "play " + alias;
    mciSendStringA(playCmd.c_str(), NULL, 0, NULL);
}



Vector3 CalculateAngle(Vector3 src, Vector3 dst) {
    Vector3 angle;
    Vector3 delta = {
        dst.x - src.x,
        dst.y - src.y,
        dst.z - src.z
    };

    float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);

    // Calculate pitch (up/down)
    angle.x = ToDegrees(atan2f(-delta.z, hyp));

    // Calculate yaw (left/right)
    angle.y = ToDegrees(atan2f(delta.y, delta.x));

    angle.z = 0.0f;

    // Clamp pitch to avoid invalid angles
    if (angle.x > 89.0f) angle.x = 89.0f;
    if (angle.x < -89.0f) angle.x = -89.0f;

    // Normalize yaw
    if (angle.y > 180.0f) angle.y -= 360.0f;
    if (angle.y < -180.0f) angle.y += 360.0f;

    return angle;
}






std::string chat::buildText(const std::string& str, const ChatColor& col) {
    static constexpr const char hexChars[] = "0123456789ABCDEF";
    unsigned char r = std::min((unsigned char)255, (unsigned char)std::round(col.r * 255.f));
    unsigned char g = std::min((unsigned char)255, (unsigned char)std::round(col.g * 255.f));
    unsigned char b = std::min((unsigned char)255, (unsigned char)std::round(col.b * 255.f));

    char color_buffer[7];
    color_buffer[0] = hexChars[(r >> 4) & 0x0F];
    color_buffer[1] = hexChars[r & 0x0F];
    color_buffer[2] = hexChars[(g >> 4) & 0x0F];
    color_buffer[3] = hexChars[g & 0x0F];
    color_buffer[4] = hexChars[(b >> 4) & 0x0F];
    color_buffer[5] = hexChars[b & 0x0F];
    color_buffer[6] = '\0';

    static constexpr char color_text_start[] = R"(<font color="#)";
    static constexpr char color_text_end[] = R"(">)";
    static constexpr char text_end[] = R"(</font>)";

    std::string ret;
    ret.reserve(str.length() + sizeof(color_text_start) + sizeof(color_buffer) + sizeof(color_text_end) + sizeof(text_end) - 4);
    ret.append(color_text_start);
    ret.append(color_buffer);
    ret.append(color_text_end);
    ret.append(str);
    ret.append(text_end);

    return ret;
}

void chat::sendMessage(const std::string& raw_string) {
    if (!oFindHudElement || !oSendMessageClient) return;

    uint64_t v41 = (uint64_t)oFindHudElement("CCSGO_HudVoiceStatus");
    if (v41 && v41 != 32) {
        uint64_t v163 = 1;
        oSendMessageClient((void*)(v41 - 32), raw_string.c_str(), 0xFFFFFFFF, &v163);
    }
}


void* __fastcall HK_CreateSubtickMoveStep(void* thisptr, void* cmd, int unk) {
    if (deSubtickEnabled) {
        // Skip subtick processing to de-subtick movement
        return nullptr;
    }
    return oCreateSubtickMoveStep(thisptr, cmd, unk);
}


Vector3 SmoothAngle(Vector3 current, Vector3 target, float smoothing) {
    Vector3 delta = {
    target.x - current.x,
    target.y - current.y,
    0.0f
    };


    while (delta.y > 180.0f) delta.y -= 360.0f;
    while (delta.y < -180.0f) delta.y += 360.0f;

    Vector3 smoothed = {
        current.x + delta.x / smoothing,
        current.y + delta.y / smoothing,
        0.0f
    };

    return smoothed;
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

uintptr_t GetPawnByIndex(int index) {
    if (index < 1 || index > 64) return 0;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return 0;


    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((index & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return 0;

    uintptr_t controller = ReadMemory<uintptr_t>(listEntry + 0x70 * (index & 0x1FF));
    if (!controller) return 0;

    uint32_t pawnHandle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
    if (!pawnHandle) return 0;

    uintptr_t pawnListEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);
    if (!pawnListEntry) return 0;

    uintptr_t pawn = ReadMemory<uintptr_t>(pawnListEntry + 0x70 * (pawnHandle & 0x1FF));
    return pawn;
}

uintptr_t GetControllerByIndex(int index) {
    if (index < 1 || index > 64) return 0;
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return 0;


    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((index & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return 0;

    uintptr_t controller = ReadMemory<uintptr_t>(listEntry + 0x70 * (index & 0x1FF));
    return controller;
}

uintptr_t GetPawnFromHandle(uint32_t handle) {
    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return 0;

    // The handle contains the index. We need to mask it (0x7FFF) and shift it.
    uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((handle & 0x7FFF) >> 9) + 0x10);
    if (!listEntry) return 0;

    // Now we get the actual entity pointer
    return ReadMemory<uintptr_t>(listEntry + 120 * (handle & 0x1FF));
}

uintptr_t GetObserverTarget(uintptr_t pawn) {
    if (!pawn) return 0;

    // 1. Read Observer Services (m_pObserverServices)
    uintptr_t observerServices = ReadMemory<uintptr_t>(pawn + observerServicesOffset);
    if (!observerServices) return 0;

    // 2. Read Target Handle (m_hObserverTarget)
    uint32_t targetHandle = ReadMemory<uint32_t>(observerServices + observerTargetOffset);
    if (!targetHandle || targetHandle == 0xFFFFFFFF) return 0;

    // 3. Resolve Handle to Pointer
    return GetPawnFromHandle(targetHandle);
}

// 2. The working Logic
std::vector<SpectatorInfo> GetSpectatorsList() {
    std::vector<SpectatorInfo> specs;

    // 1. Basic Safety Checks
    if (!clientBase) return specs;

    LocalPlayer lp = GetLocalPlayer();
    if (!lp.pawn) return specs;

    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return specs;

    // 2. Determine who we are looking for
    // TargetPawn is the entity that spectators must be watching to show up on the list.
    uintptr_t targetPawnToMatch = lp.pawn;
    std::string status = "Alive";

    // If we are dead, we want to see who is watching the guy WE are spectating
    if (lp.health <= 0) {
        uintptr_t spectatedPawn = GetObserverTarget(lp.pawn);
        if (spectatedPawn) {
            targetPawnToMatch = spectatedPawn;
            status = "Dead (Spectating someone)";
        }
        else {
            status = "Dead (No target)";
        }
    }

    // DEBUG: Uncomment this if list is empty to see what the cheat thinks you are doing
    // debugLog += "[SPEC DEBUG] Me: " + std::string(lp.name) + " | Status: " + status + "\n";

    // 3. Iterate Player List (Max 64 for players)
    for (int i = 0; i < 64; i++) {
        // Get List Entry
        uintptr_t listEntry = ReadMemory<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!listEntry) continue;

        // Get Controller
        uintptr_t controller = ReadMemory<uintptr_t>(listEntry + 120 * (i & 0x1FF));
        if (!controller) continue;

        // Skip ourselves (checking controller address)
        uintptr_t localController = ReadMemory<uintptr_t>(clientBase + localPlayerControllerOffset);
        if (controller == localController) continue;

        // Get the Pawn of this player
        uint32_t pawnHandle = ReadMemory<uint32_t>(controller + 0x6B4);
        uintptr_t spectatorPawn = GetPawnFromHandle(pawnHandle);

        // If they don't have a pawn, they might be a dead spectator in roaming mode, 
        // but usually they still have a pawn handle in CS2. If 0, skip.
        if (!spectatorPawn) continue;

        // 4. Check who THIS player is watching
        uintptr_t theirSubject = GetObserverTarget(spectatorPawn);

        // 5. The Match Check
        if (theirSubject != 0 && theirSubject == targetPawnToMatch) {
            SpectatorInfo info;

            // Read Name
            uintptr_t nameAddr = ReadMemory<uintptr_t>(controller + playerNameOffset);
            if (nameAddr) {
                char nameBuf[128];
                ReadProcessMemory(GetCurrentProcess(), (LPCVOID)nameAddr, nameBuf, sizeof(nameBuf), nullptr);
                info.name = std::string(nameBuf);
            }
            else {
                info.name = "Unknown";
            }

            // Read Mode (Optional)
            uintptr_t obsServices = ReadMemory<uintptr_t>(spectatorPawn + observerServicesOffset);
            int mode = ReadMemory<int>(obsServices + observerModeOffset);

            // Map modes to text
            switch (mode) {
            case 4: info.mode = "First Person"; break;
            case 5: info.mode = "Third Person"; break;
            case 6: info.mode = "Free Cam"; break;
            case 1: info.mode = "DeathCam"; break;
            case 2: info.mode = "FreezeCam"; break;
            case 3: info.mode = "Fixed"; break;
            default: info.mode = "Obs"; break;
            }

            specs.push_back(info);
        }
    }
    return specs;
}

// 3. The Rendering Function (ImGui)
void RenderSpectatorListWindow() {
    if (!showSpectatorList) return;

    // Set constraints so the window doesn't disappear
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(300, 600));

    // Use AlwaysAutoResize to fit the content
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::Begin("Spectators", &showSpectatorList, flags)) {

        std::vector<SpectatorInfo> specs = GetSpectatorsList();

        if (specs.empty()) {
            ImGui::TextDisabled("No spectators.");
            // Debug Tip: 
            // ImGui::TextDisabled("Debug: List is 0"); 
        }
        else {
            for (const auto& s : specs) {
                // Draw Name
                ImGui::TextColored(themeAccent, "%s", s.name.c_str());

                // Draw Mode aligned to right
                ImGui::SameLine();
                float width = ImGui::GetContentRegionAvail().x;
                float textW = ImGui::CalcTextSize(s.mode.c_str()).x;
                if (width > textW) ImGui::SameLine(ImGui::GetWindowWidth() - textW - 20);

                ImGui::TextDisabled("%s", s.mode.c_str());
            }
        }
    }
    ImGui::End();
}

LocalPlayer GetLocalPlayer() {
    LocalPlayer lp = {};


    if (!clientBase) {
        debugLog += "[ERROR] GetLocalPlayer: Invalid client base\n";
        return lp;
    }

    lp.pawn = ReadMemory<uintptr_t>(clientBase + localPlayerOffset);

    if (!lp.pawn) {
        debugLog += "[INFO] GetLocalPlayer: No local player pawn (probably in menu)\n";
        return lp;
    }

    uintptr_t sceneNode = ReadMemory<uintptr_t>(lp.pawn + sceneNodeOffset);
    if (!sceneNode) {
        std::stringstream ss;
        ss << "[INFO] GetLocalPlayer: Invalid scene node for pawn 0x" << std::hex << lp.pawn << " (probably in menu)\n";
        debugLog += ss.str();
        return lp;
    }

    Vector3 testOrigin = ReadMemory<Vector3>(sceneNode + originOffset);
    if (testOrigin.x == 0.0f && testOrigin.y == 0.0f && testOrigin.z == 0.0f) {
        debugLog += "[INFO] GetLocalPlayer: Zero origin (probably in menu)\n";
        return lp;
    }

    lp.origin = testOrigin;
    lp.viewAngles = ReadMemory<Vector3>(lp.pawn + viewAnglesOffset);
    lp.team = ReadMemory<int>(lp.pawn + teamOffset);
    lp.health = ReadMemory<int>(lp.pawn + healthOffset);
    lp.shotsFired = ReadMemory<int>(lp.pawn + shotsFiredOffset);
    lp.activeWeapon = ReadMemory<uintptr_t>(lp.pawn + activeWeaponOffset);
    lp.ammo = lp.activeWeapon ? ReadMemory<int>(lp.activeWeapon + ammoOffset) : 0;
    lp.viewAngles = ReadMemory<Vector3>(lp.pawn + viewAnglesOffset);  // Eye angles

    uintptr_t localController = ReadMemory<uintptr_t>(clientBase + localPlayerControllerOffset);
    if (localController) {
        lp.index = ReadMemory<int>(localController + playerSlotOffset);
        uintptr_t namePtr = ReadMemory<uintptr_t>(localController + playerNameOffset);
        if (namePtr && IsValidPtr((void*)namePtr)) {
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)namePtr, lp.name, sizeof(lp.name) - 1, nullptr);
            lp.name[sizeof(lp.name) - 1] = '\0';
        }
        else {
            strcpy_s(lp.name, sizeof(lp.name), "Unknown");
        }
    }
    else {
        strcpy_s(lp.name, sizeof(lp.name), "Unknown");
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
        uintptr_t list_entry = ReadMemory<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
        if (!list_entry) continue;

        uintptr_t controller = ReadMemory<uintptr_t>(list_entry + 0x70 * (i & 0x1FF));
        if (!controller) continue;

        uint32_t pawn_handle = ReadMemory<uint32_t>(controller + pawnHandleOffset);
        if (!pawn_handle || pawn_handle == 0xFFFFFFFF) continue;

        uintptr_t p_index = pawn_handle & 0x7FFF;
        uintptr_t list_entry2 = ReadMemory<uintptr_t>(entityList + 0x8 * ((p_index & 0x7FFF) >> 9) + 0x10);
        if (!list_entry2) continue;

        uintptr_t entity = ReadMemory<uintptr_t>(list_entry2 + 0x70 * (p_index & 0x1FF));
        if (!entity) continue;

        int health = ReadMemory<int>(entity + healthOffset);
        if (health < 0 || health > 100) continue;

        Entity e;
        e.health = health;

        uintptr_t sceneNode = ReadMemory<uintptr_t>(entity + sceneNodeOffset);
        if (!sceneNode) continue;

        e.origin = ReadMemory<Vector3>(sceneNode + originOffset);
        e.team = ReadMemory<int>(entity + teamOffset);
        e.boneMatrix = ReadMemory<uintptr_t>(sceneNode + boneMatrixOffset);

        uintptr_t namePtr = ReadMemory<uintptr_t>(controller + playerNameOffset);
        if (namePtr) {
            ReadProcessMemory(GetCurrentProcess(), (LPCVOID)namePtr, e.name, 64, nullptr);
            e.name[63] = '\0';
        }
        else {
            strcpy_s(e.name, "Unknown");
        }

        // FIXED: Proper weapon reading for enemies
        uintptr_t weaponServices = ReadMemory<uintptr_t>(entity + m_pWeaponServices); // m_pWeaponServices
        if (weaponServices) {
            uintptr_t activeWeapon = ReadMemory<uintptr_t>(weaponServices + activeWeaponOffset); // m_hActiveWeapon
            if (activeWeapon) {
                // Convert handle to entity pointer
                uint32_t weaponHandle = ReadMemory<uint32_t>(activeWeapon);
                if (weaponHandle && weaponHandle != 0xFFFFFFFF) {
                    uintptr_t weaponIndex = weaponHandle & 0x7FFF;
                    uintptr_t weaponListEntry = ReadMemory<uintptr_t>(entityList + 0x8 * ((weaponIndex & 0x7FFF) >> 9) + 0x10);
                    if (weaponListEntry) {
                        uintptr_t weaponEntity = ReadMemory<uintptr_t>(weaponListEntry + 0x70 * (weaponIndex & 0x1FF));
                        if (weaponEntity) {
                            e.weaponDefIndex = ReadMemory<int>(weaponEntity + itemDefIndexOffset);
                            e.ammo = ReadMemory<int>(weaponEntity + ammoOffset);

                            // DEBUG: Log weapon reading
                            debugLog += "[WEAPON] Enemy " + std::string(e.name) + " weapon defIndex: " +
                                std::to_string(e.weaponDefIndex) + "\n";
                        }
                    }
                }
            }
        }

        // Fallback if weapon services method fails
        if (e.weaponDefIndex == 0) {
            uintptr_t activeWeapon = ReadMemory<uintptr_t>(entity + activeWeaponOffset);
            if (activeWeapon) {
                e.weaponDefIndex = ReadMemory<int>(activeWeapon + itemDefIndexOffset);
                e.ammo = ReadMemory<int>(activeWeapon + ammoOffset);

                // DEBUG: Log alternative method
                debugLog += "[WEAPON-ALT] Enemy " + std::string(e.name) + " weapon defIndex: " +
                    std::to_string(e.weaponDefIndex) + " (using alt method)\n";
            }
        }

        e.armor = ReadMemory<int>(entity + armorOffset);
        e.hasDefuser = ReadMemory<bool>(entity + hasDefuserOffset);
        e.hasHelmet = ReadMemory<bool>(entity + hasHelmetOffset);
        e.flashDuration = ReadMemory<float>(entity + flashDurationOffset);
        e.scoped = ReadMemory<bool>(entity + scopedOffset);
        e.defusing = ReadMemory<bool>(entity + isDefusingOffset);

        if (entitySpottedByMaskOffset && lp.index >= 0 && lp.index < 64) {
            e.spottedByMask = ReadMemory<uint64_t>(entity + entitySpottedStateOffset + entitySpottedByMaskOffset);
            e.visible = (e.spottedByMask & (1ULL << lp.index)) != 0;
        }
        else {
            if (lp.index >= 0 && lp.index < 64) {
                uintptr_t spottedAddr = entity + entitySpottedStateOffset + 0x8 + (lp.index * sizeof(bool));
                e.visible = ReadMemory<bool>(spottedAddr);
            }
            else {
                e.visible = false;
            }
        }

        e.pawn = entity;
        e.lastHitGroup = ReadMemory<int>(entity + lastHitGroupOffset);

        entities.push_back(e);
    }
    return entities;
}

std::vector<WeaponEntity> GetDroppedWeapons() {
    std::vector<WeaponEntity> weapons;

    if (!clientBase) return weapons;

    uintptr_t entityList = ReadMemory<uintptr_t>(clientBase + entityListOffset);
    if (!entityList) return weapons;

    for (int i = 64; i < 1024; i++) {
        uintptr_t list_entry = ReadMemory<uintptr_t>(entityList + 0x8 * ((i & 0x7FFF) >> 9) + 0x10);
        if (!list_entry) continue;

        uintptr_t entity = ReadMemory<uintptr_t>(list_entry + 0x70 * (i & 0x1FF));
        if (!entity) continue;

        // Check if it's a weapon entity by checking if it has weapon properties
        int defIndex = ReadMemory<int>(entity + itemDefIndexOffset);
        if (defIndex <= 0) continue;

        // Check if it's a dropped weapon (no owner or world entity)
        uint32_t ownerHandle = ReadMemory<uint32_t>(entity + ownerEntityOffset);
        if (ownerHandle != 0xFFFFFFFF) continue; // Skip if owned by player

        // Get the position through scene node
        uintptr_t sceneNode = ReadMemory<uintptr_t>(entity + sceneNodeOffset);
        if (!sceneNode) continue;

        Vector3 origin = ReadMemory<Vector3>(sceneNode + originOffset);

        // Validate position (not zero)
        if (origin.x == 0 && origin.y == 0 && origin.z == 0) continue;

        WeaponEntity w;
        w.origin = origin;
        w.defIndex = defIndex;
        weapons.push_back(w);
    }

    return weapons;
}

bool IsGameInFocus() {
    HWND foregroundWindow = GetForegroundWindow();
    return (foregroundWindow == g_hwnd);
}