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
bool espEnabled = false;
bool aimbotEnabled = false;
bool menuOpen = false;
bool healthBarEnabled = false;
bool snaplinesEnabled = false;
bool distanceEnabled = false;
bool boneESPEnabled = false;
bool headCircleEnabled = false;
bool showTeammates = false;
bool box3DEnabled = false;
bool glowESPEnabled = false;
bool ammoEnabled = false;
bool radarEnabled = false;
bool healthTextEnabled = false;
bool armorEnabled = false;
bool fovCircleEnabled = false;
bool flagsEnabled = false;
bool tracersEnabled = false;
bool noFlashEnabled = false;
bool revealAllEnabled = false;
bool infiniteArmorEnabled = false;
bool hitsoundEnabled = false; // New: Hitsound toggle
bool outlineEnabled = false; // New: Global outline toggle for ESP features
bool noRecoilEnabled = false;
bool watermarkEnabled = false;
bool crosshairEnabled = false;
bool customCrosshairEnabled = false;
bool showFPS = false;
bool showPosition = false;
bool showVelocity = false;
bool showWeaponInfo = false;
bool showSpectators = false;
bool showGameTime = false;
bool showKeysPressed = false;
bool showLocalPlayerInfo = false;
bool showEntityCount = false;
bool showSystemInfo = false;
bool showNetworkInfo = false;
bool showPlayerInfo = false;
bool showMatchInfo = false;
bool showAngleLines = false;
bool showTargetInfo = false;
bool showMinimap = false;
bool showRadarHud = false;
bool showGrenadeWarning = false;
bool showBombTimer = false;
bool showDefuseTimer = false;
bool showKillFeed = false;
bool showDamageIndicator = false;
bool showRecoilCrosshair = false;
bool showSpreadCircle = false;
bool showAmmoWarning = false;
bool showMoneyDisplay = false;
bool showRoundTimer = false;
bool showBombIcon = false;
bool showGrenadeTrajectory = false;
bool showSmokeWarning = false;
bool showFireWarning = false;
bool showFlashWarning = false;
bool showScopeLines = false;
bool showFovChangerIndicator = false;
bool showThirdPersonIndicator = false;
bool showSpectatorCount = false;
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
bool showMenuKeybind = false;
bool showSpectatorList = false; // NEW: Default off
bool externalAimbotEnabled = false;
bool externalAimbotUseHead = false;
bool recordingExternalAimKey = false;
bool fovChangerEnabled = false;
bool glowThroughWalls = false;
bool wireframeBoneESPEnabled = false;
bool nameESPEnabled = false;
bool weaponTextESPEnabled = false;
bool box2DEnabled = false;
bool skeletonEnabled = false;
bool armorBarEnabled = false;
bool distanceTextEnabled = false;
bool weaponESPEnabled = false;
bool soundESPEnabled = false;
bool backtrackESPEnabled = false;
bool velocityESPEnabled = false;
bool advancedFlagsEnabled = false;
bool espGradientHealthBar = false;
bool espGlowEffect = false;
bool espTextShadow = false;
bool espDistanceFade = false;
bool espOutlineGlow = false;
bool externalAimbotAimlock = false;
bool externalAimbotPredictMovement = false;
bool externalAimbotAutoSwitch = false;
bool bulletRedirectionEnabled = false;
bool recordingBulletRedirectionKey = false;
float bulletRedirectionYaw = 45.0f;   // Default 45 degrees right
float bulletRedirectionPitch = 0.0f;  // Default no pitch change
float externalAimbotMaxLockDistance = 2000.0f;
float espMaxDistance = 10000.0f;
float espTextScale = 1.0f;
float espIconScale = 1.0f;
float espGlowIntensity = 1.0f;
float pelvisWireRadius = 4.919f;
float spine0WireRadius = 6.982f;
float spine1WireRadius = 7.956f;
float spine2WireRadius = 7.922f;
float spine3WireRadius = 7.074f;
float neckWireRadius = 1.0f;
float headWireRadius = 4.527f;
float clavicleLWireRadius = 4.530f;
float upperarmLWireRadius = 4.429f;
float forearmLWireRadius = 1.686f;
float handLWireRadius = 1.882f;
float clavicleRWireRadius = 4.530f;
float upperarmRWireRadius = 4.429f;
float forearmRWireRadius = 1.686f;
float handRWireRadius = 1.882f;
float thighLWireRadius = 3.155f;
float calfLWireRadius = 4.625f;
float footLWireRadius = 1.0f;
float thighRWireRadius = 3.155f;
float calfRWireRadius = 4.625f;
float footRWireRadius = 1.0f;
float glowMaxDistance = 5000.0f;
float fovChangerValue = 90.0f;
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
bool aimOnVisibleOnly = false; // New
bool aimTeammates = false; // New
int aimKey = VK_LBUTTON; // New
int externalAimbotKey = VK_XBUTTON1; // Mouse side button by default
int externalAimbotHitgroup = 0; // 0=head, 1=neck, 2=chest, 3=pelvis, 4=random
int hitgroupBones[] = { bone_head, bone_neck, bone_spine_3, bone_pelvis, bone_head };
int wireframeNumSides = 6;     // Number of longitudinal lines
int wireframeNumRings = 4;     // Number of circumferential rings per segment
int espBoxStyle = ESP_BOX_STANDARD;
int skeletonStyle = SKELETON_STANDARD;
int healthBarStyle = HEALTH_BAR_VERTICAL;
int externalAimbotTargetSelection = TARGET_CLOSEST_TO_CROSSHAIR;
int bulletRedirectionKey = VK_CAPITAL; // Default to Caps Lock
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
ImVec4 weaponColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
ImVec4 wireframeVisibleColor = ImVec4(0.0f, 1.0f, 0.0f, 0.6f);  // Green for visible
ImVec4 wireframeInvisibleColor = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);  // Red for invisible
ImVec4 backtrackESPColor = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);
ImVec4 soundESPColor = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);
ImVec4 espTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 espIconColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 espWeaponColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
ImVec4 espDistanceColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 skeletonStartColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
ImVec4 skeletonEndColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
ImVec4 espBoxFillColor = ImVec4(0.1f, 0.1f, 0.1f, 0.3f);
ImVec4 espBoxGradientTop = ImVec4(0.2f, 0.8f, 0.2f, 0.8f);
ImVec4 espBoxGradientBottom = ImVec4(0.8f, 0.2f, 0.2f, 0.8f);
HWND g_hwnd = NULL;
std::string debugLog = "";
bool showDebugConsole = false;
bool recordingAimKey = false;
int recordedAimKey = VK_LBUTTON;
bool keyRecorded = false;
GlowSettings enemyGlowSettings;
GlowSettings teammateGlowSettings;






float weaponMaxDistance = 2000.0f;

float soundMaxTime = 5.0f;
std::map<Vector3, float> soundLocations;

bool showAdvancedFlags = false;

float backtrackDuration = 0.2f;
std::map<uintptr_t, std::vector<std::pair<Vector3, float>>> playerHistory;

bool autoStrafeEnabled = false;
float strafeSpeed = 1.0f;

std::string hitSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\sounds\\hitsound.mp3";
std::string headSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\sounds\\headsound.mp3";
std::string killSoundPath = "C:\\Users\\dionr\\Downloads\\cs2-full-main\\cs2-full-main\\sounds\\killsound.mp3";
float soundVolume = 1000.0f; // MCI volume (0-1000)

// Notifications global
std::vector<Notification> notifications;

// For health animation and hitmarkers
std::map<uintptr_t, float> displayedHealth; // For smooth health bar animation
std::vector<Hitmarker> hitmarkers; // List of active hitmarkers
float healthAnimationSpeed = 200.0f; // HP per second decrease rate
float hitmarkerSpeed = 50.0f; // Pixels per second upward movement
float hitmarkerFadeDuration = 1.0f; // Seconds to fade out






bool oofArrowsEnabled = false;
float oofArrowsRadius = 150.0f; // Distance from center of screen
float oofArrowsSize = 12.0f;    // Size of the triangle
bool oofArrowsDistanceColor = false;
float oofMarginDegrees = 10.0f;




bool triggerbotEnabled = false;
float triggerbotDelay = 0.0f;
bool triggerbotVisibleOnly = false;
bool triggerbotTeammates = false;
bool recordingTriggerKey = false;
bool triggerKeyRecorded = false;
bool triggerbotRequireStill = false;
float triggerbotStillThreshold = 5.0f;
int triggerbotKey = 0;  // 0 means always active when enabled
float triggerbotFirstShotDelay = 0.0f;  // ms, 0 = instant first shot
ULONGLONG triggerbotAcquireTime = 0;
int triggerbotLastID = 0;
ULONGLONG triggerbotLastShotTime = 0;



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





uintptr_t findHudElementAddr = 0;
uintptr_t sendMessageClientAddr = 0;
FindHudElementFn oFindHudElement = nullptr;
SendMessageClientFn oSendMessageClient = nullptr;
bool clientChatEnabled = false;







bool bhopThreadRunning = false;
bool bunnyhopEnabled = false;
bool edgeJumpEnabled = false;

bool autoAcceptEnabled = false;
uintptr_t setLocalPlayerReadyAddr = 0;
uintptr_t popupAcceptMatchFoundAddr = 0;
PopupAcceptMatchFoundFn oPopupAcceptMatchFound = nullptr;





bool bulletTracersEnabled = false;
ImVec4 bulletTracerColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Default Red
std::vector<BulletTracer> localBulletTracers;
float bulletTracerDuration = 2.0f; // Default 2 seconds (Adjustable)
float bulletTracerThickness = 1.5f; // Default thickness










bool thirdPersonEnabled = false;
float thirdPersonDistance = 50.0f;
int thirdPersonKey = 0;  // VK_F1 or something, but 0 unbound
bool thirdPersonActive = false;
bool thirdPersonToggleMode = false;  // default toggle
bool recordingThirdPersonKey = false;

