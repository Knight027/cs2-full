#include "cheat.h"
#include "fontawesome.h"

OverrideViewFn oOverrideView = nullptr;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dContext = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
PresentFn oPresent = nullptr;
WndProcFn oWndProc = nullptr;
CreateMoveFn oCreateMove = nullptr;
// SDL function pointers - ADD THESE DEFINITIONS
SDL_GetGrabbedWindowFn SDL_GetGrabbedWindow = nullptr;
SDL_GetWindowRelativeMouseModeFn SDL_GetWindowRelativeMouseMode = nullptr;
SDL_SetWindowRelativeMouseModeFn SDL_SetWindowRelativeMouseMode = nullptr;
SDL_SetWindowGrabFn SDL_SetWindowGrab = nullptr;
SDL_GetWindowGrabFn SDL_GetWindowGrab = nullptr;
SDL_ShowCursorFn SDL_ShowCursor = nullptr;
RenderParticleFn oRenderParticle = nullptr;
LightSceneObjectFn oLightSceneObject = nullptr;
GetInaccuracyFn oGetInaccuracy = nullptr;
GetSpreadFn oGetSpread = nullptr;
UpdateAccuracyPenaltyFn oUpdateAccuracyPenalty = nullptr;
CreateSubtickMoveStepFn oCreateSubtickMoveStep = nullptr;
RenderLegsFn oRenderLegs = nullptr;
FrameStageNotifyFn oFrameStageNotify = nullptr;

// For saving/restoring mouse mode
SDL_bool savedRelativeMouseMode = SDL_FALSE;
SDL_bool savedGrabMode = SDL_FALSE;





bool SetupRenderLegsHook() {
    if (!renderLegsAddr) {
        debugLog += "[ERROR] RenderLegs address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)renderLegsAddr, &HK_RenderLegs, (LPVOID*)&oRenderLegs) != MH_OK) {
        debugLog += "[ERROR] Failed to create RenderLegs hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)renderLegsAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable RenderLegs hook\n";
        return false;
    }

    debugLog += "[DEBUG] RenderLegs hook set up successfully\n";
    return true;
}


bool SetupFrameStageNotifyHook() {
    // The signature you provided
    const char* pattern = "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B F9 8B DA";

    uintptr_t address = PatternScan("client.dll", pattern);

    if (address) {
        if (MH_CreateHook((LPVOID)address, &HK_FrameStageNotify, (LPVOID*)&oFrameStageNotify) != MH_OK) {
            debugLog += "[ERROR] Failed to create FSN hook\n";
            return false;
        }
        if (MH_EnableHook((LPVOID)address) != MH_OK) {
            debugLog += "[ERROR] Failed to enable FSN hook\n";
            return false;
        }
        debugLog += "[DEBUG] FrameStageNotify hook active\n";
        return true;
    }
    else {
        debugLog += "[ERROR] FSN Pattern not found\n";
        return false;
    }
}


