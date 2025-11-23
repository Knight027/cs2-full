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
uintptr_t createMoveAddr = 0;
uintptr_t m_pCameraServicesOffset = 0;
uintptr_t m_iFOVOffset = 0;
uintptr_t m_iFOVStartOffset = 0;
uintptr_t m_flFOVTimeOffset = 0;
uintptr_t m_flFOVRateOffset = 0;
uintptr_t entitySpottedByMaskOffset = 0;
uintptr_t inputOffset = 0;
uintptr_t m_pWeaponServices = 0;
uintptr_t externalAimbotLockedTarget = 0;
uintptr_t overrideViewAddr = 0;
uintptr_t renderParticleAddr = 0;
uintptr_t lightSceneObjectAddr = 0;
uintptr_t getInaccuracyAddr = 0;
uintptr_t getSpreadAddr = 0;
uintptr_t updateAccuracyPenaltyAddr = 0;  // Optional
uintptr_t createSubtickMoveStepAddr = 0;
uintptr_t dwCSGOInputOffset = 0;
uintptr_t renderBatchListAddr = 0;
uintptr_t updateSkyboxAddr = 0;
uintptr_t findMaterialAddr = 0;
uintptr_t shotsFiredOffset = 0;
uintptr_t updateAnimAddr = 0;  // NEW
uintptr_t m_hActivePostProcessingVolumeOffset = 0;  // NEW
uintptr_t m_bExposureControlOffset = 0;  // NEW
uintptr_t m_flMinExposureOffset = 0;  // NEW
uintptr_t m_flMaxExposureOffset = 0;  // NEW
uintptr_t skyTintColorOffset = 0;  // NEW
uintptr_t skyTintLightingOffset = 0;  // NEW
uintptr_t dwMapnameOffset = 0;
uintptr_t renderLegsAddr = 0;
uintptr_t m_pBulletServicesOffset = 0;
uintptr_t m_hMyWeaponsOffset = 0; // Usually 0x40 or 0x48 inside WeaponServices
uintptr_t m_iItemIDHighOffset = 0; // m_iItemIDHigh
uintptr_t m_nFallbackPaintKitOffset = 0; // m_nFallbackPaintKit
uintptr_t m_flFallbackWearOffset = 0; // m_flFallbackWear
uintptr_t m_nFallbackSeedOffset = 0; // m_nFallbackSeed
uintptr_t m_nFallbackStatTrakOffset = 0; // m_nFallbackStatTrak
int offsetToIsGlowing = 0;
int offsetToGlowSceneObjectEntity = 0;
int offsetToGlowSceneObjectAttachedSceneObject = 0;

// Forward declare SDL_Window since no header is included
typedef struct SDL_Window SDL_Window;

// Updated typedefs for SDL3 functions
// Updated typedefs for SDL3 functions
typedef SDL_Window* (*SDL_GetGrabbedWindowFn)(void);
typedef int (*SDL_GetWindowRelativeMouseModeFn)(SDL_Window* window);
typedef int (*SDL_SetWindowRelativeMouseModeFn)(SDL_Window* window, int enabled);
typedef int (*SDL_SetWindowMouseGrabFn)(SDL_Window* window, int grabbed);
typedef int (*SDL_GetWindowMouseGrabFn)(SDL_Window* window);
typedef int (*SDL_ShowCursorFn)(int toggle);  // Fixed signature


// Updated variables
SDL_SetWindowMouseGrabFn SDL_SetWindowMouseGrab = nullptr;
SDL_GetWindowMouseGrabFn SDL_GetWindowMouseGrab = nullptr;
uintptr_t ResolveRelative(uintptr_t addr, int relOffset, int instrSize) {
    if (!addr) return 0;
    int32_t rel = ReadMemory<int32_t>(addr + relOffset);
    return addr + relOffset + sizeof(int32_t) + rel;

}


uintptr_t PatternScan(const char* module, const char* pattern) {
    static auto patternToBytes = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?') ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
        };

    HMODULE hModule = GetModuleHandleA(module);
    if (!hModule) return 0;

    auto dosHeader = (PIMAGE_DOS_HEADER)hModule;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)dosHeader + dosHeader->e_lfanew);
    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = patternToBytes(pattern);
    auto scanBytes = reinterpret_cast<uint8_t*>(hModule);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return (uintptr_t)&scanBytes[i];
        }
    }

    debugLog += "[ERROR] Pattern not found: " + std::string(pattern) + "\n";
    return 0;
}

