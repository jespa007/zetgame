/* Private display data */

typedef struct{
    ZG_Window *window;
    HWND hwnd;
    HWND parent;
    HDC hdc;
    HDC mdc;
    HINSTANCE hinstance;
    HBITMAP hbm;
    WNDPROC wndproc;
    HHOOK keyboard_hook;
    bool created;
    WPARAM mouse_button_flags;
    LPARAM last_pointer_update;
    WCHAR high_surrogate;
    bool initializing;
    bool expected_resize;
    bool in_border_change;
    bool in_title_click;
    Uint8 focus_click_pending;
    bool skip_update_clipcursor;
    Uint32 last_updated_clipcursor;
    bool mouse_relative_mode_center;
    bool windowed_mode_was_maximized;
    bool in_window_deactivation;
    RECT cursor_clipped_rect;
    SDL_Point last_raw_mouse_position;
    bool mouse_tracked;
    WCHAR *ICMFileName;
    struct SDL_VideoData *videodata;
#if SDL_VIDEO_OPENGL_EGL
    EGLSurface egl_surface;
#endif
    /**
     * Cached value of GetDpiForWindow, for use for scaling points in the client area
     * between dpi-scaled points and pixels. Only used if videodata->dpi_scaling_enabled.
     */
    int scaling_dpi;
} ZG_WindowDriverData_WIN;

typedef struct{
    int render;

    DWORD clipboard_count;

    /* Touch input functions */
    void* userDLL;
    BOOL (WINAPI *CloseTouchInputHandle)( HTOUCHINPUT );
    BOOL (WINAPI *GetTouchInputInfo)( HTOUCHINPUT, UINT, PTOUCHINPUT, int );
    BOOL (WINAPI *RegisterTouchWindow)( HWND, ULONG );
    BOOL (WINAPI *SetProcessDPIAware)( void );
    BOOL (WINAPI *SetProcessDpiAwarenessContext)( DPI_AWARENESS_CONTEXT );
    DPI_AWARENESS_CONTEXT (WINAPI *SetThreadDpiAwarenessContext)( DPI_AWARENESS_CONTEXT );
    DPI_AWARENESS_CONTEXT (WINAPI *GetThreadDpiAwarenessContext)( void );
    DPI_AWARENESS (WINAPI *GetAwarenessFromDpiAwarenessContext)( DPI_AWARENESS_CONTEXT );
    BOOL (WINAPI *EnableNonClientDpiScaling)( HWND );
    BOOL (WINAPI *AdjustWindowRectExForDpi)( LPRECT, DWORD, BOOL, DWORD, UINT );
    UINT (WINAPI *GetDpiForWindow)( HWND );
    BOOL (WINAPI *AreDpiAwarenessContextsEqual)(DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT);
    BOOL (WINAPI *IsValidDpiAwarenessContext)(DPI_AWARENESS_CONTEXT);

    void* shcoreDLL;
    HRESULT (WINAPI *GetDpiForMonitor)( HMONITOR         hmonitor,
                                        MONITOR_DPI_TYPE dpiType,
                                        UINT             *dpiX,
                                        UINT             *dpiY );
    HRESULT (WINAPI *SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness);

    bool dpi_scaling_enabled;

    BYTE pre_hook_key_state[256];
    UINT _SDL_WAKEUP;
} ZG_Window_WGL_Data;