bool SetupNoSpreadHooks() {
    if (!getInaccuracyAddr || !getSpreadAddr) {
        debugLog += "[ERROR] NoSpread addresses not found\n";
        return false;
    }

    // Hook GetInaccuracy
    if (MH_CreateHook((LPVOID)getInaccuracyAddr, &HK_GetInaccuracy, (LPVOID*)&oGetInaccuracy) != MH_OK) {
        debugLog += "[ERROR] Failed to create GetInaccuracy hook\n";
        return false;
    }
    if (MH_EnableHook((LPVOID)getInaccuracyAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable GetInaccuracy hook\n";
        return false;
    }

    // Hook GetSpread
    if (MH_CreateHook((LPVOID)getSpreadAddr, &HK_GetSpread, (LPVOID*)&oGetSpread) != MH_OK) {
        debugLog += "[ERROR] Failed to create GetSpread hook\n";
        return false;
    }
    if (MH_EnableHook((LPVOID)getSpreadAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable GetSpread hook\n";
        return false;
    }
    if (MH_CreateHook((LPVOID)updateAccuracyPenaltyAddr, &HK_UpdateAccuracyPenalty, (LPVOID*)&oUpdateAccuracyPenalty) != MH_OK) {
        debugLog += "[ERROR] Failed to create UpdateAccuracyPenalty hook\n";
        return false;
    }
    if (MH_EnableHook((LPVOID)updateAccuracyPenaltyAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable UpdateAccuracyPenalty hook\n";
        return false;
    }

    debugLog += "[DEBUG] NoSpread hooks set up successfully\n";
    return true;
}

bool SetupUpdateSkyboxHook() {
    if (!updateSkyboxAddr) {
        debugLog += "[ERROR] UpdateSkybox address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)updateSkyboxAddr, &HK_UpdateSkybox, (LPVOID*)&oUpdateSkybox) != MH_OK) {
        debugLog += "[ERROR] Failed to create UpdateSkybox hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)updateSkyboxAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable UpdateSkybox hook\n";
        return false;
    }

    debugLog += "[DEBUG] UpdateSkybox hook set up successfully\n";
    return true;
}

bool SetupSubtickHook() {
    if (!createSubtickMoveStepAddr) {
        debugLog += "[ERROR] CreateSubtickMoveStep address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)createSubtickMoveStepAddr, &HK_CreateSubtickMoveStep, (LPVOID*)&oCreateSubtickMoveStep) != MH_OK) {
        debugLog += "[ERROR] Failed to create CreateSubtickMoveStep hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)createSubtickMoveStepAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable CreateSubtickMoveStep hook\n";
        return false;
    }

    debugLog += "[DEBUG] CreateSubtickMoveStep hook set up successfully\n";
    return true;
}


bool SetupRenderParticleHook() {
    if (!renderParticleAddr) {
        debugLog += "[ERROR] RenderParticle address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)renderParticleAddr, &NewRenderParticle, (LPVOID*)&oRenderParticle) != MH_OK) {
        debugLog += "[ERROR] Failed to create RenderParticle hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)renderParticleAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable RenderParticle hook\n";
        return false;
    }

    debugLog += "[DEBUG] RenderParticle hook set up successfully\n";
    return true;
}


bool SetupGetFOVHook() {
    if (!getFOVAddr) {
        debugLog += "[ERROR] GetFOV address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)getFOVAddr, &HK_GetFOV, (LPVOID*)&oGetFOV) != MH_OK) {
        debugLog += "[ERROR] Failed to create GetFOV hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)getFOVAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable GetFOV hook\n";
        return false;
    }

    debugLog += "[DEBUG] GetFOV hook set up successfully\n";
    return true;
}


void* __fastcall HK_LightSceneObject(void* rcx, void* rdx, void* r8) {  // <- void* return!
    static auto original = reinterpret_cast<LightSceneObjectFn>(oLightSceneObject);
    auto result = original(rcx, rdx, r8);

    if (lightModEnabled) {
        auto object = reinterpret_cast<C_SceneLightObject*>(rdx);
        if (IsValidPtr(object)) {
            object->r = lightModColor.x;
            object->g = lightModColor.y;
            object->b = lightModColor.z;
        }
    }

    return result;  // Now valid: returns void*
}

bool SetupLightSceneHook() {
    if (!lightSceneObjectAddr) {
        debugLog += "[ERROR] LightSceneObject address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)lightSceneObjectAddr, &HK_LightSceneObject, (LPVOID*)&oLightSceneObject) != MH_OK) {
        debugLog += "[ERROR] Failed to create LightSceneObject hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)lightSceneObjectAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable LightSceneObject hook\n";
        return false;
    }

    debugLog += "[DEBUG] LightSceneObject hook set up successfully\n";
    std::cout << "[DEBUG] LightSceneObject hook set up successfully" << std::endl;
    return true;
}



bool SetupOverrideViewHook() {
    if (!overrideViewAddr) {
        debugLog += "[ERROR] OverrideView address not found\n";
        return false;
    }

    if (MH_CreateHook((LPVOID)overrideViewAddr, &HK_OverrideView, (LPVOID*)&oOverrideView) != MH_OK) {
        debugLog += "[ERROR] Failed to create OverrideView hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)overrideViewAddr) != MH_OK) {
        debugLog += "[ERROR] Failed to enable OverrideView hook\n";
        return false;
    }

    debugLog += "[DEBUG] OverrideView hook set up successfully\n";
    return true;
}


// Add CreateMove hook setup
bool SetupCreateMoveHook() {
    std::cout << "[DEBUG] Setting up CreateMove hook" << std::endl;

    if (!createMoveAddr) {
        std::cout << "[ERROR] CreateMove address not found" << std::endl;
        debugLog += "[ERROR] CreateMove address not found\n";
        return false;
    }

    std::cout << "[DEBUG] CreateMove address: 0x" << std::hex << createMoveAddr << std::endl;

    if (MH_CreateHook((LPVOID)createMoveAddr, &HK_CreateMove, (LPVOID*)&oCreateMove) != MH_OK) {
        std::cout << "[ERROR] Failed to create CreateMove hook" << std::endl;
        debugLog += "[ERROR] Failed to create CreateMove hook\n";
        return false;
    }

    if (MH_EnableHook((LPVOID)createMoveAddr) != MH_OK) {
        std::cout << "[ERROR] Failed to enable CreateMove hook" << std::endl;
        debugLog += "[ERROR] Failed to enable CreateMove hook\n";
        return false;
    }

    std::cout << "[DEBUG] CreateMove hook set up successfully at 0x" << std::hex << createMoveAddr << std::endl;
    debugLog += "[DEBUG] CreateMove hook set up successfully\n";
    return true;
}

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);  // Always pass to ImGui
    if (menuOpen) {
        return TRUE;  // Block all from game
    }
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool init = false;
    if (!init) {
        std::cout << "[DEBUG] Initializing ImGui and DX11" << std::endl;
        pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice);
        g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
        g_hwnd = sd.OutputWindow;
        ID3D11Texture2D* pBackBuffer;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
        if (!g_pd3dDevice) {
            pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice);
            g_pd3dDevice->GetImmediateContext(&g_pd3dContext);
        }
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // --- NEW: Load fonts ---
        ImGuiIO& io = ImGui::GetIO();
        // Load default font (for the rest of the menu)
        defaultFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);  // Example: Arial at 16px
        if (defaultFont == nullptr) {
            std::cout << "[ERROR] Failed to load default font! Using ImGui default." << std::endl;
            debugLog += "[ERROR] Failed to load default font! Using ImGui default.\n";
            defaultFont = io.Fonts->AddFontDefault();  // Fallback to built-in
        }
        else {
            io.FontDefault = defaultFont;  // Set as global default
            std::cout << "[DEBUG] Default font loaded successfully." << std::endl;
            debugLog += "[DEBUG] Default font loaded successfully.\n";
        }

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;           // <--- This merges icons into the previous font (Arial)
        icons_config.PixelSnapH = true;
        icons_config.FontDataOwnedByAtlas = false; // <--- Essential for header-based fonts so ImGui doesn't try to free const memory

        // 3. LOAD FONT AWESOME FROM MEMORY
        // Ensure 'fontAwesome' is the name of the byte array in your header
        iconFont = io.Fonts->AddFontFromMemoryTTF((void*)fontAwesome, sizeof(fontAwesome), 16.0f, &icons_config, icons_ranges);

        if (iconFont == nullptr) {
            std::cout << "[ERROR] Failed to load FontAwesome from memory!" << std::endl;
        }
        else {
            std::cout << "[DEBUG] FontAwesome loaded successfully." << std::endl;
        }

        // Load navigation font (for sidebar only) don't need it though
        navFont = io.Fonts->AddFontFromFileTTF("C:\\Users\\dionr\\Downloads\\race-sport\\Race Sport.ttf", 18.0f);  // Example: Consolas at 14px
        if (navFont == nullptr) {
            std::cout << "[ERROR] Failed to load navigation font! Using default font." << std::endl;
            debugLog += "[ERROR] Failed to load navigation font! Using default font.\n";
            navFont = defaultFont;  // Fallback to default
        }
        else {
            std::cout << "[DEBUG] Navigation font loaded successfully." << std::endl;
            debugLog += "[DEBUG] Navigation font loaded successfully.\n";
        }

        // Load icon font (e.g., FontAwesome for icons)
        ImFontConfig iconConfig;
        iconConfig.MergeMode = true; // Merge with previous font 
        static const ImWchar iconRanges[] = { 0x0000, 0xffff, 0 }; // FontAwesome range
        iconFont = io.Fonts->AddFontFromFileTTF("C:\\Users\\dionr\\Downloads\\fontawesome-free-7.1.0-desktop\\fontawesome-free-7.1.0-desktop\\otfs\\Font Awesome 7 Free-Solid-900.otf", 16.0f, &iconConfig, iconRanges);
        if (iconFont == nullptr) {
            std::cout << "[ERROR] Failed to load icon font! Icons may not display." << std::endl;
            debugLog += "[ERROR] Failed to load icon font! Icons may not display.\n";
        }
        else {
            std::cout << "[DEBUG] Icon font loaded successfully." << std::endl;
            debugLog += "[DEBUG] Icon font loaded successfully.\n";
        }

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_hwnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
        oWndProc = (WndProcFn)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);
        init = true;
        std::cout << "[DEBUG] ImGui and DX11 initialized" << std::endl;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (menuOpen) {
        ImGuiIO& io = ImGui::GetIO();
        io.WantCaptureMouse = true;
        io.MouseDrawCursor = true;  // Draw ImGui's cursor when menu is open
    }
    else {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDrawCursor = false;  // Hide ImGui cursor when menu closed
    }

    RenderCleanMenu();
    RenderESP();
    RenderNotifications(); // New: Render notifications

    ImGui::EndFrame();
    ImGui::Render();
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, SyncInterval, Flags);
}