bool LoadOffsets() {
    std::ifstream file("C:\\Offsets.txt");
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
            else if (key == "createMove") {}
            else if (key == "m_pCameraServices") m_pCameraServicesOffset = offset;
            else if (key == "m_iFOV") m_iFOVOffset = offset;
            else if (key == "m_iFOVStart") m_iFOVStartOffset = offset;
            else if (key == "m_flFOVTime") m_flFOVTimeOffset = offset;
            else if (key == "m_flFOVRate") m_flFOVRateOffset = offset;
            else if (key == "m_bSpottedByMask") entitySpottedByMaskOffset = offset;
            else if (key == "inputOffset") inputOffset = offset;
            else if (key == "m_pWeaponServices") m_pWeaponServices = offset;
            else if (key == "dwCSGOInput") dwCSGOInputOffset = offset;
            else if (key == "m_iShotsFired") shotsFiredOffset = offset;
            else if (key == "m_hActivePostProcessingVolume") m_hActivePostProcessingVolumeOffset = offset;
            else if (key == "m_bExposureControl") m_bExposureControlOffset = offset;
            else if (key == "m_flMinExposure") m_flMinExposureOffset = offset;
            else if (key == "m_flMaxExposure") m_flMaxExposureOffset = offset;
            else if (key == "skyTintColor") skyTintColorOffset = offset;
            else if (key == "skyTintLighting") skyTintLightingOffset = offset;
            else if (key == "dwMapname") dwMapnameOffset = offset;
            else if (key == "m_pBulletServices") m_pBulletServicesOffset = offset;
            else if (key == "m_hMyWeapons") m_hMyWeaponsOffset = offset;
            else if (key == "m_nFallbackPaintKit") m_nFallbackPaintKitOffset = offset;
            else if (key == "m_flFallbackWear") m_flFallbackWearOffset = offset;
            else if (key == "m_iItemIDHigh") m_iItemIDHighOffset = offset;
            else if (key == "m_nFallbackStatTrak") m_nFallbackStatTrakOffset = offset;
            else if (key == "m_nFallbackSeed") m_nFallbackSeedOffset = offset;
        }
        catch (const std::exception& e) {
            debugLog += "[ERROR] Failed to parse offset for " + key + ": " + e.what() + "\n";
            std::cout << "[ERROR] Failed to parse offset for " << key << ": " << e.what() << std::endl;
        }
    }
    file.close();

    std::ifstream file2("C:\\Offsets.txt");
    std::string createMovePattern;
    while (std::getline(file2, line)) {
        size_t pos = line.find("=");
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());

        if (key == "createMove") {
            createMovePattern = value;
            break;
        }
    }
    file2.close();

    if (!createMovePattern.empty()) {
        createMoveAddr = PatternScan("client.dll", createMovePattern.c_str());
        if (createMoveAddr) {
            debugLog += "[DEBUG] CreateMove pattern found at: 0x" + std::to_string(createMoveAddr) + "\n";
            std::cout << "[DEBUG] CreateMove pattern found at: 0x" << std::hex << createMoveAddr << std::endl;
        }
        else {
            debugLog += "[ERROR] Failed to find CreateMove pattern\n";
            std::cout << "[ERROR] Failed to find CreateMove pattern" << std::endl;
        }
    }

    std::string overrideViewPattern = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 20 57 41 56 41 57 48 83 EC 40";
    overrideViewAddr = PatternScan("client.dll", overrideViewPattern.c_str());
    if (overrideViewAddr) {
        debugLog += "[DEBUG] OverrideView pattern found at: 0x" + std::to_string(overrideViewAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find OverrideView pattern\n";
    }


    auto addr = PatternScan("client.dll", "C6 43 ? 00 48 85 C9 74 ? E8");
    if (addr) {
        offsetToIsGlowing = ReadMemory<int8_t>(addr + 2);
        debugLog += "[DEBUG] offsetToIsGlowing = 0x" + std::to_string(offsetToIsGlowing) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find OffsetToIsGlowing pattern\n";
    }

    addr = PatternScan("client.dll", "48 89 B0 ? ? ? ? 48 8B ? ? 48 85");
    if (addr) {
        offsetToGlowSceneObjectEntity = ReadMemory<int32_t>(addr + 3);
        debugLog += "[DEBUG] offsetToGlowSceneObjectEntity = 0x" + std::to_string(offsetToGlowSceneObjectEntity) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find OffsetToGlowSceneObjectEntity pattern\n";
    }

    addr = PatternScan("client.dll", "48 89 ? 48 89 ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? 48");
    if (addr) {
        offsetToGlowSceneObjectAttachedSceneObject = ReadMemory<int32_t>(addr + 6);
        debugLog += "[DEBUG] offsetToGlowSceneObjectAttachedSceneObject = 0x" + std::to_string(offsetToGlowSceneObjectAttachedSceneObject) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find OffsetToGlowSceneObjectAttachedSceneObject pattern\n";
    }
    std::string getFOVPattern = "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 74 ? 48 8B C8 48 83 C4";
    getFOVAddr = PatternScan("client.dll", getFOVPattern.c_str());
    if (getFOVAddr) {
        debugLog += "[DEBUG] GetFOV pattern found at: 0x" + std::to_string(getFOVAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find GetFOV pattern\n";
    }
    std::string renderParticlePattern = "48 89 5C 24 ? 4C 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24";
    renderParticleAddr = PatternScan("particles.dll", renderParticlePattern.c_str());
    if (renderParticleAddr) {
        debugLog += "[DEBUG] RenderParticle pattern found at: 0x" + std::to_string(renderParticleAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find RenderParticle pattern\n";
    }
    std::string lightSceneSig = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 54 24";
    lightSceneObjectAddr = PatternScan("scenesystem.dll", lightSceneSig.c_str());
    if (lightSceneObjectAddr) {
        debugLog += "[DEBUG] LightSceneObject pattern found at: 0x" + std::to_string(lightSceneObjectAddr) + "\n";
        std::cout << "[DEBUG] LightSceneObject pattern found at: 0x" << std::hex << lightSceneObjectAddr << std::endl;
    }
    else {
        debugLog += "[ERROR] Failed to find LightSceneObject pattern\n";
        std::cout << "[ERROR] Failed to find LightSceneObject pattern" << std::endl;
    }
    std::string getInaccuracyPattern = "48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 44 0F 29 84 24";
    getInaccuracyAddr = PatternScan("client.dll", getInaccuracyPattern.c_str());
    if (getInaccuracyAddr) {
        debugLog += "[DEBUG] GetInaccuracy pattern found at: 0x" + std::to_string(getInaccuracyAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find GetInaccuracy pattern\n";
    }

    std::string getSpreadPattern = "48 83 EC ? 48 63 91";
    getSpreadAddr = PatternScan("client.dll", getSpreadPattern.c_str());
    if (getSpreadAddr) {
        debugLog += "[DEBUG] GetSpread pattern found at: 0x" + std::to_string(getSpreadAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find GetSpread pattern\n";
    }

    std::string updateAccuracyPenaltyPattern = "40 53 57 48 83 EC ? 48 8B F9 E8";
    updateAccuracyPenaltyAddr = PatternScan("client.dll", updateAccuracyPenaltyPattern.c_str());
    if (updateAccuracyPenaltyAddr) {
        debugLog += "[DEBUG] UpdateAccuracyPenalty pattern found at: 0x" + std::to_string(updateAccuracyPenaltyAddr) + "\n";
    }
    else {
        debugLog += "[WARNING] Failed to find UpdateAccuracyPenalty pattern (optional for no-spread)\n";
    }

    std::string findHudElementPattern = "4C 8B DC 53 48 83 EC ? 48 8B 05 ";
    findHudElementAddr = PatternScan("client.dll", findHudElementPattern.c_str());
    if (findHudElementAddr) {
        oFindHudElement = (FindHudElementFn)findHudElementAddr;
        debugLog += "[DEBUG] FindHudElement found at 0x" + std::to_string(findHudElementAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] FindHudElement pattern not found\n";
    }

    std::string sendMessageClientPattern = "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 45 33 FF 41 8B D8";
    sendMessageClientAddr = PatternScan("client.dll", sendMessageClientPattern.c_str());
    if (sendMessageClientAddr) {
        oSendMessageClient = (SendMessageClientFn)sendMessageClientAddr;
        debugLog += "[DEBUG] SendMessageClient found at 0x" + std::to_string(sendMessageClientAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] SendMessageClient pattern not found\n";
    }

    std::string createSubtickMoveStepPattern = "E8 ? ? ? ? 48 8B D0 49 8D 4E 18 E8 ? ? ? ? 4C 8B C8 4C 8D 43 02 49 8B D1 48 8B CE E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 48 3B 06 0F 83 ? ? ? ? 0F B7 00 66 C7 45 67 ? ? 66 3B 45 67 74 ? E9 ? ? ? ? 40 80 FF ? 0F 85 ? ? ? ? 41 83 4E 10 ?";
    createSubtickMoveStepAddr = PatternScan("client.dll", createSubtickMoveStepPattern.c_str());
    if (createSubtickMoveStepAddr) {
        debugLog += "[DEBUG] CreateSubtickMoveStep found at 0x" + std::to_string(createSubtickMoveStepAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find CreateSubtickMoveStep pattern\n";
    }

    std::string renderBatchListPattern = "4C 8B DC 53 48 81 EC ? ? ? ? 83 79";
    renderBatchListAddr = PatternScan("scenesystem.dll", renderBatchListPattern.c_str());

    if (renderBatchListAddr) {
        debugLog += "[DEBUG] RenderBatchList pattern found at: 0x" + std::to_string(renderBatchListAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find RenderBatchList pattern\n";
    }

    std::string updateSkyboxPattern = "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 48 83 B9";
    updateSkyboxAddr = PatternScan("client.dll", updateSkyboxPattern.c_str());
    if (updateSkyboxAddr) {
        debugLog += "[DEBUG] UpdateSkybox pattern found at: 0x" + std::to_string(updateSkyboxAddr) + "\n";
        std::cout << "[DEBUG] UpdateSkybox pattern found at: 0x" << std::hex << updateSkyboxAddr << std::endl;
    }
    else {
        debugLog += "[ERROR] Failed to find UpdateSkybox pattern\n";
        std::cout << "[ERROR] Failed to find UpdateSkybox pattern" << std::endl;
    }

    std::string csgoInputPat = "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B DF";
    uintptr_t csgoInputSig = PatternScan("client.dll", csgoInputPat.c_str());
    if (csgoInputSig) {
        dwCSGOInputOffset = ResolveRelative(csgoInputSig, 3, 7);
        debugLog += "[DEBUG] dwCSGOInputOffset resolved to 0x" + std::to_string(dwCSGOInputOffset) + "\n";

    }
    else {
        debugLog += "[ERROR] Failed to find CCSGOInput pattern\n";

    }

    std::string renderLegsPattern = "40 55 53 56 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? F2 0F 10";
    renderLegsAddr = PatternScan("client.dll", renderLegsPattern.c_str());
    if (renderLegsAddr) {
        debugLog += "[DEBUG] RenderLegs found at 0x" + std::to_string(renderLegsAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] RenderLegs pattern not found\n";
    }

    std::string setReadyPat = "40 53 48 83 EC ? 48 8B DA 48 8D 15 ? ? ? ? 48 8B CB FF 15";
    setLocalPlayerReadyAddr = PatternScan("client.dll", setReadyPat.c_str());
    if (setLocalPlayerReadyAddr) {
        debugLog += "[DEBUG] SetLocalPlayerReady found at 0x" + std::to_string(setLocalPlayerReadyAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find SetLocalPlayerReady pattern\n";
    }

    std::string popupPat = "40 56 57 41 57 48 83 EC ? 48 8B 3D ? ? ? ? 4D 85 C0";
    popupAcceptMatchFoundAddr = PatternScan("client.dll", popupPat.c_str());
    if (popupAcceptMatchFoundAddr) {
        debugLog += "[DEBUG] PopupAcceptMatchFound found at 0x" + std::to_string(popupAcceptMatchFoundAddr) + "\n";
    }
    else {
        debugLog += "[ERROR] Failed to find PopupAcceptMatchFound pattern\n";
    }

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
        aimPunchOffset && m_fFlagsOffset && forceJumpOffset && forceAttackOffset && crosshairIDOffset &&
        m_pCameraServicesOffset && m_iFOVOffset && m_iFOVStartOffset &&
        m_flFOVTimeOffset && m_flFOVRateOffset;
}