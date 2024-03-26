static void
X11_DispatchEvent(_THIS, XEvent *xevent)
{
    SDL_VideoData *videodata = (SDL_VideoData *) _this->driverdata;
    Display *display;
    SDL_WindowData *data;
    int orig_event_type;
    KeyCode orig_keycode;
    XClientMessageEvent m;
    int i;

    SDL_assert(videodata != NULL);
    display = videodata->display;

    /* Save the original keycode for dead keys, which are filtered out by
       the XFilterEvent() call below.
    */
    orig_event_type = xevent->type;
    if (orig_event_type == KeyPress || orig_event_type == KeyRelease) {
        orig_keycode = xevent->xkey.keycode;
    } else {
        orig_keycode = 0;
    }

    /* filter events catchs XIM events and sends them to the correct handler */
    if (X11_XFilterEvent(xevent, None) == True) {
#if 0
        printf("Filtered event type = %d display = %d window = %d\n",
               xevent->type, xevent->xany.display, xevent->xany.window);
#endif
        /* Make sure dead key press/release events are sent */
        /* But only if we're using one of the DBus IMEs, otherwise
           some XIM IMEs will generate duplicate events */
        if (orig_keycode) {
#if defined(HAVE_IBUS_IBUS_H) || defined(HAVE_FCITX)
            SDL_Scancode scancode = videodata->key_layout[orig_keycode];
            videodata->filter_code = orig_keycode;
            videodata->filter_time = xevent->xkey.time;

            if (orig_event_type == KeyPress) {
                SDL_SendKeyboardKey(SDL_PRESSED, scancode);
            } else {
                SDL_SendKeyboardKey(SDL_RELEASED, scancode);
            }
#endif
        }
        return;
    }

#if SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS
    if(xevent->type == GenericEvent) {
        X11_HandleGenericEvent(videodata, xevent);
        return;
    }
#endif

#if SDL_VIDEO_DRIVER_X11_XRANDR
    if (videodata->xrandr_event_base && (xevent->type == (videodata->xrandr_event_base + RRNotify))) {
        X11_HandleXRandREvent(_this, xevent);
    }
#endif

    /* Send a SDL_SYSWMEVENT if the application wants them */
    if (SDL_GetEventState(SDL_SYSWMEVENT) == SDL_ENABLE) {
        SDL_SysWMmsg wmmsg;

        SDL_VERSION(&wmmsg.version);
        wmmsg.subsystem = SDL_SYSWM_X11;
        wmmsg.msg.x11.event = *xevent;
        SDL_SendSysWMEvent(&wmmsg);
    }

#if 0
    printf("type = %d display = %d window = %d\n",
           xevent->type, xevent->xany.display, xevent->xany.window);
#endif

    if ((videodata->clipboard_window != None) &&
        (videodata->clipboard_window == xevent->xany.window)) {
        X11_HandleClipboardEvent(_this, xevent);
        return;
    }

    data = NULL;
    if (videodata && videodata->windowlist) {
        for (i = 0; i < videodata->numwindows; ++i) {
            if ((videodata->windowlist[i] != NULL) &&
                (videodata->windowlist[i]->xwindow == xevent->xany.window)) {
                data = videodata->windowlist[i];
                break;
            }
        }
    }
    if (!data) {
        /* The window for KeymapNotify, etc events is 0 */
        if (xevent->type == KeymapNotify) {
#ifdef DEBUG_XEVENTS
            printf("window %p: KeymapNotify!\n", data);
#endif
            if (SDL_GetKeyboardFocus() != NULL) {
                X11_ReconcileKeyboardState(_this);
            }
        } else if (xevent->type == MappingNotify) {
            /* Has the keyboard layout changed? */
            const int request = xevent->xmapping.request;

#ifdef DEBUG_XEVENTS
            printf("window %p: MappingNotify!\n", data);
#endif
            if ((request == MappingKeyboard) || (request == MappingModifier)) {
                X11_XRefreshKeyboardMapping(&xevent->xmapping);
            }

            X11_UpdateKeymap(_this, SDL_TRUE);
        } else if (xevent->type == PropertyNotify && videodata && videodata->windowlist) {
            char* name_of_atom = X11_XGetAtomName(display, xevent->xproperty.atom);

            if (SDL_strncmp(name_of_atom, "_ICC_PROFILE", sizeof("_ICC_PROFILE") - 1) == 0) {
                XWindowAttributes attrib;
                int screennum;
                for (i = 0; i < videodata->numwindows; ++i) {
                    if (videodata->windowlist[i] != NULL) {
                        data = videodata->windowlist[i];
                        X11_XGetWindowAttributes(display, data->xwindow, &attrib);
                        screennum = X11_XScreenNumberOfScreen(attrib.screen);
                        if (screennum == 0 && SDL_strcmp(name_of_atom, "_ICC_PROFILE") == 0) {
                            SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_ICCPROF_CHANGED, 0, 0);
                        } else if (SDL_strncmp(name_of_atom, "_ICC_PROFILE_", sizeof("_ICC_PROFILE_") - 1) == 0 && SDL_strlen(name_of_atom) > sizeof("_ICC_PROFILE_") - 1) {
                            int iccscreennum = SDL_atoi(&name_of_atom[sizeof("_ICC_PROFILE_") - 1]);

                            if (screennum == iccscreennum) {
                                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_ICCPROF_CHANGED, 0, 0);
                            }
                        }
                    }
                }
            }

            if (name_of_atom) X11_XFree(name_of_atom);
        }
        return;
    }

    switch (xevent->type) {

        /* Gaining mouse coverage? */
    case EnterNotify:{
            SDL_Mouse *mouse = SDL_GetMouse();
#ifdef DEBUG_XEVENTS
            printf("window %p: EnterNotify! (%d,%d,%d)\n", data,
                   xevent->xcrossing.x,
                   xevent->xcrossing.y,
                   xevent->xcrossing.mode);
            if (xevent->xcrossing.mode == NotifyGrab)
                printf("Mode: NotifyGrab\n");
            if (xevent->xcrossing.mode == NotifyUngrab)
                printf("Mode: NotifyUngrab\n");
#endif
            SDL_SetMouseFocus(data->window);

            mouse->last_x = xevent->xcrossing.x;
            mouse->last_y = xevent->xcrossing.y;

#if SDL_VIDEO_DRIVER_X11_XFIXES
            {
                /* Only create the barriers if we have input focus */
                SDL_WindowData* windowdata = (SDL_WindowData *) data->window->driverdata;
                if ((data->pointer_barrier_active == SDL_TRUE) && windowdata->window->flags & SDL_WINDOW_INPUT_FOCUS) {
                    X11_ConfineCursorWithFlags(_this, windowdata->window, &windowdata->barrier_rect, X11_BARRIER_HANDLED_BY_EVENT);
                }
            }
#endif

            if (!mouse->relative_mode) {
                SDL_SendMouseMotion(data->window, 0, 0, xevent->xcrossing.x, xevent->xcrossing.y);
            }

            /* We ungrab in LeaveNotify, so we may need to grab again here */
            SDL_UpdateWindowGrab(data->window);
        }
        break;
        /* Losing mouse coverage? */
    case LeaveNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: LeaveNotify! (%d,%d,%d)\n", data,
                   xevent->xcrossing.x,
                   xevent->xcrossing.y,
                   xevent->xcrossing.mode);
            if (xevent->xcrossing.mode == NotifyGrab)
                printf("Mode: NotifyGrab\n");
            if (xevent->xcrossing.mode == NotifyUngrab)
                printf("Mode: NotifyUngrab\n");
