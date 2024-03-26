

/* Define the SDL video driver structure */
typedef struct ZG_VideoDevice{
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
    //int (*GL_LoadLibrary) (_THIS, const char *path);
    //void *(*GL_GetProcAddress) (_THIS, const char *proc);
    //void (*GL_UnloadLibrary) (_THIS);
    //SDL_GLContext(*GL_CreateContext) (_THIS, SDL_Window * window);
    //int (*GL_MakeCurrent) (_THIS, SDL_Window * window, SDL_GLContext context);
    void (*GL_GetDrawableSize) (_THIS, SDL_Window * window, int *w, int *h);
    int (*GL_SetSwapInterval) (_THIS, int interval);
    int (*GL_GetSwapInterval) (_THIS);
    int (*GL_SwapWindow) (_THIS, SDL_Window * window);
    //void (*GL_DeleteContext) (_THIS, SDL_GLContext context);
    void (*GL_DefaultProfileConfig) (_THIS, int *mask, int *major, int *minor);

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


/* Available video drivers */
static VideoBootStrap *bootstrap[] = {
	//...
	#if SDL_VIDEO_DRIVER_WINDOWS
		&WINDOWS_bootstrap, // defined at VideoDevice_WIN
	#endif
	//...
};


typedef struct VideoBootStrap
{
    const char *name;
    const char *desc;
    SDL_VideoDevice *(*create) (void);
} VideoBootStrap;

ZG_VideoDevice *g_video_device=NULL;

/*
 * Initialize the video and event subsystems -- determine native pixel format
 */
bool ZG_VideoDevice_Init(void) {
    ZG_VideoDevice *video;
    SDL_bool init_events = SDL_FALSE;
    SDL_bool init_keyboard = SDL_FALSE;
    SDL_bool init_mouse = SDL_FALSE;
    SDL_bool init_touch = SDL_FALSE;
    int i;

    /* Check to make sure we don't overwrite '_this' */
    /*if (_this != NULL) {
        SDL_VideoQuit();
    }*/


    /* Start the event loop */
   /* if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
        goto pre_driver_error;
    }
    init_events = SDL_TRUE;*/
    if (SDL_KeyboardInit() < 0) {
        goto pre_driver_error;
    }
    //init_keyboard = SDL_TRUE;
    if (SDL_MouseInit() < 0) {
        goto pre_driver_error;
    }
    //init_mouse = SDL_TRUE;
    /*if (SDL_TouchInit() < 0) {
        goto pre_driver_error;
    }
    init_touch = SDL_TRUE;*/

    /* Select the proper video driver */
    /*video = NULL;
    if (driver_name == NULL) {
        driver_name = SDL_GetHint(SDL_HINT_VIDEODRIVER);
    }
    if (driver_name != NULL && *driver_name != 0) {
        const char *driver_attempt = driver_name;
        while(driver_attempt != NULL && *driver_attempt != 0 && video == NULL) {
            const char* driver_attempt_end = SDL_strchr(driver_attempt, ',');
            size_t driver_attempt_len = (driver_attempt_end != NULL) ? (driver_attempt_end - driver_attempt)
                                                                     : SDL_strlen(driver_attempt);

            for (i = 0; bootstrap[i]; ++i) {
                if ((driver_attempt_len == SDL_strlen(bootstrap[i]->name)) &&
                    (SDL_strncasecmp(bootstrap[i]->name, driver_attempt, driver_attempt_len) == 0)) {
                    video = bootstrap[i]->create();
                    break;
                }
            }

            driver_attempt = (driver_attempt_end != NULL) ? (driver_attempt_end + 1) : NULL;
        }
    } else {
        for (i = 0; bootstrap[i]; ++i) {
            video = bootstrap[i]->create();
            if (video != NULL) {
                break;
            }
        }
    }
    if (video == NULL) {
        if (driver_name) {
            SDL_SetError("%s not available", driver_name);
            goto pre_driver_error;
        }
        SDL_SetError("No available video device");
        goto pre_driver_error;
    }*/
#ifdef _WIN32
    ZG_VideoDevice_WIN_Create();
#endif

    /* From this point on, use SDL_VideoQuit to cleanup on error, rather than
    pre_driver_error. */
    //_this = video;
    //_this->name = bootstrap[i]->name;
    //_this->next_object_id = 1;
    //_this->thread = SDL_ThreadID();


    /* Set some very sane GL defaults */
    //_this->gl_config.driver_loaded = 0;
    //_this->gl_config.dll_handle = NULL;
    SDL_GL_ResetAttributes();

    //_this->current_glwin_tls = SDL_TLSCreate();
    //_this->current_glctx_tls = SDL_TLSCreate();

    /* Initialize the video subsystem */
    if (_this->VideoInit(_this) < 0) {
        SDL_VideoQuit();
        return -1;
    }

    /* Make sure some displays were added */
    if (_this->num_displays == 0) {
        SDL_VideoQuit();
        return SDL_SetError("The video driver did not add any displays");
    }

    /* Disable the screen saver by default. This is a change from <= 2.0.1,
       but most things using SDL are games or media players; you wouldn't
       want a screensaver to trigger if you're playing exclusively with a
       joystick, or passively watching a movie. Things that use SDL but
       function more like a normal desktop app should explicitly reenable the
       screensaver. */
    if (!SDL_GetHintBoolean(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, SDL_FALSE)) {
        SDL_DisableScreenSaver();
    }

    /* If we don't use a screen keyboard, turn on text input by default,
       otherwise programs that expect to get text events without enabling
       UNICODE input won't get any events.

       Actually, come to think of it, you needed to call SDL_EnableUNICODE(1)
       in SDL 1.2 before you got text input events.  Hmm...
     */
    if (!SDL_HasScreenKeyboardSupport()) {
        SDL_StartTextInput();
    }

    /* We're ready to go! */
    return 0;

pre_driver_error:
    SDL_assert(_this == NULL);
    if (init_touch) {
        SDL_TouchQuit();
    }
    if (init_mouse) {
        SDL_MouseQuit();
    }
    if (init_keyboard) {
        SDL_KeyboardQuit();
    }
    if (init_events) {
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }
    return -1;
}

void
SDL_SetWindowKeyboardGrab(SDL_Window * window, SDL_bool grabbed)
{
    CHECK_WINDOW_MAGIC(window,);

    if (!!grabbed == !!(window->flags & SDL_WINDOW_KEYBOARD_GRABBED)) {
        return;
    }
    if (grabbed) {
        window->flags |= SDL_WINDOW_KEYBOARD_GRABBED;
    } else {
        window->flags &= ~SDL_WINDOW_KEYBOARD_GRABBED;
    }
    SDL_UpdateWindowGrab(window);
}

void
SDL_UpdateWindowGrab(SDL_Window * window)
{
    SDL_bool keyboard_grabbed, mouse_grabbed;

    if (window->flags & SDL_WINDOW_INPUT_FOCUS) {
        if (SDL_GetMouse()->relative_mode || (window->flags & SDL_WINDOW_MOUSE_GRABBED)) {
            mouse_grabbed = SDL_TRUE;
        } else {
            mouse_grabbed = SDL_FALSE;
        }

        if (window->flags & SDL_WINDOW_KEYBOARD_GRABBED) {
            keyboard_grabbed = SDL_TRUE;
        } else {
            keyboard_grabbed = SDL_FALSE;
        }
    } else {
        mouse_grabbed = SDL_FALSE;
        keyboard_grabbed = SDL_FALSE;
    }

    if (mouse_grabbed || keyboard_grabbed) {
        if (_this->grabbed_window && (_this->grabbed_window != window)) {
            /* stealing a grab from another window! */
            _this->grabbed_window->flags &= ~(SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_KEYBOARD_GRABBED);
            if (_this->SetWindowMouseGrab) {
                _this->SetWindowMouseGrab(_this, _this->grabbed_window, SDL_FALSE);
            }
            if (_this->SetWindowKeyboardGrab) {
                _this->SetWindowKeyboardGrab(_this, _this->grabbed_window, SDL_FALSE);
            }
        }
        _this->grabbed_window = window;
    } else if (_this->grabbed_window == window) {
        _this->grabbed_window = NULL;  /* ungrabbing input. */
    }

    if (_this->SetWindowMouseGrab) {
        _this->SetWindowMouseGrab(_this, window, mouse_grabbed);
    }
    if (_this->SetWindowKeyboardGrab) {
        _this->SetWindowKeyboardGrab(_this, window, keyboard_grabbed);
    }
}

void
SDL_SetWindowMouseGrab(SDL_Window * window, SDL_bool grabbed)
{
    CHECK_WINDOW_MAGIC(window,);

    if (!!grabbed == !!(window->flags & SDL_WINDOW_MOUSE_GRABBED)) {
        return;
    }
    if (grabbed) {
        window->flags |= SDL_WINDOW_MOUSE_GRABBED;
    } else {
        window->flags &= ~SDL_WINDOW_MOUSE_GRABBED;
    }
    SDL_UpdateWindowGrab(window);
}

void
SDL_OnWindowFocusGained(SDL_Window * window)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    if (window->gamma && _this->SetWindowGammaRamp) {
        _this->SetWindowGammaRamp(_this, window, window->gamma);
    }

    if (mouse && mouse->relative_mode) {
        SDL_SetMouseFocus(window);
        if (mouse->relative_mode_warp) {
            SDL_PerformWarpMouseInWindow(window, window->w/2, window->h/2, SDL_TRUE);
        }
    }

    SDL_UpdateWindowGrab(window);
}

void
SDL_OnWindowFocusLost(SDL_Window * window)
{
    if (window->gamma && _this->SetWindowGammaRamp) {
        _this->SetWindowGammaRamp(_this, window, window->saved_gamma);
    }

    SDL_UpdateWindowGrab(window);

    if (ShouldMinimizeOnFocusLoss(window)) {
        SDL_MinimizeWindow(window);
    }
}