typedef struct{
    bool HAS_WGL_ARB_pixel_format;
    bool HAS_WGL_EXT_swap_control_tear;
    bool HAS_WGL_ARB_context_flush_control;
    bool HAS_WGL_ARB_create_context_robustness;
    bool HAS_WGL_ARB_create_context_no_error;

    /* Max version of OpenGL ES context that can be created if the
       implementation supports WGL_EXT_create_context_es2_profile.
       major = minor = 0 when unsupported.
     */
    struct {
        int major;
        int minor;
    } es_profile_max_supported_version;

    void *(WINAPI * wglGetProcAddress) (const char *proc);
    HGLRC(WINAPI * wglCreateContext) (HDC hdc);
    BOOL(WINAPI * wglDeleteContext) (HGLRC hglrc);
    BOOL(WINAPI * wglMakeCurrent) (HDC hdc, HGLRC hglrc);
    BOOL(WINAPI * wglShareLists) (HGLRC hglrc1, HGLRC hglrc2);
    BOOL(WINAPI * wglChoosePixelFormatARB) (HDC hdc,
                                            const int *piAttribIList,
                                            const FLOAT * pfAttribFList,
                                            UINT nMaxFormats,
                                            int *piFormats,
                                            UINT * nNumFormats);
    BOOL(WINAPI * wglGetPixelFormatAttribivARB) (HDC hdc, int iPixelFormat,
                                                 int iLayerPlane,
                                                 UINT nAttributes,
                                                 const int *piAttributes,
                                                 int *piValues);
    BOOL (WINAPI * wglSwapIntervalEXT) (int interval);
    int (WINAPI * wglGetSwapIntervalEXT) (void);
}ZG_WindowDriverData_GLW;



#ifndef WGL_ARB_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C
#endif

#ifndef WGL_ARB_multisample
#define WGL_SAMPLE_BUFFERS_ARB         0x2041
#define WGL_SAMPLES_ARB                0x2042
#endif


static void
ZG_Window_WGL_SetupPixelFormat(_THIS, PIXELFORMATDESCRIPTOR * pfd)
{
    SDL_zerop(pfd);
    pfd->nSize = sizeof(*pfd);
    pfd->nVersion = 1;
    pfd->dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL);
    if (_this->gl_config.double_buffer) {
        pfd->dwFlags |= PFD_DOUBLEBUFFER;
    }
    if (_this->gl_config.stereo) {
        pfd->dwFlags |= PFD_STEREO;
    }
    pfd->iLayerType = PFD_MAIN_PLANE;
    pfd->iPixelType = PFD_TYPE_RGBA;
    pfd->cRedBits = _this->gl_config.red_size;
    pfd->cGreenBits = _this->gl_config.green_size;
    pfd->cBlueBits = _this->gl_config.blue_size;
    pfd->cAlphaBits = _this->gl_config.alpha_size;
    if (_this->gl_config.buffer_size) {
        pfd->cColorBits =
            _this->gl_config.buffer_size - _this->gl_config.alpha_size;
    } else {
        pfd->cColorBits = (pfd->cRedBits + pfd->cGreenBits + pfd->cBlueBits);
    }
    pfd->cAccumRedBits = _this->gl_config.accum_red_size;
    pfd->cAccumGreenBits = _this->gl_config.accum_green_size;
    pfd->cAccumBlueBits = _this->gl_config.accum_blue_size;
    pfd->cAccumAlphaBits = _this->gl_config.accum_alpha_size;
    pfd->cAccumBits =
        (pfd->cAccumRedBits + pfd->cAccumGreenBits + pfd->cAccumBlueBits +
         pfd->cAccumAlphaBits);
    pfd->cDepthBits = _this->gl_config.depth_size;
    pfd->cStencilBits = _this->gl_config.stencil_size;
}