bool SetupHook() {
    std::cout << "[DEBUG] Setting up DX11 Present hook" << std::endl;
    if (MH_Initialize() != MH_OK) {
        std::cout << "[ERROR] MH_Initialize failed" << std::endl;
        return false;
    }

    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = "DummyClass";
    RegisterClassA(&wc);
    HWND dummyWnd = CreateWindowA("DummyClass", "Dummy", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, wc.hInstance, NULL);
    if (!dummyWnd) {
        std::cout << "[ERROR] Failed to create dummy window" << std::endl;
        return false;
    }

    DXGI_SWAP_CHAIN_DESC sd = { 0 };
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = dummyWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (FAILED(hr)) {
        std::cout << "[ERROR] Failed to create dummy DX11" << std::endl;
        DestroyWindow(dummyWnd);
        return false;
    }

    void** pVTable = *(void***)g_pSwapChain;
    void* target = pVTable[8];

    if (MH_CreateHook(target, &hkPresent, (LPVOID*)&oPresent) != MH_OK) {
        std::cout << "[ERROR] MH_CreateHook failed" << std::endl;
        g_pSwapChain->Release();
        g_pd3dDevice->Release();
        g_pd3dDeviceContext->Release();
        DestroyWindow(dummyWnd);
        return false;
    }
    if (MH_EnableHook(target) != MH_OK) {
        std::cout << "[ERROR] MH_EnableHook failed" << std::endl;
        g_pSwapChain->Release();
        g_pd3dDevice->Release();
        g_pd3dDeviceContext->Release();
        DestroyWindow(dummyWnd);
        return false;
    }

    g_pSwapChain->Release();
    g_pd3dDevice->Release();
    g_pd3dDeviceContext->Release();
    DestroyWindow(dummyWnd);
    UnregisterClassA("DummyClass", wc.hInstance);

    std::cout << "[DEBUG] Present hook set up successfully" << std::endl;
    return true;
}

