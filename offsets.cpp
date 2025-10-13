#include "cheat.h"

uintptr_t localPlayerOffset = 0;
uintptr_t entityListOffset = 0;
uintptr_t viewMatrixOffset = 0;
uintptr_t clientBase = 0;
uintptr_t sceneNodeOffset = 0;
uintptr_t boneMatrixOffset = 0;
uintptr_t activeWeaponOffset = 0;
uintptr_t itemDefIndexOffset = 0;
uintptr_t ownerEntityOffset = 0;
uintptr_t playerNameOffset = 0;
uintptr_t pawnHandleOffset = 0;
uintptr_t healthOffset = 0;
uintptr_t teamOffset = 0;
uintptr_t originOffset = 0;
uintptr_t ammoOffset = 0;
uintptr_t armorOffset = 0;
uintptr_t localPlayerControllerOffset = 0;
uintptr_t hControllerOffset = 0;
uintptr_t playerSlotOffset = 0;
uintptr_t entitySpottedStateOffset = 0;
uintptr_t hasDefuserOffset = 0;
uintptr_t hasHelmetOffset = 0;
uintptr_t flashDurationOffset = 0;
uintptr_t scopedOffset = 0;
uintptr_t isDefusingOffset = 0;
uintptr_t viewAnglesOffset = 0; // New offset for view angles
uintptr_t lastHitGroupOffset = 0;
uintptr_t mouseDisableInputOffset = 0;  // Load from offsets.txt if needed
uintptr_t aimPunchOffset = 0;
uintptr_t m_fFlagsOffset = 0;
uintptr_t forceJumpOffset = 0;
uintptr_t forceAttackOffset = 0;
uintptr_t crosshairIDOffset = 0;
uintptr_t observerModeOffset = 0; // NEW: For spectator list
uintptr_t observerTargetOffset = 0; // NEW: For spectator list
uintptr_t observerServicesOffset = 0; // NEW: m_pObserverServices
SDL_GetGrabbedWindowFn SDL_GetGrabbedWindow = nullptr;
SDL_GetWindowRelativeMouseModeFn SDL_GetWindowRelativeMouseMode = nullptr;
SDL_SetWindowRelativeMouseModeFn SDL_SetWindowRelativeMouseMode = nullptr;
SDL_SetWindowGrabFn SDL_SetWindowGrab = nullptr;
SDL_GetWindowGrabFn SDL_GetWindowGrab = nullptr;
SDL_ShowCursorFn SDL_ShowCursor = nullptr;
SDL_bool savedRelativeMouseMode = SDL_TRUE;
SDL_bool savedGrabMode = SDL_TRUE;