/* Choose the closest pixel format that meets or exceeds the target.
   FIXME: Should we weight any particular attribute over any other?
*/
static int
ZG_Window_WGL_ChoosePixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR * target)
{
    PIXELFORMATDESCRIPTOR pfd;
    int count, index, best = 0;
    unsigned int dist, best_dist = ~0U;

    count = DescribePixelFormat(hdc, 1, sizeof(pfd), NULL);

    for (index = 1; index <= count; index++) {

        if (!DescribePixelFormat(hdc, index, sizeof(pfd), &pfd)) {
            continue;
        }

        if ((pfd.dwFlags & target->dwFlags) != target->dwFlags) {
            continue;
        }

        if (pfd.iLayerType != target->iLayerType) {
            continue;
        }
        if (pfd.iPixelType != target->iPixelType) {
            continue;
        }

        dist = 0;

        if (pfd.cColorBits < target->cColorBits) {
            continue;
        } else {
            dist += (pfd.cColorBits - target->cColorBits);
        }
        if (pfd.cRedBits < target->cRedBits) {
            continue;
        } else {
            dist += (pfd.cRedBits - target->cRedBits);
        }
        if (pfd.cGreenBits < target->cGreenBits) {
            continue;
        } else {
            dist += (pfd.cGreenBits - target->cGreenBits);
        }
        if (pfd.cBlueBits < target->cBlueBits) {
            continue;
        } else {
            dist += (pfd.cBlueBits - target->cBlueBits);
        }
        if (pfd.cAlphaBits < target->cAlphaBits) {
            continue;
        } else {
            dist += (pfd.cAlphaBits - target->cAlphaBits);
        }
        if (pfd.cAccumBits < target->cAccumBits) {
            continue;
        } else {
            dist += (pfd.cAccumBits - target->cAccumBits);
        }
        if (pfd.cAccumRedBits < target->cAccumRedBits) {
            continue;
        } else {
            dist += (pfd.cAccumRedBits - target->cAccumRedBits);
        }
        if (pfd.cAccumGreenBits < target->cAccumGreenBits) {
            continue;
        } else {
            dist += (pfd.cAccumGreenBits - target->cAccumGreenBits);
        }
        if (pfd.cAccumBlueBits < target->cAccumBlueBits) {
            continue;
        } else {
            dist += (pfd.cAccumBlueBits - target->cAccumBlueBits);
        }
        if (pfd.cAccumAlphaBits < target->cAccumAlphaBits) {
            continue;
        } else {
            dist += (pfd.cAccumAlphaBits - target->cAccumAlphaBits);
        }
        if (pfd.cDepthBits < target->cDepthBits) {
            continue;
        } else {
            dist += (pfd.cDepthBits - target->cDepthBits);
        }
        if (pfd.cStencilBits < target->cStencilBits) {
            continue;
        } else {
            dist += (pfd.cStencilBits - target->cStencilBits);
        }

        if (dist < best_dist) {
            best = index;
            best_dist = dist;
        }
    }

    return best;
}