std::string GetKeyName(int key) {
    switch (key) {
    case VK_LBUTTON: return "Left Mouse";
    case VK_RBUTTON: return "Right Mouse";
    case VK_MBUTTON: return "Middle Mouse";
    case VK_XBUTTON1: return "Mouse 4";
    case VK_XBUTTON2: return "Mouse 5";
    case VK_SHIFT: return "Shift";
    case VK_CONTROL: return "Ctrl";
    case VK_MENU: return "Alt";
    case VK_SPACE: return "Space";
    case VK_CAPITAL: return "Caps Lock";
    case VK_TAB: return "Tab";
    default: return "Key " + std::to_string(key);
    }
}


float NormalizeYaw(float yaw) {
    while (yaw > 180.0f) yaw -= 360.0f;
    while (yaw < -180.0f) yaw += 360.0f;
    return yaw;
}
float ToDegrees(float radians) { return radians * 180.0f / 3.1415926535f; }
float ToRadians(float degrees) { return degrees * 3.1415926535f / 180.0f; }
bool jumpBugEnabled = false;

bool advancedModEnabled = false;
ImVec4 worldModColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // Default white
ImVec4 propModColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

bool worldModEnabled = false;
bool propModEnabled = false;
float worldBrightness = 1.0f;







// Globals (initialize here or in cheat.h externs)
bool skyModEnabled = false;
bool skyNightMode = false;
bool skyNoSky = false;
std::string customSkyName = "sky_csgo_night02";
ImVec4 skyColor = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);  // Dark blue tint for night
float skyBrightness = 0.3f;  // Dim for night mode

UpdateSkyboxFn oUpdateSkybox = nullptr;
uintptr_t entityIdentityOffset = 0x10;
uintptr_t designerNameOffset = 0x20;
uintptr_t brightnessScaleOffset = 0xECC;
uintptr_t tintColorOffset = 0xEC1;
uintptr_t skyMaterialOffset = 0xEB0;
uintptr_t skyBrightnessOffset = 0xECC;







bool antiAimEnabled = false;
int antiAimType = 0; // 0: Backward, 1: Jitter, 2: Spin
float antiAimJitterRange = 15.0f;
float antiAimSpinSpeed = 1.0f; // 1.0f means 1 rotation per second (360 degrees)
int antiAimKey = 0; // Key to hold for AA (optional)
bool recordingAntiAimKey = false;




bool noLegsEnabled = false;



bool deSubtickEnabled = false;
ULONGLONG lastShotTime = 0;
int lastShotTarget = -1;






bool glowV2Enabled = false;
ImVec4 glowV2Color = ImVec4(1.0f, 0.0f, 0.0f, 0.5f);
float glowV2Brightness = 1.0f;

bool fovHookV2Enabled = false;
uintptr_t getFOVAddr = 0;
GetFOVFn oGetFOV = nullptr;

bool rainbowEnabled = false;
float rainbowSpeed = 1.0f;  // Cycles per second

bool particleModEnabled = false;
ImVec4 particleColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Default red

