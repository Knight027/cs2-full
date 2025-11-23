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
#include "dependencies/imgui/imgui_stdlib.h"
#include <string>
#include <ctime>
#include <map>
#include <mmsystem.h> // For MCI
#include <sstream>
#include <iomanip>  // For std::hex

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
typedef SDL_bool(*SDL_SetWindowGrabFn)(SDL_Window* window, SDL_bool grabbed);
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
        const float epsilon = 1e-6f;
        if (std::fabs(x - other.x) > epsilon) return x < other.x;
        if (std::fabs(y - other.y) > epsilon) return y < other.y;
        return z < other.z;
    }
    // New: Operators for wireframe math
    Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
    Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vector3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
    Vector3 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar }; }
    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 Normalize() const { float len = Length(); return len > 0.001f ? *this / len : *this; }
    Vector3 Cross(const Vector3& other) const {
        return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
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
    uintptr_t pawn;
    uint64_t spottedByMask; // NEW: For proper visibility check
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
    char name[64];
    int shotsFired;
    bool scoped;
};
struct WeaponEntity {
    Vector3 origin;
    int defIndex;
};
enum ESPBoxStyle {
    ESP_BOX_STANDARD = 0,
    ESP_BOX_CORNERED,
    ESP_BOX_FILLED,
    ESP_BOX_GRADIENT,
    ESP_BOX_OUTLINE_ONLY
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
enum SkeletonStyle {
    SKELETON_STANDARD = 0,
    SKELETON_DOTTED,
    SKELETON_GRADIENT
};
// Hitmarker struct
struct Hitmarker {
    Vector3 worldPos; // World position where hit occurred (e.g., head)
    int damage;
    float startTime;
    float duration = 1.0f; // How long the animation lasts
    ImU32 color = IM_COL32(255, 255, 255, 255); // White text
};
enum HealthBarStyle {
    HEALTH_BAR_VERTICAL = 0,
    HEALTH_BAR_HORIZONTAL,
    HEALTH_BAR_TEXT_ONLY,
    HEALTH_BAR_GRADIENT
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

enum ExternalAimbotTarget {
    TARGET_CLOSEST = 0,
    TARGET_CLOSEST_TO_CROSSHAIR,
    TARGET_LOWEST_HEALTH,
    TARGET_HIGHEST_THREAT
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
extern const char* targetSelectionNames[];
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
extern uintptr_t observerModeOffset;
extern uintptr_t observerTargetOffset;
extern uintptr_t observerServicesOffset; // m_pObserverServices in C_CSPlayerPawn
extern uintptr_t createMoveAddr;
extern uintptr_t fovOffset;
extern uintptr_t m_pCameraServicesOffset;
extern uintptr_t m_iFOVOffset;
extern uintptr_t m_iFOVStartOffset;  // New: To set starting FOV and prevent resets
extern uintptr_t m_flFOVTimeOffset;
extern uintptr_t m_flFOVRateOffset;
extern uintptr_t entitySpottedByMaskOffset;
extern uintptr_t inputOffset;
extern uintptr_t m_pWeaponServices;
extern uintptr_t externalAimbotLockedTarget;
extern uintptr_t renderParticleAddr;
extern uintptr_t m_hMyWeaponsOffset; // Inside WeaponServices
extern uintptr_t m_AttributeManagerOffset;
extern uintptr_t m_ItemOffset;
extern uintptr_t m_iItemDefinitionIndexOffset;
extern uintptr_t m_iItemIDHighOffset;
extern uintptr_t m_nFallbackPaintKitOffset;
extern uintptr_t m_flFallbackWearOffset;
extern uintptr_t m_nFallbackStatTrakOffset;
extern uintptr_t m_nFallbackSeedOffset;
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
extern bool edgeJumpEnabled;
extern bool recordingAimKey;
extern bool ragebotSilent;
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
extern bool keyRecorded;
extern bool wireframeBoneESPEnabled;
extern bool nameESPEnabled;
extern bool weaponTextESPEnabled;
extern bool box2DEnabled;
extern bool skeletonEnabled;
extern bool armorBarEnabled;
extern bool distanceTextEnabled;
extern bool weaponESPEnabled;
extern bool soundESPEnabled;
extern bool backtrackESPEnabled;
extern bool velocityESPEnabled;
extern bool advancedFlagsEnabled;
extern bool memoryAimbotEnabled;
extern bool memoryAimbotSilent;
extern bool memoryAimbotVisibleOnly;
extern bool espGradientHealthBar;
extern bool espGlowEffect;
extern bool espTextShadow;
extern bool espDistanceFade;
extern bool espOutlineGlow;
extern bool externalAimbotAimlock;
extern bool externalAimbotPredictMovement;
extern bool externalAimbotAutoSwitch;
extern bool bulletRedirectionEnabled;
extern bool recordingBulletRedirectionKey;
extern float bulletRedirectionYaw;   // Yaw offset in degrees
extern float bulletRedirectionPitch; // Pitch offset in degrees
extern float externalAimbotMaxLockDistance;
extern float espMaxDistance;
extern float espTextScale;
extern float espIconScale;
extern float espGlowIntensity;
extern float memoryAimbotFOV;
extern float memoryAimbotSmoothness;
extern float pelvisWireRadius;
extern float spine0WireRadius;
extern float spine1WireRadius;
extern float spine2WireRadius;
extern float spine3WireRadius;
extern float neckWireRadius;
extern float headWireRadius;
extern float clavicleLWireRadius;
extern float upperarmLWireRadius;
extern float forearmLWireRadius;
extern float handLWireRadius;
extern float clavicleRWireRadius;
extern float upperarmRWireRadius;
extern float forearmRWireRadius;
extern float handRWireRadius;
extern float thighLWireRadius;
extern float calfLWireRadius;
extern float footLWireRadius;
extern float thighRWireRadius;
extern float calfRWireRadius;
extern float footRWireRadius;
extern float glowMaxDistance;
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
extern int wireframeNumSides;
extern int wireframeNumRings;
extern int memoryAimbotKey;
extern int memoryAimbotBone;
extern int espBoxStyle;
extern int healthBarStyle;
extern int skeletonStyle;
extern int externalAimbotTargetSelection;
extern int bulletRedirectionKey;
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
extern ImVec4 weaponColor;
extern ImVec4 soundESPColor;
extern ImVec4 backtrackESPColor;
extern ImVec4 velocityIndicatorColor;
extern ImVec4 advancedFlagsColor;
extern ImVec4 wireframeVisibleColor;  // Green for visible
extern ImVec4 wireframeInvisibleColor;  // Red for invisible
extern ImVec4 espTextColor;
extern ImVec4 espIconColor;
extern ImVec4 espWeaponColor;
extern ImVec4 espDistanceColor;
extern ImVec4 espBoxFillColor;
extern ImVec4 espBoxGradientTop;
extern ImVec4 espBoxGradientBottom;
extern ImVec4 skeletonStartColor;
extern ImVec4 skeletonEndColor;
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



extern void RenderKeybindState(const char* label, int key, bool& recordingMode);


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





// [Add to the Structs section]
struct BulletTracer {
    Vector3 start;
    Vector3 end;
    float timeCreated;      // When it was created
    float alpha;     // Current opacity
};

// [Add to Globals section]
extern bool bulletTracersEnabled;
extern ImVec4 bulletTracerColor;
extern std::vector<BulletTracer> localBulletTracers;
extern uintptr_t m_pBulletServicesOffset;

// [Add to Function Declarations]
void UpdateBulletTracers(const LocalPlayer& lp);
void RenderBulletTracers(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList);
void AngleVectors(const Vector3& angles, Vector3& forward);
extern float bulletTracerDuration; // How long they stay (seconds)
extern float bulletTracerThickness; // How thick the beam is





extern bool autoAcceptEnabled;
extern uintptr_t setLocalPlayerReadyAddr;
extern uintptr_t popupAcceptMatchFoundAddr;

typedef void(__fastcall* SetLocalPlayerReadyFn)(const char*);
typedef void(__fastcall* PopupAcceptMatchFoundFn)(void* thisptr);
extern PopupAcceptMatchFoundFn oPopupAcceptMatchFound;
void __fastcall HK_PopupAcceptMatchFound(void* thisptr);
bool SetupPopupAcceptMatchHook();



inline uintptr_t GetAbsoluteAddress(uintptr_t instruction_ptr, int offset, int size) {
    if (!instruction_ptr) return 0;
    // Read the 32-bit offset from the instruction
    int32_t relative_offset = *reinterpret_cast<int32_t*>(instruction_ptr + offset);
    // Calculate absolute address: (Instruction Start + Instruction Size + Offset)
    return instruction_ptr + size + relative_offset;
}




extern float weaponMaxDistance;
extern ImVec4 weaponColor;

extern float soundMaxTime;
extern std::map<Vector3, float> soundLocations;

extern bool showAdvancedFlags;

extern float backtrackDuration;
extern std::map<uintptr_t, std::vector<std::pair<Vector3, float>>> playerHistory;

extern bool autoStrafeEnabled;
extern float strafeSpeed;

// Add these function declarations
void RenderWeaponESP(const std::vector<WeaponEntity>& weapons, Matrix4x4 viewMatrix,
    float screenWidth, float screenHeight, ImDrawList* drawList);
void ProcessSounds();
void RenderSoundESP(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList);
void RenderAdvancedFlags(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color);
void RenderVelocityIndicator(Entity& e, ImVec2 boxPos, ImDrawList* drawList, ImU32 color);
void UpdateBacktrackData(const std::vector<Entity>& entities);
void RenderBacktrackESP(Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImDrawList* drawList);
void AdvancedBulletRedirection();


extern uintptr_t overrideViewAddr;


// Additions to cheat.h

// In structs section, before Entity:
struct Vector_t {
    float x, y, z;
};

struct QAngle_t {
    float x, y, z;  // pitch, yaw, roll
};

class CViewSetup {
public:
    unsigned char pad0[0x490];
    float flOrthoLeft;    // 0x0494
    float flOrthoTop;     // 0x0498
    float flOrthoRight;   // 0x049C
    float flOrthoBottom;  // 0x04A0
    unsigned char pad1[0x38];
    float flFov;          // 0x04D8
    float flFovViewmodel; // 0x04DC
    Vector_t vecOrigin;   // 0x04E0
    unsigned char pad2[0xC]; // 0x04EC
    QAngle_t angView;     // 0x04F8
    unsigned char pad3[0x14]; // 0x0504
    float flAspectRatio;  // 0x0518
    unsigned char pad4[0x71];
    BYTE nSomeFlags;
};





static bool prevOnGround = false;
static float prevVelZ = 0.0f;
extern bool bhopThreadRunning;
DWORD WINAPI RunBhopThread(LPVOID param);



// New globals for features
extern bool thirdPersonEnabled;
extern float thirdPersonDistance;  // How far back the camera is pulled
extern int thirdPersonKey;
extern bool thirdPersonActive;
extern bool thirdPersonToggleMode;
extern bool recordingThirdPersonKey;

std::string GetKeyName(int key);

// Function declarations (add to existing)
typedef void(__fastcall* OverrideViewFn)(void* thisptr, CViewSetup* setup);
extern OverrideViewFn oOverrideView;
void __fastcall HK_OverrideView(void* thisptr, CViewSetup* setup);
bool SetupOverrideViewHook();

extern uintptr_t dwCSGOInputOffset;
extern float NormalizeYaw(float yaw);
extern float ToDegrees(float radians);
extern float ToRadians(float degrees);








extern bool oofArrowsEnabled;
extern float oofArrowsRadius;
extern float oofArrowsSize;
extern bool oofArrowsDistanceColor; // Optional: Fade color based on distance
extern float oofMarginDegrees;




extern uintptr_t findHudElementAddr;
extern uintptr_t sendMessageClientAddr;
typedef void* (__fastcall* FindHudElementFn)(const char* name);
extern FindHudElementFn oFindHudElement;
typedef void* (__fastcall* SendMessageClientFn)(void*, const char*, unsigned int, uint64_t*);
extern SendMessageClientFn oSendMessageClient;
extern bool clientChatEnabled;
extern std::map<uintptr_t, int> previousHitGroup;  // If not already declared

// Function declaration
void SendChatMessage(const char* fmt, ...);
struct ChatColor {
    float r, g, b;
    ChatColor(float rr = 1.f, float gg = 1.f, float bb = 1.f) : r(rr), g(gg), b(bb) {}
};

class chat {
public:
    class builder {
    public:
        builder& operator<<(const std::string& s) {
            text_ += buildText(s, current_);
            return *this;
        }
        builder& operator<<(const char* s) {
            return *this << std::string(s);
        }
        builder& operator<<(int n) {
            return *this << std::to_string(n);
        }
        builder& operator<<(const ChatColor& c) {
            current_ = c;
            return *this;
        }
        inline std::string get() { return text_; }
    private:
        std::string text_;
        ChatColor current_ = { 1.f, 1.f, 1.f };
    };