/* actual work of ZG_Window_WGL_SetupWindow() happens here. */
static int
ZG_Window_WGL_SetupWindowInternal(_THIS, ZG_Window * window)
{
    HDC hdc = ((ZG_WindowDriverData_WIN *) window->driverdata)->hdc;
    PIXELFORMATDESCRIPTOR pfd;
    int pixel_format = 0;
    int iAttribs[64];
    int *iAttr;
    int *iAccelAttr;
    float fAttribs[1] = { 0 };

    ZG_Window_WGL_SetupPixelFormat(_this, &pfd);

    /* setup WGL_ARB_pixel_format attribs */
    iAttr = &iAttribs[0];

    *iAttr++ = WGL_DRAW_TO_WINDOW_ARB;
    *iAttr++ = GL_TRUE;
    *iAttr++ = WGL_RED_BITS_ARB;
    *iAttr++ = _this->gl_config.red_size;
    *iAttr++ = WGL_GREEN_BITS_ARB;
    *iAttr++ = _this->gl_config.green_size;
    *iAttr++ = WGL_BLUE_BITS_ARB;
    *iAttr++ = _this->gl_config.blue_size;

    if (_this->gl_config.alpha_size) {
        *iAttr++ = WGL_ALPHA_BITS_ARB;
        *iAttr++ = _this->gl_config.alpha_size;
    }

    *iAttr++ = WGL_DOUBLE_BUFFER_ARB;
    *iAttr++ = _this->gl_config.double_buffer;

    *iAttr++ = WGL_DEPTH_BITS_ARB;
    *iAttr++ = _this->gl_config.depth_size;

    if (_this->gl_config.stencil_size) {
        *iAttr++ = WGL_STENCIL_BITS_ARB;
        *iAttr++ = _this->gl_config.stencil_size;
    }

    if (_this->gl_config.accum_red_size) {
        *iAttr++ = WGL_ACCUM_RED_BITS_ARB;
        *iAttr++ = _this->gl_config.accum_red_size;
    }

    if (_this->gl_config.accum_green_size) {
        *iAttr++ = WGL_ACCUM_GREEN_BITS_ARB;
        *iAttr++ = _this->gl_config.accum_green_size;
    }

    if (_this->gl_config.accum_blue_size) {
        *iAttr++ = WGL_ACCUM_BLUE_BITS_ARB;
        *iAttr++ = _this->gl_config.accum_blue_size;
    }

    if (_this->gl_config.accum_alpha_size) {
        *iAttr++ = WGL_ACCUM_ALPHA_BITS_ARB;
        *iAttr++ = _this->gl_config.accum_alpha_size;
    }

    if (_this->gl_config.stereo) {
        *iAttr++ = WGL_STEREO_ARB;
        *iAttr++ = GL_TRUE;
    }

    if (_this->gl_config.multisamplebuffers) {
        *iAttr++ = WGL_SAMPLE_BUFFERS_ARB;
        *iAttr++ = _this->gl_config.multisamplebuffers;
    }

    if (_this->gl_config.multisamplesamples) {
        *iAttr++ = WGL_SAMPLES_ARB;
        *iAttr++ = _this->gl_config.multisamplesamples;
    }

    if (_this->gl_config.floatbuffers) {
        *iAttr++ = WGL_TYPE_RGBA_FLOAT_ARB;
    }

    if (_this->gl_config.framebuffer_srgb_capable) {
        *iAttr++ = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
        *iAttr++ = _this->gl_config.framebuffer_srgb_capable;
    }

    /* We always choose either FULL or NO accel on Windows, because of flaky
       drivers. If the app didn't specify, we use FULL, because that's
       probably what they wanted (and if you didn't care and got FULL, that's
       a perfectly valid result in any case). */
    *iAttr++ = WGL_ACCELERATION_ARB;
    iAccelAttr = iAttr;
    if (_this->gl_config.accelerated) {
        *iAttr++ = WGL_FULL_ACCELERATION_ARB;
    } else {
        *iAttr++ = WGL_NO_ACCELERATION_ARB;
    }

    *iAttr = 0;

    /* Choose and set the closest available pixel format */
    pixel_format = ZG_Window_WGL_ChoosePixelFormatARB(_this, iAttribs, fAttribs);

    /* App said "don't care about accel" and FULL accel failed. Try NO. */
    if ( ( !pixel_format ) && ( _this->gl_config.accelerated < 0 ) ) {
        *iAccelAttr = WGL_NO_ACCELERATION_ARB;
        pixel_format = ZG_Window_WGL_ChoosePixelFormatARB(_this, iAttribs, fAttribs);
        *iAccelAttr = WGL_FULL_ACCELERATION_ARB;  /* if we try again. */
    }
    if (!pixel_format) {
        pixel_format = ZG_Window_WGL_ChoosePixelFormat(hdc, &pfd);
    }
    if (!pixel_format) {
        return SDL_SetError("No matching GL pixel format available");
    }
    if (!SetPixelFormat(hdc, pixel_format, &pfd)) {
        return ZG_Window_WGL_SetError("SetPixelFormat()");
    }
    return 0;
}

int
ZG_Window_WGL_SetupWindow(_THIS, ZG_Window * window)
{
    /* The current context is lost in here; save it and reset it. */
    ZG_Window *current_win = SDL_GL_GetCurrentWindow();
    SDL_GLContext current_ctx = SDL_GL_GetCurrentContext();
    const int retval = ZG_Window_WGL_SetupWindowInternal(_this, window);
    ZG_Window_WGL_MakeCurrent(_this, current_win, current_ctx);
    return retval;
}

