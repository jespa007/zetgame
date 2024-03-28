VideoBootStrap WINDOWS_bootstrap = {
    "windows", "SDL Windows video driver", WIN_CreateDevice
};

static void WIN_CleanRegisterApp(WNDCLASSEX wcex)
{
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    if (wcex.hIcon) DestroyIcon(wcex.hIcon);
    if (wcex.hIconSm) DestroyIcon(wcex.hIconSm);
#endif
    SDL_free(SDL_Appname);
    SDL_Appname = NULL;

    // TODO : Call unregisterclass
    /*BOOL UnregisterClassA(
      [in]           LPCSTR    lpClassName,
      [in, optional] HINSTANCE hInstance
    );*/
}

int
SDL_RegisterApp(const char *name, Uint32 style, void *hInst)
{
    WNDCLASSEX wcex;
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    const char *hint;
    TCHAR path[MAX_PATH];
#endif

    /* Only do this once... */
    if (app_registered) {
        ++app_registered;
        return (0);
    }
    SDL_assert(SDL_Appname == NULL);
    if (!name) {
        name = "SDL_app";
#if defined(CS_BYTEALIGNCLIENT) || defined(CS_OWNDC)
        style = (CS_BYTEALIGNCLIENT | CS_OWNDC);
#endif
    }
    SDL_Appname = WIN_UTF8ToString(name);
    SDL_Appstyle = style;
    SDL_Instance = hInst ? hInst : GetModuleHandle(NULL);

    /* Register the application class */
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.hCursor        = NULL;
    wcex.hIcon          = NULL;
    wcex.hIconSm        = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = SDL_Appname;
    wcex.style          = SDL_Appstyle;
    wcex.hbrBackground  = NULL;
    wcex.lpfnWndProc    = WIN_WindowProc;
    wcex.hInstance      = SDL_Instance;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    hint = SDL_GetHint(SDL_HINT_WINDOWS_INTRESOURCE_ICON);
    if (hint && *hint) {
        wcex.hIcon = LoadIcon(SDL_Instance, MAKEINTRESOURCE(SDL_atoi(hint)));

        hint = SDL_GetHint(SDL_HINT_WINDOWS_INTRESOURCE_ICON_SMALL);
        if (hint && *hint) {
            wcex.hIconSm = LoadIcon(SDL_Instance, MAKEINTRESOURCE(SDL_atoi(hint)));
        }
    } else {
        /* Use the first icon as a default icon, like in the Explorer */
        GetModuleFileName(SDL_Instance, path, MAX_PATH);
        ExtractIconEx(path, 0, &wcex.hIcon, &wcex.hIconSm, 1);
    }
#endif /*!defined(__XBOXONE__) && !defined(__XBOXSERIES__)*/

    if (!RegisterClassEx(&wcex)) {
        WIN_CleanRegisterApp(wcex);
        return SDL_SetError("Couldn't register application class");
    }

    app_registered = 1;
    return 0;
}

