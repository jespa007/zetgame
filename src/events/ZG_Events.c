#include "@zg_events.h"

typedef struct SDL_VideoDevice SDL_VideoDevice;
/* Define the SDL video driver structure */
#define _THIS   SDL_VideoDevice *_this
static SDL_VideoDevice *_this = NULL;

struct SDL_VideoDevice
{
    /* * * */
    /* The name of this video driver */
    const char *name;

    /* * * */
    /* Initialization/Query functions */

    /*
     * Initialize the native video subsystem, filling in the list of
     * displays for this driver, returning 0 or -1 if there's an error.
     */
    int (*VideoInit) (_THIS);

    /*
     * Reverse the effects VideoInit() -- called if VideoInit() fails or
     * if the application is shutting down the video subsystem.
     */
    void (*VideoQuit) (_THIS);

    /*
     * Reinitialize the touch devices -- called if an unknown touch ID occurs.
     */
    void (*ResetTouch) (_THIS);

    /* * * */
    /*
     * Display functions
     */

    /*
     * Get the bounds of a display
     */
    int (*GetDisplayBounds) (_THIS, SDL_VideoDisplay * display, SDL_Rect * rect);

    /*
     * Get the usable bounds of a display (bounds minus menubar or whatever)
     */
    int (*GetDisplayUsableBounds) (_THIS, SDL_VideoDisplay * display, SDL_Rect * rect);

    /*
     * Get the dots/pixels-per-inch of a display
     */
    int (*GetDisplayDPI) (_THIS, SDL_VideoDisplay * display, float * ddpi, float * hdpi, float * vdpi);

    /*
     * Get a list of the available display modes for a display.
     */
    void (*GetDisplayModes) (_THIS, SDL_VideoDisplay * display);

    /*
     * Setting the display mode is independent of creating windows, so
     * when the display mode is changed, all existing windows should have
     * their data updated accordingly, including the display surfaces
     * associated with them.
     */
    int (*SetDisplayMode) (_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);

    /* * * */
    /*
     * Window functions
     */
    int (*CreateSDLWindow) (_THIS, SDL_Window * window);
    int (*CreateSDLWindowFrom) (_THIS, SDL_Window * window, const void *data);
    void (*SetWindowTitle) (_THIS, SDL_Window * window);
    void (*SetWindowIcon) (_THIS, SDL_Window * window, SDL_Surface * icon);
    void (*SetWindowPosition) (_THIS, SDL_Window * window);
    void (*SetWindowSize) (_THIS, SDL_Window * window);
    void (*SetWindowMinimumSize) (_THIS, SDL_Window * window);
    void (*SetWindowMaximumSize) (_THIS, SDL_Window * window);
    int (*GetWindowBordersSize) (_THIS, SDL_Window * window, int *top, int *left, int *bottom, int *right);
    int (*SetWindowOpacity) (_THIS, SDL_Window * window, float opacity);
    int (*SetWindowModalFor) (_THIS, SDL_Window * modal_window, SDL_Window * parent_window);
    int (*SetWindowInputFocus) (_THIS, SDL_Window * window);
    void (*ShowWindow) (_THIS, SDL_Window * window);
    void (*HideWindow) (_THIS, SDL_Window * window);
    void (*RaiseWindow) (_THIS, SDL_Window * window);
    void (*MaximizeWindow) (_THIS, SDL_Window * window);
    void (*MinimizeWindow) (_THIS, SDL_Window * window);
    void (*RestoreWindow) (_THIS, SDL_Window * window);
    void (*SetWindowBordered) (_THIS, SDL_Window * window, SDL_bool bordered);
    void (*SetWindowResizable) (_THIS, SDL_Window * window, SDL_bool resizable);
    void (*SetWindowFullscreen) (_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen);
    int (*SetWindowGammaRamp) (_THIS, SDL_Window * window, const Uint16 * ramp);
    int (*GetWindowGammaRamp) (_THIS, SDL_Window * window, Uint16 * ramp);
    void (*SetWindowGrab) (_THIS, SDL_Window * window, SDL_bool grabbed);
    void (*DestroyWindow) (_THIS, SDL_Window * window);
    int (*CreateWindowFramebuffer) (_THIS, SDL_Window * window, Uint32 * format, void ** pixels, int *pitch);
    int (*UpdateWindowFramebuffer) (_THIS, SDL_Window * window, const SDL_Rect * rects, int numrects);
    void (*DestroyWindowFramebuffer) (_THIS, SDL_Window * window);
    void (*OnWindowEnter) (_THIS, SDL_Window * window);

