#include "cheat.h"

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
PresentFn oPresent = nullptr;
WndProcFn oWndProc = nullptr;
CreateMoveFn oCreateMove = nullptr;

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

        // Load navigation font (for sidebar only)
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
        static const ImWchar iconRanges[] = { 0xf000, 0xf3ff, 0 }; // FontAwesome range
        iconFont = io.Fonts->AddFontFromFileTTF("C:\\Users\\dionr\\Downloads\\Font-Awesome-7.x\\Font-Awesome-7.x\\otfs\\Font Awesome 7 Free-Solid-900.otf", 16.0f, &iconConfig, iconRanges); // Assume FontAwesome TTF path
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
        io.WantCaptureKeyboard = true;
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
                    debugLog += "[DEBUG] SDL mouse mode (relative/grab/cursor) set to " + std::string(menuOpen ? "menu-only" : "restored") + "\n";
                }
                else {
                    debugLog += "[ERROR] Failed to get SDL grabbed window\n";
                }
            }
            else if (mouseDisableInputOffset) {
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
            CreateThread(NULL, 0, KeyThread, NULL, 0, NULL);
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