#endif
            if (!SDL_GetMouse()->relative_mode) {
                SDL_SendMouseMotion(data->window, 0, 0, xevent->xcrossing.x, xevent->xcrossing.y);
            }

            if (xevent->xcrossing.mode != NotifyGrab &&
                xevent->xcrossing.mode != NotifyUngrab &&
                xevent->xcrossing.detail != NotifyInferior) {

                /* In order for interaction with the window decorations and menu to work properly
                   on Mutter, we need to ungrab the keyboard when the the mouse leaves. */
                if (!(data->window->flags & SDL_WINDOW_FULLSCREEN)) {
                    X11_SetWindowKeyboardGrab(_this, data->window, SDL_FALSE);
                }

                SDL_SetMouseFocus(NULL);
            }
        }
        break;

        /* Gaining input focus? */
    case FocusIn:{
            if (xevent->xfocus.mode == NotifyGrab || xevent->xfocus.mode == NotifyUngrab) {
                /* Someone is handling a global hotkey, ignore it */
#ifdef DEBUG_XEVENTS
                printf("window %p: FocusIn (NotifyGrab/NotifyUngrab, ignoring)\n", data);
#endif
                break;
            }

            if (xevent->xfocus.detail == NotifyInferior || xevent->xfocus.detail == NotifyPointer) {
#ifdef DEBUG_XEVENTS
                printf("window %p: FocusIn (NotifyInferior/NotifyPointer, ignoring)\n", data);
#endif
                break;
            }
#ifdef DEBUG_XEVENTS
            printf("window %p: FocusIn!\n", data);
#endif
            if (!videodata->last_mode_change_deadline) /* no recent mode changes */
            {
                data->pending_focus = PENDING_FOCUS_NONE;
                data->pending_focus_time = 0;
                X11_DispatchFocusIn(_this, data);
            }
            else
            {
                data->pending_focus = PENDING_FOCUS_IN;
                data->pending_focus_time = SDL_GetTicks() + PENDING_FOCUS_TIME;
            }
            data->last_focus_event_time = SDL_GetTicks();
        }
        break;

        /* Losing input focus? */
    case FocusOut:{
            if (xevent->xfocus.mode == NotifyGrab || xevent->xfocus.mode == NotifyUngrab) {
                /* Someone is handling a global hotkey, ignore it */
#ifdef DEBUG_XEVENTS
                printf("window %p: FocusOut (NotifyGrab/NotifyUngrab, ignoring)\n", data);
#endif
                break;
            }
            if (xevent->xfocus.detail == NotifyInferior || xevent->xfocus.detail == NotifyPointer) {
                /* We still have focus if a child gets focus. We also don't
                   care about the position of the pointer when the keyboard
                   focus changed. */
#ifdef DEBUG_XEVENTS
                printf("window %p: FocusOut (NotifyInferior/NotifyPointer, ignoring)\n", data);
#endif
                break;
            }
#ifdef DEBUG_XEVENTS
            printf("window %p: FocusOut!\n", data);
#endif
            if (!videodata->last_mode_change_deadline) /* no recent mode changes */
            {
                data->pending_focus = PENDING_FOCUS_NONE;
                data->pending_focus_time = 0;
                X11_DispatchFocusOut(_this, data);
            }
            else
            {
                data->pending_focus = PENDING_FOCUS_OUT;
                data->pending_focus_time = SDL_GetTicks() + PENDING_FOCUS_TIME;
            }

#if SDL_VIDEO_DRIVER_X11_XFIXES
            /* Disable confinement if it is activated. */
            if (data->pointer_barrier_active == SDL_TRUE) {
                X11_ConfineCursorWithFlags(_this, data->window, NULL, X11_BARRIER_HANDLED_BY_EVENT);
            }
#endif /* SDL_VIDEO_DRIVER_X11_XFIXES */
        }
        break;

        /* Key press/release? */
    case KeyPress:
    case KeyRelease: {
            KeyCode keycode = xevent->xkey.keycode;
            KeySym keysym = NoSymbol;
            char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
            Status status = 0;
            SDL_bool handled_by_ime = SDL_FALSE;

#ifdef DEBUG_XEVENTS
            printf("window %p: %s (X11 keycode = 0x%X)\n", data, (xevent->type == KeyPress ? "KeyPress" : "KeyRelease"), xevent->xkey.keycode);
#endif
#if 1
            if (videodata->key_layout[keycode] == SDL_SCANCODE_UNKNOWN && keycode) {
                int min_keycode, max_keycode;
                X11_XDisplayKeycodes(display, &min_keycode, &max_keycode);
                keysym = X11_KeyCodeToSym(_this, keycode, xevent->xkey.state >> 13);
                fprintf(stderr,
                        "The key you just pressed is not recognized by SDL. To help get this fixed, please report this to the SDL forums/mailing list <https://discourse.libsdl.org/> X11 KeyCode %d (%d), X11 KeySym 0x%lX (%s).\n",
                        keycode, keycode - min_keycode, keysym,
                        X11_XKeysymToString(keysym));
            }
#endif
            /* */
            SDL_zeroa(text);
#ifdef X_HAVE_UTF8_STRING
            if (data->ic && xevent->type == KeyPress) {
                X11_Xutf8LookupString(data->ic, &xevent->xkey, text, sizeof(text),
                                  &keysym, &status);
            } else {
                XLookupStringAsUTF8(&xevent->xkey, text, sizeof(text), &keysym, NULL);
            }
#else
            XLookupStringAsUTF8(&xevent->xkey, text, sizeof(text), &keysym, NULL);
#endif

#ifdef SDL_USE_IME
            if(SDL_GetEventState(SDL_TEXTINPUT) == SDL_ENABLE){
                handled_by_ime = SDL_IME_ProcessKeyEvent(keysym, keycode, (xevent->type == KeyPress ? SDL_PRESSED : SDL_RELEASED));
            }
#endif
            if (!handled_by_ime) {
                if (xevent->type == KeyPress) {
                    /* Don't send the key if it looks like a duplicate of a filtered key sent by an IME */
                    if (xevent->xkey.keycode != videodata->filter_code || xevent->xkey.time != videodata->filter_time) {
                        SDL_SendKeyboardKey(SDL_PRESSED, videodata->key_layout[keycode]);
                    }
                    if (*text) {
                        SDL_SendKeyboardText(text);
                    }
                } else {
                    if (X11_KeyRepeat(display, xevent)) {
                        /* We're about to get a repeated key down, ignore the key up */
                        break;
                    }
                    SDL_SendKeyboardKey(SDL_RELEASED, videodata->key_layout[keycode]);
                }
            }

            if (xevent->type == KeyPress) {
                X11_UpdateUserTime(data, xevent->xkey.time);
            }
        }
        break;

        /* Have we been iconified? */
    case UnmapNotify:{
            XEvent ev;

#ifdef DEBUG_XEVENTS
            printf("window %p: UnmapNotify!\n", data);
#endif

            if (X11_XCheckIfEvent(display, &ev, &isReparentNotify, (XPointer)&xevent->xunmap)) {
                X11_XCheckIfEvent(display, &ev, &isMapNotify, (XPointer)&xevent->xunmap);
            } else {
                X11_DispatchUnmapNotify(data);
            }

#if SDL_VIDEO_DRIVER_X11_XFIXES
            /* Disable confinement if the window gets hidden. */
            if (data->pointer_barrier_active == SDL_TRUE) {
                X11_ConfineCursorWithFlags(_this, data->window, NULL, X11_BARRIER_HANDLED_BY_EVENT);
            }
#endif /* SDL_VIDEO_DRIVER_X11_XFIXES */
        }
        break;

        /* Have we been restored? */
    case MapNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: MapNotify!\n", data);