    /* * * */
    /*
     * Shaped-window functions
     */
    SDL_ShapeDriver shape_driver;

    /* Get some platform dependent window information */
    SDL_bool(*GetWindowWMInfo) (_THIS, SDL_Window * window,
                                struct SDL_SysWMinfo * info);

    /* * * */
    /*
     * OpenGL support
     */
    int (*GL_LoadLibrary) (_THIS, const char *path);
    void *(*GL_GetProcAddress) (_THIS, const char *proc);
    void (*GL_UnloadLibrary) (_THIS);
      SDL_GLContext(*GL_CreateContext) (_THIS, SDL_Window * window);
    int (*GL_MakeCurrent) (_THIS, SDL_Window * window, SDL_GLContext context);
    void (*GL_GetDrawableSize) (_THIS, SDL_Window * window, int *w, int *h);
    int (*GL_SetSwapInterval) (_THIS, int interval);
    int (*GL_GetSwapInterval) (_THIS);
    int (*GL_SwapWindow) (_THIS, SDL_Window * window);
    void (*GL_DeleteContext) (_THIS, SDL_GLContext context);
    void (*GL_DefaultProfileConfig) (_THIS, int *mask, int *major, int *minor);

    /* * * */
    /*
     * Vulkan support
     */
    int (*Vulkan_LoadLibrary) (_THIS, const char *path);
    void (*Vulkan_UnloadLibrary) (_THIS);
    SDL_bool (*Vulkan_GetInstanceExtensions) (_THIS, SDL_Window *window, unsigned *count, const char **names);
    SDL_bool (*Vulkan_CreateSurface) (_THIS, SDL_Window *window, VkInstance instance, VkSurfaceKHR *surface);
    void (*Vulkan_GetDrawableSize) (_THIS, SDL_Window * window, int *w, int *h);

    /* * * */
    /*
     * Metal support
     */
    SDL_MetalView (*Metal_CreateView) (_THIS, SDL_Window * window);
    void (*Metal_DestroyView) (_THIS, SDL_MetalView view);
    void *(*Metal_GetLayer) (_THIS, SDL_MetalView view);
    void (*Metal_GetDrawableSize) (_THIS, SDL_Window * window, int *w, int *h);

    /* * * */
    /*
     * Event manager functions
     */
    void (*PumpEvents) (_THIS);

    /* Suspend the screensaver */
    void (*SuspendScreenSaver) (_THIS);

    /* Text input */
    void (*StartTextInput) (_THIS);
    void (*StopTextInput) (_THIS);
    void (*SetTextInputRect) (_THIS, SDL_Rect *rect);

    /* Screen keyboard */
    SDL_bool (*HasScreenKeyboardSupport) (_THIS);
    void (*ShowScreenKeyboard) (_THIS, SDL_Window *window);
    void (*HideScreenKeyboard) (_THIS, SDL_Window *window);
    SDL_bool (*IsScreenKeyboardShown) (_THIS, SDL_Window *window);

    /* Clipboard */
    int (*SetClipboardText) (_THIS, const char *text);
    char * (*GetClipboardText) (_THIS);
    SDL_bool (*HasClipboardText) (_THIS);

    /* MessageBox */
    int (*ShowMessageBox) (_THIS, const SDL_MessageBoxData *messageboxdata, int *buttonid);

    /* Hit-testing */
    int (*SetWindowHitTest)(SDL_Window * window, SDL_bool enabled);

    /* Tell window that app enabled drag'n'drop events */
    void (*AcceptDragAndDrop)(SDL_Window * window, SDL_bool accept);