int
ZG_Window_WGL_CreateWindow(_THIS, ZG_Window * window)
{
    HWND hwnd, parent = NULL;
    DWORD style = STYLE_BASIC;
    int x, y;
    int w, h;

    if (window->flags & ZG_WINDOW_SKIP_TASKBAR) {
        parent = CreateWindow(SDL_Appname, TEXT(""), STYLE_BASIC, 0, 0, 32, 32, NULL, NULL, SDL_Instance, NULL);
    }

    style |= GetWindowStyle(window);

    /* Figure out what the window area will be */
    ZG_Window_WGL_AdjustWindowRectWithStyle(window, style, FALSE, &x, &y, &w, &h, SDL_FALSE);

    hwnd =
        CreateWindow(SDL_Appname, TEXT(""), style, x, y, w, h, parent, NULL,
                     SDL_Instance, NULL);
    if (!hwnd) {
        return ZG_Window_WGL_SetError("Couldn't create window");
    }

    ZG_Window_WGL_PumpEvents(_this);

    if (SetupWindowData(_this, window, hwnd, parent, SDL_TRUE) < 0) {
        DestroyWindow(hwnd);
        if (parent) {
            DestroyWindow(parent);
        }
        return -1;
    }

    /* Inform Windows of the frame change so we can respond to WM_NCCALCSIZE */
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

    if (window->flags & ZG_WINDOW_MINIMIZED) {
        ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
    }

    if (!(window->flags & ZG_WINDOW_OPENGL)) {
        return 0;
    }

    /* The rest of this macro mess is for OpenGL or OpenGL ES windows */
#if SDL_VIDEO_OPENGL_ES2
    if (_this->gl_config.profile_mask == SDL_GL_CONTEXT_PROFILE_ES
#if SDL_VIDEO_OPENGL_WGL
        && (!_this->gl_data || ZG_Window_WGL_UseEGL(_this))
#endif /* SDL_VIDEO_OPENGL_WGL */
    ) {
#if SDL_VIDEO_OPENGL_EGL
        if (ZG_Window_WGL_GLES_SetupWindow(_this, window) < 0) {
            ZG_Window_WGL_DestroyWindow(_this, window);
            return -1;
        }
        return 0;
#else
        return SDL_SetError("Could not create GLES window surface (EGL support not configured)");
#endif /* SDL_VIDEO_OPENGL_EGL */
    }
#endif /* SDL_VIDEO_OPENGL_ES2 */

#if SDL_VIDEO_OPENGL_WGL
    if (ZG_Window_WGL_SetupWindow(_this, window) < 0) {
        ZG_Window_WGL_DestroyWindow(_this, window);
        return -1;
    }
#else
    return SDL_SetError("Could not create GL window (WGL support not configured)");
#endif

    return 0;
}

static int
ZG_Window_WGL_SetupWindowData(_THIS, ZG_Window * window, HWND hwnd, HWND parent, bool created)
{
    ZG_Window_WGL_Data *videodata = (ZG_Window_WGL_Data *) _this->driverdata;
    ZG_WindowDriverData_WIN *data;

    /* Allocate the window data */
    data = (ZG_WindowDriverData_WIN *) SDL_calloc(1, sizeof(*data));
    if (!data) {
        return SDL_OutOfMemory();
    }
    data->window = window;
    data->hwnd = hwnd;
    data->parent = parent;
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    data->hdc = GetDC(hwnd);
#endif
    data->hinstance = (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    data->created = created;
    data->high_surrogate = 0;
    data->mouse_button_flags = (WPARAM)-1;
    data->last_pointer_update = (LPARAM)-1;
    data->videodata = videodata;
    data->initializing = SDL_TRUE;
    data->scaling_dpi = ZG_Window_WGL_GetScalingDPIForHWND(videodata, hwnd);

#ifdef HIGHDPI_DEBUG
    SDL_Log("ZG_Window_WGL_SetupWindowData: initialized data->scaling_dpi to %d", data->scaling_dpi);
#endif

    SDL_AddHintCallback(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, ZG_Window_WGL_MouseRelativeModeCenterChanged, data);

    window->driverdata = data;

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    /* Associate the data with the window */
    if (!SetProp(hwnd, TEXT("ZG_WindowDriverData_WIN"), data)) {
        ReleaseDC(hwnd, data->hdc);
        SDL_free(data);
        return ZG_Window_WGL_SetError("SetProp() failed");
    }
#endif

    /* Set up the window proc function */
#ifdef GWLP_WNDPROC
    data->wndproc = (WNDPROC) GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    if (data->wndproc == ZG_Window_WGL_WindowProc) {
        data->wndproc = NULL;
    } else {
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) ZG_Window_WGL_WindowProc);
    }