const char* particleTargets[NUM_PARTICLE_TARGETS] = {
    "tracers_taser",
    "taser_glow",
    "taser_flash",
    "fire",
    "explosion",
    "explosion_flash_1",
    "explosion_flash_1b",
    "explosion_flash_1b_child",
    "explosion_flash_1b_child2",
    "explosion_flash_1c",
    "explosion_flash_1c_child",
    "explosion_flash_1d",
    "explosion_flash_1d_child",
    "explosion_flash_1d_child2",
    "explosion_flash_1e",
    "explosion_flash_1e_child",
    "explosion_flash_1e_child2",
    "weapon_tracers",
    "impact_taser",
    "impact_taser_bodyfx",
    "impact_taser_bodyfx_arc",
    "env_fire_small",
    "env_fire_small_smoke",
    "smoke_trail",
    "smokestack",
    "smokestack_constant_wind",
    "smokestack_directional",
    "smokestack_directional_constant_wind",
    "smokestack_directional_wind",
    "smokestack_wind",
    "smokestack_constant_wind",
    "smokestack_directional_constant_wind",
    "smokestack_directional_wind",
    "smokestack_wind",
    "blood_trail_red",
    "blood_trail_red_cheap",
    "blood_trail_red_cheaper",
    "fire_smoke",
    "fire_smoke_child",
    "fire_smoke_child2",
    "fire_smoke_child3",
    "fire_smoke_child4",
    "weapon_tracers_assault",
    "weapon_tracers_machinegun",
    "weapon_tracers_pistol",
    "weapon_tracers_smg",
    "weapon_tracers_sniper",
    "weapon_tracers_shot",
    "weapon_tracers_taser",
    "weapon_tracers_taser_fallback",
    "weapon_tracers_taser_fallback2",
    "weapon_tracers_taser_wire1",
    "weapon_tracers_taser_wire1b",
    "weapon_tracers_taser_wire2",
    "electrical_arc_01_cp0",
    "electrical_arc_01_parent",
    "electrical_arc_01_system",
    "weapon_muzzle_flash_awp",
    "weapon_muzzle_flash_taser",
    "weapon_muzzle_flash_taser_cheap",
    "weapon_muzzle_flash_taser_cheaper",
    "weapon_muzzle_flash_taser_cheapest",
    "weapon_muzzle_flash_taser_parent",
    "weapon_muzzle_flash_taser_parent_cheap",
    "weapon_muzzle_flash_taser_parent_cheaper",
    "weapon_muzzle_flash_taser_parent_cheapest",
    "weapon_muzzle_smoke",
    "weapon_muzzle_smoke_child",
    "weapon_muzzle_smoke_child2",
    "weapon_muzzle_smoke_child3",
    "weapon_muzzle_smoke_child4",
    "weapon_shell_casing_556",
    "weapon_shell_casing_556_fallback",
    "weapon_shell_casing_556_fallback2",
    "weapon_shell_casing_556_parent",
    "weapon_shell_casing_9mm",
    "weapon_shell_casing_9mm_fallback",
    "weapon_shell_casing_9mm_fallback2",
    "weapon_shell_casing_9mm_parent",
    "weapon_shell_casing_rifle",
    "weapon_shell_casing_rifle_fallback",
    "weapon_shell_casing_rifle_fallback2",
    "weapon_shell_casing_rifle_parent",
    "weapon_shell_casing_shotgun",
    "weapon_shell_casing_shotgun_fallback",
    "weapon_shell_casing_shotgun_fallback2",
    "weapon_shell_casing_shotgun_parent",
    "explosion_electrical",
    "point_spotlight_cone",
    "wonderinglights",
    "Chicken_Crumble_death",
    "Chicken_gone",
    "Chicken_gone_cheaper",
    "Chicken_gone_crumble_Halloween",
    "Chicken_gone_feathers",
    "Chicken_gone_feathers_cheaper",
    "Chicken_gone_feathers_fire",
    "Chicken_gone_fleks",
    "Chicken_gone_zombie",
    "Chicken_goop",
    "Chicken_impact_burst",
    "Chicken_impact_burst_zombie",
    "Chicken_rez",
    "Chicken_rez_dirt_child",
    "Chicken_rez_dirt_child_clumps",
    "Chicken_rez_dirt_smoke_puff",
    "Chicken_roasted_firestream",
    "Chicken_roasted_steam",
    "blood_impact_basic",
    "blood_impact_chunks1",
    "blood_impact_chunks_fallback",
    "blood_impact_directional_highvel_spray",
    "blood_impact_directional_med",
    "blood_impact_directional_med_spray_med",
    "blood_impact_drops1",
    "blood_impact_dust",
    "blood_impact_dusty",
    "blood_impact_fallback",
    "blood_impact_friendly",
    "blood_impact_friendly_mist",
    "blood_impact_goop_heavy",
    "blood_impact_goop_light",
    "blood_impact_goop_light_medium",
    "blood_impact_goop_medium",
    "blood_impact_green_01",
    "blood_impact_headshot",
    "blood_impact_headshot_01b",
    "blood_impact_headshot_01c",
    "blood_impact_headshot_01d",
    "blood_impact_headshot_01e",
    "blood_impact_heavy",
    "blood_impact_heavyvpf",
    "blood_impact_high",
    "blood_impact_high_ground_decal",
    "blood_impact_high_mist_away",
    "blood_impact_high_vis_spray",
    "blood_impact_high_vis_trail",
    "blood_impact_light",
    "blood_impact_light_headshot",
    "blood_impact_localfrontenemy",
    "blood_impact_localfrontenemy_splatter",
    "blood_impact_localfrontenemy_splatter_lt",
    "blood_impact_localkillshot",
    "blood_impact_localkillshot_splatter",
    "blood_impact_localkillshot_splatter_rg",
    "blood_impact_localplayer",
    "blood_impact_localrearhit",
    "blood_impact_localrearhit_splatter",
    "blood_impact_low",
    "blood_impact_low_ground_decal",
    "blood_impact_low_ground_decal_alt",
    "blood_impact_low_mist_away",
    "blood_impact_low_spray_away",
    "blood_impact_low_vis_spray",
    "blood_impact_low_vis_spray_screen",
    "blood_impact_low_vis_spray_trail",
    "blood_impact_lp_mist",
    "blood_impact_lp_spray",
    "blood_impact_med",
    "blood_impact_med_ground_decal",
    "blood_impact_med_ground_decalalt",
    "blood_impact_med_ground_decalaltb",
    "blood_impact_med_mist_away",
    "blood_impact_med_spray_away",
    "blood_impact_med_vis_spray",
    "blood_impact_med_vis_spray_screen",
    "blood_impact_med_vis_spray_trail",
    "blood_impact_medi",
    "blood_impact_medium",
    "blood_impact_mist1",
    "blood_impact_mist_away",
    "blood_impact_mist_light",
    "blood_impact_red_01",
    "blood_impact_red_01_backspay",
    "blood_impact_red_01_chunk",
    "blood_impact_red_01_drops",
    "blood_impact_red_01_goop",
    "blood_impact_red_01_goop_a",
    "blood_impact_red_01_goop_a_backup",
    "blood_impact_red_01_goop_b",
    "blood_impact_red_01_mist",
    "blood_impact_yellow_01",
    "blood_pool",
    "gore_goop_generic",
    "impact_blood_glow",
    "blood_impact_headshot_01d",
    "impact_blood_glow_light_master",
    "impact_blood_glow_master",
    "impact_blood_glow_normal_master",
    "impact_taser_bodyfx_ashes",
    "ambient_leaves_01",
    "ambient_leaves_02",
    "ambient_paper_debris",
    "ambient_paper_debris_2",
    "directional_glow01_flare",
    "directional_light_glow01",
    "env_dust_emote",
    "env_light_glow_01",
    "env_light_glow_01_snapshot",
    "env_light_glow_beam_snapshot",
    "env_stringcablelights_cooltemp_snapshot",
    "env_stringcablelights_snapshot",
    "env_stringlights_cooltemp_snapshot",
    "env_stringlights_snapshot",
    "env_sun_clear",
    "env_sun_clear_child",
    "env_sun_cloudy",
    "env_sun_floating_island",
    "env_sun_moon",
    "env_sun_screenspace_flare",
    "env_sunray_directional_flare",
    "env_sunray_glow_beam_noflare_snapshot",
    "env_sunray_glow_beam_snapshot",
    "flared_light01",
    "glow01",
    "glow01_bloom",
    "glow01_flare",
    "glow01_snapshot",
    "glow01_snapshot_child",
    "light_glow_01",
    "omnilight_glow01",
    "soft_glow01",
    "sun_clear",
    "water_drip_area_01",
    "water_drip_area_01_fewer",
    "water_drip_area_01_small",
    "water_drip_area_01_small_origin",
    "water_drip_area_01_smaller_area",
    "water_drip_splash_01",
    "impact_dirt",
    "impact_tile",
    "impact_concrete",
    "impact_wood",
    "impact_metal",
    "impact_glass",
    "impact_rubber",
    "impact_plastic",
    "impact_chainlink",
    "impact_helmet",
    "impact_wallbang",
    "ricochet_sparks",
    "impact_light_flash",
    "impact_fx_hit_darken",
    "impact_generic_burst",
    "impact_spark_glow",
    "child_base",
    "child_burst",
    "child_chunks",
    "child_glow",
    "child_smoke",
    "child_flash",
    "water_wake_continuous_droplets",
    "water_wake_continuous_ripple_colliders",
    "water_wake_continuous_ripple_sides",
    "water_wake_continuous_sit",
    "water_wake_continuous_splash",
    "water_wake_continuous_surface_bubbles",
    "water_wake_fast",
    "water_wake_fast_droplets",
    "water_wake_fast_foam",
    "water_wake_fast_ripple_colliders",
    "water_wake_fast_ripple_rings",
    "water_wake_fast_ripple_rings_collision_bounce",
    "water_wake_fast_ripple_rings_foam",
    "water_wake_fast_ripple_ropes",
    "water_wake_fast_ripple_sides",
    "water_wake_fast_sit",
    "water_wake_fast_splash",
    "water_wake_fast_surface_bubbles",
    "water_wake_fixed_ripple_rings",
    "water_wake_fixed_sit",
    "water_wake_medium",
    "water_wake_slow",
    "water_wake_slow_ripple_left",
    "water_wake_slow_ripple_right",
    "water_wake_slow_ripple_rings",
    "water_wake_slow_ripple_rings_constant",
    "water_wake_slow_ripple_rings_later",
    "water_wake_slow_ripple_sides",
    "water_wake_slow_sit",
    "water_wake_slow_surface_bubbles",
    "water_wake_splash",
    "water_wake_splash_foam_surface",
    "water_wake_splash_foam_2nd",
    "water_wake_splash_splash",
    "water_wake_splash_splash_2nd",
    "water_wake_splash_upward",
    "water_wake_trail",
    "water_wake_trail_left",
    "water_anubis",
    "waterfall_base_foam",
    "waterfall_base",
    "waterfall_base_bubbles",
    "waterfall_base_collision_bounce",
    "waterfall_base_fish",
    "waterfall_base_foam_flat",
    "waterfall_base_ripples",
    "waterfall_base_splash_blobs",
    "boat_foam",
    "boat_foam_small",
    "slime_splash_01",
    "slime_splash_01_droplets",
    "slime_splash_01_reversed",
    "slime_splash_01_surface",
    "slime_splash_02",
    "slime_splash_03",
    "water_bubble_ambient",
    "water_bubble_trail_1",
    "water_foam_01",
    "water_foam_01_d",
    "water_foam_line_longb",
    "water_foam_line_long",
    "water_foam_line_shortb",
    "water_foam_line_shortc",
    "water_gunk_1",
    "water_impact_bubbles_1",
    "water_impact_bubbles_1b",
    "water_impact_bubbles_1c",
    "water_splash_01",
    "water_splash_01_blood",
    "water_splash_01_bubbles",
    "water_splash_01_column",
    "water_splash_01_droplet_trails",
    "water_splash_01_droplets_fallback",
    "water_splash_01_fallback",
    "water_splash_01_fish",
    "water_splash_01_foam",
    "water_splash_01_foam_delay",
    "water_splash_01_foam_surface",
    "water_splash_01_ripple_rings",
    "water_splash_01_ripple_rings_secondary",
    "water_splash_01_ripple_splash",
    "water_splash_01_sit",
    "water_splash_01_sit_gradual",
    "water_splash_01_splash",
    "water_splash_01_splash_secondary",
    "water_splash_01_splash_tall",
    "water_splash_01_surface2",
    "water_splash_01_surface3",
    "water_splash_01_surface3_dist",
    "water_splash_01_surface3_fallback",
    "water_splash_01_surface4",
    "water_splash_01_surface4_after",
    "water_splash_01_surface_delay",
    "water_splash_02",
    "water_splash_02_continuous",
    "water_splash_02_droplets",
    "water_splash_02_froth",
    "water_splash_02_froth2",
    "water_splash_02_surface",
    "water_splash_02_surface2",
    "water_splash_03",
    "water_splash_03_column",
    "water_splash_03_fallback",
    "water_splash_03_foam_surface_2nd",
    "water_splash_base_splash",
    "water_splash_column_wide",
    "water_splash_leakypipe_silo",
    "water_splash_leakypipe_silo_froth2",
    "water_splash_leakypipe_vertical",
    "water_splash_object_ripple_rings",
    "water_splash_object_splash",
    "water_splash_player_small",
    "water_splash_player_small_droplets",
    "water_splash_player_small_ripple",
    "water_splash_player_small_surface",
    "ash",
    "ash_burning",
    "ash_burning_fire",
    "ash_eddy",
    "ash_eddy_b",
    "ash_outer",
    "rain",
    "rain_edge",
    "rain_edge_ground",
    "rain_edge_sparse",
    "rain_edge_sparse_ground",
    "rain_edge_sparse_longdrop",
    "rain_impact_03",
    "rain_impact_04",
    "rain_impact_single",
    "rain_lamp_circle_drip",
    "rain_medium_area_ground",
    "rain_mist",
    "rain_mist_02",
    "rain_outer",
    "rain_outer_backup",
    "rain_outer_mist",
    "rain_puddle_ripples",
    "rain_puddle_ripples_large",
    "rain_puddle_ripples_small",
    "rain_sheet",
    "rain_sheet_mist",
    "rain_single_128",
    "rain_single_800",
    "rain_single_800_child",
    "rain_single_800_xx",
    "rain_single_drip",
    "rain_single_drip_ground",
    "rain_single_drip_test",
    "rain_single_impact_04",
    "rain_single_impact_04b",
    "rain_small_area_ground",
    "rain_small_edge",
    "rain_spot",
    "rain_storm",
    "rain_storm_impact_04",
    "rain_storm_leaves",
    "rain_storm_outer",
    "rain_stream_01_impact",
    "rain_stream_01_impact_2",
    "rain_stream_01_impact_3",
    "rain_stream_04_impact_1",
    "rain_stream_04_impact_2",
    "rain_volume",
    "rain_volume_vanity",
    "snow",
    "snow_drift",
    "snow_outer",
    "storm_cloud_lightning",
    "storm_cloud_parent",
    "storm_lightning_01_branch_01",
    "storm_lightning_01_branch_02",
    "storm_lightning_01_branch_03",
    "storm_lightning_01_branch_parent_01",
    "storm_lightning_01_branch_parent_02",
    "storm_lightning_01_branch_parent_03",
    "storm_lightning_01_cloud",
    "storm_lightning_01_glow",
    "storm_lightning_01_modglow",
    "storm_lightning_01_thin",
    "storm_fx",
    "burning_char_prev",
    "burning_test",
    "explosion_incend_air",
    "explosion_incend_air_core",
    "explosion_incend_air_debris",
    "explosion_incend_air_down",
    "explosion_incend_air_falling",
    "explosion_incend_air_fallingfire",
    "explosion_incend_air_smoke",
    "explosion_incend_air_splash01a",
    "explosion_incend_air_splash07a",
    "explosion_incend_ground_splash07a",
    "explosion_molotov_air",
    "explosion_molotov_air_core",
    "explosion_molotov_air_debris",
    "explosion_molotov_air_down",
    "explosion_molotov_air_falling",
    "explosion_molotov_air_smoke",
    "explosion_molotov_air_splash01a",
    "explosion_molotov_air_splash07a",
    "explosion_molotov_fallingfire",
    "explosion_molotov_ground_debris",
    "explosion_molotov_ground_splash07a",
    "extinguish_embers_small_01",
    "extinguish_embers_small_02",
    "extinguish_fire",
    "extinguish_fire_blastout_01",
    "extinguish_fire_swirl",
    "extinguish_fire_swirl_smoke",
    "fire_core",
    "fire_core_vsnap",
    "fire_edge",
    "fire_edge_vsnap",
    "fire_filler",
    "fire_filler_vsnap",
    "firework_crate_explosion_01",
    "firework_crate_explosion_02",
    "firework_crate_ground_effect_fallback1",
    "firework_crate_ground_effect_fallback2",
    "firework_crate_ground_embers_01",
    "firework_crate_ground_glow_01",
    "firework_crate_ground_glow_02",
    "firework_crate_ground_low_02",
    "firework_crate_ground_low_03",
    "firework_crate_ground_low_04",
    "firework_crate_ground_primary_01_fallback",
    "firework_crate_ground_smoke_01",
    "firework_crate_ground_sparks_01",
    "firework_crate_shower_01b",
    "fireworks_explosion_glow_03",
    "fireworks_explosion_trail_04",
    "incen_explosion_child_sprays",
    "incendiary_child_flame01a",
    "incendiary_child_flame03a",
    "incendiary_explosion",
    "incendiary_fire01",
    "incendiary_groundfire",
    "incendiary_groundfire_climbing_outline",
    "incendiary_groundfire_climbingset",
    "incendiary_groundfire_fillsparks_low",
    "incendiary_groundfire_filltest",
    "incendiary_groundfire_filter_napalm",
    "incendiary_groundfire_filter_replicator",
    "incendiary_groundfire_lightning",
    "incendiary_groundfire_main_center",
    "incendiary_groundfire_main_fancy",
    "incendiary_groundfire_outline",
    "incendiary_groundfire_projected",
    "incendiary_groundfire_snapshot_child_base",
    "incgrenade_throw_trail",
    "incgrenade_throw_trail_glow",
    "molotov_bodyburn",
    "molotov_bodyburn_footprint",
    "molotov_bodyburn_smoke",
    "molotov_center_smoking_ground",
    "molotov_center_smoking_ground_child01",
    "molotov_child_flame01a",
    "molotov_child_flame01b",
    "molotov_child_flame01c",
    "molotov_child_flame02a",
    "molotov_child_flame02b",
    "molotov_child_flame02c",
    "molotov_child_flame03a",
    "molotov_child_flame03b",
    "molotov_child_flame03c",
    "molotov_child_flame04a",
    "molotov_child_flame04c",
    "molotov_child_flame05a",
    "molotov_child_glow01a",
    "molotov_child_glow01b",
    "molotov_child_glow02a",
    "molotov_child_glow02b",
    "molotov_child_glow03",
    "molotov_explosion",
    "molotov_explosion_child_fireball",
    "molotov_explosion_child_fireball2",
    "molotov_explosion_child_fireball3",
    "molotov_explosion_child_fireball4",
    "molotov_explosion_child_flash",
    "molotov_explosion_child_ground1",
    "molotov_explosion_child_ground2",
    "molotov_explosion_child_sprays",
    "molotov_fire01_cheap",
    "molotov_groundfire",
    "molotov_groundfire_00high",
    "molotov_groundfire_00medium",
    "molotov_groundfire_child_base",
    "molotov_groundfire_child_base8",
    "molotov_groundfire_child_base_glow",
    "molotov_groundfire_child_base_hcp",
    "molotov_groundfire_child_embers",
    "molotov_groundfire_child_glow",
    "molotov_groundfire_child_glow2",
    "molotov_groundfire_child_glow2_child",
    "molotov_groundfire_climbing_debug",
    "molotov_groundfire_climbingset",
    "molotov_groundfire_endcap",
    "molotov_groundfire_endcap_ground",
    "molotov_groundfire_endcap_ground_smoke",
    "molotov_groundfire_endcap_wall",
    "molotov_groundfire_fallback",
    "molotov_groundfire_fallback2",
    "molotov_groundfire_filltest",
    "molotov_groundfire_lighting",
    "molotov_groundfire_main",
    "molotov_groundfire_main_center",
    "molotov_groundfire_main_fancy",
    "molotov_groundfire_outline",
    "molotov_groundfire_outline_burn",
    "molotov_groundfire_outline_embers",
    "molotov_groundfire_outlineset_debug",
    "molotov_groundfire_projected",
    "molotov_groundfire_remnant",
    "molotov_groundfire_scotch",
    "molotov_groundfire_snapshot_child_base",
    "molotov_groundfire_snapshot_water_sit",
    "molotov_groundfire_streaks",
    "molotov_smoke_screen",
    "molotov_smoking_ground_child01",
    "molotov_smoking_ground_child02",
    "molotov_smoking_ground_child02_cheap",
    "molotov_smoking_ground_child02_cheapov",
    "molotov_smoking_ground_child03",
    "molotov_smoking_ground_child03_cheap",
    "molotov_smoking_ground_child03_cheapov",
    "perf_test_combined",
    "perf_test_separate",
    "perf_test_separate_childa",
    "perf_test_separate_childb",
    "perf_test_separate_childc",
    "spark_explosive_test",
    "bumpmine_active",
    "bumpmine_active_glow",
    "bumpmine_active_glow2",
    "bumpmine_active_glow_outer",
    "bumpmine_player_trail",
    "confetti_a",
    "confetti_a_omni",
    "confetti_b",
    "confetti_b_omni",
    "confetti_balloons",
    "confetti_c",
    "confetti_c_omni",
    "dronegun_laser_sight",
    "dronegun_laser_sight_dot",
    "dronegun_laser_sight_dot_0",
    "mvp_tracer",
    "mvp_tracer_here",
    "snow_hit_player_screeneffect",
    "snow_hit_player_screeneffect_b",
    "snow_hit_player_screeneffect_bb",
    "snowball_pile",
    "snowball_pile_puddle",
    "snowball_pile_snow",
    "weapon_confetti",
    "weapon_confetti_balloons",
    "weapon_confetti_cheap",
    "weapon_confetti_omni",
    "weapon_confetti_omni_cheap",
    "weapon_confetti_sparks",
    "weapon_confetti_sparks_2",
    "weapon_decoy_ground_effect",
    "weapon_decoy_ground_effect_shot",
    "weapon_decoy_ground_embers_01",
    "weapon_decoy_ground_low_02",
    "weapon_decoy_ground_low_03",
    "weapon_decoy_ground_low_04",
    "weapon_decoy_ground_primary_01",
    "weapon_decoy_ground_smoke_01",
    "weapon_empty",
    "weapon_grenade_pin",
    "weapon_incend_core_sparks",
    "weapon_incend_held",
    "weapon_incend_held_sparks",
    "weapon_incend_throw",
    "weapon_molotov_fp",
    "weapon_molotov_fire2",
    "weapon_molotov_fire3",
    "weapon_molotov_fire3b",
    "weapon_molotov_fire3d",
    "weapon_molotov_fire_blue",
    "weapon_molotov_flick_glow",
    "weapon_molotov_held",
    "weapon_molotov_held_fps",
    "weapon_molotov_throw",
    "weapon_molotov_thrown_child1",
    "weapon_molotov_thrown_child3",
    "weapon_molotov_thrown_glow",
    "weapon_muzzle_flash_assault_rif",
    "weapon_muzzle_flash_assault_rifle_fallback",
    "weapon_muzzle_flash_assault_rifle_fp",
    "weapon_muzzle_flash_assault_rifle_glow",
    "weapon_muzzle_flash_assault_rifle_main",
    "weapon_muzzle_flash_assault_rifle_main_sub1",
    "weapon_muzzle_flash_assault_rifle_main_sub2",
    "weapon_muzzle_flash_assault_rifle_parent_silenc",
    "weapon_muzzle_flash_assault_rifle_silenced_glow",
    "weapon_muzzle_flash_assault_rifle_vent_flash",
    "weapon_muzzle_flash_assault_rifle_vent_fp",
    "weapon_muzzle_flash_assault_rifle_vent_tp_cheap",
    "weapon_muzzle_flash_assault_rifle_flame",
    "weapon_muzzle_flash_autoshotgun",
    "weapon_muzzle_flash_autoshotgun_fallback",
    "weapon_muzzle_flash_autoshotgun_fp",
    "weapon_muzzle_flash_autoshotgun_cheap",
    "weapon_muzzle_flash_autoshotgun_glow",
    "weapon_muzzle_flash_autoshotgun_main",
    "weapon_muzzle_flash_autoshotgun_main2",
    "weapon_muzzle_flash_awp_tracer",
    "weapon_muzzle_flash_huntingrifle",
    "weapon_muzzle_flash_huntingrifle_fp_glow",
    "weapon_muzzle_flash_huntingrifle_glow_cheap",
    "weapon_muzzle_flash_huntingrifle_main",
    "weapon_muzzle_flash_huntingrifle_main2",
    "weapon_muzzle_flash_huntingrifle_vent_fallback",
    "weapon_muzzle_flash_para",
    "weapon_muzzle_flash_para_fallback",
    "weapon_muzzle_flash_para_fallback2",
    "weapon_muzzle_flash_para_flash",
    "weapon_muzzle_flash_para_fp_cheap",
    "weapon_muzzle_flash_para_glow",
    "weapon_muzzle_flash_para_main",
    "weapon_muzzle_flash_para_main2_fallback",
    "weapon_muzzle_flash_para_tracers",
    "weapon_muzzle_flash_para_vent",
    "weapon_muzzle_flash_pistol_core",
    "weapon_muzzle_flash_pistol_core2",
    "weapon_muzzle_flash_pistol_elite",
    "weapon_muzzle_flash_pistol_elite_fallback",
    "weapon_muzzle_flash_pistol_fallback2",
    "weapon_muzzle_flash_pistol_fp",
    "weapon_muzzle_flash_pistol_main_cheap",
    "weapon_muzzle_flash_pistol_main2",
    "weapon_muzzle_flash_pistol_main2_silenced",
    "weapon_muzzle_flash_shotgun",
    "weapon_muzzle_flash_shotgun_fp",
    "weapon_muzzle_flash_shotgun_fp_cheap",
    "weapon_muzzle_flash_shotgun_glow",
    "weapon_muzzle_flash_shotgun_glow2",
    "weapon_muzzle_flash_shotgun_glow3",
    "weapon_muzzle_flash_shotgun_main",
    "weapon_muzzle_flash_smg",
    "weapon_muzzle_flash_smg_backup",
    "weapon_muzzle_flash_smg_fallback",
    "weapon_muzzle_flash_smg_fallback2",
    "weapon_muzzle_flash_smg_fp",
    "weapon_muzzle_flash_smg_fp_cheap",
    "weapon_muzzle_flash_smg_glow",
    "weapon_muzzle_flash_smg_glow2",
    "weapon_muzzle_flash_smg_glow3",
    "weapon_muzzle_flash_smg_main",
    "weapon_muzzle_flash_smg_main2",
    "weapon_muzzle_flash_smg_main_silenced",
    "weapon_muzzle_flash_smg_parent_silenced",
    "weapon_muzzle_flash_smg_silenced",
    "weapon_muzzle_flash_smg_silenced_fp_cheap",
    "weapon_muzzle_flash_smoke_medium",
    "weapon_muzzle_flash_smoke_small",
    "weapon_muzzle_flash_smoke_small2",
    "weapon_muzzle_flash_smoke_small4",
    "weapon_muzzle_flash_smoke_smallest",
    "weapon_muzzle_flash_sparks2",
    "weapon_muzzle_flash_sparks3",
    "weapon_muzzle_flash_sparks4",
    "weapon_muzzle_flash_sparks5",
    "weapon_muzzle_flash_sparks6",
    "weapon_muzzle_flash_sparks7",
    "weapon_muzzle_flash_taser_fallback",
    "weapon_muzzle_flash_taser_fallback2",
    "weapon_muzzle_smoke",
    "weapon_muzzle_smoke_b",
    "weapon_muzzle_smoke_b_version2",
    "weapon_muzzle_smoke_long",
    "weapon_muzzleflash_illumination",
    "weapon_sensorgren_beep",
    "weapon_sensorgren_beep_light_inner",
    "weapon_sensorgren_debris",
    "weapon_sensorgren_detlight",
    "weapon_sensorgren_detlight_child01",
    "weapon_sensorgren_detlight_child02",
    "weapon_sensorgren_detonate",
    "weapon_sensorgren_glowing",
    "weapon_sensorgren_smoke",
    "weapon_sensorgren_spark_01",
    "weapon_sensorgren_spark_02",
    "weapon_sensorgren_spark_02b",
    "weapon_sensorgren_spark_03",
    "weapon_sensorgren_wiggly",
    "weapon_shell_casing_45acp",
    "weapon_shell_casing_57",
    "weapon_shell_casing_9mm",
    "weapon_shell_casing_awp",
    "weapon_shell_casing_deagle",
    "weapon_shell_casing_rifle",
    "weapon_shell_casing_shotgun",
    "weapon_shell_casing_shotgun_nova",
    "weapon_shell_casing_super_trail",
    "weapon_shell_casing_super_trail_base",
    "weapon_shell_casing_super_trail_glow",
    "weapon_shell_casing_super_trail_smoke",
    "weapon_shell_casing_super_trail_sparks",
    "weapon_shell_eject_smoke_assrifle2",
    "weapon_shell_eject_smoke_assrifle3",
    "weapon_shell_eject_smoke_awp2",
    "weapon_shell_eject_smoke_awp3",
    "weapon_shell_eject_smoke_para2",
    "weapon_shell_eject_smoke_para3",
    "weapon_shell_eject_smoke_pistol1",
    "weapon_shell_eject_smoke_pistol2",
    "weapon_shell_eject_smoke_shotgun",
    "weapon_shell_eject_smoke_shotgun_nova",
    "weapon_shell_eject_smoke_pistol3",
    "weapon_shell_eject_smoke_pistol_elite",
    "weapon_shell_eject_smoke_rifle2",
    "weapon_shell_eject_smoke_rifle3",
    "weapon_shell_eject_smoke_shotgun1",
    "weapon_shell_eject_smoke_shotgun2",
    "weapon_shell_eject_smoke_shotgun3",
    "weapon_shell_eject_smoke_smg1",
    "weapon_shell_eject_smoke_smg2",
    "weapon_shell_eject_smoke_smg3",
    "weapon_snowball_impact_child_base",
    "weapon_snowball_impact_child_smoke_puff",
    "weapon_snowball_impact_screen",
    "weapon_snowball_impact_splash",
    "weapon_snowball_impact_splat",
    "weapon_snowball_impact_splat2",
    "weapon_snowball_impact_stuck_wall",
    "weapon_snowball_trail",
    "weapon_taser_flash",
    "weapon_taser_flash_impact",
    "weapon_taser_glow_impact",
    "weapon_taser_sparks",
    "weapon_taser_sparks_impact",
    "weapon_tracers_50cal",
    "weapon_tracers_50cal_glow",
    "weapon_tracers_50cal_low_glow",
    "weapon_tracers_assrifle_glow",
    "weapon_tracers_assrifle_aug",
    "weapon_tracers_assrifle_aug_b",
    "weapon_tracers_mach",
    "weapon_tracers_original",
    "weapon_tracers_pistol",
    "weapon_tracers_rifle",
    "weapon_tracers_rifle_scar",
    "weapon_tracers_rifle_ssg",
    "weapon_tracers_rifle_wisp_sst",
    "weapon_tracers_rifle_wisp_scar",
    "weapon_tracers_taser_wire1a",
    "weapon_tracers_taser_fallback2",
    "weapon_tracers_shot",
    "weapon_tracers_taser_wire2",
    "weapon_wallbang_residual_tracers",
    "weapon_tracers_taser_wire1b",
    "weapon_tracers_taser",
    "weapon_tracers_rifle_wisp_ssg",
    "weapon_tracers_smg",
    "weapon_tracers_taser_fallback",
    "weapon_tracers_taser_fallback",
    "uweapon_muzflsh_ak47_compensator_flash",
    "uweapon_muzflsh_ak47_fps",
    "uweapon_muzflsh_ak47_primaryflash_alt",
    "uweapon_muzflsh_ak47_primaryflash",
    "uweapon_muzflsh_ak47",
    "uweapon_muzflsh_aug_compensator_flash",
    "uweapon_muzflsh_aug_fps",
    "uweapon_muzflsh_aug_ironsight",
    "uweapon_muzflsh_aug_primarybeam",
    "uweapon_muzflsh_aug_primaryflash",
    "uweapon_muzflsh_aug",
    "uweapon_muzflsh_awp_primaryflash_alt",
    "uweapon_muzflsh_awp",
    "uweapon_muzflsh_deagle_compensator_flash",
    "uweapon_muzflsh_deagle_fakeflash",
    "uweapon_muzflsh_deagle_fps",
    "uweapon_muzflsh_deagle_gen_smoke",
    "uweapon_muzflsh_deagle_primaryflash_fps",
    "uweapon_muzflsh_deagle",
    "uweapon_muzflsh_fakelight_64",
    "uweapon_muzflsh_fakelight_ironsight",
    "uweapon_muzflsh_fakelight",
    "uweapon_muzflsh_g3sg1_smoke",
    "uweapon_muzflsh_gen_beam",
    "uweapon_muzflsh_gen_empty_gp1",
    "uweapon_muzflsh_gen_empty",
    "uweapon_muzflsh_gen_large_smoke",
    "uweapon_muzflsh_gen_smoke",
    "uweapon_muzflsh_gen_smokeleft",
    "uweapon_muzflsh_gen_smokeright",
    "uweapon_muzflsh_gen_spark",
    "uweapon_muzflsh_ground_smoke",
    "uweapon_muzflsh_mach_compensator_flash",
    "uweapon_muzflsh_mach_empty_gp1",
    "uweapon_muzflsh_mach_fps",
    "uweapon_muzflsh_mach_primarybeam",
    "uweapon_muzflsh_mach_primaryflash",
    "uweapon_muzflsh_mach_smoke",
    "uweapon_muzflsh_mach_spark",
    "uweapon_muzflsh_mach",
    "uweapon_muzflsh_riffle_fps",
    "uweapon_muzflsh_riffle",
    "uweapon_muzflsh_riffle_lrg",
    "uweapon_muzflsh_riffle_lrg_fps",
    "uweapon_muzflsh_shot",
    "uweapon_muzflsh_shot_compensator_flash",
    "uweapon_muzflsh_shot_empty_gp1",
    "uweapon_muzflsh_shot_fps",
    "uweapon_muzflsh_shot_primarybeam",
    "uweapon_muzflsh_shot_primaryflash",
    "uweapon_muzflsh_shot_primaryflash_alt",
    "uweapon_muzflsh_shot_smoke",
    "uweapon_muzflsh_shot_spark",
    "uweapon_muzflsh_ssg08_leftbreak",
    "uweapon_muzflsh_ssg08_rightbreak",
    "uweapon_muzsilenced_p05subm",
    "uweapon_muzsilenced_riff",
    "uweapon_muzsilenced_rif_fps",
    "uweapon_muzsilenced_subm",
    "uweapon_muzsilenced_subm_beam",
    "uweapon_muzsilenced_subm_beam_fps",
    "uweapon_muzsilenced_subm_smoke",
    "uweapon_muzflash_pist_beam",
    "uweapon_muzflash_pist_empty",
    "uweapon_muzflash_pist_fakelight",
    "uweapon_muzflash_pist_fire",
    "uweapon_muzflash_pist_fire_alt",
    "uweapon_muzflash_pist_revolver",
    "uweapon_muzflash_pist_revolver_fps",
    "uweapon_muzflash_pist_smoke",
    "uweapon_muzflash_pist_spark",
    "uweapon_muzflash_revolver_empty",
    "uweapon_muzflash_revolver_smoke",
    "uweapon_muzflash_rif_break",
    "uweapon_muzflash_rif_smoke",
    "uweapon_muzflash_subm",
    "uweapon_muzflash_subm_beam",
    "uweapon_muzflash_subm_empty",
    "uweapon_muzflash_subm_fakelight",
    "uweapon_muzflash_subm_fire",
    "uweapon_muzflash_subm_fps",
    "uweapon_muzzleflash_subm_p90beam_vpc",
    "uweapon_muzzleflash_subm_smoke_vpc",
    "uweapon_muzzleflash_subm_spark_vpc",
    "weapon_muzzleflash_awp_flare_vpc",
    "weapon_muzzleflash_awp_spark_vpc",
    "weapon_muzzleflash_basic_vpc",
    "weapon_muzzleflash_basic_empty_vpc",
    "weapon_muzzleflash_basic_fire_alt_vpc",
    "weapon_muzzleflash_basic_fire_long_vpc",
    "weapon_muzzleflash_basic_smoke_vpc",
    "weapon_muzzleflash_basic_spark_vpc",
    "weapon_muzzleflash_snp_vpc",
    "weapon_muzzleflash_snp_ar_vpc",
    "weapon_muzzleflash_snp_ar_fps_vpc",
    "weapon_muzzleflash_snp_fps_vpc",
    "weapon_muzzleflash_ssg08_vpc",
    "fountain",
    "lights",
    "modelfountain",
    "point_spotlight_cone",
    "wonderinglights",
    "msg_damage",
    "path_particle_cable_default",
    "path_particle_cable_nuke",
    "path_particle_cable_static",
    "path_particle_cable_static_cheap",
    "path_particle_cable_static_cheap_opaque",
    "path_particle_cable_static_translucent",
    "path_particle_cable_static_white",
    "path_particle_cable_static_tintable",
    "path_particle_cable_static_yellow",
    "spectator_utility_trail",

    "rain_edge",
    "rain_small_edge",
    "rain_edge_ground",
    "rain_edge_sparse_ground",
    "rain_medium_area_ground",
    "rain_volume",
    "rain_volume",
    "rain_single_drip_ground",
    "rain_small_area_ground",
    "rain_large_area_ground",
    "rain_single_drip_ground",

    "env_light_glow_01",
    "env_light_glow_beam_snapshot",
    "glow_01_snapshot",
    "omnilight_glow01",
    "env_stringlights_snapshot",
    "directionallight_glow01",
    "env_sunray_glow_beam_snapshot",

    "ambient_sparks",
    "env_sparks_directional_e",
    "explosion_basic_water_smoke_linger",
    
    "waterfall_anubis",

    "dust_fog_volume_initial",
    "overpass_steam_rising_noise",
    "gnats_illum2",
    "dust_fog_volume_128_128_noise",
    
    "tv_monitor_impact",
    
    "train_dust",
    "bunker_steam_vertical_path",
    "overpass_steam_vertical_large",
    
    "dustmodes_slower",
    
    "aircraft_navred",
    "aircraft_navgreen",
    
    "barrel_burning_engine_fire",
    "barrel_burning_engine_smoke2",
    
    "glow03",
    "survival_bunker_light_dustmodes",
    "baggage_steam_vent_e_detail",
    
    "footstep_dirt",
    "footstep_grass",
    "footstep_grass_b",
    "footstep_mud",
    "footstep_puddle",
    "footstep_puddle_b",
    "footstep_wet"
};