    /* * * */
    /* Data common to all drivers */
    SDL_bool is_dummy;
    SDL_bool suspend_screensaver;
    int num_displays;
    SDL_VideoDisplay *displays;
    SDL_Window *windows;
    SDL_Window *grabbed_window;
    Uint8 window_magic;
    Uint32 next_object_id;
    char *clipboard_text;

    /* * * */
    /* Data used by the GL drivers */
    struct
    {
        int red_size;
        int green_size;
        int blue_size;
        int alpha_size;
        int depth_size;
        int buffer_size;
        int stencil_size;
        int double_buffer;
        int accum_red_size;
        int accum_green_size;
        int accum_blue_size;
        int accum_alpha_size;
        int stereo;
        int multisamplebuffers;
        int multisamplesamples;
        int accelerated;
        int major_version;
        int minor_version;
        int flags;
        int profile_mask;
        int share_with_current_context;
        int release_behavior;
        int reset_notification;
        int framebuffer_srgb_capable;
        int no_error;
        int retained_backing;
        int driver_loaded;
        char driver_path[256];
        void *dll_handle;
    } gl_config;

    /* * * */
    /* Cache current GL context; don't call the OS when it hasn't changed. */
    /* We have the global pointers here so Cocoa continues to work the way
       it always has, and the thread-local storage for the general case.
     */
    SDL_Window *current_glwin;
    SDL_GLContext current_glctx;
    SDL_TLSID current_glwin_tls;
    SDL_TLSID current_glctx_tls;

    /* Flag that stores whether it's allowed to call SDL_GL_MakeCurrent()
     * with a NULL window, but a non-NULL context. (Not allowed in most cases,
     * except on EGL under some circumstances.) */
    SDL_bool gl_allow_no_surface;

    /* * * */
    /* Data used by the Vulkan drivers */
    struct
    {
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
        int loader_loaded;
        char loader_path[256];
        void *loader_handle;
    } vulkan_config;

    /* * * */
    /* Data private to this driver */
    void *driverdata;
    struct SDL_GLDriverData *gl_data;

#if SDL_VIDEO_OPENGL_EGL
    struct SDL_EGL_VideoData *egl_data;
#endif

#if SDL_VIDEO_OPENGL_ES || SDL_VIDEO_OPENGL_ES2
    struct SDL_PrivateGLESData *gles_data;
#endif

    /* * * */
    /* The function used to dispose of this structure */
    void (*free) (_THIS);
};

/* Define the SDL window structure, corresponding to toplevel windows */
struct SDL_Window
{
    const void *magic;
    Uint32 id;
    char *title;
    SDL_Surface *icon;
    int x, y;
    int w, h;
    int min_w, min_h;
    int max_w, max_h;
    Uint32 flags;
    Uint32 last_fullscreen_flags;
    Uint32 display_index;

    /* Stored position and size for windowed mode */
    SDL_Rect windowed;

    SDL_DisplayMode fullscreen_mode;

    float opacity;

    float brightness;
    Uint16 *gamma;
    Uint16 *saved_gamma;        /* (just offset into gamma) */

    SDL_Surface *surface;
    SDL_bool surface_valid;

    SDL_bool is_hiding;
    SDL_bool is_destroying;
    SDL_bool is_dropping;       /* drag/drop in progress, expecting SDL_SendDropComplete(). */

    SDL_Rect mouse_rect;

    SDL_WindowShaper *shaper;

    SDL_HitTest hit_test;
    void *hit_test_data;

    SDL_WindowUserData *data;

    void *driverdata;