static ZG_VideoDevice *
ZG_VideoDevice_WIN_Create(void)
{
    SDL_VideoDevice *device;
    SDL_VideoData *data;

    SDL_RegisterApp(NULL, 0, NULL);

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (device) {
        data = (struct SDL_VideoData *) SDL_calloc(1, sizeof(SDL_VideoData));
    } else {
        data = NULL;
    }
    if (!data) {
        SDL_free(device);
        SDL_OutOfMemory();
        return NULL;
    }
    device->driverdata = data;
    device->wakeup_lock = SDL_CreateMutex();

    /* Set the function pointers */
    device->VideoInit = WIN_VideoInit;
    device->VideoQuit = WIN_VideoQuit;
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    device->GetDisplayBounds = WIN_GetDisplayBounds;
    device->GetDisplayUsableBounds = WIN_GetDisplayUsableBounds;
    device->GetDisplayDPI = WIN_GetDisplayDPI;
    device->GetDisplayModes = WIN_GetDisplayModes;
    device->SetDisplayMode = WIN_SetDisplayMode;
#endif
    device->PumpEvents = WIN_PumpEvents;
    device->WaitEventTimeout = WIN_WaitEventTimeout;
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    device->SendWakeupEvent = WIN_SendWakeupEvent;
    device->SuspendScreenSaver = WIN_SuspendScreenSaver;
#endif

    device->CreateSDLWindow = WIN_CreateWindow;
    device->CreateSDLWindowFrom = WIN_CreateWindowFrom;
    device->SetWindowTitle = WIN_SetWindowTitle;
    device->SetWindowIcon = WIN_SetWindowIcon;
    device->SetWindowPosition = WIN_SetWindowPosition;
    device->SetWindowSize = WIN_SetWindowSize;
    device->GetWindowBordersSize = WIN_GetWindowBordersSize;
    device->SetWindowOpacity = WIN_SetWindowOpacity;
    device->ShowWindow = WIN_ShowWindow;
    device->HideWindow = WIN_HideWindow;
    device->RaiseWindow = WIN_RaiseWindow;
    device->MaximizeWindow = WIN_MaximizeWindow;
    device->MinimizeWindow = WIN_MinimizeWindow;
    device->RestoreWindow = WIN_RestoreWindow;
    device->SetWindowBordered = WIN_SetWindowBordered;
    device->SetWindowResizable = WIN_SetWindowResizable;
    device->SetWindowAlwaysOnTop = WIN_SetWindowAlwaysOnTop;
    device->SetWindowFullscreen = WIN_SetWindowFullscreen;
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    device->SetWindowGammaRamp = WIN_SetWindowGammaRamp;
    device->GetWindowICCProfile = WIN_GetWindowICCProfile;
    device->GetWindowGammaRamp = WIN_GetWindowGammaRamp;
    device->SetWindowMouseRect = WIN_SetWindowMouseRect;
    device->SetWindowMouseGrab = WIN_SetWindowMouseGrab;
    device->SetWindowKeyboardGrab = WIN_SetWindowKeyboardGrab;
#endif
    device->DestroyWindow = WIN_DestroyWindow;
    device->GetWindowWMInfo = WIN_GetWindowWMInfo;
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    device->CreateWindowFramebuffer = WIN_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = WIN_UpdateWindowFramebuffer;
    device->DestroyWindowFramebuffer = WIN_DestroyWindowFramebuffer;
    device->OnWindowEnter = WIN_OnWindowEnter;
    device->SetWindowHitTest = WIN_SetWindowHitTest;
    device->AcceptDragAndDrop = WIN_AcceptDragAndDrop;
    device->FlashWindow = WIN_FlashWindow;

    device->shape_driver.CreateShaper = Win32_CreateShaper;
    device->shape_driver.SetWindowShape = Win32_SetWindowShape;
    device->shape_driver.ResizeWindowShape = Win32_ResizeWindowShape;
#endif

#if SDL_VIDEO_OPENGL_WGL
    device->GL_LoadLibrary = WIN_GL_LoadLibrary;
    device->GL_GetProcAddress = WIN_GL_GetProcAddress;
    device->GL_UnloadLibrary = WIN_GL_UnloadLibrary;
    device->GL_CreateContext = WIN_GL_CreateContext;
    device->GL_MakeCurrent = WIN_GL_MakeCurrent;
    device->GL_GetDrawableSize = WIN_GL_GetDrawableSize;
    device->GL_SetSwapInterval = WIN_GL_SetSwapInterval;
    device->GL_GetSwapInterval = WIN_GL_GetSwapInterval;
    device->GL_SwapWindow = WIN_GL_SwapWindow;
    device->GL_DeleteContext = WIN_GL_DeleteContext;
#elif SDL_VIDEO_OPENGL_EGL
    /* Use EGL based functions */
    device->GL_LoadLibrary = WIN_GLES_LoadLibrary;
    device->GL_GetProcAddress = WIN_GLES_GetProcAddress;
    device->GL_UnloadLibrary = WIN_GLES_UnloadLibrary;
    device->GL_CreateContext = WIN_GLES_CreateContext;
    device->GL_MakeCurrent = WIN_GLES_MakeCurrent;
    device->GL_GetDrawableSize = WIN_GLES_GetDrawableSize;
    device->GL_SetSwapInterval = WIN_GLES_SetSwapInterval;
    device->GL_GetSwapInterval = WIN_GLES_GetSwapInterval;
    device->GL_SwapWindow = WIN_GLES_SwapWindow;
    device->GL_DeleteContext = WIN_GLES_DeleteContext;
#endif
#if SDL_VIDEO_VULKAN
    device->Vulkan_LoadLibrary = WIN_Vulkan_LoadLibrary;
    device->Vulkan_UnloadLibrary = WIN_Vulkan_UnloadLibrary;
    device->Vulkan_GetInstanceExtensions = WIN_Vulkan_GetInstanceExtensions;
    device->Vulkan_CreateSurface = WIN_Vulkan_CreateSurface;
    device->Vulkan_GetDrawableSize = WIN_GL_GetDrawableSize;
#endif

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    device->StartTextInput = WIN_StartTextInput;
    device->StopTextInput = WIN_StopTextInput;
    device->SetTextInputRect = WIN_SetTextInputRect;
    device->ClearComposition = WIN_ClearComposition;
    device->IsTextInputShown = WIN_IsTextInputShown;

    device->SetClipboardText = WIN_SetClipboardText;
    device->GetClipboardText = WIN_GetClipboardText;
    device->HasClipboardText = WIN_HasClipboardText;
#endif

    device->free = WIN_DeleteDevice;

    return device;
}

