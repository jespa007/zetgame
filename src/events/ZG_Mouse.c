static void SDLCALL
SDL_MouseDoubleClickTimeChanged(void *userdata, const char *name, const char *oldValue, const char *hint)
{
    SDL_Mouse *mouse = (SDL_Mouse *)userdata;

    if (hint && *hint) {
        mouse->double_click_time = SDL_atoi(hint);
    } else {
#if defined(__WIN32__) || defined(__WINGDK__)
        mouse->double_click_time = GetDoubleClickTime();
#elif defined(__OS2__)
        mouse->double_click_time = WinQuerySysValue(HWND_DESKTOP, SV_DBLCLKTIME);
#else
        mouse->double_click_time = 500;
#endif
    }
}

/* Public functions */
int
SDL_MouseInit(void)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    SDL_zerop(mouse);

    SDL_AddHintCallback(SDL_HINT_MOUSE_DOUBLE_CLICK_TIME,
                        SDL_MouseDoubleClickTimeChanged, mouse);

    SDL_AddHintCallback(SDL_HINT_MOUSE_DOUBLE_CLICK_RADIUS,
                        SDL_MouseDoubleClickRadiusChanged, mouse);

    SDL_AddHintCallback(SDL_HINT_MOUSE_NORMAL_SPEED_SCALE,
                        SDL_MouseNormalSpeedScaleChanged, mouse);

    SDL_AddHintCallback(SDL_HINT_MOUSE_RELATIVE_SPEED_SCALE,
                        SDL_MouseRelativeSpeedScaleChanged, mouse);

    SDL_AddHintCallback(SDL_HINT_TOUCH_MOUSE_EVENTS,
                        SDL_TouchMouseEventsChanged, mouse);

#if defined(__vita__)
    SDL_AddHintCallback(SDL_HINT_VITA_TOUCH_MOUSE_DEVICE,
                        SDL_VitaTouchMouseDeviceChanged, mouse);
#endif

    SDL_AddHintCallback(SDL_HINT_MOUSE_TOUCH_EVENTS,
                        SDL_MouseTouchEventsChanged, mouse);

    SDL_AddHintCallback(SDL_HINT_MOUSE_AUTO_CAPTURE,
                        SDL_MouseAutoCaptureChanged, mouse);

    SDL_AddHintCallback(SDL_HINT_MOUSE_RELATIVE_WARP_MOTION,
                        SDL_MouseRelativeWarpMotionChanged, mouse);

    mouse->was_touch_mouse_events = SDL_FALSE; /* no touch to mouse movement event pending */

    mouse->cursor_shown = SDL_TRUE;

    return (0);
}
