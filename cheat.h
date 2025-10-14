// cheat.h
#ifndef CHEAT_H
#define CHEAT_H

#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <d3d11.h>
#include <cmath>
#include "dependencies/MinHook.h"
#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_win32.h"
#include "dependencies/imgui/imgui_impl_dx11.h"
#include <string>
#include <ctime>
#include <map>
#include <mmsystem.h> // For MCI
#include <sstream>
#include <iomanip>  // For std::hex
#include <cmath>  // For std::hex

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "minhook.lib")
#pragma comment(lib, "winmm.lib") // For MCI

// Forward declare ImGui WndProc handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// SDL definitions (dynamic load, no need for SDL2.lib or header)
typedef struct SDL_Window SDL_Window;  // Forward declare
typedef int SDL_bool;
#define SDL_FALSE 0
#define SDL_TRUE 1

typedef SDL_Window* (*SDL_GetGrabbedWindowFn)();
typedef SDL_bool(*SDL_GetWindowRelativeMouseModeFn)(SDL_Window* window);
typedef int (*SDL_SetWindowRelativeMouseModeFn)(SDL_Window* window, SDL_bool enabled);

// SDL function pointers
extern SDL_GetGrabbedWindowFn SDL_GetGrabbedWindow;
extern SDL_GetWindowRelativeMouseModeFn SDL_GetWindowRelativeMouseMode;
extern SDL_SetWindowRelativeMouseModeFn SDL_SetWindowRelativeMouseMode;

// For saving/restoring mouse mode
extern SDL_bool savedRelativeMouseMode;

// Fallback ConVar offset (add to offsets.txt if using, e.g., mouse_disableinput=0xADDRESS - find via sigscan or cheat engine)
extern uintptr_t mouseDisableInputOffset;

// Additional SDL
typedef int (*SDL_SetWindowGrabFn)(SDL_Window* window, SDL_bool grabbed);
typedef SDL_bool(*SDL_GetWindowGrabFn)(SDL_Window* window);
typedef int (*SDL_ShowCursorFn)(int toggle);

#define SDL_ENABLE 1
#define SDL_DISABLE 0

// SDL function pointers (add to existing)
extern SDL_SetWindowGrabFn SDL_SetWindowGrab;
extern SDL_GetWindowGrabFn SDL_GetWindowGrab;
extern SDL_ShowCursorFn SDL_ShowCursor;

// For saving/restoring
extern SDL_bool savedGrabMode;

// Structs
struct Vector3 {
    float x, y, z;
    bool operator<(const Vector3& other) const {
        const float epsilon = 1e-6f;  // Adjust as needed
        if (std::fabs(x - other.x) > epsilon) return x < other.x;
        if (std::fabs(y - other.y) > epsilon) return y < other.y;
        return z < other.z;
    }
};
struct Vector2 { float x, y; };
struct Vector4 {
    float x, y, z, w;
    float& operator[](int i) { return (&x)[i]; }
    const float& operator[](int i) const { return (&x)[i]; }
};
struct Matrix4x4 { Vector4 m[4]; };
struct Entity {
    int health;
    Vector3 origin;
    uint8_t team;
    char name[64];
    uintptr_t boneMatrix;
    int weaponDefIndex;
    int ammo;
    int armor;
    bool hasDefuser;
    bool hasHelmet;
    float flashDuration;
    bool scoped;
    bool defusing;
    bool visible;
    int lastHitGroup;
    uintptr_t pawn; // Added for hitsound tracking
};
struct LocalPlayer {
    Vector3 origin;
    uint8_t team;
    int index;
    uintptr_t pawn;
    Vector3 viewAngles;
    uintptr_t activeWeapon; // New: Active weapon pointer
    int ammo; // New: Current ammo in clip
    int health;
    int armor;
    int weaponDefIndex;
};
struct WeaponEntity {
    Vector3 origin;
    int defIndex;
};

struct GlowSettings {
    bool enabled = true;
    ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 0.5f);
    float brightness = 1.0f;
    bool outline = true;
    float outlineWidth = 1.5f;
    bool pulse = false;
    float pulseSpeed = 1.0f;
};

// Hitmarker struct
struct Hitmarker {
    Vector3 worldPos; // World position where hit occurred (e.g., head)
    int damage;
    float startTime;
    float duration = 1.0f; // How long the animation lasts
    ImU32 color = IM_COL32(255, 255, 255, 255); // White text
};

// Notification struct
struct Notification {
    std::string message;
    float startTime;
    float duration;
    ImVec4 color;