#endif
            X11_DispatchMapNotify(data);

#if SDL_VIDEO_DRIVER_X11_XFIXES
            /* Enable confinement if it was activated. */
            if (data->pointer_barrier_active == SDL_TRUE) {
                X11_ConfineCursorWithFlags(_this, data->window, &data->barrier_rect, X11_BARRIER_HANDLED_BY_EVENT);
            }
#endif /* SDL_VIDEO_DRIVER_X11_XFIXES */
        }
        break;

        /* Have we been resized or moved? */
    case ConfigureNotify:{
#ifdef DEBUG_XEVENTS
            printf("window %p: ConfigureNotify! (position: %d,%d, size: %dx%d)\n", data,
                   xevent->xconfigure.x, xevent->xconfigure.y,
                   xevent->xconfigure.width, xevent->xconfigure.height);
#endif
            /* Real configure notify events are relative to the parent, synthetic events are absolute. */
            if (!xevent->xconfigure.send_event) {
                unsigned int NumChildren;
                Window ChildReturn, Root, Parent;
                Window * Children;
                /* Translate these coodinates back to relative to root */
                X11_XQueryTree(data->videodata->display, xevent->xconfigure.window, &Root, &Parent, &Children, &NumChildren);
                X11_XTranslateCoordinates(xevent->xconfigure.display,
                                        Parent, DefaultRootWindow(xevent->xconfigure.display),
                                        xevent->xconfigure.x, xevent->xconfigure.y,
                                        &xevent->xconfigure.x, &xevent->xconfigure.y,
                                        &ChildReturn);
            }

            if (xevent->xconfigure.x != data->last_xconfigure.x ||
                xevent->xconfigure.y != data->last_xconfigure.y) {
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_MOVED,
                                    xevent->xconfigure.x, xevent->xconfigure.y);
#ifdef SDL_USE_IME
                if(SDL_GetEventState(SDL_TEXTINPUT) == SDL_ENABLE){
                    /* Update IME candidate list position */
                    SDL_IME_UpdateTextRect(NULL);
                }
#endif
            }
            if (xevent->xconfigure.width != data->last_xconfigure.width ||
                xevent->xconfigure.height != data->last_xconfigure.height) {
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_RESIZED,
                                    xevent->xconfigure.width,
                                    xevent->xconfigure.height);
            }
            data->last_xconfigure = xevent->xconfigure;
        }
        break;

        /* Have we been requested to quit (or another client message?) */
    case ClientMessage:{

            static int xdnd_version=0;

            if (xevent->xclient.message_type == videodata->XdndEnter) {

                SDL_bool use_list = xevent->xclient.data.l[1] & 1;
                data->xdnd_source = xevent->xclient.data.l[0];
                xdnd_version = (xevent->xclient.data.l[1] >> 24);
#ifdef DEBUG_XEVENTS
                printf("XID of source window : %ld\n", data->xdnd_source);
                printf("Protocol version to use : %d\n", xdnd_version);
                printf("More then 3 data types : %d\n", (int) use_list);
#endif

                if (use_list) {
                    /* fetch conversion targets */
                    SDL_x11Prop p;
                    X11_ReadProperty(&p, display, data->xdnd_source, videodata->XdndTypeList);
                    /* pick one */
                    data->xdnd_req = X11_PickTarget(display, (Atom*)p.data, p.count);
                    X11_XFree(p.data);
                } else {
                    /* pick from list of three */
                    data->xdnd_req = X11_PickTargetFromAtoms(display, xevent->xclient.data.l[2], xevent->xclient.data.l[3], xevent->xclient.data.l[4]);
                }
            }
            else if (xevent->xclient.message_type == videodata->XdndPosition) {

#ifdef DEBUG_XEVENTS
                Atom act= videodata->XdndActionCopy;
                if(xdnd_version >= 2) {
                    act = xevent->xclient.data.l[4];
                }
                printf("Action requested by user is : %s\n", X11_XGetAtomName(display , act));
#endif


                /* reply with status */
                SDL_memset(&m, 0, sizeof(XClientMessageEvent));
                m.type = ClientMessage;
                m.display = xevent->xclient.display;
                m.window = xevent->xclient.data.l[0];
                m.message_type = videodata->XdndStatus;
                m.format=32;
                m.data.l[0] = data->xwindow;
                m.data.l[1] = (data->xdnd_req != None);
                m.data.l[2] = 0; /* specify an empty rectangle */
                m.data.l[3] = 0;
                m.data.l[4] = videodata->XdndActionCopy; /* we only accept copying anyway */

                X11_XSendEvent(display, xevent->xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                X11_XFlush(display);
            }
            else if(xevent->xclient.message_type == videodata->XdndDrop) {
                if (data->xdnd_req == None) {
                    /* say again - not interested! */
                    SDL_memset(&m, 0, sizeof(XClientMessageEvent));
                    m.type = ClientMessage;
                    m.display = xevent->xclient.display;
                    m.window = xevent->xclient.data.l[0];
                    m.message_type = videodata->XdndFinished;
                    m.format=32;
                    m.data.l[0] = data->xwindow;
                    m.data.l[1] = 0;
                    m.data.l[2] = None; /* fail! */
                    X11_XSendEvent(display, xevent->xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                } else {
                    /* convert */
                    if(xdnd_version >= 1) {
                        X11_XConvertSelection(display, videodata->XdndSelection, data->xdnd_req, videodata->PRIMARY, data->xwindow, xevent->xclient.data.l[2]);
                    } else {
                        X11_XConvertSelection(display, videodata->XdndSelection, data->xdnd_req, videodata->PRIMARY, data->xwindow, CurrentTime);
                    }
                }
            }
            else if ((xevent->xclient.message_type == videodata->WM_PROTOCOLS) &&
                (xevent->xclient.format == 32) &&
                (xevent->xclient.data.l[0] == videodata->_NET_WM_PING)) {
                Window root = DefaultRootWindow(display);

#ifdef DEBUG_XEVENTS
                printf("window %p: _NET_WM_PING\n", data);
#endif
                xevent->xclient.window = root;
                X11_XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, xevent);
                break;
            }

            else if ((xevent->xclient.message_type == videodata->WM_PROTOCOLS) &&
                (xevent->xclient.format == 32) &&
                (xevent->xclient.data.l[0] == videodata->WM_DELETE_WINDOW)) {

#ifdef DEBUG_XEVENTS
                printf("window %p: WM_DELETE_WINDOW\n", data);
#endif
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_CLOSE, 0, 0);
                break;
            }
            else if ((xevent->xclient.message_type == videodata->WM_PROTOCOLS) &&
                (xevent->xclient.format == 32) &&
                (xevent->xclient.data.l[0] == videodata->WM_TAKE_FOCUS)) {

#ifdef DEBUG_XEVENTS
                printf("window %p: WM_TAKE_FOCUS\n", data);
#endif
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_TAKE_FOCUS, 0, 0);
                break;
            }
        }
        break;

        /* Do we need to refresh ourselves? */
    case Expose:{
#ifdef DEBUG_XEVENTS
            printf("window %p: Expose (count = %d)\n", data, xevent->xexpose.count);
#endif
            SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_EXPOSED, 0, 0);
        }
        break;

    case MotionNotify:{
            SDL_Mouse *mouse = SDL_GetMouse();
            if(!mouse->relative_mode || mouse->relative_mode_warp) {
#ifdef DEBUG_MOTION
                printf("window %p: X11 motion: %d,%d\n", data, xevent->xmotion.x, xevent->xmotion.y);
#endif

                SDL_SendMouseMotion(data->window, 0, 0, xevent->xmotion.x, xevent->xmotion.y);
            }
        }
        break;

    case ButtonPress:{
            int xticks = 0, yticks = 0;
#ifdef DEBUG_XEVENTS
            printf("window %p: ButtonPress (X11 button = %d)\n", data, xevent->xbutton.button);
#endif
            if (X11_IsWheelEvent(display,xevent,&xticks, &yticks)) {
                SDL_SendMouseWheel(data->window, 0, (float) -xticks, (float) yticks, SDL_MOUSEWHEEL_NORMAL);
            } else {
                SDL_bool ignore_click = SDL_FALSE;
                int button = xevent->xbutton.button;
                if(button == Button1) {
                    if (ProcessHitTest(_this, data, xevent)) {
                        SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_HIT_TEST, 0, 0);
                        break;  /* don't pass this event on to app. */
                    }
                }
                else if(button > 7) {
                    /* X button values 4-7 are used for scrolling, so X1 is 8, X2 is 9, ...
                       => subtract (8-SDL_BUTTON_X1) to get value SDL expects */
                    button -= (8-SDL_BUTTON_X1);
                }
                if (data->last_focus_event_time) {
                    const int X11_FOCUS_CLICK_TIMEOUT = 10;
                    if (!SDL_TICKS_PASSED(SDL_GetTicks(), data->last_focus_event_time + X11_FOCUS_CLICK_TIMEOUT)) {
                        ignore_click = !SDL_GetHintBoolean(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, SDL_FALSE);
                    }
                    data->last_focus_event_time = 0;
                }
                if (!ignore_click) {
                    SDL_SendMouseButton(data->window, 0, SDL_PRESSED, button);
                }
            }
            X11_UpdateUserTime(data, xevent->xbutton.time);
        }
        break;

    case ButtonRelease:{
            int button = xevent->xbutton.button;
            /* The X server sends a Release event for each Press for wheels. Ignore them. */
            int xticks = 0, yticks = 0;
#ifdef DEBUG_XEVENTS
            printf("window %p: ButtonRelease (X11 button = %d)\n", data, xevent->xbutton.button);
#endif
            if (!X11_IsWheelEvent(display, xevent, &xticks, &yticks)) {
                if (button > 7) {
                    /* see explanation at case ButtonPress */
                    button -= (8-SDL_BUTTON_X1);
                }
                SDL_SendMouseButton(data->window, 0, SDL_RELEASED, button);
            }
        }
        break;

    case PropertyNotify:{
#ifdef DEBUG_XEVENTS
            unsigned char *propdata;
            int status, real_format;
            Atom real_type;
            unsigned long items_read, items_left;

            char *name = X11_XGetAtomName(display, xevent->xproperty.atom);
            if (name) {
                printf("window %p: PropertyNotify: %s %s time=%lu\n", data, name, (xevent->xproperty.state == PropertyDelete) ? "deleted" : "changed", xevent->xproperty.time);
                X11_XFree(name);
            }

            status = X11_XGetWindowProperty(display, data->xwindow, xevent->xproperty.atom, 0L, 8192L, False, AnyPropertyType, &real_type, &real_format, &items_read, &items_left, &propdata);
            if (status == Success && items_read > 0) {
                if (real_type == XA_INTEGER) {
                    int *values = (int *)propdata;

                    printf("{");
                    for (i = 0; i < items_read; i++) {
                        printf(" %d", values[i]);
                    }
                    printf(" }\n");
                } else if (real_type == XA_CARDINAL) {
                    if (real_format == 32) {
                        Uint32 *values = (Uint32 *)propdata;

                        printf("{");
                        for (i = 0; i < items_read; i++) {
                            printf(" %d", values[i]);
                        }
                        printf(" }\n");
                    } else if (real_format == 16) {
                        Uint16 *values = (Uint16 *)propdata;

                        printf("{");
                        for (i = 0; i < items_read; i++) {
                            printf(" %d", values[i]);
                        }
                        printf(" }\n");
                    } else if (real_format == 8) {
                        Uint8 *values = (Uint8 *)propdata;

                        printf("{");
                        for (i = 0; i < items_read; i++) {
                            printf(" %d", values[i]);
                        }
                        printf(" }\n");
                    }
                } else if (real_type == XA_STRING ||
                           real_type == videodata->UTF8_STRING) {
                    printf("{ \"%s\" }\n", propdata);
                } else if (real_type == XA_ATOM) {
                    Atom *atoms = (Atom *)propdata;

                    printf("{");
                    for (i = 0; i < items_read; i++) {
                        char *atomname = X11_XGetAtomName(display, atoms[i]);
                        if (atomname) {
                            printf(" %s", atomname);
                            X11_XFree(atomname);
                        }
                    }
                    printf(" }\n");
                } else {
                    char *atomname = X11_XGetAtomName(display, real_type);
                    printf("Unknown type: %ld (%s)\n", real_type, atomname ? atomname : "UNKNOWN");
                    if (atomname) {
                        X11_XFree(atomname);
                    }
                }
            }
            if (status == Success) {
                X11_XFree(propdata);
            }
#endif /* DEBUG_XEVENTS */

            /* Take advantage of this moment to make sure user_time has a
                valid timestamp from the X server, so if we later try to
                raise/restore this window, _NET_ACTIVE_WINDOW can have a
                non-zero timestamp, even if there's never been a mouse or
                key press to this window so far. Note that we don't try to
                set _NET_WM_USER_TIME here, though. That's only for legit
                user interaction with the window. */
            if (!data->user_time) {
                data->user_time = xevent->xproperty.time;
            }

            if (xevent->xproperty.atom == data->videodata->_NET_WM_STATE) {
                /* Get the new state from the window manager.
                   Compositing window managers can alter visibility of windows
                   without ever mapping / unmapping them, so we handle that here,
                   because they use the NETWM protocol to notify us of changes.
                 */
                const Uint32 flags = X11_GetNetWMState(_this, xevent->xproperty.window);
                const Uint32 changed = flags ^ data->window->flags;

                if ((changed & SDL_WINDOW_HIDDEN) || (changed & SDL_WINDOW_FULLSCREEN)) {
                     if (flags & SDL_WINDOW_HIDDEN) {
                         X11_DispatchUnmapNotify(data);
                     } else {
                         X11_DispatchMapNotify(data);
                    }
                }

                if (changed & SDL_WINDOW_MAXIMIZED) {
                    if (flags & SDL_WINDOW_MAXIMIZED) {
                        SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_MAXIMIZED, 0, 0);
                    } else {
                        SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_RESTORED, 0, 0);
                    }
                }
            } else if (xevent->xproperty.atom == videodata->XKLAVIER_STATE) {
                /* Hack for Ubuntu 12.04 (etc) that doesn't send MappingNotify
                   events when the keyboard layout changes (for example,
                   changing from English to French on the menubar's keyboard
                   icon). Since it changes the XKLAVIER_STATE property, we
                   notice and reinit our keymap here. This might not be the
                   right approach, but it seems to work. */
                X11_UpdateKeymap(_this, SDL_TRUE);
            } else if (xevent->xproperty.atom == videodata->_NET_FRAME_EXTENTS) {
                Atom type;
                int format;
                unsigned long nitems, bytes_after;
                unsigned char *property;
                if (X11_XGetWindowProperty(display, data->xwindow, videodata->_NET_FRAME_EXTENTS, 0, 16, 0, XA_CARDINAL, &type, &format, &nitems, &bytes_after, &property) == Success) {
                    if (type != None && nitems == 4) {
                        data->border_left = (int) ((long*)property)[0];
                        data->border_right = (int) ((long*)property)[1];
                        data->border_top = (int) ((long*)property)[2];
                        data->border_bottom = (int) ((long*)property)[3];
                    }
                    X11_XFree(property);

                    #ifdef DEBUG_XEVENTS
                    printf("New _NET_FRAME_EXTENTS: left=%d right=%d, top=%d, bottom=%d\n", data->border_left, data->border_right, data->border_top, data->border_bottom);
                    #endif
                }
            }
        }
        break;

    case SelectionNotify: {
            Atom target = xevent->xselection.target;
#ifdef DEBUG_XEVENTS
            printf("window %p: SelectionNotify (requestor = %ld, target = %ld)\n", data,
                xevent->xselection.requestor, xevent->xselection.target);
#endif
            if (target == data->xdnd_req) {
                /* read data */
                SDL_x11Prop p;
                X11_ReadProperty(&p, display, data->xwindow, videodata->PRIMARY);

                if (p.format == 8) {
                    char *saveptr = NULL;
                    char *name = X11_XGetAtomName(display, target);
                    if (name) {
                        char *token = SDL_strtokr((char *) p.data, "\r\n", &saveptr);
                        while (token != NULL) {
                            if (SDL_strcmp("text/plain", name) == 0) {
                                SDL_SendDropText(data->window, token);
                            } else if (SDL_strcmp("text/uri-list", name) == 0) {
                                char *fn = X11_URIToLocal(token);
                                if (fn) {
                                    SDL_SendDropFile(data->window, fn);
                                }
                            }
                            token = SDL_strtokr(NULL, "\r\n", &saveptr);
                        }
                        X11_XFree(name);
                    }
                    SDL_SendDropComplete(data->window);
                }
                X11_XFree(p.data);

                /* send reply */
                SDL_memset(&m, 0, sizeof(XClientMessageEvent));
                m.type = ClientMessage;
                m.display = display;
                m.window = data->xdnd_source;
                m.message_type = videodata->XdndFinished;
                m.format = 32;
                m.data.l[0] = data->xwindow;
                m.data.l[1] = 1;
                m.data.l[2] = videodata->XdndActionCopy;
                X11_XSendEvent(display, data->xdnd_source, False, NoEventMask, (XEvent*)&m);

                X11_XSync(display, False);
            }
        }
        break;

    default:{
#ifdef DEBUG_XEVENTS
            printf("window %p: Unhandled event %d\n", data, xevent->type);
#endif
        }
        break;
    }
}