DWORD WINAPI KeyThread(LPVOID param) {
    while (true) {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            menuOpen = !menuOpen;
            debugLog += "[INFO] Menu toggled: " + std::string(menuOpen ? "Open" : "Closed") + "\n";
            Sleep(250);

            if (SDL_SetWindowRelativeMouseMode && SDL_GetWindowRelativeMouseMode && SDL_GetGrabbedWindow &&
                SDL_SetWindowGrab && SDL_GetWindowGrab && SDL_ShowCursor) {
                SDL_Window* window = SDL_GetGrabbedWindow();
                if (window) {
                    if (menuOpen) {
                        savedRelativeMouseMode = SDL_GetWindowRelativeMouseMode(window);
                        savedGrabMode = SDL_GetWindowGrab(window);
                        SDL_SetWindowRelativeMouseMode(window, SDL_FALSE);  // Disable relative mode
                        SDL_SetWindowGrab(window, SDL_FALSE);  // Release window grab
                        SDL_ShowCursor(SDL_ENABLE);  // Show cursor
                    }
                    else {
                        SDL_SetWindowRelativeMouseMode(window, savedRelativeMouseMode);  // Restore relative
                        SDL_SetWindowGrab(window, savedGrabMode);  // Restore grab
                        SDL_ShowCursor(SDL_DISABLE);  // Hide cursor
                    }
                }
                else {
                    debugLog += "[ERROR] Failed to get SDL grabbed window\n";
                }
            }
            if (mouseDisableInputOffset) {
                // Fallback ConVar (unchanged)
                bool disable = menuOpen ? true : false;
                WriteMemory<bool>(clientBase + mouseDisableInputOffset, disable);
                debugLog += "[DEBUG] mouse_disableinput ConVar set to " + std::to_string(disable) + "\n";
            }
        }
        Sleep(20);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        AllocConsole();
        FILE* dummy;
        if (freopen_s(&dummy, "CONOUT$", "w", stdout) != 0) {
            MessageBoxA(nullptr, "Failed to redirect stdout in DLL", "DLL Error", MB_OK | MB_ICONERROR);
            return FALSE;
        }
        std::cout << "[DEBUG] cs2WoW.dll DllMain called, hModule=0x" << std::hex << (uintptr_t)hModule << std::endl;
        try {
            if (!LoadOffsets()) {
                std::cout << "[ERROR] LoadOffsets failed in DllMain" << std::endl;
                return FALSE;
            }
            if (!SetupHook()) {
                std::cout << "[ERROR] SetupHook failed in DllMain" << std::endl;
                return FALSE;
            }
            if (!SetupCreateMoveHook()) {
                std::cout << "[WARNING] CreateMove hook failed, FOV changer will not work" << std::endl;
            }
            if (!SetupOverrideViewHook()) {
                std::cout << "[WARNING] OverrideView hook failed, third-person won't work" << std::endl;
            }
            if (!SetupGetFOVHook()) {
                std::cout << "[WARNING] GetFOV hook failed, FOV Changer V2 will not work" << std::endl;
            }
            if (!SetupRenderParticleHook()) {
                std::cout << "[WARNING] RenderParticle hook failed, particle modulation will not work" << std::endl;
            }
            if (!SetupLightSceneHook()) {
                std::cout << "[WARNING] LightSceneObject hook failed, light modulation will not work" << std::endl;
            }
            if (!SetupNoSpreadHooks()) {
                std::cout << "[WARNING] NoSpread hooks failed, feature will not work" << std::endl;
            }
            if (!SetupUpdateSkyboxHook()) {
                std::cout << "[WARNING] UpdateSkybox hook failed, sky modulation will not work" << std::endl;
            }
            if (!SetupRenderLegsHook()) {
                std::cout << "[WARNING] RenderLegs hook failed, no legs feature will not work" << std::endl;
            }
            if (!SetupSubtickHook()) {
                std::cout << "[WARNING] CreateSubtickMoveStep hook failed, de-subtick will not work" << std::endl;
            }
            if (!SetupPopupAcceptMatchHook()) {
                std::cout << "[WARNING] PopupAcceptMatchFound hook failed, auto accept will not work" << std::endl;
            }

            CreateThread(NULL, 0, KeyThread, NULL, 0, NULL);
            HANDLE bhopThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunBhopThread, NULL, 0, NULL);
            if (bhopThread) {
                SetThreadPriority(bhopThread, THREAD_PRIORITY_HIGHEST);
                bhopThreadRunning = true;
            }
            else {
                debugLog += "[ERROR] Failed to create bhop thread\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "[ERROR] Exception in DllMain: " << e.what() << std::endl;
            return FALSE;
        }
    }
    case DLL_PROCESS_DETACH:
        std::cout << "[DEBUG] cs2WoW.dll detaching" << std::endl;
        break;
    }
    return TRUE;
}