static BOOL
WIN_DeclareDPIAwarePerMonitor(_THIS)
{
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;

    if (data->SetProcessDpiAwarenessContext) {
        /* Windows 10, version 1607 */
        return data->SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    } else if (data->SetProcessDpiAwareness) {
        /* Windows 8.1 */
        return SUCCEEDED(data->SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE));
    } else {
        /* Older OS: fall back to system DPI aware */
        return WIN_DeclareDPIAwareSystem(_this);
    }
#endif
    return FALSE;
}


static BOOL
WIN_DeclareDPIAwarePerMonitorV2(_THIS)
{
#if defined(__XBOXONE__) || defined(__XBOXSERIES__)
    return FALSE;
#else
    SDL_VideoData* data = (SDL_VideoData*)_this->driverdata;

    /* Declare DPI aware (may have been done in external code or a manifest, as well) */
    if (data->SetProcessDpiAwarenessContext) {
        /* Windows 10, version 1607 */

        /* NOTE: SetThreadDpiAwarenessContext doesn't work here with OpenGL - the OpenGL contents
           end up still getting OS scaled. (tested on Windows 10 21H1 19043.1348, NVIDIA 496.49)

           NOTE: Enabling DPI awareness through Windows Explorer
           (right click .exe -> Properties -> Compatibility -> High DPI Settings ->
           check "Override high DPI Scaling behaviour", select Application) gives
           a DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE context (at least on Windows 10 21H1), and
           setting DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 will fail.

           NOTE: Entering exclusive fullscreen in a DPI_AWARENESS_CONTEXT_UNAWARE process
           appears to cause Windows to change the .exe manifest to DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE
           on future launches. This means attempting to use DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
           will fail in the future until you manually clear the "Override high DPI Scaling behaviour"
           setting in Windows Explorer (tested on Windows 10 21H2).
         */
        if (data->SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
            return TRUE;
        } else {
            return WIN_DeclareDPIAwarePerMonitor(_this);
        }
    } else {
        /* Older OS: fall back to per-monitor (or system) */
        return WIN_DeclareDPIAwarePerMonitor(_this);
    }
#endif
}

static BOOL
WIN_DeclareDPIAwareSystem(_THIS)
{
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    SDL_VideoData* data = (SDL_VideoData*)_this->driverdata;

    if (data->SetProcessDpiAwarenessContext) {
        /* Windows 10, version 1607 */
        return data->SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    } else if (data->SetProcessDpiAwareness) {
        /* Windows 8.1 */
        return SUCCEEDED(data->SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE));
    } else if (data->SetProcessDPIAware) {
        /* Windows Vista */
        return data->SetProcessDPIAware();
    }