    static std::string buildText(const std::string& str, const ChatColor& col);
    static void sendMessage(const std::string& raw_string);
};



// Advanced Scene Modulation
extern bool advancedModEnabled;
extern ImVec4 worldModColor;  // Default white
extern ImVec4 propModColor;

// Structs for scene objects (reverse-engineered)
struct Color_t {
    uint8_t r, g, b, a;
};

#define PAD(size) char pad_##__LINE__[size];

struct CAggregateSceneObjectData {
    char pad_0[0x38];
    Color_t m_rgba;
    char pad_1[0x8];
};

struct CAggregateSceneObject {
    char pad_2[0x120];
    int count;
    char pad_3[0x4];
    CAggregateSceneObjectData* data;
};

struct CMeshDrawPrimitive_t {
    char pad_4[0x18];
    CAggregateSceneObject* m_pObject;
    void* m_pMaterial;  // CMaterial2*
    char pad_5[0x28];
    Color_t m_rgba;
};


// Hook typedef
typedef void(__fastcall* RenderBatchListFn)(void* a1);
extern RenderBatchListFn oRenderBatchList;
void __fastcall HK_RenderBatchList(void* a1);

extern uintptr_t renderBatchListAddr;
extern bool worldModEnabled;
extern bool propModEnabled;






extern bool deSubtickEnabled;
extern uintptr_t createSubtickMoveStepAddr;
typedef void* (__fastcall* CreateSubtickMoveStepFn)(void*, void*, int);
extern CreateSubtickMoveStepFn oCreateSubtickMoveStep;
void* __fastcall HK_CreateSubtickMoveStep(void* thisptr, void* cmd, int unk);


// Auto strafe
extern bool autoStrafeEnabled;
extern float strafeSpeed;

// Jump bug
extern bool jumpBugEnabled;  // NEW: Enable auto jump bug


void RunAutoStrafe(const LocalPlayer& lp);
void RunJumpBug(const LocalPlayer& lp);  // NEW: Jump bug function


// Addresses
extern uintptr_t mouseInputEnabledAddr;
extern uintptr_t isRelativeMouseModeAddr;

// Original function pointers
typedef bool(__fastcall* MouseInputEnabledFn)(void* thisptr);
extern MouseInputEnabledFn oMouseInputEnabled;

typedef bool(__fastcall* IsRelativeMouseModeFn)(void* thisptr, bool enable);
extern IsRelativeMouseModeFn oIsRelativeMouseMode;

// Hook functions
bool __fastcall HK_MouseInputEnabled(void* thisptr);
bool __fastcall HK_IsRelativeMouseMode(void* thisptr, bool enable);

// Setup functions
bool SetupMouseInputEnabledHook();
bool SetupIsRelativeMouseModeHook();



extern bool fovHookV2Enabled;  // New toggle for this hook
extern uintptr_t getFOVAddr;   // Address found via pattern
typedef float(__fastcall* GetFOVFn)(void* thisptr);
extern GetFOVFn oGetFOV;
float __fastcall HK_GetFOV(void* thisptr);
bool SetupGetFOVHook();


// Additions to cheat.h

// New dynamic offsets
extern int offsetToIsGlowing;
extern int offsetToGlowSceneObjectEntity;
extern int offsetToGlowSceneObjectAttachedSceneObject;

// New globals for second glow
extern bool glowV2Enabled;
extern ImVec4 glowV2Color;
extern float glowV2Brightness;

// Function declarations
void ApplyGlowV2(uintptr_t entity, bool isTeammate, bool isVisible);






// In cheat.h

// The stages of a frame in Source 2
enum ClientFrameStage_t {
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START, // <--- Best for Skin Changer
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,                    // <--- Best for Visuals / No Recoil
    FRAME_RENDER_END
};

// Function pointer definition
typedef void(__fastcall* FrameStageNotifyFn)(void* thisptr, int stage);
extern FrameStageNotifyFn oFrameStageNotify;
void __fastcall HK_FrameStageNotify(void* thisptr, int stage);







extern bool noSpreadEnabled;
extern uintptr_t getInaccuracyAddr;
extern uintptr_t getSpreadAddr;
extern uintptr_t updateAccuracyPenaltyAddr;  // Optional, if you want to hook it later

// Hook typedefs (assuming __thiscall for member functions on C_WeaponCSBase)
typedef float(__thiscall* GetInaccuracyFn)(void* thisptr);
typedef float(__thiscall* GetSpreadFn)(void* thisptr);
typedef void(__thiscall* UpdateAccuracyPenaltyFn)(void* thisptr);

// Original function pointers
extern GetInaccuracyFn oGetInaccuracy;
extern GetSpreadFn oGetSpread;
extern UpdateAccuracyPenaltyFn oUpdateAccuracyPenalty;
extern float __fastcall HK_GetInaccuracy(void* thisptr);
extern float __fastcall HK_GetSpread(void* thisptr);
extern void __fastcall HK_UpdateAccuracyPenalty(void* thisptr);


class CParticleColor {
public:
    char pad[0x388];
    float r;
    float g;
    float b;
};

class CParticleCollectionData {
public:
    CParticleColor* color;
    char** particleName;
};

class CParticleCollection {
public:
    char pad[0x18];
    CParticleCollectionData* data;