    Notification(const std::string& msg, float start, float dur, ImVec4 col)
        : message(msg), startTime(start), duration(dur), color(col) {
    }
};

// Aimbot settings
struct AimbotSettings {
    bool enabled = true;
    float fov = 10.0f;
    float smoothing = 5.0f;
    int aimKey = VK_LBUTTON; // Left Mouse Button
    bool aimOnVisibleOnly = true;
    bool aimTeammates = false;
};

// Globals
extern ImFont* defaultFont;
extern ImFont* navFont;
extern ImFont* iconFont; // New: For icons
extern uintptr_t localPlayerOffset;
extern uintptr_t entityListOffset;
extern uintptr_t viewMatrixOffset;
extern uintptr_t clientBase;
extern uintptr_t sceneNodeOffset;
extern uintptr_t boneMatrixOffset;
extern uintptr_t activeWeaponOffset;
extern uintptr_t itemDefIndexOffset;
extern uintptr_t ownerEntityOffset;
extern uintptr_t playerNameOffset;
extern uintptr_t pawnHandleOffset;
extern uintptr_t healthOffset;
extern uintptr_t teamOffset;
extern uintptr_t originOffset;
extern uintptr_t ammoOffset;
extern uintptr_t armorOffset;
extern uintptr_t localPlayerControllerOffset;
extern uintptr_t hControllerOffset;
extern uintptr_t playerSlotOffset;
extern uintptr_t entitySpottedStateOffset;
extern uintptr_t hasDefuserOffset;
extern uintptr_t hasHelmetOffset;
extern uintptr_t flashDurationOffset;
extern uintptr_t scopedOffset;
extern uintptr_t isDefusingOffset;
extern uintptr_t viewAnglesOffset; // New offset for view angles
extern uintptr_t aimPunchOffset;
extern uintptr_t m_fFlagsOffset;
extern uintptr_t forceJumpOffset;
extern uintptr_t forceAttackOffset;
extern uintptr_t crosshairIDOffset;
extern uintptr_t observerModeOffset;
extern uintptr_t observerTargetOffset;
extern uintptr_t observerServicesOffset; // m_pObserverServices in C_CSPlayerPawn
extern uintptr_t createMoveAddr;
extern uintptr_t fovOffset;
extern bool espEnabled;
extern bool externalAimbotEnabled;
extern bool externalAimbotUseHead;
extern bool recordingExternalAimKey;
extern bool aimbotEnabled;
extern bool menuOpen;
extern bool healthBarEnabled;
extern bool snaplinesEnabled;
extern bool distanceEnabled;
extern bool boneESPEnabled;
extern bool headCircleEnabled;
extern bool showTeammates;
extern bool box3DEnabled;
extern bool glowESPEnabled;
extern bool ammoEnabled;
extern bool radarEnabled;
extern bool healthTextEnabled;
extern bool armorEnabled;
extern bool fovCircleEnabled;
extern bool flagsEnabled;
extern bool tracersEnabled;
extern bool noFlashEnabled;
extern bool revealAllEnabled;
extern bool ragebotOnVisibleOnly;
extern bool ragebotTeammates;
extern bool ragebotEnabled;
extern bool recordingRagebotKey;
extern bool infiniteArmorEnabled;
extern bool hitsoundEnabled; // New: Hitsound toggle
extern bool outlineEnabled; // New: Global outline toggle for ESP features
extern bool noRecoilEnabled;
extern bool bunnyhopEnabled;
extern bool recordingAimKey;
extern bool ragebotSilent;
extern bool triggerbotEnabled;
extern bool showSpectatorList; // NEW: Toggle for separate spectator window
extern bool fovChangerEnabled;
extern bool glowThroughWalls;
extern bool watermarkEnabled;
extern bool crosshairEnabled;
extern bool customCrosshairEnabled;
extern bool showFPS;
extern bool showPosition;
extern bool showVelocity;
extern bool showWeaponInfo;
extern bool showSpectators;
extern bool showGameTime;
extern bool showKeysPressed;
extern bool showLocalPlayerInfo;
extern bool showEntityCount;
extern bool showSystemInfo;
extern bool showNetworkInfo;
extern bool showPlayerInfo;
extern bool showMatchInfo;
extern bool showAngleLines;
extern bool showTargetInfo;
extern bool showMinimap;
extern bool showRadarHud;
extern bool showGrenadeWarning;
extern bool showBombTimer;
extern bool showDefuseTimer;
extern bool showKillFeed;
extern bool showDamageIndicator;
extern bool showRecoilCrosshair;
extern bool showSpreadCircle;
extern bool showAmmoWarning;
extern bool showMoneyDisplay;
extern bool showRoundTimer;
extern bool showBombIcon;
extern bool showGrenadeTrajectory;
extern bool showSmokeWarning;
extern bool showFireWarning;
extern bool showFlashWarning;
extern bool showScopeLines;
extern bool showFovChangerIndicator;
extern bool showThirdPersonIndicator;
extern bool showSpectatorCount;
extern bool showTeamInfo;
extern bool showClantag;
extern bool showRank;
extern bool showWinProbability;
extern bool showEconomyInfo;
extern bool showBuyMenuHelper;
extern bool showGrenadeHelper;
extern bool showJumpStats;
extern bool showStaminaBar;
extern bool showKeybinds;
extern bool showMenuKeybind;
extern float glowMaxDistance;
extern float triggerbotDelay;  // Random delay in ms (0-100)
extern float fovChangerValue;
extern float boxThickness;
extern float skeletonThickness;
extern float snaplineThickness;
extern float crosshairSize;
extern float crosshairThickness;
extern float ragebotMaxDistance;
extern float tracerThickness;
extern float headCircleThickness;
extern float externalAimbotFOV;
extern float ragebotFOV;
extern float externalAimbotSmoothness;
extern float glowOpacity;
extern float aimbotFOV;
extern float aimbotSmoothing; // New
extern bool aimOnVisibleOnly; // New
extern bool aimTeammates; // New
extern int aimKey; // New
extern int externalAimbotKey;
extern int hitgroupBones[];
extern int externalAimbotHitgroup;
extern int ragebotKey;
extern int ragebotHitgroup;
extern float boxRounding; // New: Rounding for box corners
extern ImVec4 espColor;
extern ImVec4 teammateColor;
extern ImVec4 visibleColor;
extern ImVec4 skeletonColor;
extern ImVec4 snaplineColor;
extern ImVec4 tracerColor;
extern ImVec4 headCircleColor;
extern ImVec4 glowColor;
extern ImVec4 crosshairColor;
extern ImVec4 fovCircleColor;
extern ImVec4 chamsEnemyColor;
extern ImVec4 chamsTeammateColor;
extern ImVec4 chamsVisibleColor;
extern ImVec4 weaponColor;
extern ImVec4 soundESPColor;
extern ImVec4 backtrackESPColor;
extern ImVec4 velocityIndicatorColor;
extern ImVec4 advancedFlagsColor;
extern HWND g_hwnd;
extern std::string debugLog;
extern bool showDebugConsole;
extern GlowSettings enemyGlowSettings;
extern GlowSettings teammateGlowSettings;
// Add these to cheat.h (globals)
extern HMODULE g_hModule;
extern HANDLE g_hKeyThread;
extern bool g_running;
extern void* g_presentTarget; // To store the hooked Present target for cleanup
extern bool g_unloadRequested;
extern DWORD WINAPI CleanupThread(LPVOID param);
extern LONG volatile g_inPresent;
extern const char* hitgroupNames[];