#endif
    return FALSE;
}

static void
WIN_InitDPIAwareness(_THIS)
{
    const char* hint = SDL_GetHint(SDL_HINT_WINDOWS_DPI_AWARENESS);

    if (hint != NULL) {
        if (SDL_strcmp(hint, "permonitorv2") == 0) {
            WIN_DeclareDPIAwarePerMonitorV2(_this);
        } else if (SDL_strcmp(hint, "permonitor") == 0) {
            WIN_DeclareDPIAwarePerMonitor(_this);
        } else if (SDL_strcmp(hint, "system") == 0) {
            WIN_DeclareDPIAwareSystem(_this);
        } else if (SDL_strcmp(hint, "unaware") == 0) {
            WIN_DeclareDPIAwareUnaware(_this);
        }
    }
}

static BOOL
WIN_DeclareDPIAwareUnaware(_THIS)
{
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    SDL_VideoData* data = (SDL_VideoData*)_this->driverdata;

    if (data->SetProcessDpiAwarenessContext) {
        return data->SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
    } else if (data->SetProcessDpiAwareness) {
        /* Windows 8.1 */
        return SUCCEEDED(data->SetProcessDpiAwareness(PROCESS_DPI_UNAWARE));
    }
#endif
    return FALSE;
}

static void
WIN_InitDPIScaling(_THIS)
{
    SDL_VideoData* data = (SDL_VideoData*)_this->driverdata;

    if (SDL_GetHintBoolean(SDL_HINT_WINDOWS_DPI_SCALING, SDL_FALSE)) {
        WIN_DeclareDPIAwarePerMonitorV2(_this);

        data->dpi_scaling_enabled = SDL_TRUE;
    }
}

int
WIN_VideoInit(_THIS)
{
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;

    // I don't know if is it needed or not
    WIN_InitDPIAwareness(_this);
    WIN_InitDPIScaling(_this);

#ifdef HIGHDPI_DEBUG
    SDL_Log("DPI awareness: %s", WIN_GetDPIAwareness(_this));
#endif

#if defined(__XBOXONE__) || defined(__XBOXSERIES__)
    /* For Xbox, we just need to create the single display */
    {
        SDL_VideoDisplay display;
        SDL_DisplayMode current_mode;

        SDL_zero(current_mode);
        D3D12_XBOX_GetResolution(&current_mode.w, &current_mode.h);
        current_mode.refresh_rate = 60;
        current_mode.format = SDL_PIXELFORMAT_ARGB8888;

        SDL_zero(display);
        display.desktop_mode = current_mode;
        display.current_mode = current_mode;

        SDL_AddVideoDisplay(&display, SDL_FALSE);
    }
#else /*!defined(__XBOXONE__) && !defined(__XBOXSERIES__)*/
    if (WIN_InitModes(_this) < 0) {
        return -1;
    }

    WIN_InitKeyboard(_this);
    WIN_InitMouse(_this);
#endif

    SDL_AddHintCallback(SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP, UpdateWindowsEnableMessageLoop, NULL);
    SDL_AddHintCallback(SDL_HINT_WINDOW_FRAME_USABLE_WHILE_CURSOR_HIDDEN, UpdateWindowFrameUsableWhileCursorHidden, NULL);

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    data->_SDL_WAKEUP = RegisterWindowMessageA("_SDL_WAKEUP");
#endif

    return 0;
}

void
WIN_VideoQuit(_THIS)
{
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    WIN_QuitModes(_this);
    WIN_QuitKeyboard(_this);
    WIN_QuitMouse(_this);
#endif
}

static void
WIN_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_VideoData *data = (SDL_VideoData *) device->driverdata;

    SDL_UnregisterApp();
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    if (data->userDLL) {
        SDL_UnloadObject(data->userDLL);
    }
    if (data->shcoreDLL) {
        SDL_UnloadObject(data->shcoreDLL);
    }
#endif
    if (device->wakeup_lock) {
        SDL_DestroyMutex(device->wakeup_lock);
    }
    SDL_free(device->driverdata);
    SDL_free(device);
}