bool LoadOffsets() {
    std::ifstream file("C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\Offsets.txt");
    if (!file.is_open()) {
        debugLog += "[ERROR] Failed to open offsets.txt\n";
        std::cout << "[ERROR] Failed to open offsets.txt" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("=");
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());
        value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());

        if (value.substr(0, 2) == "0x") {
            value = value.substr(2);
        }

        try {
            uintptr_t offset = std::stoul(value, nullptr, 16);
            if (key == "localPlayer") localPlayerOffset = offset;
            else if (key == "entityList") entityListOffset = offset;
            else if (key == "viewMatrix") viewMatrixOffset = offset;
            else if (key == "m_pGameSceneNode") sceneNodeOffset = offset;
            else if (key == "m_modelState") boneMatrixOffset = offset;
            else if (key == "m_pClippingWeapon") activeWeaponOffset = offset;
            else if (key == "m_iItemDefinitionIndex") itemDefIndexOffset = offset;
            else if (key == "m_hOwnerEntity") ownerEntityOffset = offset;
            else if (key == "m_sSanitizedPlayerName") playerNameOffset = offset;
            else if (key == "m_hPlayerPawn") pawnHandleOffset = offset;
            else if (key == "m_iHealth") healthOffset = offset;
            else if (key == "m_iTeamNum") teamOffset = offset;
            else if (key == "m_vecAbsOrigin") originOffset = offset;
            else if (key == "m_iClip1") ammoOffset = offset;
            else if (key == "m_ArmorValue") armorOffset = offset;
            else if (key == "localPlayerController") localPlayerControllerOffset = offset;
            else if (key == "m_hController") hControllerOffset = offset;
            else if (key == "m_nPlayerSlot") playerSlotOffset = offset;
            else if (key == "m_entitySpottedState") entitySpottedStateOffset = offset;
            else if (key == "m_bHasDefuser") hasDefuserOffset = offset;
            else if (key == "m_bHasHelmet") hasHelmetOffset = offset;
            else if (key == "m_flFlashDuration") flashDurationOffset = offset;
            else if (key == "m_bScoped") scopedOffset = offset;
            else if (key == "m_bIsDefusing") isDefusingOffset = offset;
            else if (key == "m_viewAngles") viewAnglesOffset = offset; // New
            else if (key == "m_lastHitGroup") lastHitGroupOffset = offset;
            else if (key == "m_aimPunchAngle") aimPunchOffset = offset;
            else if (key == "m_fFlags") m_fFlagsOffset = offset;
            else if (key == "forceJump") forceJumpOffset = offset;
            else if (key == "forceAttack") forceAttackOffset = offset;
            else if (key == "crosshairID") crosshairIDOffset = offset;
            else if (key == "m_iObserverMode") observerModeOffset = offset;
            else if (key == "m_hObserverTarget") observerTargetOffset = offset;
            else if (key == "m_pObserverServices") observerServicesOffset = offset;
        }
        catch (const std::exception& e) {
            debugLog += "[ERROR] Failed to parse offset for " + key + ": " + e.what() + "\n";
            std::cout << "[ERROR] Failed to parse offset for " << key << ": " << e.what() << std::endl;
        }
        HMODULE sdlModule = GetModuleHandleA("sdl2.dll");
        if (sdlModule) {
            SDL_GetGrabbedWindow = (SDL_GetGrabbedWindowFn)GetProcAddress(sdlModule, "SDL_GetGrabbedWindow");
            SDL_GetWindowRelativeMouseMode = (SDL_GetWindowRelativeMouseModeFn)GetProcAddress(sdlModule, "SDL_GetWindowRelativeMouseMode");
            SDL_SetWindowRelativeMouseMode = (SDL_SetWindowRelativeMouseModeFn)GetProcAddress(sdlModule, "SDL_SetWindowRelativeMouseMode");

            if (SDL_GetGrabbedWindow && SDL_GetWindowRelativeMouseMode && SDL_SetWindowRelativeMouseMode) {
                debugLog += "[DEBUG] SDL functions loaded successfully\n";
            }
            else {
                debugLog += "[ERROR] Failed to load SDL functions - falling back to ConVar if available\n";
            }
            SDL_SetWindowGrab = (SDL_SetWindowGrabFn)GetProcAddress(sdlModule, "SDL_SetWindowGrab");
            SDL_GetWindowGrab = (SDL_GetWindowGrabFn)GetProcAddress(sdlModule, "SDL_GetWindowGrab");
            SDL_ShowCursor = (SDL_ShowCursorFn)GetProcAddress(sdlModule, "SDL_ShowCursor");
            if (SDL_SetWindowGrab && SDL_GetWindowGrab && SDL_ShowCursor) {
                debugLog += "[DEBUG] Additional SDL grab and cursor functions loaded successfully\n";
            }
            else {
                debugLog += "[ERROR] Failed to load SDL grab/cursor functions\n";
            }
        }
        else {
            debugLog += "[ERROR] sdl2.dll not found - cannot disable relative mouse mode\n";
        }
    }
    file.close();

    clientBase = (uintptr_t)GetModuleHandleA("client.dll");
    debugLog += "[DEBUG] Loaded offsets\n";
    std::cout << "[DEBUG] Loaded offsets: Local=0x" << std::hex << localPlayerOffset
        << ", Entity=0x" << entityListOffset << ", Matrix=0x" << viewMatrixOffset
        << ", SceneNode=0x" << sceneNodeOffset << ", BoneMatrix=0x" << boneMatrixOffset
        << ", ActiveWeapon=0x" << activeWeaponOffset << ", ItemDefIndex=0x" << itemDefIndexOffset
        << ", OwnerEntity=0x" << ownerEntityOffset << ", PlayerName=0x" << playerNameOffset
        << ", PawnHandle=0x" << pawnHandleOffset << ", Health=0x" << healthOffset
        << ", Team=0x" << teamOffset << ", Origin=0x" << originOffset
        << ", Ammo=0x" << ammoOffset << ", Armor=0x" << armorOffset
        << ", ViewAngles=0x" << viewAnglesOffset << std::endl;

    return localPlayerOffset && entityListOffset && viewMatrixOffset && sceneNodeOffset &&
        boneMatrixOffset && activeWeaponOffset && itemDefIndexOffset && ownerEntityOffset &&
        playerNameOffset && pawnHandleOffset && healthOffset && teamOffset && originOffset &&
        ammoOffset && armorOffset && localPlayerControllerOffset && hControllerOffset &&
        playerSlotOffset && entitySpottedStateOffset && hasDefuserOffset && hasHelmetOffset &&
        flashDurationOffset && scopedOffset && isDefusingOffset && viewAnglesOffset &&
        aimPunchOffset && m_fFlagsOffset && forceJumpOffset && forceAttackOffset && crosshairIDOffset;
}