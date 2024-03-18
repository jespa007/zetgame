static int
SDL_SendKeyboardKeyInternal(Uint8 source, Uint8 state, SDL_Scancode scancode)
{
    SDL_Keyboard *keyboard = &SDL_keyboard;
    int posted;
    SDL_Keymod modifier;
    SDL_Keycode keycode;
    Uint32 type;
    Uint8 repeat = SDL_FALSE;

    if (scancode == SDL_SCANCODE_UNKNOWN || scancode >= SDL_NUM_SCANCODES) {
        return 0;
    }

#ifdef DEBUG_KEYBOARD
    printf("The '%s' key has been %s\n", SDL_GetScancodeName(scancode),
           state == SDL_PRESSED ? "pressed" : "released");
#endif

    /* Figure out what type of event this is */
    switch (state) {
    case SDL_PRESSED:
        type = SDL_KEYDOWN;
        break;
    case SDL_RELEASED:
        type = SDL_KEYUP;
        break;
    default:
        /* Invalid state -- bail */
        return 0;
    }

    /* Drop events that don't change state */
    if (state) {
        if (keyboard->keystate[scancode]) {
            if (!(keyboard->keysource[scancode] & source)) {
                keyboard->keysource[scancode] |= source;
                return 0;
            }
            repeat = SDL_TRUE;
        }
        keyboard->keysource[scancode] |= source;
    } else {
        if (!keyboard->keystate[scancode]) {
            return 0;
        }
        keyboard->keysource[scancode] = 0;
    }

    /* Update internal keyboard state */
    keyboard->keystate[scancode] = state;

    keycode = keyboard->keymap[scancode];

    if (source == KEYBOARD_AUTORELEASE) {
        keyboard->autorelease_pending = SDL_TRUE;
    }

    /* Update modifiers state if applicable */
    switch (keycode) {
    case SDLK_LCTRL:
        modifier = KMOD_LCTRL;
        break;
    case SDLK_RCTRL:
        modifier = KMOD_RCTRL;
        break;
    case SDLK_LSHIFT:
        modifier = KMOD_LSHIFT;
        break;
    case SDLK_RSHIFT:
        modifier = KMOD_RSHIFT;
        break;
    case SDLK_LALT:
        modifier = KMOD_LALT;
        break;
    case SDLK_RALT:
        modifier = KMOD_RALT;
        break;
    case SDLK_LGUI:
        modifier = KMOD_LGUI;
        break;
    case SDLK_RGUI:
        modifier = KMOD_RGUI;
        break;
    case SDLK_MODE:
        modifier = KMOD_MODE;
        break;
    default:
        modifier = KMOD_NONE;
        break;
    }
    if (SDL_KEYDOWN == type) {
        switch (keycode) {
        case SDLK_NUMLOCKCLEAR:
            keyboard->modstate ^= KMOD_NUM;
            break;
        case SDLK_CAPSLOCK:
            keyboard->modstate ^= KMOD_CAPS;
            break;
        case SDLK_SCROLLLOCK:
            keyboard->modstate ^= KMOD_SCROLL;
            break;
        default:
            keyboard->modstate |= modifier;
            break;
        }
    } else {
        keyboard->modstate &= ~modifier;
    }

    /* Post the event, if desired */
    posted = 0;
    if (SDL_GetEventState(type) == SDL_ENABLE) {
        SDL_Event event;
        event.key.type = type;
        event.key.state = state;
        event.key.repeat = repeat;
        event.key.keysym.scancode = scancode;
        event.key.keysym.sym = keycode;
        event.key.keysym.mod = keyboard->modstate;
        event.key.windowID = keyboard->focus ? keyboard->focus->id : 0;
        posted = (SDL_PushEvent(&event) > 0);
    }

    /* If the keyboard is grabbed and the grabbed window is in full-screen,
       minimize the window when we receive Alt+Tab, unless the application
       has explicitly opted out of this behavior. */
    if (keycode == SDLK_TAB &&
        state == SDL_PRESSED &&
        (keyboard->modstate & KMOD_ALT) &&
        keyboard->focus &&
        (keyboard->focus->flags & SDL_WINDOW_KEYBOARD_GRABBED) &&
        (keyboard->focus->flags & SDL_WINDOW_FULLSCREEN) &&
        SDL_GetHintBoolean(SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED, SDL_TRUE)) {
        /* We will temporarily forfeit our grab by minimizing our window,
           allowing the user to escape the application */
        SDL_MinimizeWindow(keyboard->focus);
    }

    return (posted);
}

int
SDL_SendKeyboardKey(Uint8 state, SDL_Scancode scancode)
{
    return SDL_SendKeyboardKeyInternal(KEYBOARD_HARDWARE, state, scancode);
}

int
SDL_SendKeyboardKeyAutoRelease(SDL_Scancode scancode)
{
    return SDL_SendKeyboardKeyInternal(KEYBOARD_AUTORELEASE, SDL_PRESSED, scancode);
}

void
SDL_ReleaseAutoReleaseKeys(void)
{
    SDL_Keyboard *keyboard = &SDL_keyboard;
    SDL_Scancode scancode;

    if (keyboard->autorelease_pending) {
        for (scancode = SDL_SCANCODE_UNKNOWN; scancode < SDL_NUM_SCANCODES; ++scancode) {
            if (keyboard->keysource[scancode] == KEYBOARD_AUTORELEASE) {
                SDL_SendKeyboardKeyInternal(KEYBOARD_AUTORELEASE, SDL_RELEASED, scancode);
            }
        }
        keyboard->autorelease_pending = SDL_FALSE;
    }
}
