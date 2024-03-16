



/* Available video drivers */
static VideoBootStrap *bootstrap[] = {
	//...
	#if SDL_VIDEO_DRIVER_WINDOWS
		&WINDOWS_bootstrap,
	#endif
	//...
}


typedef struct VideoBootStrap
{
    const char *name;
    const char *desc;
    SDL_VideoDevice *(*create) (void);
} VideoBootStrap;

/*
 * Initialize the video and event subsystems -- determine native pixel format
 */
int
SDL_VideoInit(const char *driver_name)
{
    SDL_VideoDevice *video;
    SDL_bool init_events = SDL_FALSE;
    SDL_bool init_keyboard = SDL_FALSE;
    SDL_bool init_mouse = SDL_FALSE;
    SDL_bool init_touch = SDL_FALSE;
    int i;

    /* Check to make sure we don't overwrite '_this' */
    if (_this != NULL) {
        SDL_VideoQuit();
    }

#if !SDL_TIMERS_DISABLED
    SDL_TicksInit();
#endif

    /* Start the event loop */
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
        goto pre_driver_error;
    }
    init_events = SDL_TRUE;
    if (SDL_KeyboardInit() < 0) {
        goto pre_driver_error;
    }
    init_keyboard = SDL_TRUE;
    if (SDL_MouseInit() < 0) {
        goto pre_driver_error;
    }
    init_mouse = SDL_TRUE;
    if (SDL_TouchInit() < 0) {
        goto pre_driver_error;
    }
    init_touch = SDL_TRUE;

    /* Select the proper video driver */
    video = NULL;
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
    }

    /* From this point on, use SDL_VideoQuit to cleanup on error, rather than
    pre_driver_error. */
    _this = video;
    _this->name = bootstrap[i]->name;
    _this->next_object_id = 1;
    _this->thread = SDL_ThreadID();


    /* Set some very sane GL defaults */
    _this->gl_config.driver_loaded = 0;
    _this->gl_config.dll_handle = NULL;
    SDL_GL_ResetAttributes();

    _this->current_glwin_tls = SDL_TLSCreate();
    _this->current_glctx_tls = SDL_TLSCreate();

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
