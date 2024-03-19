static void
WIN_GL_SetupPixelFormat(_THIS, PIXELFORMATDESCRIPTOR * pfd)
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
WIN_GL_ChoosePixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR * target)
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

static int
SetupWindowData(_THIS, SDL_Window * window, HWND hwnd, HWND parent, SDL_bool created)
{
    SDL_VideoData *videodata = (SDL_VideoData *) _this->driverdata;
    SDL_WindowData *data;

    /* Allocate the window data */
    data = (SDL_WindowData *) SDL_calloc(1, sizeof(*data));
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
    data->scaling_dpi = WIN_GetScalingDPIForHWND(videodata, hwnd);

#ifdef HIGHDPI_DEBUG
    SDL_Log("SetupWindowData: initialized data->scaling_dpi to %d", data->scaling_dpi);
#endif

    SDL_AddHintCallback(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, WIN_MouseRelativeModeCenterChanged, data);

    window->driverdata = data;

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    /* Associate the data with the window */
    if (!SetProp(hwnd, TEXT("SDL_WindowData"), data)) {
        ReleaseDC(hwnd, data->hdc);
        SDL_free(data);
        return WIN_SetError("SetProp() failed");
    }
#endif

    /* Set up the window proc function */
#ifdef GWLP_WNDPROC
    data->wndproc = (WNDPROC) GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    if (data->wndproc == WIN_WindowProc) {
        data->wndproc = NULL;
    } else {
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) WIN_WindowProc);
    }
#else
    data->wndproc = (WNDPROC) GetWindowLong(hwnd, GWL_WNDPROC);
    if (data->wndproc == WIN_WindowProc) {
        data->wndproc = NULL;
    } else {
        SetWindowLong(hwnd, GWL_WNDPROC, (LONG_PTR) WIN_WindowProc);
    }
#endif

    /* Fill in the SDL window with the window data */
    {
        RECT rect;
        if (GetClientRect(hwnd, &rect)) {
            int w = rect.right;
            int h = rect.bottom;

            WIN_ClientPointToSDL(window, &w, &h);
            if ((window->windowed.w && window->windowed.w != w) || (window->windowed.h && window->windowed.h != h)) {
                /* We tried to create a window larger than the desktop and Windows didn't allow it.  Override! */
                int x, y;
                /* Figure out what the window area will be */
                WIN_AdjustWindowRect(window, &x, &y, &w, &h, SDL_FALSE);
                data->expected_resize = SDL_TRUE;
                SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, w, h, SWP_NOCOPYBITS | SWP_NOZORDER | SWP_NOACTIVATE);
                data->expected_resize = SDL_FALSE;
            } else {
                window->w = w;
                window->h = h;
            }
        }
    }
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    {
        POINT point;
        point.x = 0;
        point.y = 0;
        if (ClientToScreen(hwnd, &point)) {
            int x = point.x;
            int y = point.y;
            WIN_ScreenPointToSDL(&x, &y);
            window->x = x;
            window->y = y;
        }
    }
    WIN_UpdateWindowICCProfile(window, SDL_FALSE);
#endif
    {
        DWORD style = GetWindowLong(hwnd, GWL_STYLE);
        if (style & WS_VISIBLE) {
            window->flags |= SDL_WINDOW_SHOWN;
        } else {
            window->flags &= ~SDL_WINDOW_SHOWN;
        }
        if (style & WS_POPUP) {
            window->flags |= SDL_WINDOW_BORDERLESS;
        } else {
            window->flags &= ~SDL_WINDOW_BORDERLESS;
        }
        if (style & WS_THICKFRAME) {
            window->flags |= SDL_WINDOW_RESIZABLE;
        } else {
            window->flags &= ~SDL_WINDOW_RESIZABLE;
        }
#ifdef WS_MAXIMIZE
        if (style & WS_MAXIMIZE) {
            window->flags |= SDL_WINDOW_MAXIMIZED;
        } else
#endif
        {
            window->flags &= ~SDL_WINDOW_MAXIMIZED;
        }
#ifdef WS_MINIMIZE
        if (style & WS_MINIMIZE) {
            window->flags |= SDL_WINDOW_MINIMIZED;
        } else
#endif
        {
            window->flags &= ~SDL_WINDOW_MINIMIZED;
        }
    }
#if defined(__XBOXONE__) || defined(__XBOXSERIES__)
    window->flags |= SDL_WINDOW_INPUT_FOCUS;
#else
    if (GetFocus() == hwnd) {
        window->flags |= SDL_WINDOW_INPUT_FOCUS;
        SDL_SetKeyboardFocus(window);
        WIN_UpdateClipCursor(window);
    }
#endif

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    /* Enable multi-touch */
    if (videodata->RegisterTouchWindow) {
        videodata->RegisterTouchWindow(hwnd, (TWF_FINETOUCH|TWF_WANTPALM));
    }
#endif

    /* Force the SDL_WINDOW_ALLOW_HIGHDPI window flag if we are doing DPI scaling */
    if (videodata->dpi_scaling_enabled) {
        window->flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    }

    data->initializing = SDL_FALSE;

    /* All done! */
    return 0;
}