    SDL_Window *prev;
    SDL_Window *next;
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

/*
BOOL PeekMessageA(
  [out]          LPMSG lpMsg,
  [in, optional] HWND  hWnd,
  [in]           UINT  wMsgFilterMin,
  [in]           UINT  wMsgFilterMax,
  [in]           UINT  wRemoveMsg
);

	while (true)
	{
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT) break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			Sleep(1);

			if (timeGetTime() > next_game_tick)
			{
				FRAMES++;
				Update();
				Render();
				next_game_tick += SKIP_TICKS;
			}
		}
	}
*/

void
WIN_PumpEvents(_THIS)
{
    const Uint8 *keystate;
    MSG msg;
    DWORD start_ticks = GetTickCount();
    int new_messages = 0;

    if (g_WindowsEnableMessageLoop) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (g_WindowsMessageHook) {
                g_WindowsMessageHook(g_WindowsMessageHookData, msg.hwnd, msg.message, msg.wParam, msg.lParam);
            }

            /* Always translate the message in case it's a non-SDL window (e.g. with Qt integration) */
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            /* Make sure we don't busy loop here forever if there are lots of events coming in */
            if (SDL_TICKS_PASSED(msg.time, start_ticks)) {
                /* We might get a few new messages generated by the Steam overlay or other application hooks
                   In this case those messages will be processed before any pending input, so we want to continue after those messages.
                   (thanks to Peter Deayton for his investigation here)
                 */
                const int MAX_NEW_MESSAGES = 3;
                ++new_messages;
                if (new_messages > MAX_NEW_MESSAGES) {
                    break;
                }
            }
        }
    }

    /* Windows loses a shift KEYUP event when you have both pressed at once and let go of one.
       You won't get a KEYUP until both are released, and that keyup will only be for the second
       key you released. Take heroic measures and check the keystate as of the last handled event,
       and if we think a key is pressed when Windows doesn't, unstick it in SDL's state. */
    keystate = SDL_GetKeyboardState(NULL);
    if ((keystate[SDL_SCANCODE_LSHIFT] == SDL_PRESSED) && !(GetKeyState(VK_LSHIFT) & 0x8000)) {
        SDL_SendKeyboardKey(SDL_RELEASED, SDL_SCANCODE_LSHIFT);
    }
    if ((keystate[SDL_SCANCODE_RSHIFT] == SDL_PRESSED) && !(GetKeyState(VK_RSHIFT) & 0x8000)) {
        SDL_SendKeyboardKey(SDL_RELEASED, SDL_SCANCODE_RSHIFT);
    }

    /* Update the clipping rect in case someone else has stolen it */
    WIN_UpdateClipCursorForWindows();
}

// define a queue of events
/* Run the system dependent event loops */
void
SDL_PumpEvents(void)
{
    SDL_VideoDevice *_this = SDL_GetVideoDevice();

    /* Release any keys held down from last frame */
    SDL_ReleaseAutoReleaseKeys();

    /* Get events from the video subsystem */
    if (_this) {
        _this->PumpEvents(_this);
    }

#if !SDL_JOYSTICK_DISABLED
    /* Check for joystick state change */
    if (SDL_update_joysticks) {
        SDL_JoystickUpdate();
    }
#endif

#if !SDL_SENSOR_DISABLED
    /* Check for sensor state change */
    if (SDL_update_sensors) {
        SDL_SensorUpdate();
    }
#endif

    SDL_SendPendingSignalEvents();  /* in case we had a signal handler fire, etc. */
}

/* Public functions */

int
SDL_PollEvent(SDL_Event * event)
{
    return SDL_WaitEventTimeout(event, 0);
}

int
SDL_WaitEvent(SDL_Event * event)
{
    return SDL_WaitEventTimeout(event, -1);
}

int
SDL_WaitEventTimeout(SDL_Event * event, int timeout)
{
    Uint32 expiration = 0;

    if (timeout > 0)
        expiration = SDL_GetTicks() + timeout;

    for (;;) {
        SDL_PumpEvents();
        switch (SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)) {
        case -1:
            return 0;
        case 0:
            if (timeout == 0) {
                /* Polling and no events, just return */
                return 0;
            }
            if (timeout > 0 && SDL_TICKS_PASSED(SDL_GetTicks(), expiration)) {
                /* Timeout expired and no events */
                return 0;
            }
            SDL_Delay(1);
            break;
        default:
            /* Has events */
            return 1;
        }
    }
}