ImVec4 globalParticleColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

bool particleTargetsEnabled[NUM_PARTICLE_TARGETS] = {
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false
};

ImVec4 particleColors[NUM_PARTICLE_TARGETS] = {
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), 
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), 
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), 
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), 
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
    ImVec4(1.0f, 0.8f, 0.2f, 1.0f), ImVec4(1.0f, 0.8f, 0.2f, 1.0f)
    
};

std::vector<ParticleGroup> particleGroups;

bool particleConfigPopupOpen = false;  // Controls popup visibility

struct ParticleInit {
    ParticleInit() {
        for (int i = 0; i < NUM_PARTICLE_TARGETS; ++i) {
            particleTargetsEnabled[i] = false;
            particleColors[i] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        }

        // Override specific colors for original particles (as before)
        particleColors[0] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red for tracers_taser
        particleColors[1] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green for taser_glow
        particleColors[2] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);  // Blue for taser_flash
        particleColors[3] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow for fire
        particleColors[4] = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);  // Magenta for explosion
        particleColors[17] = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);  // Light blue for tracers (index 17 is weapon_tracers)
        particleColors[18] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange for impact_taser
        particleColors[19] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);   // Cyan for impact
        particleColors[149] = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);  // Green for ambient_leaves_01
        particleColors[184] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_drip_area_01
        particleColors[229] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_dirt
        particleColors[230] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_tile
        particleColors[231] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_concrete
        particleColors[232] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_wood
        particleColors[233] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_metal
        particleColors[234] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_glass
        particleColors[235] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_rubber
        particleColors[236] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_plastic
        particleColors[237] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_chainlink
        particleColors[238] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_helmet
        particleColors[239] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_wallbang
        particleColors[240] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // ricochet_sparks
        particleColors[241] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_light_flash
        particleColors[242] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_fx_hit_darken
        particleColors[243] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_generic_burst
        particleColors[244] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // impact_spark_glow
        particleColors[245] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // child_base
        particleColors[246] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // child_burst
        particleColors[247] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // child_chunks
        particleColors[248] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // child_glow
        particleColors[249] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // child_smoke
        particleColors[250] = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);  // child_flash
        particleColors[251] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_continuous_droplets
        particleColors[252] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_continuous_ripple_colliders
        particleColors[253] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_continuous_ripple_sides
        particleColors[254] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_continuous_sit
        particleColors[255] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_continuous_splash
        particleColors[256] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_continuous_surface_bubbles
        particleColors[257] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast
        particleColors[258] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_droplets
        particleColors[259] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_foam
        particleColors[260] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_ripple_colliders
        particleColors[261] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_ripple_rings
        particleColors[262] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_ripple_rings_collision_bounce
        particleColors[263] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_ripple_rings_foam
        particleColors[264] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_ripple_ropes
        particleColors[265] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_ripple_sides
        particleColors[266] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_sit
        particleColors[267] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_splash
        particleColors[268] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fast_surface_bubbles
        particleColors[269] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fixed_ripple_rings
        particleColors[270] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_fixed_sit
        particleColors[271] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_medium
        particleColors[272] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow
        particleColors[273] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_ripple_left
        particleColors[274] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_ripple_right
        particleColors[275] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_ripple_rings
        particleColors[276] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_ripple_rings_constant
        particleColors[277] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_ripple_rings_later
        particleColors[278] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_ripple_sides
        particleColors[279] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_sit
        particleColors[280] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_slow_surface_bubbles
        particleColors[281] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_splash
        particleColors[282] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_splash_foam_surface
        particleColors[283] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_splash_foam_2nd
        particleColors[284] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_splash_splash
        particleColors[285] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_splash_splash_2nd
        particleColors[286] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_splash_upward
        particleColors[287] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_trail
        particleColors[288] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_wake_trail_left
        particleColors[289] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_anubis
        particleColors[290] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_foam
        particleColors[291] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base
        particleColors[292] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_bubbles
        particleColors[293] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_collision_bounce
        particleColors[294] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_fish
        particleColors[295] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_foam_flat
        particleColors[296] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_ripples
        particleColors[297] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for waterfall_base_splash_blobs
        particleColors[298] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for boat_foam
        particleColors[299] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for boat_foam_small
        particleColors[300] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for slime_splash_01
        particleColors[301] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for slime_splash_01_droplets
        particleColors[302] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for slime_splash_01_reversed
        particleColors[303] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for slime_splash_01_surface
        particleColors[304] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for slime_splash_02
        particleColors[305] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for slime_splash_03
        particleColors[306] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_bubble_ambient
        particleColors[307] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_bubble_trail_1
        particleColors[308] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_foam_01
        particleColors[309] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_foam_01_d
        particleColors[310] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_foam_line_longb
        particleColors[311] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_foam_line_long
        particleColors[312] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_foam_line_shortb
        particleColors[313] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_foam_line_shortc
        particleColors[314] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_gunk_1
        particleColors[315] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_impact_bubbles_1
        particleColors[316] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_impact_bubbles_1b
        particleColors[317] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_impact_bubbles_1c
        particleColors[318] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01
        particleColors[319] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_blood
        particleColors[320] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_bubbles
        particleColors[321] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_column
        particleColors[322] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_droplet_trails
        particleColors[323] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_droplets_fallback
        particleColors[324] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_fallback
        particleColors[325] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_fish
        particleColors[326] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_foam
        particleColors[327] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_foam_delay
        particleColors[328] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_foam_surface
        particleColors[329] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_ripple_rings
        particleColors[330] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_ripple_rings_secondary
        particleColors[331] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_ripple_splash
        particleColors[332] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_sit
        particleColors[333] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_sit_gradual
        particleColors[334] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_splash
        particleColors[335] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_splash_secondary
        particleColors[336] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_splash_tall
        particleColors[337] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface2
        particleColors[338] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface3
        particleColors[339] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface3_dist
        particleColors[340] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface3_fallback
        particleColors[341] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface4
        particleColors[342] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface4_after
        particleColors[343] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_01_surface_delay
        particleColors[344] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02
        particleColors[345] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02_continuous
        particleColors[346] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02_droplets
        particleColors[347] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02_froth
        particleColors[348] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02_froth2
        particleColors[349] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02_surface
        particleColors[350] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_02_surface2
        particleColors[351] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_03
        particleColors[352] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_03_column
        particleColors[353] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_03_fallback
        particleColors[354] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_03_foam_surface_2nd
        particleColors[355] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_base_splash
        particleColors[356] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_column_wide
        particleColors[357] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_leakypipe_silo
        particleColors[358] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_leakypipe_silo_froth2
        particleColors[359] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_leakypipe_vertical
        particleColors[360] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_object_ripple_rings
        particleColors[361] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_object_splash
        particleColors[362] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_player_small
        particleColors[363] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_player_small_droplets
        particleColors[364] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_player_small_ripple
        particleColors[365] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);  // Blue for water_splash_player_small_surface
        particleColors[366] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // ash
        particleColors[367] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // ash_burning
        particleColors[368] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // ash_burning_fire
        particleColors[369] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // ash_eddy
        particleColors[370] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // ash_eddy_b
        particleColors[371] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // ash_outer
        particleColors[372] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain
        particleColors[373] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_edge
        particleColors[374] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_edge_ground
        particleColors[375] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_edge_sparse
        particleColors[376] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_edge_sparse_ground
        particleColors[377] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_edge_sparse_longdrop
        particleColors[378] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_impact_03
        particleColors[379] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_impact_04
        particleColors[380] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_impact_single
        particleColors[381] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_lamp_circle_drip
        particleColors[382] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_medium_area_ground
        particleColors[383] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_mist
        particleColors[384] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_mist_02
        particleColors[385] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_outer
        particleColors[386] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_outer_backup
        particleColors[387] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_outer_mist
        particleColors[388] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_puddle_ripples
        particleColors[389] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_puddle_ripples_large
        particleColors[390] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_puddle_ripples_small
        particleColors[391] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_sheet
        particleColors[392] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_sheet_mist
        particleColors[393] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_128
        particleColors[394] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_800
        particleColors[395] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_800_child
        particleColors[396] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_800_xx
        particleColors[397] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_drip
        particleColors[398] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_drip_ground
        particleColors[399] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_drip_test
        particleColors[400] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_impact_04
        particleColors[401] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_single_impact_04b
        particleColors[402] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_small_area_ground
        particleColors[403] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_small_edge
        particleColors[404] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_spot
        particleColors[405] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_storm
        particleColors[406] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_storm_impact_04
        particleColors[407] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_storm_leaves
        particleColors[408] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_storm_outer
        particleColors[409] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_stream_01_impact
        particleColors[410] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_stream_01_impact_2
        particleColors[411] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_stream_01_impact_3
        particleColors[412] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_stream_04_impact_1
        particleColors[413] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_stream_04_impact_2
        particleColors[414] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_volume
        particleColors[415] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // rain_volume_vanity
        particleColors[416] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // snow
        particleColors[417] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // snow_drift
        particleColors[418] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // snow_outer
        particleColors[419] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_cloud_lightning
        particleColors[420] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_cloud_parent
        particleColors[421] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_branch_01
        particleColors[422] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_branch_02
        particleColors[423] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_branch_03
        particleColors[424] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_branch_parent_01
        particleColors[425] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_branch_parent_02
        particleColors[426] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_branch_parent_03
        particleColors[427] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_cloud
        particleColors[428] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_glow
        particleColors[429] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_modglow
        particleColors[430] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_lightning_01_thin
        particleColors[431] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // storm_fx
        particleColors[432] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[433] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[434] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[435] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[436] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[437] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[438] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[439] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[440] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[441] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[442] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[443] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[444] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[445] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[446] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[447] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[448] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[449] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[450] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[451] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[452] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[453] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[454] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[455] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[456] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[457] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[458] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[459] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[460] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[461] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[462] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[463] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[464] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[465] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[466] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[467] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[468] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[469] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[470] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[471] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[472] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[473] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[474] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[475] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[476] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[477] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[478] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[479] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[480] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[481] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[482] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[483] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[484] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[485] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[486] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[487] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[488] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[489] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[490] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[491] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[492] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[493] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[494] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[495] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[496] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[497] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[498] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[499] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[500] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[501] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[502] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[503] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[504] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[505] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[506] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[507] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[508] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[509] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[510] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[511] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[512] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[513] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[514] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[515] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[516] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[517] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[518] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[519] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[520] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[521] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[522] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[523] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[524] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[525] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[526] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[527] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[528] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[529] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[530] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[531] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[532] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[533] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[534] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[535] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[536] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[537] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[538] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[539] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[540] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[541] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[542] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[543] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[544] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[545] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[546] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[547] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[548] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[549] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[550] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[551] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[552] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[553] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[554] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[555] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[556] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[557] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[558] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[559] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[560] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[561] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[562] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[563] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[564] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[565] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[566] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[567] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[568] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[569] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[570] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[571] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[572] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[573] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[574] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[575] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[576] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[577] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[578] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[579] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[580] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[581] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[582] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
        particleColors[583] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // bumpmine_active
        particleColors[584] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // bumpmine_active_glow
        particleColors[585] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // bumpmine_active_glow2
        particleColors[586] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // bumpmine_active_glow_outer
        particleColors[587] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // bumpmine_player_trail
        particleColors[588] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_a
        particleColors[589] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_a_omni
        particleColors[590] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_b
        particleColors[591] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_b_omni
        particleColors[592] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_balloons
        particleColors[593] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_c
        particleColors[594] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // confetti_c_omni
        particleColors[595] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // dronegun_laser_sight
        particleColors[596] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // dronegun_laser_sight_dot
        particleColors[597] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // dronegun_laser_sight_dot_0
        particleColors[598] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // mvp_tracer
        particleColors[599] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // mvp_tracer_here
        particleColors[600] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // snow_hit_player_screeneffect
        particleColors[601] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // snow_hit_player_screeneffect_b
        particleColors[602] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // snow_hit_player_screeneffect_bb
        particleColors[603] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // snowball_pile
        particleColors[604] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // snowball_pile_puddle
        particleColors[605] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // snowball_pile_snow
        particleColors[606] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti
        particleColors[607] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti_balloons
        particleColors[608] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti_cheap
        particleColors[609] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti_omni
        particleColors[610] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti_omni_cheap
        particleColors[611] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti_sparks
        particleColors[612] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_confetti_sparks_2
        particleColors[613] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_effect
        particleColors[614] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_effect_shot
        particleColors[615] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_embers_01
        particleColors[616] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_low_02
        particleColors[617] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_low_03
        particleColors[618] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_low_04
        particleColors[619] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_primary_01
        particleColors[620] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_decoy_ground_smoke_01
        particleColors[621] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // weapon_empty (neutral white)
        particleColors[622] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);  // weapon_grenade_pin (silver/metal)
        particleColors[623] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // weapon_incend_core_sparks
        particleColors[624] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // weapon_incend_held
        particleColors[625] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // weapon_incend_held_sparks
        particleColors[626] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // weapon_incend_throw
        particleColors[627] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_fp
        particleColors[628] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_fire2
        particleColors[629] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_fire3
        particleColors[630] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_fire3b
        particleColors[631] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_fire3d
        particleColors[632] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_fire_blue
        particleColors[633] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // weapon_molotov_flick_glow (yellow glow)
        particleColors[634] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_held
        particleColors[635] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_held_fps
        particleColors[636] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_throw
        particleColors[637] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_thrown_child1
        particleColors[638] = ImVec4(1.0f, 0.4f, 0.0f, 1.0f);  // weapon_molotov_thrown_child3
        particleColors[639] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // weapon_molotov_thrown_glow (yellow glow)
        particleColors[640] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rif
        particleColors[641] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_fallback
        particleColors[642] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_fp
        particleColors[643] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // weapon_muzzle_flash_assault_rifle_glow (yellow glow)
        particleColors[644] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_main
        particleColors[645] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_main_sub1
        particleColors[646] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_main_sub2
        particleColors[647] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_parent_silenc
        particleColors[648] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // weapon_muzzle_flash_assault_rifle_silenced_glow (yellow glow)
        particleColors[649] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_vent_flash
        particleColors[650] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_vent_fp
        particleColors[651] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_vent_tp_cheap
        particleColors[652] = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // weapon_muzzle_flash_assault_rifle_flame
        particleColors[653] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun
        particleColors[654] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun_fallback
        particleColors[655] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun_fp
        particleColors[656] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun_cheap
        particleColors[657] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun_glow
        particleColors[658] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun_main
        particleColors[659] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_autoshotgun_main2
        particleColors[660] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_awp_tracer
        particleColors[661] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_huntingrifle
        particleColors[662] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_huntingrifle_fp_glow
        particleColors[663] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_huntingrifle_glow_cheap
        particleColors[664] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_huntingrifle_main
        particleColors[665] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_huntingrifle_main2
        particleColors[666] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_huntingrifle_vent_fallback
        particleColors[667] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para
        particleColors[668] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_fallback
        particleColors[669] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_fallback2
        particleColors[670] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_flash
        particleColors[671] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_fp_cheap
        particleColors[672] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_glow
        particleColors[673] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_main
        particleColors[674] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_main2_fallback
        particleColors[675] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_tracers
        particleColors[676] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_para_vent
        particleColors[677] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_core
        particleColors[678] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_core2
        particleColors[679] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_elite
        particleColors[680] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_elite_fallback
        particleColors[681] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_fallback2
        particleColors[682] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_fp
        particleColors[683] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_main_cheap
        particleColors[684] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);  // weapon_muzzle_flash_pistol_main2
        particleColors[685] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[686] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[687] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[688] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[689] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[690] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[691] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[692] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[693] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[694] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[695] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[696] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[697] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[698] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[699] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[700] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[701] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[702] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[703] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[704] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[705] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[706] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[707] = ImVec4(1.0f, 0.9f, 0.2f, 1.0f);
        particleColors[708] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // smoke – gray
        particleColors[709] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[710] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[711] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[712] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[713] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);  // sparks – bright yellow-white
        particleColors[714] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[715] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[716] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[717] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[718] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[719] = ImVec4(0.0f, 0.8f, 1.0f, 1.0f);  // taser – electric blue
        particleColors[720] = ImVec4(0.0f, 0.8f, 1.0f, 1.0f);
        particleColors[721] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // smoke
        particleColors[722] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[723] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[724] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        particleColors[725] = ImVec4(1.0f, 0.9f, 0.6f, 1.0f);  // illumination – warm white

        // Sensorgren (beeps, sparks, detonate)
        particleColors[726] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // beep – green
        particleColors[727] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        particleColors[728] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // debris
        particleColors[729] = ImVec4(1.0f, 0.3f, 0.0f, 1.0f);  // detlight – red flash
        particleColors[730] = ImVec4(1.0f, 0.3f, 0.0f, 1.0f);
        particleColors[731] = ImVec4(1.0f, 0.3f, 0.0f, 1.0f);
        particleColors[732] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // detonate – orange
        particleColors[733] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);  // glowing – cyan
        particleColors[734] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // smoke
        particleColors[735] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);  // sparks
        particleColors[736] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[737] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[738] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        particleColors[739] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);  // wiggly – cyan

        // Shell casings (metallic)
        for (int i = 740; i <= 752; ++i)
            particleColors[i] = ImVec4(0.9f, 0.8f, 0.6f, 1.0f);  // brass/gold

        // Shell eject smoke
        for (int i = 753; i <= 762; ++i)
            particleColors[i] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // light gray smoke

        // Shell eject smoke (append more to existing)
        for (int i = 763; i <= 772; ++i)
            particleColors[i] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // light gray smoke

        // Snowball impacts/trails (white theme)
        for (int i = 773; i <= 780; ++i)
            particleColors[i] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // pure white

        // Taser effects (electric blue)
        for (int i = 781; i <= 785; ++i)
            particleColors[i] = ImVec4(0.0f, 0.8f, 1.0f, 1.0f);  // cyan-blue

        // Weapon tracers (bright yellow-white)
        for (int i = 786; i <= 799; ++i)
            particleColors[i] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);  // tracer glow

        for (int i = 800; i <= 809; ++i)
            particleColors[i] = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);  // tracer glow



        // Initialize groups
        particleGroups = {
            {"Weapon", {0,1,2,17,18,19,20,41,148}, false, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)},  // Added 148
            {"Fire", {3,37,39,45}, false, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)},
            {"Explosion", {4,5,6,7,8,9,10,11,12,13,14,15,16,46}, false, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)},
            {"Smoke", {23,24,25,26,27,28,29,30,31,32,33,38,40,42,43}, false, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)},
            // Subdivided Blood groups
            {"Blood Impact", {67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143}, false, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)},  // blood_impact_* basics
            {"Blood Headshot", {85,86,87,88,89,144}, false, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)},  // headshot specifics + blood_impact_headshot_01d
            {"Blood Goop", {78,79,80,81,82,83,84,145}, false, ImVec4(0.6f, 0.0f, 0.0f, 1.0f)},  // goop + gore_goop_generic
            {"Lights", {21,22,47,48}, false, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)},
            {"Beam", {44}, false, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)},
            {"Chicken", {}, false, ImVec4(1.0f, 0.8f, 0.0f, 1.0f)}
        };

        for (int i = 49; i <= 66; ++i) {
            particleGroups.back().indices.push_back(i);  // Chicken is last
        }

        particleGroups.push_back({ "Ambient", {149,150,151,152,153}, false, ImVec4(0.5f, 0.5f, 0.5f, 1.0f) });  // leaves, paper, dust_emote
        particleGroups.push_back({ "Glow", {154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181}, false, ImVec4(1.0f, 1.0f, 0.5f, 1.0f) });  // glow*, env_light_glow*
        particleGroups.push_back({ "Sun", {162,163,164,165,166,167,168,169,170,171,172,173,182}, false, ImVec4(1.0f, 0.8f, 0.0f, 1.0f) });  // env_sun*, sun_clear
        particleGroups.push_back({ "Water", {183,184,185,186,187,188,189}, false, ImVec4(0.0f, 0.5f, 1.0f, 1.0f) });  // water_drip*
        particleGroups.push_back({ "Light", {174,180,181}, false, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) });  // flared_light01, light_glow_01, omnilight_glow01
        particleGroups.push_back({ "Impacts", {229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250}, false, ImVec4(1.0f, 0.65f, 0.0f, 1.0f) });
        particleGroups[13].indices.insert(particleGroups[13].indices.end(), {
        251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365
            });
        particleGroups.push_back({ "Ash", {366,367,368,369,370,371}, false, ImVec4(0.5f, 0.3f, 0.1f, 1.0f) });  // Brownish-gray for ash
        particleGroups.push_back({ "Rain", {372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415}, false, ImVec4(0.6f, 0.6f, 0.8f, 1.0f) });  // Light blue-gray for rain
        particleGroups.push_back({ "Snow", {416,417,418,773,774,775,776,777,778,779,780}, false, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) });  // White for snow
        particleGroups.push_back({ "Storm", {419,420,421,422,423,424,425,426,427,428,429,430,431}, false, ImVec4(0.2f, 0.2f, 0.4f, 1.0f) });  // Dark blue-gray for storm
        particleGroups[2].indices.insert(particleGroups[2].indices.end(), { 434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,483,486,525 });

        // Append new fire_* particles to existing Fire group (assuming it's index 1 in particleGroups)
        particleGroups[1].indices.insert(particleGroups[1].indices.end(), { 461,462,463,464,465,466,481,482 });

        // Add new groups for the remaining particles
        particleGroups.push_back({ "Burning", {432,433, 908, 909, 910}, false, ImVec4(1.0f, 0.5f, 0.0f, 1.0f) });
        particleGroups.push_back({ "Extinguish", {455,456,457,458,459,460}, false, ImVec4(0.5f, 0.5f, 0.5f, 1.0f) });
        particleGroups.push_back({ "Firework", {467,468,469,470,471,472,473,474,475,476,477,478,479,480}, false, ImVec4(1.0f, 1.0f, 0.0f, 1.0f) });
        particleGroups.push_back({ "Incendiary", {484,485,487,488,489,490,491,492,493,494,495,496,497,498,499,500}, false, ImVec4(1.0f, 0.3f, 0.0f, 1.0f) });
        particleGroups.push_back({ "Incgrenade", {501,502}, false, ImVec4(1.0f, 0.3f, 0.0f, 1.0f) });
        particleGroups.push_back({ "Molotov", {503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,732,734}, false, ImVec4(1.0f, 0.4f, 0.0f, 1.0f) });
        particleGroups.push_back({ "Perf Test", {577,578,579,580,581}, false, ImVec4(0.5f, 0.5f, 0.5f, 1.0f) });
        particleGroups.push_back({ "Spark", {582, 911, 912, 913}, false, ImVec4(1.0f, 1.0f, 0.5f, 1.0f) });
        particleGroups[particleGroups.size() - 2].indices.insert(particleGroups[particleGroups.size() - 2].indices.end(), { 600,601,602,603,604,605 });

        // Add new groups for the remaining particles
        particleGroups.push_back({ "Bumpmine", {583,584,585,586,587}, false, ImVec4(0.0f, 1.0f, 0.0f, 1.0f) });  // Green for bumpmine effects
        particleGroups.push_back({ "Confetti", {588,589,590,591,592,593,594}, false, ImVec4(1.0f, 0.8f, 0.0f, 1.0f) });  // Yellow for confetti
        particleGroups.push_back({ "Dronegun", {595,596,597}, false, ImVec4(1.0f, 0.0f, 0.0f, 1.0f) });  // Red for laser sights
        particleGroups.push_back({ "MVP Tracer", {598,599}, false, ImVec4(0.0f, 1.0f, 1.0f, 1.0f) });  // Cyan for tracers
        particleGroups.push_back({ "Snow Hit", {600,601,602}, false, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) });  // White for snow hits (already appended above, but included for clarity)
        particleGroups.push_back({ "Snowball", {603,604,605}, false, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) });  // White for snowballs (already appended above, but included for clarity)
        particleGroups.push_back({ "Weapon Confetti", {606,607,608,609,610,611,612}, false, ImVec4(1.0f, 0.8f, 0.0f, 1.0f) });  // Yellow for weapon confetti
        particleGroups.push_back({ "Weapon Decoy", {613,614,615,616,617,618,619,620}, false, ImVec4(0.5f, 0.5f, 0.5f, 1.0f) });  // Gray for decoy effects
        // Append new weapon_incend_* particles to existing Incendiary group (adjust index; e.g., if it's the 4th added in fire update, use particleGroups[some_index])

        particleGroups[particleGroups.size() - 7].indices.insert(particleGroups[particleGroups.size() - 7].indices.end(), { 627,628,629,630,631,632,633,634,635,636,637,638,639 });

        // Add new groups for muzzle flashes and misc
        particleGroups.push_back({ "Weapon Muzzle Flash", {640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,
            700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,
            715,716,717,718,719,720,721,722,723,724,725,781,782,783,784,785}, false, ImVec4(1.0f, 0.8f, 0.2f, 1.0f) });
        particleGroups.push_back({ "Weapon Misc", {621,622}, false, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) });
        particleGroups.push_back({ "Sensorgren", {
        726,727,728,729,730,731,732,733,734,735,736,737,738,739
        }, false, ImVec4(0.0f, 1.0f, 1.0f, 1.0f) });  // Cyan theme

        particleGroups.push_back({ "Shell Casings", {
            740,741,742,743,744,745,746,747,748,749,750,751,752
        }, false, ImVec4(0.9f, 0.8f, 0.3f, 1.0f) });  // Brass gold

        particleGroups.push_back({ "Shell Eject Smoke", {
            753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772
        }, false, ImVec4(0.7f, 0.7f, 0.7f, 1.0f) });  // Light gray
        particleGroups.push_back({ "Weapon Tracers", {
        786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809
        }, false, ImVec4(1.0f, 1.0f, 0.5f, 1.0f) });  // Bright tracer yellow
        // Optionally, add a new ParticleGroup for these muzzle flashes
        particleGroups.push_back({
            "Misc (Muzzle Flash)",
            {}, // indices will be from 810 to 851
            false,
            ImVec4(1.0f, 0.8f, 0.2f, 1.0f) // Yellow-orange for flashes
            });
        for (int i = 810; i < 852; ++i) {
            particleGroups.back().indices.push_back(i);
        }
        for (int i = 852; i < 891; ++i) {
            particleGroups.back().indices.push_back(i);
        }
        for (int i = 891; i < 907; ++i) {
            particleGroups.back().indices.push_back(i);
        }
        for (int i = 914; i < 969; ++i) {
            particleGroups.back().indices.push_back(i);
        }

    }
};

ParticleInit particleInit;



bool noSpreadEnabled = false;  // Default off


bool lightModEnabled = false;
ImVec4 lightModColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);