    void SetColor(ImVec4 col, const char* target) {
        char* name = *this->data->particleName;
        if (strstr(name, target)) {
            this->data->color->r = col.x;
            this->data->color->g = col.y;
            this->data->color->b = col.z;
        }
    }
};





extern uintptr_t m_hActivePostProcessingVolumeOffset;
extern uintptr_t m_bExposureControlOffset;
extern uintptr_t m_flMinExposureOffset;
extern uintptr_t m_flMaxExposureOffset;

void UpdatePostProcessingExposure(const LocalPlayer& lp);

// Add variables for world modulation (in globals)
extern float worldBrightness;  // 0.0f dark to 1.0f normal



// Particle modulation globals
extern bool particleModEnabled;
extern ImVec4 particleColor;
extern std::string particleTarget;

extern bool rainbowEnabled;
extern float rainbowSpeed;

// Hook for particle rendering
typedef void (*RenderParticleFn)(CParticleCollection*, void*, void*, void*);
extern RenderParticleFn oRenderParticle;
void NewRenderParticle(CParticleCollection* collection, void* a2, void* a3, void* a4);

#define NUM_PARTICLE_TARGETS 969
extern const char* particleTargets[NUM_PARTICLE_TARGETS];
extern bool particleTargetsEnabled[NUM_PARTICLE_TARGETS];
extern ImVec4 particleColors[NUM_PARTICLE_TARGETS];
extern bool particleConfigPopupOpen;

struct ParticleGroup {
    std::string name;
    std::vector<int> indices;
    bool enabled = false;
    ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
};

// Add with other globals
extern ImVec4 globalParticleColor;
extern std::vector<ParticleGroup> particleGroups;


extern uintptr_t shotsFiredOffset;

extern bool skyModEnabled;
extern bool skyNightMode;
extern bool skyNoSky;
extern std::string customSkyName;  // e.g., "sky_csgo_night02"
extern ImVec4 skyColor;  // For tinting (r,g,b,a)
extern float skyBrightness;  // 0.0f (dark) to 1.0f (bright)

// Hook for UpdateSkybox
extern uintptr_t updateSkyboxAddr;
typedef void(__fastcall* UpdateSkyboxFn)(void* thisptr, void* skyentity);
extern UpdateSkyboxFn oUpdateSkybox;
void __fastcall HK_UpdateSkybox(void* thisptr, void* skyentity);
bool SetupUpdateSkyboxHook();



extern uintptr_t entityIdentityOffset;
extern uintptr_t designerNameOffset;
extern uintptr_t brightnessScaleOffset;
extern uintptr_t tintColorOffset;
extern uintptr_t skyMaterialOffset;  // m_hSkyMaterial = 0x570 (CStrongHandle<IMaterial2>)
extern uintptr_t skyTintColorOffset;  // m_vTintColor = 0x589 (Color_t, but actually uint8[4])
extern uintptr_t skyTintLightingOffset;  // m_vTintColorLightingOnly = 0x58D (Color_t)
extern uintptr_t skyBrightnessOffset;  // m_flBrightnessScale = 0x591 (float)

template<typename T>
T clamp(T x, T min, T max) {
    return (x < min) ? min : (x > max) ? max : x;
}

extern uintptr_t dwMapnameOffset;

struct C_SceneLightObject {
    char pad_0000[0xE4];
    float r;     // 0xE4
    float g;     // 0xE8
    float b;     // 0xEC
    // Add more if needed
};

extern bool lightModEnabled;
extern ImVec4 lightModColor;
extern uintptr_t lightSceneObjectAddr;

typedef void* (__fastcall* LightSceneObjectFn)(void*, void*, void*);
extern LightSceneObjectFn oLightSceneObject;
void* __fastcall HK_LightSceneObject(void* rcx, void* rdx, void* r8);
bool SetupLightSceneHook();







extern bool noLegsEnabled;  // Toggle for no legs feature
extern uintptr_t renderLegsAddr;  // Address found via pattern scan

typedef void(__fastcall* RenderLegsFn)(__int64* a1, __int64* a2, __int64 a3, __int64 a4, __int64 a5);
extern RenderLegsFn oRenderLegs;
void __fastcall HK_RenderLegs(__int64* a1, __int64* a2, __int64 a3, __int64 a4, __int64 a5);
bool SetupRenderLegsHook();

extern bool triggerbotEnabled;
extern float triggerbotDelay;
extern bool triggerbotVisibleOnly;
extern bool triggerbotTeammates;
extern int triggerbotKey;  // Optional: Bind to a key, e.g., VK_ALT
extern uintptr_t crosshairIDOffset;
extern uintptr_t forceAttackOffset;
extern bool recordingTriggerKey;
extern bool triggerKeyRecorded;
uintptr_t GetEntityFromSerialIndex(int index);
extern bool triggerbotRequireStill;
extern float triggerbotStillThreshold;
extern float triggerbotFirstShotDelay;
extern ULONGLONG triggerbotAcquireTime;
extern int triggerbotLastID;
extern ULONGLONG triggerbotLastShotTime;


extern bool antiAimEnabled;
extern int antiAimType; // 0: Backward, 1: Jitter, 2: Spin
extern float antiAimJitterRange;
extern float antiAimSpinSpeed; // 1.0f means 1 rotation per second (360 degrees)
extern int antiAimKey; // Key to hold for AA (optional)
extern bool recordingAntiAimKey;
void RunAntiAim();


struct SpectatorInfo {
    std::string name;
    std::string mode;
    bool isWatchingMe;    // New boolean
    uint32_t targetHandle; // Debug info
};

// Add these to your function declarations in cheat.h
uintptr_t GetPawnFromHandle(uint32_t handle);
uintptr_t GetObserverTarget(uintptr_t pawn); // Add this line
std::vector<SpectatorInfo> GetSpectatorsList(); // Renamed to avoid conflict
void RenderSpectatorListWindow(); // The UI function




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
extern bool IsValidPtr(void* ptr);

// Function declarations
bool LoadOffsets();
void PlayMP3(const std::string& filePath);
uintptr_t GetControllerByIndex(int index); // NEW: For spectator list
uintptr_t PatternScan(const char* module, const char* pattern);
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
void SetupImGuiStyle();
void RenderCleanMenu();
void RunExternalAimbot();
void RenderExternalAimbotFOV(ImDrawList* drawList, float screenWidth, float screenHeight);
void RunFOVChanger();
void DrawWireframeBone(ImDrawList* drawList, const Vector3& start3D, const Vector3& end3D, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImU32 color, float radius, int numSides, int numRings);
std::vector<WeaponEntity> GetDroppedWeapons();
void DrawWireframeSphere(ImDrawList* drawList, const Vector3& center, float radius, Matrix4x4 viewMatrix, float screenWidth, float screenHeight, ImU32 color, int numMeridians, int numParallels);
DWORD WINAPI KeyThread(LPVOID param);
LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
bool SetupHook();
bool IsGameInFocus();

#endif // CHEAT_H