int
SDL_PushEvent(SDL_Event * event)
{
    event->common.timestamp = SDL_GetTicks();

    if (SDL_EventOK.callback || SDL_event_watchers_count > 0) {
        if (!SDL_event_watchers_lock || SDL_LockMutex(SDL_event_watchers_lock) == 0) {
            if (SDL_EventOK.callback && !SDL_EventOK.callback(SDL_EventOK.userdata, event)) {
                if (SDL_event_watchers_lock) {
                    SDL_UnlockMutex(SDL_event_watchers_lock);
                }
                return 0;
            }

            if (SDL_event_watchers_count > 0) {
                /* Make sure we only dispatch the current watcher list */
                int i, event_watchers_count = SDL_event_watchers_count;

                SDL_event_watchers_dispatching = SDL_TRUE;
                for (i = 0; i < event_watchers_count; ++i) {
                    if (!SDL_event_watchers[i].removed) {
                        SDL_event_watchers[i].callback(SDL_event_watchers[i].userdata, event);
                    }
                }
                SDL_event_watchers_dispatching = SDL_FALSE;

                if (SDL_event_watchers_removed) {
                    for (i = SDL_event_watchers_count; i--; ) {
                        if (SDL_event_watchers[i].removed) {
                            --SDL_event_watchers_count;
                            if (i < SDL_event_watchers_count) {
                                SDL_memmove(&SDL_event_watchers[i], &SDL_event_watchers[i+1], (SDL_event_watchers_count - i) * sizeof(SDL_event_watchers[i]));
                            }
                        }
                    }
                    SDL_event_watchers_removed = SDL_FALSE;
                }
            }

            if (SDL_event_watchers_lock) {
                SDL_UnlockMutex(SDL_event_watchers_lock);
            }
        }
    }

    if (SDL_PeepEvents(event, 1, SDL_ADDEVENT, 0, 0) <= 0) {
        return -1;
    }

    SDL_GestureProcessEvent(event);

    return 1;
}

