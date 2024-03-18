static void WIN_GrabKeyboard(SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    HMODULE module;

    if (data->keyboard_hook) {
        return;
    }

    /* SetWindowsHookEx() needs to know which module contains the hook we
       want to install. This is complicated by the fact that SDL can be
       linked statically or dynamically. Fortunately XP and later provide
       this nice API that will go through the loaded modules and find the
       one containing our code.
    */
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                           (LPTSTR)WIN_KeyboardHookProc,
                           &module)) {
        return;
    }

    /* Capture a snapshot of the current keyboard state before the hook */
    if (!GetKeyboardState(data->videodata->pre_hook_key_state)) {
        return;
    }

    /* To grab the keyboard, we have to install a low-level keyboard hook to
       intercept keys that would normally be captured by the OS. Intercepting
       all key events on the system is rather invasive, but it's what Microsoft
       actually documents that you do to capture these.
    */
    data->keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, WIN_KeyboardHookProc, module, 0);
}

void WIN_UngrabKeyboard(SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;

    if (data->keyboard_hook) {
        UnhookWindowsHookEx(data->keyboard_hook);
        data->keyboard_hook = NULL;
    }
}

void
WIN_SetWindowKeyboardGrab(_THIS, SDL_Window * window, SDL_bool grabbed)
{
    if (grabbed) {
        WIN_GrabKeyboard(window);
    } else {
        WIN_UngrabKeyboard(window);
    }
}
