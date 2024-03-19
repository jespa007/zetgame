
VideoBootStrap WINDOWS_bootstrap = {
    "windows", "SDL Windows video driver", WIN_CreateDevice
};


static SDL_VideoDevice *
WIN_CreateDevice(void)
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

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    data->userDLL = SDL_LoadObject("USER32.DLL");
    if (data->userDLL) {
        data->CloseTouchInputHandle = (BOOL (WINAPI *)(HTOUCHINPUT)) SDL_LoadFunction(data->userDLL, "CloseTouchInputHandle");
        data->GetTouchInputInfo = (BOOL (WINAPI *)(HTOUCHINPUT, UINT, PTOUCHINPUT, int)) SDL_LoadFunction(data->userDLL, "GetTouchInputInfo");
        data->RegisterTouchWindow = (BOOL (WINAPI *)(HWND, ULONG)) SDL_LoadFunction(data->userDLL, "RegisterTouchWindow");
        data->SetProcessDPIAware = (BOOL (WINAPI *)(void)) SDL_LoadFunction(data->userDLL, "SetProcessDPIAware");
        data->SetProcessDpiAwarenessContext = (BOOL (WINAPI *)(DPI_AWARENESS_CONTEXT)) SDL_LoadFunction(data->userDLL, "SetProcessDpiAwarenessContext");
        data->SetThreadDpiAwarenessContext = (DPI_AWARENESS_CONTEXT (WINAPI *)(DPI_AWARENESS_CONTEXT)) SDL_LoadFunction(data->userDLL, "SetThreadDpiAwarenessContext");
        data->GetThreadDpiAwarenessContext = (DPI_AWARENESS_CONTEXT (WINAPI *)(void)) SDL_LoadFunction(data->userDLL, "GetThreadDpiAwarenessContext");
        data->GetAwarenessFromDpiAwarenessContext = (DPI_AWARENESS (WINAPI *)(DPI_AWARENESS_CONTEXT)) SDL_LoadFunction(data->userDLL, "GetAwarenessFromDpiAwarenessContext");
        data->EnableNonClientDpiScaling = (BOOL (WINAPI *)(HWND)) SDL_LoadFunction(data->userDLL, "EnableNonClientDpiScaling");
        data->AdjustWindowRectExForDpi = (BOOL (WINAPI *)(LPRECT, DWORD, BOOL, DWORD, UINT)) SDL_LoadFunction(data->userDLL, "AdjustWindowRectExForDpi");
        data->GetDpiForWindow = (UINT (WINAPI *)(HWND)) SDL_LoadFunction(data->userDLL, "GetDpiForWindow");
        data->AreDpiAwarenessContextsEqual = (BOOL (WINAPI *)(DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT)) SDL_LoadFunction(data->userDLL, "AreDpiAwarenessContextsEqual");
        data->IsValidDpiAwarenessContext = (BOOL (WINAPI *)(DPI_AWARENESS_CONTEXT)) SDL_LoadFunction(data->userDLL, "IsValidDpiAwarenessContext");
    } else {
        SDL_ClearError();
    }

    data->shcoreDLL = SDL_LoadObject("SHCORE.DLL");
    if (data->shcoreDLL) {
        data->GetDpiForMonitor = (HRESULT (WINAPI *)(HMONITOR, MONITOR_DPI_TYPE, UINT *, UINT *)) SDL_LoadFunction(data->shcoreDLL, "GetDpiForMonitor");
        data->SetProcessDpiAwareness = (HRESULT (WINAPI *)(PROCESS_DPI_AWARENESS)) SDL_LoadFunction(data->shcoreDLL, "SetProcessDpiAwareness");
    } else {
        SDL_ClearError();
    }
#endif /* #if !defined(__XBOXONE__) && !defined(__XBOXSERIES__) */

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

int
WIN_CreateWindow(_THIS, SDL_Window * window)
{
    HWND hwnd, parent = NULL;
    DWORD style = STYLE_BASIC;
    int x, y;
    int w, h;

    if (window->flags & SDL_WINDOW_SKIP_TASKBAR) {
        parent = CreateWindow(SDL_Appname, TEXT(""), STYLE_BASIC, 0, 0, 32, 32, NULL, NULL, SDL_Instance, NULL);
    }

    style |= GetWindowStyle(window);

    /* Figure out what the window area will be */
    WIN_AdjustWindowRectWithStyle(window, style, FALSE, &x, &y, &w, &h, SDL_FALSE);

    hwnd =
        CreateWindow(SDL_Appname, TEXT(""), style, x, y, w, h, parent, NULL,
                     SDL_Instance, NULL);
    if (!hwnd) {
        return WIN_SetError("Couldn't create window");
    }

    WIN_PumpEvents(_this);

    if (SetupWindowData(_this, window, hwnd, parent, SDL_TRUE) < 0) {
        DestroyWindow(hwnd);
        if (parent) {
            DestroyWindow(parent);
        }
        return -1;
    }

    /* Inform Windows of the frame change so we can respond to WM_NCCALCSIZE */
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

    if (window->flags & SDL_WINDOW_MINIMIZED) {
        ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
    }

    if (!(window->flags & SDL_WINDOW_OPENGL)) {
        return 0;
    }

    /* The rest of this macro mess is for OpenGL or OpenGL ES windows */
#if SDL_VIDEO_OPENGL_ES2
    if (_this->gl_config.profile_mask == SDL_GL_CONTEXT_PROFILE_ES
#if SDL_VIDEO_OPENGL_WGL
        && (!_this->gl_data || WIN_GL_UseEGL(_this))
#endif /* SDL_VIDEO_OPENGL_WGL */
    ) {
#if SDL_VIDEO_OPENGL_EGL
        if (WIN_GLES_SetupWindow(_this, window) < 0) {
            WIN_DestroyWindow(_this, window);
            return -1;
        }
        return 0;
#else
        return SDL_SetError("Could not create GLES window surface (EGL support not configured)");
#endif /* SDL_VIDEO_OPENGL_EGL */
    }
#endif /* SDL_VIDEO_OPENGL_ES2 */

#if SDL_VIDEO_OPENGL_WGL
    if (WIN_GL_SetupWindow(_this, window) < 0) {
        WIN_DestroyWindow(_this, window);
        return -1;
    }
#else
    return SDL_SetError("Could not create GL window (WGL support not configured)");
#endif

    return 0;
}