#else
    data->wndproc = (WNDPROC) GetWindowLong(hwnd, GWL_WNDPROC);
    if (data->wndproc == ZG_Window_WGL_WindowProc) {
        data->wndproc = NULL;
    } else {
        SetWindowLong(hwnd, GWL_WNDPROC, (LONG_PTR) ZG_Window_WGL_WindowProc);
    }
#endif

    /* Fill in the SDL window with the window data */
    {
        RECT rect;
        if (GetClientRect(hwnd, &rect)) {
            int w = rect.right;
            int h = rect.bottom;

            ZG_Window_WGL_ClientPointToSDL(window, &w, &h);
            if ((window->windowed.w && window->windowed.w != w) || (window->windowed.h && window->windowed.h != h)) {
                /* We tried to create a window larger than the desktop and Windows didn't allow it.  Override! */
                int x, y;
                /* Figure out what the window area will be */
                ZG_Window_WGL_AdjustWindowRect(window, &x, &y, &w, &h, SDL_FALSE);
                data->expected_resize = SDL_TRUE;
                SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, w, h, SWP_NOCOPYBITS | SWP_NOZORDER | SWP_NOACTIVATE);
                data->expected_resize = SDL_FALSE;
            } else {
                window->w = w;
                window->h = h;
            }
        }
    }

    {
        POINT point;
        point.x = 0;
        point.y = 0;
        if (ClientToScreen(hwnd, &point)) {
            int x = point.x;
            int y = point.y;
            ZG_Window_WGL_ScreenPointToSDL(&x, &y);
            window->x = x;
            window->y = y;
        }
    }
    ZG_Window_WGL_UpdateWindowICCProfile(window, SDL_FALSE);

    {
        DWORD style = GetWindowLong(hwnd, GWL_STYLE);
        if (style & WS_VISIBLE) {
            window->flags |= ZG_WINDOW_SHOWN;
        } else {
            window->flags &= ~ZG_WINDOW_SHOWN;
        }
        if (style & WS_POPUP) {
            window->flags |= ZG_WINDOW_BORDERLESS;
        } else {
            window->flags &= ~ZG_WINDOW_BORDERLESS;
        }
        if (style & WS_THICKFRAME) {
            window->flags |= ZG_WINDOW_RESIZABLE;
        } else {
            window->flags &= ~ZG_WINDOW_RESIZABLE;
        }
#ifdef WS_MAXIMIZE
        if (style & WS_MAXIMIZE) {
            window->flags |= ZG_WINDOW_MAXIMIZED;
        } else
#endif
        {
            window->flags &= ~ZG_WINDOW_MAXIMIZED;
        }
#ifdef WS_MINIMIZE
        if (style & WS_MINIMIZE) {
            window->flags |= ZG_WINDOW_MINIMIZED;
        } else
#endif
        {
            window->flags &= ~ZG_WINDOW_MINIMIZED;
        }
    }
#if defined(__XBOXONE__) || defined(__XBOXSERIES__)
    window->flags |= ZG_WINDOW_INPUT_FOCUS;
#else
    if (GetFocus() == hwnd) {
        window->flags |= ZG_WINDOW_INPUT_FOCUS;
        SDL_SetKeyboardFocus(window);
        ZG_Window_WGL_UpdateClipCursor(window);
    }
#endif

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    /* Enable multi-touch */
    if (videodata->RegisterTouchWindow) {
        videodata->RegisterTouchWindow(hwnd, (TWF_FINETOUCH|TWF_WANTPALM));
    }
#endif

    /* Force the ZG_WINDOW_ALLOW_HIGHDPI window flag if we are doing DPI scaling */
    if (videodata->dpi_scaling_enabled) {
        window->flags |= ZG_WINDOW_ALLOW_HIGHDPI;
    }

    data->initializing = SDL_FALSE;

    /* All done! */
    return 0;
}