void ZG_Events_Update(void){
	ZG_MouseInfo * mouse_info = &g_zg_input_vars->mouse_info;
	// have a bad behaviour .... it's better to put own icon mouse...
	//resetCursor();
	SDL_Event event;
	ZG_KeyEvent key_event;
	ZG_MouseEvent mouse_event;
	ZG_DropEvent ev_drop;

	memset(g_zg_key_pressed,0,sizeof(g_zg_key_pressed));

	//left_mouse_button_repeat_pressed = false;
	mouse_info->motion = mouse_info->left_press =  false;

	uint32_t id_key;

	while  (SDL_PollEvent  (&event)) {

		memset(&key_event,0,sizeof(key_event));
		memset(&mouse_event,0,sizeof(mouse_event));

		if  (event.type  ==  SDL_QUIT)
			return ;


		if(event.type==g_zg_input_vars->second_mouse_motion){

			SDL_MouseMotionEvent *mouse_motion_2=&event.motion;


			mouse_event.position=Vector2i_Mulv2f(ZG_Vector2i_New(
					mouse_motion_2->x
					,mouse_motion_2->y
			),ZG_Graphics_GetScale());

			//mouse_event.position.x = mouse_motion_2->x*Graphics_GetScaleX();
			//mouse_event.position.y = mouse_motion_2->y*Graphics_GetScaleY();
			mouse_info->position = mouse_event.position;


			ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_motion_callbacks ,&mouse_event);

		}else{

			switch(event.type) {
			case SDL_DROPFILE:

				ev_drop.file = event.drop.file;
				ev_drop.timestamp = event.drop.timestamp;
				ev_drop.type = event.drop.type;

				ZG_Input_UpdateDropEvents(&ev_drop);

				break;
			case SDL_MOUSEMOTION:


				if(!g_zg_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_info->motion = true;

				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,ZG_Graphics_GetScale());

				ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_motion_callbacks ,&mouse_event);

				break;
			case SDL_MOUSEBUTTONDOWN:

				if(!g_zg_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_event.left_press = true;
				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->left_press = true;
				mouse_info->left_hold = true;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,ZG_Graphics_GetScale());

				ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_button_down_callbacks ,&mouse_event);

				break;
			case SDL_MOUSEBUTTONUP:

				if(!g_zg_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_event.left_press = false;
				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->left_press=false;
				mouse_info->left_hold =false;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,ZG_Graphics_GetScale());

				ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_button_up_callbacks ,&mouse_event);
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				id_key = event.key.keysym.sym;

				//ZG_LOG_INFO("UnPress");

				if(SDLK_UP ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_UP; key_event.press_up = true;}
				else if(SDLK_DOWN ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_DOWN; key_event.press_down = true;}
				else if(SDLK_LEFT ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_LEFT; key_event.press_left = true;}
				else if(SDLK_RIGHT ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_RIGHT; key_event.press_right = true;}
				else if(SDLK_ESCAPE ==event.key.keysym.sym) {key_event.press_escape = true;}
				else if(SDLK_BACKSPACE ==event.key.keysym.sym) {key_event.press_backspace = true;}
				else if(SDLK_RETURN ==event.key.keysym.sym) {key_event.press_return = true;}
				else if(SDLK_HOME ==event.key.keysym.sym) {key_event.press_home = true;}
				else if(SDLK_END ==event.key.keysym.sym) {key_event.press_end = true;}
				else {
					if(ZG_KEY_ID_SPACE <= event.key.keysym.sym && event.key.keysym.sym <= ZG_KEY_ID_Z) {
						key_event.code_key = event.key.keysym.sym;
					}
					else if(SDLK_F1 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F1;
					else if(SDLK_F2 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F2;
					else if(SDLK_F3 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F3;
					else if(SDLK_F4 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F4;
					else if(SDLK_F5 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F5;
					else if(SDLK_F6 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F6;
					else if(SDLK_F7 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F7;
					else if(SDLK_F8 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F8;
					else if(SDLK_F9 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F9;
					else if(SDLK_F10 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F10;
					else if(SDLK_F11 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F11;
					else if(SDLK_F12 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F12;
					else if(SDLK_KP_PLUS ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZG_KEY_ID_KP_PLUS;
					}else if(SDLK_KP_MINUS ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZG_KEY_ID_KP_MINUS;
					}else if(SDLK_PAUSE ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZG_KEY_ID_PAUSE;
					}
				}
				if(id_key >= ZG_KEY_ID_LAST) {
					ZG_LOG_ERROR("unhandled id key %i", id_key);
				}
				else{
					if(id_key) {
						g_zg_key[id_key]=false;

						if(event.type == SDL_KEYDOWN) {
							g_zg_key_pressed[id_key]=true;
							g_zg_key[id_key]=true;
						}

						if(event.type == SDL_KEYUP){
							ZG_Input_UpdateKeyEvents(g_zg_input_vars->on_key_up_callbacks,&key_event);
						}
						else{ // is key down...
							ZG_Input_UpdateKeyEvents(g_zg_input_vars->on_key_down_callbacks,&key_event);
						}
					}
				}
				break;
			default:
				//ZG_LOG_INFO("unknow event %i",event.type);
				break;
			}
		}
	}

    //mouse_info->pos_diff.x = mouse_info->pos.x-mouse_info->pos_old.x;//mouse_info.pos_diff.x
	//mouse_info->pos_diff.y = mouse_info->pos.y-mouse_info->pos_old.y;

#if defined(ZG_USING_ASYNC_SPACE_ESC_KEY)
#ifdef WIN32
	// check generic keys ...
	//for(int i=8; i<190; i++){
	if(id_key==0){ // check only escape/space keys...

		// required global detect keys...
		ZG_Input_Win32_CheckKey(VK_PAUSE);
		ZG_Input_Win32_CheckKey(VK_SPACE);

		if(Graphics_GetNumMonitors() == 1){
			ZG_Input_Win32_CheckKey(VK_ESCAPE);
			ZG_Input_Win32_CheckKey(VK_C);
			ZG_Input_Win32_CheckKey(VK_A);
			ZG_Input_Win32_CheckKey(VK_R);
			ZG_Input_Win32_CheckKey(VK_T);
			ZG_Input_Win32_CheckKey(VK_1);
			ZG_Input_Win32_CheckKey(VK_2);
			ZG_Input_Win32_CheckKey(VK_3);
			ZG_Input_Win32_CheckKey(VK_4);
		}
	}
	//}

#endif
#endif
}