// Notifications global
extern std::vector<Notification> notifications;

// For health animation and hitmarkers
extern std::map<uintptr_t, float> displayedHealth; // For smooth health bar animation
extern std::vector<Hitmarker> hitmarkers; // List of active hitmarkers
extern float healthAnimationSpeed; // HP per second decrease rate
extern float hitmarkerSpeed; // Pixels per second upward movement
extern float hitmarkerFadeDuration; // Seconds to fade out

// For lingering ESP on dead entities
extern std::map<uintptr_t, float> deathTimes; // pawn -> death timestamp

// Bone indices
extern const int bone_pelvis;
extern const int bone_spine_0;
extern const int bone_spine_1;
extern const int bone_spine_2;
extern const int bone_spine_3;
extern const int bone_neck;
extern const int bone_head;
extern const int bone_clavicle_L;
extern const int bone_upperarm_L;
extern const int bone_forearm_L;
extern const int bone_hand_L;
extern const int bone_clavicle_R;
extern const int bone_upperarm_R;
extern const int bone_forearm_R;
extern const int bone_hand_R;
extern const int bone_thigh_L;
extern const int bone_calf_L;
extern const int bone_foot_L;
extern const int bone_thigh_R;
extern const int bone_calf_R;
extern const int bone_foot_R;





// Add these with other feature toggles in cheat.h
extern bool chamsEnabled;
extern bool chamsThroughWalls;
extern bool chamsVisibleOnly;
extern ImVec4 chamsEnemyColor;
extern ImVec4 chamsTeammateColor;
extern ImVec4 chamsVisibleColor;

