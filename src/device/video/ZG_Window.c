#include "@zg_device_video.h"

ZG_Window * ZG_CreateWindow(
		const char *_title
		, int _x
		, int _y
		, uint16_t _width
		, uint16_t _height
		, uint32_t _flags
)
{
    SDL_Window *window;
    uint32_t graphics_flags = _flags;

    /* Some platforms can't create zero-sized windows */
    if (_width == 0) {
    	ZG_LOG_ERROR("ZG_CreateWindow : _width == 0");
    	return NULL;
    }

    if (_height == 0) {
    	ZG_LOG_ERROR("ZG_CreateWindow : _height == 0");
    	return NULL;
    }

    if (_width > 16384) {
    	ZG_LOG_ERROR("ZG_CreateWindow : _width > 16384");
    	return NULL;
    }

    if (_height > 16384) {
    	ZG_LOG_ERROR("ZG_CreateWindow : _height > 16384");
    	return NULL;
    }

    /* Some platforms have certain graphics backends enabled by default */
    if (!graphics_flags && !SDL_IsVideoContextExternal()) {
#if (SDL_VIDEO_OPENGL && __MACOSX__) || (__IPHONEOS__ && !TARGET_OS_MACCATALYST) || __ANDROID__ || __NACL__
        if (_this->GL_CreateContext != NULL) {
            flags |= SDL_WINDOW_OPENGL;
        }
#endif
#if SDL_VIDEO_METAL && (TARGET_OS_MACCATALYST || __MACOSX__ || __IPHONEOS__)
        if (_this->Metal_CreateView != NULL) {
            flags |= SDL_WINDOW_METAL;
        }
#endif
    }

    if (flags & SDL_WINDOW_OPENGL) {
        if (!_this->GL_CreateContext) {
            SDL_SetError("OpenGL support is either not configured in SDL "
                         "or not available in current SDL video driver "
                         "(%s) or platform", _this->name);
            return NULL;
        }
        if (SDL_GL_LoadLibrary(NULL) < 0) {
            return NULL;
        }
    }

    if (flags & SDL_WINDOW_VULKAN) {
        if (!_this->Vulkan_CreateSurface) {
            SDL_SetError("Vulkan support is either not configured in SDL "
                         "or not available in current SDL video driver "
                         "(%s) or platform", _this->name);
            return NULL;
        }
        if (graphics_flags & SDL_WINDOW_OPENGL) {
            SDL_SetError("Vulkan and OpenGL not supported on same window");
            return NULL;
        }
        if (SDL_Vulkan_LoadLibrary(NULL) < 0) {
            return NULL;
        }
    }

    if (flags & SDL_WINDOW_METAL) {
        if (!_this->Metal_CreateView) {
            SDL_SetError("Metal support is either not configured in SDL "
                         "or not available in current SDL video driver "
                         "(%s) or platform", _this->name);
            return NULL;
        }
        /* 'flags' may have default flags appended, don't check against that. */
        if (graphics_flags & SDL_WINDOW_OPENGL) {
            SDL_SetError("Metal and OpenGL not supported on same window");
            return NULL;
        }
        if (graphics_flags & SDL_WINDOW_VULKAN) {
            SDL_SetError("Metal and Vulkan not supported on same window. "
                         "To use MoltenVK, set SDL_WINDOW_VULKAN only.");
            return NULL;
        }
    }

    /* Unless the user has specified the high-DPI disabling hint, respect the
     * SDL_WINDOW_ALLOW_HIGHDPI flag.
     */
    if (flags & SDL_WINDOW_ALLOW_HIGHDPI) {
        if (SDL_GetHintBoolean(SDL_HINT_VIDEO_HIGHDPI_DISABLED, SDL_FALSE)) {
            flags &= ~SDL_WINDOW_ALLOW_HIGHDPI;
        }
    }

    window = (SDL_Window *)SDL_calloc(1, sizeof(*window));
    if (!window) {
        SDL_OutOfMemory();
        return NULL;
    }
    window->magic = &_this->window_magic;
    window->id = _this->next_object_id++;
    window->x = x;
    window->y = y;
    window->w = w;
    window->h = h;
    if (SDL_WINDOWPOS_ISUNDEFINED(x) || SDL_WINDOWPOS_ISUNDEFINED(y) ||
        SDL_WINDOWPOS_ISCENTERED(x) || SDL_WINDOWPOS_ISCENTERED(y)) {
        SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
        int displayIndex;
        SDL_Rect bounds;

        displayIndex = SDL_GetIndexOfDisplay(display);
        SDL_GetDisplayBounds(displayIndex, &bounds);
        if (SDL_WINDOWPOS_ISUNDEFINED(x) || SDL_WINDOWPOS_ISCENTERED(x)) {
            window->x = bounds.x + (bounds.w - w) / 2;
        }
        if (SDL_WINDOWPOS_ISUNDEFINED(y) || SDL_WINDOWPOS_ISCENTERED(y)) {
            window->y = bounds.y + (bounds.h - h) / 2;
        }
    }
    window->windowed.x = window->x;
    window->windowed.y = window->y;
    window->windowed.w = window->w;
    window->windowed.h = window->h;

    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_VideoDisplay *display = SDL_GetDisplayForWindow(window);
        int displayIndex;
        SDL_Rect bounds;

        displayIndex = SDL_GetIndexOfDisplay(display);
        SDL_GetDisplayBounds(displayIndex, &bounds);

        /* for real fullscreen we might switch the resolution, so get width and height
         * from closest supported mode and use that instead of current resolution
         */
        if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != SDL_WINDOW_FULLSCREEN_DESKTOP
              && (bounds.w != w || bounds.h != h)) {
            SDL_DisplayMode fullscreen_mode, closest_mode;
            SDL_zero(fullscreen_mode);
            fullscreen_mode.w = w;
            fullscreen_mode.h = h;
            if (SDL_GetClosestDisplayModeForDisplay(display, &fullscreen_mode, &closest_mode) != NULL) {
                bounds.w = closest_mode.w;
                bounds.h = closest_mode.h;
            }
        }
        window->fullscreen_mode.w = bounds.w;
        window->fullscreen_mode.h = bounds.h;
        window->x = bounds.x;
        window->y = bounds.y;
        window->w = bounds.w;
        window->h = bounds.h;
    }

    window->flags = ((flags & CREATE_FLAGS) | SDL_WINDOW_HIDDEN);
    window->last_fullscreen_flags = window->flags;
    window->opacity = 1.0f;
    window->brightness = 1.0f;
    window->next = _this->windows;
    window->is_destroying = SDL_FALSE;
    window->display_index = SDL_GetWindowDisplayIndex(window);

    if (_this->windows) {
        _this->windows->prev = window;
    }
    _this->windows = window;

    if (_this->CreateSDLWindow && _this->CreateSDLWindow(_this, window) < 0) {
        SDL_DestroyWindow(window);
        return NULL;
    }

    /* Clear minimized if not on windows, only windows handles it at create rather than FinishWindowCreation,
     * but it's important or window focus will get broken on windows!
     */
#if !defined(__WIN32__) && !defined(__GDK__)
    if (window->flags & SDL_WINDOW_MINIMIZED) {
        window->flags &= ~SDL_WINDOW_MINIMIZED;
    }
#endif

#if __WINRT__ && (NTDDI_VERSION < NTDDI_WIN10)
    /* HACK: WinRT 8.x apps can't choose whether or not they are fullscreen
       or not.  The user can choose this, via OS-provided UI, but this can't
       be set programmatically.

       Just look at what SDL's WinRT video backend code detected with regards
       to fullscreen (being active, or not), and figure out a return/error code
       from that.
    */
    flags = window->flags;
#endif

    if (title) {
        SDL_SetWindowTitle(window, title);
    }
    SDL_FinishWindowCreation(window, flags);

    /* If the window was created fullscreen, make sure the mode code matches */
    SDL_UpdateFullscreenMode(window, FULLSCREEN_VISIBLE(window));

    return window;
}