// It restricts the mouse within the window
void ZG_Window_WGL_UpdateClipCursor(
	ZG_Window *window
)
{
    ZG_WindowDriverData_WIN *data = (ZG_WindowDriverData_WIN *) window->driverdata;
    SDL_Mouse *mouse = SDL_GetMouse();
    RECT rect, clipped_rect;

    if (data->in_title_click || data->focus_click_pending) {
        return;
    }
    if (data->skip_update_clipcursor) {
        return;
    }
    if (!GetClipCursor(&clipped_rect)) {
        return;
    }

    if ((mouse->relative_mode || (window->flags & ZG_WINDOW_MOUSE_GRABBED) ||
         (window->mouse_rect.w > 0 && window->mouse_rect.h > 0)) &&
        (window->flags & ZG_WINDOW_INPUT_FOCUS)) {
        if (mouse->relative_mode && !mouse->relative_mode_warp && data->mouse_relative_mode_center) {
            if (GetWindowRect(data->hwnd, &rect)) {
                LONG cx, cy;

                cx = (rect.left + rect.right) / 2;
                cy = (rect.top + rect.bottom) / 2;

                /* Make an absurdly small clip rect */
                rect.left = cx - 1;
                rect.right = cx + 1;
                rect.top = cy - 1;
                rect.bottom = cy + 1;

                if (SDL_memcmp(&rect, &clipped_rect, sizeof(rect)) != 0) {
                    if (ClipCursor(&rect)) {
                        data->cursor_clipped_rect = rect;
                    }
                }
            }
        } else {
            if (GetClientRect(data->hwnd, &rect)) {
                ClientToScreen(data->hwnd, (LPPOINT) & rect);
                ClientToScreen(data->hwnd, (LPPOINT) & rect + 1);
                if (window->mouse_rect.w > 0 && window->mouse_rect.h > 0) {
                    SDL_Rect mouse_rect_win_client;
                    RECT mouse_rect, intersection;

                    /* mouse_rect_win_client is the mouse rect in Windows client space */
                    mouse_rect_win_client = window->mouse_rect;
                    ZG_Window_WGL_ClientPointFromSDL(window, &mouse_rect_win_client.x, &mouse_rect_win_client.y);
                    ZG_Window_WGL_ClientPointFromSDL(window, &mouse_rect_win_client.w, &mouse_rect_win_client.h);

                    /* mouse_rect is the rect in Windows screen space */
                    mouse_rect.left = rect.left + mouse_rect_win_client.x;
                    mouse_rect.top = rect.top + mouse_rect_win_client.y;
                    mouse_rect.right = mouse_rect.left + mouse_rect_win_client.w;
                    mouse_rect.bottom = mouse_rect.top + mouse_rect_win_client.h;
                    if (IntersectRect(&intersection, &rect, &mouse_rect)) {
                        SDL_memcpy(&rect, &intersection, sizeof(rect));
                    } else if ((window->flags & ZG_WINDOW_MOUSE_GRABBED) != 0) {
                        /* Mouse rect was invalid, just do the normal grab */
                    } else {
                        SDL_zero(rect);
                    }
                }
                if (SDL_memcmp(&rect, &clipped_rect, sizeof(rect)) != 0) {
                    if (!IsRectEmpty(&rect)) {
                        if (ClipCursor(&rect)) {
                            data->cursor_clipped_rect = rect;
                        }
                    } else {
                        ClipCursor(NULL);
                        SDL_zero(data->cursor_clipped_rect);
                    }
                }
            }
        }
    } else {
        POINT first, second;

        first.x = clipped_rect.left;
        first.y = clipped_rect.top;
        second.x = clipped_rect.right - 1;
        second.y = clipped_rect.bottom - 1;
        if (PtInRect(&data->cursor_clipped_rect, first) &&
            PtInRect(&data->cursor_clipped_rect, second)) {
            ClipCursor(NULL);
            SDL_zero(data->cursor_clipped_rect);
        }
    }
    data->last_updated_clipcursor = SDL_GetTicks();
}

