void
X11_PumpEvents(_THIS)
{
    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;
    XEvent xevent;
    int i;

    if (data->last_mode_change_deadline) {
        if (SDL_TICKS_PASSED(SDL_GetTicks(), data->last_mode_change_deadline)) {
            data->last_mode_change_deadline = 0;  /* assume we're done. */
        }
    }

    /* Update activity every 30 seconds to prevent screensaver */
    if (_this->suspend_screensaver) {
        const Uint32 now = SDL_GetTicks();
        if (!data->screensaver_activity ||
            SDL_TICKS_PASSED(now, data->screensaver_activity + 30000)) {
            X11_XResetScreenSaver(data->display);

#if SDL_USE_LIBDBUS
            SDL_DBus_ScreensaverTickle();
#endif

            data->screensaver_activity = now;
        }
    }

    SDL_zero(xevent);

    /* Keep processing pending events */
    while (X11_PollEvent(data->display, &xevent)) {
        X11_DispatchEvent(_this, &xevent);
    }

#ifdef SDL_USE_IME
    if(SDL_GetEventState(SDL_TEXTINPUT) == SDL_ENABLE){
        SDL_IME_PumpEvents();
    }
#endif

    /* FIXME: Only need to do this when there are pending focus changes */
    X11_HandleFocusChanges(_this);

    /* FIXME: Only need to do this when there are flashing windows */
    for (i = 0; i < data->numwindows; ++i) {
        if (data->windowlist[i] != NULL &&
            data->windowlist[i]->flash_cancel_time &&
            SDL_TICKS_PASSED(SDL_GetTicks(), data->windowlist[i]->flash_cancel_time)) {
            X11_FlashWindow(_this, data->windowlist[i]->window, SDL_FLASH_CANCEL);
        }
    }
}


int
X11_WaitEventTimeout(_THIS, int timeout)
{
    SDL_VideoData *videodata = (SDL_VideoData *) _this->driverdata;
    Display *display;
    XEvent xevent;
    display = videodata->display;

    SDL_zero(xevent);

    /* Flush and poll to grab any events already read and queued */
    X11_XFlush(display);
    if (X11_PollEvent(display, &xevent)) {
        /* Fall through */
    } else if (timeout == 0) {
        return 0;
    } else {
        /* Use SDL_IOR_NO_RETRY to ensure SIGINT will break us out of our wait */
        int err = SDL_IOReady(ConnectionNumber(display), SDL_IOR_READ | SDL_IOR_NO_RETRY, timeout);
        if (err > 0) {
            if (!X11_PollEvent(display, &xevent)) {
                /* Someone may have beat us to reading the fd. Return 1 here to
                 * trigger the normal spurious wakeup logic in the event core. */
                return 1;
            }
        } else if (err == 0) {
            /* Timeout */
            return 0;
        } else {
            /* Error returned from poll()/select() */

            if (errno == EINTR) {
                /* If the wait was interrupted by a signal, we may have generated a
                 * SDL_QUIT event. Let the caller know to call SDL_PumpEvents(). */
                return 1;
            } else {
                return err;
            }
        }
    }

    X11_DispatchEvent(_this, &xevent);

#ifdef SDL_USE_IME
    if(SDL_GetEventState(SDL_TEXTINPUT) == SDL_ENABLE){
        SDL_IME_PumpEvents();
    }
#endif
    return 1;
}