extern bool weaponESPEnabled;
extern float weaponMaxDistance;
extern ImVec4 weaponColor;

extern bool soundESPEnabled;
extern float soundMaxTime;
extern std::map<Vector3, float> soundLocations;

extern bool showAdvancedFlags;
extern bool velocityESPEnabled;

extern bool backtrackESPEnabled;
extern float backtrackDuration;
extern std::map<uintptr_t, std::vector<std::pair<Vector3, float>>> playerHistory;

extern bool autoStrafeEnabled;
extern float strafeSpeed;

// Add these function declarations
void ApplyChams(uintptr_t entity, bool isTeammate, bool isVisible);
void RenderWeaponESP(const std::vector<WeaponEntity>& weapons, Matrix4x4 viewMatrix,
    float screenWidth, float screenHeight, ImDrawList* drawList);
void ProcessSounds();
void RenderSoundESP(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList);
void RenderAdvancedFlags(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color);
void RenderVelocityIndicator(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color);
void UpdateBacktrackData(const std::vector<Entity>& entities);
void RenderBacktrackESP(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList);
void RunAutoStrafe(const LocalPlayer& lp);





extern std::vector<std::pair<int, int>> boneConnections;

extern std::string hitSoundPath;
extern std::string headSoundPath;
extern std::string killSoundPath;
extern float soundVolume;
extern uintptr_t lastHitGroupOffset;

// DX11 globals
extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
typedef HRESULT(__stdcall* PresentFn)(IDXGISwapChain*, UINT, UINT);
extern PresentFn oPresent;
typedef LRESULT(CALLBACK* WndProcFn)(HWND, UINT, WPARAM, LPARAM);
extern WndProcFn oWndProc;

// Function declarations
bool LoadOffsets();
void PlayMP3(const std::string& filePath);
uintptr_t GetControllerByIndex(int index); // NEW: For spectator list
uintptr_t PatternScan(const char* module, const char* pattern);
std::vector<std::string> GetSpectators(); // NEW: For spectator list
float ToDegrees(float radians);
float ToRadians(float degrees);
Vector3 CalculateAngle(Vector3 src, Vector3 dst);
Vector3 SmoothAngle(Vector3 current, Vector3 target, float smoothing);
void AddNotification(const std::string& msg, float duration, ImVec4 color);
uintptr_t GetPawnByIndex(int index);
void RenderNotifications();
// Update CreateMove function signature
typedef __int64(__fastcall* CreateMoveFn)(void* thisptr, int a2, int a3, float a4, bool a5);
extern CreateMoveFn oCreateMove;
__int64 __fastcall HK_CreateMove(void* thisptr, int a2, int a3, float a4, bool a5);
template<typename T>
inline T ReadMemory(uintptr_t address) {
    T value = {};
    if (!address || address < 0x10000) { // Basic sanity check
        return value;
    }
    ReadProcessMemory(GetCurrentProcess(), (LPCVOID)address, &value, sizeof(T), nullptr);
    return value;
}
template<typename T>
inline void WriteMemory(uintptr_t address, T value) {
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)address, &value, sizeof(T), nullptr);
}
Vector3 GetBonePosition(uintptr_t boneMatrix, int boneIndex);
Vector3 ClampAngle(Vector3 angle);
LocalPlayer GetLocalPlayer();
std::vector<Entity> GetEntities();
std::vector<WeaponEntity> GetDroppedWeapons();
const char* GetWeaponName(int defIndex);
void Render3DBox(Entity& e, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList, ImU32 color);
void RenderRadar(const LocalPlayer& lp, const std::vector<Entity>& entities, ImDrawList* drawList, float screenWidth, float screenHeight);
void RunAimbot();
void ApplyFeatures(const LocalPlayer& lp, const std::vector<Entity>& entities);
bool WorldToScreen(Vector3 pos, Vector3& screen, Matrix4x4 viewMatrix, float screenWidth, float screenHeight);
void DrawTextWithShadow(ImDrawList* drawList, ImVec2 pos, ImU32 color, const char* text);
void RenderHitmarkers(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList, float deltaTime);
void RunTriggerbot(const LocalPlayer& lp, const std::vector<Entity>& entities);
void RenderESP();
void SaveConfig();
void LoadConfig();
void SetupImGuiStyle();
void RenderCleanMenu();
void RunExternalAimbot();
void RenderExternalAimbotFOV(ImDrawList* drawList, float screenWidth, float screenHeight);
DWORD WINAPI KeyThread(LPVOID param);
LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
bool SetupHook();
bool IsGameInFocus();

#endif // CHEAT_H