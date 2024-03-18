
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
LRESULT CALLBACK
WIN_KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* hookData = (KBDLLHOOKSTRUCT*)lParam;
    SDL_VideoData* data = SDL_GetVideoDevice()->driverdata;
    SDL_Scancode scanCode;

    if (nCode < 0 || nCode != HC_ACTION) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    switch (hookData->vkCode) {
    case VK_LWIN:
        scanCode = SDL_SCANCODE_LGUI;
        break;
    case VK_RWIN:
        scanCode = SDL_SCANCODE_RGUI;
        break;
    case VK_LMENU:
        scanCode = SDL_SCANCODE_LALT;
        break;
    case VK_RMENU:
        scanCode = SDL_SCANCODE_RALT;
        break;
    case VK_LCONTROL:
        scanCode = SDL_SCANCODE_LCTRL;
        break;
    case VK_RCONTROL:
        scanCode = SDL_SCANCODE_RCTRL;
        break;

    /* These are required to intercept Alt+Tab and Alt+Esc on Windows 7 */
    case VK_TAB:
        scanCode = SDL_SCANCODE_TAB;
        break;
    case VK_ESCAPE:
        scanCode = SDL_SCANCODE_ESCAPE;
        break;

    default:
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        SDL_SendKeyboardKey(SDL_PRESSED, scanCode);
    } else {
        SDL_SendKeyboardKey(SDL_RELEASED, scanCode);

        /* If the key was down prior to our hook being installed, allow the
           key up message to pass normally the first time. This ensures other
           windows have a consistent view of the key state, and avoids keys
           being stuck down in those windows if they are down when the grab
           happens and raised while grabbed. */
        if (hookData->vkCode <= 0xFF && data->pre_hook_key_state[hookData->vkCode]) {
            data->pre_hook_key_state[hookData->vkCode] = 0;
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
    }

    return 1;
}

#endif /*!defined(__XBOXONE__) && !defined(__XBOXSERIES__)*/

LRESULT CALLBACK
WIN_WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    SDL_WindowData *data;
    LRESULT returnCode = -1;

    /* Send a SDL_SYSWMEVENT if the application wants them */
    if (SDL_GetEventState(SDL_SYSWMEVENT) == SDL_ENABLE) {
        SDL_SysWMmsg wmmsg;

        SDL_VERSION(&wmmsg.version);
        wmmsg.subsystem = SDL_SYSWM_WINDOWS;
        wmmsg.msg.win.hwnd = hwnd;
        wmmsg.msg.win.msg = msg;
        wmmsg.msg.win.wParam = wParam;
        wmmsg.msg.win.lParam = lParam;
        SDL_SendSysWMEvent(&wmmsg);
    }

    /* Get the window data for the window */
    data = WIN_GetWindowDataFromHWND(hwnd);
#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    if (!data) {
        /* Fallback */
        data = (SDL_WindowData *) GetProp(hwnd, TEXT("SDL_WindowData"));
    }
#endif
    if (!data) {
        return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
    }

#ifdef WMMSG_DEBUG
    {
        char message[1024];
        if (msg > MAX_WMMSG) {
            SDL_snprintf(message, sizeof(message), "Received windows message: %p UNKNOWN (%d) -- 0x%p, 0x%p\n", hwnd, msg, wParam, lParam);
        } else {
            SDL_snprintf(message, sizeof(message), "Received windows message: %p %s -- 0x%p, 0x%p\n", hwnd, wmtab[msg], wParam, lParam);
        }
        OutputDebugStringA(message);
    }
#endif /* WMMSG_DEBUG */

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    if (IME_HandleMessage(hwnd, msg, wParam, &lParam, data->videodata))
        return 0;
#endif

    switch (msg) {

    case WM_SHOWWINDOW:
        {
            if (wParam) {
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_SHOWN, 0, 0);
            } else {
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_HIDDEN, 0, 0);
            }
        }
        break;

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
    case WM_NCACTIVATE:
        {
            /* Don't immediately clip the cursor in case we're clicking minimize/maximize buttons */
            data->skip_update_clipcursor = SDL_TRUE;

            /* Update the focus here, since it's possible to get WM_ACTIVATE and WM_SETFOCUS without
               actually being the foreground window, but this appears to get called in all cases where
               the global foreground window changes to and from this window. */
            WIN_UpdateFocus(data->window, !!wParam);
            WIN_UpdateWindowICCProfile(data->window, SDL_TRUE);
        }
        break;

    case WM_ACTIVATE:
        {
            /* Update the focus in case we changed focus to a child window and then away from the application */
            WIN_UpdateFocus(data->window, !!LOWORD(wParam));
        }
        break;

    case WM_SETFOCUS:
        {
            /* Update the focus in case it's changing between top-level windows in the same application */
            WIN_UpdateFocus(data->window, SDL_TRUE);
        }
        break;

    case WM_KILLFOCUS:
    case WM_ENTERIDLE:
        {
            /* Update the focus in case it's changing between top-level windows in the same application */
            WIN_UpdateFocus(data->window, SDL_FALSE);
        }
        break;

    case WM_POINTERUPDATE:
        {
            data->last_pointer_update = lParam;
            break;
        }

    case WM_MOUSEMOVE:
        {
            SDL_Mouse *mouse = SDL_GetMouse();

            if (!data->mouse_tracked) {
                TRACKMOUSEEVENT trackMouseEvent;

                trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
                trackMouseEvent.dwFlags = TME_LEAVE;
                trackMouseEvent.hwndTrack = data->hwnd;

                if (TrackMouseEvent(&trackMouseEvent)) {
                    data->mouse_tracked = SDL_TRUE;
                }
            }

            if (!mouse->relative_mode || mouse->relative_mode_warp) {
                /* Only generate mouse events for real mouse */
                if (GetMouseMessageSource() != SDL_MOUSE_EVENT_SOURCE_TOUCH &&
                    lParam != data->last_pointer_update) {
                    int x = GET_X_LPARAM(lParam);
                    int y = GET_Y_LPARAM(lParam);

                    WIN_ClientPointToSDL(data->window, &x, &y);

                    SDL_SendMouseMotion(data->window, 0, 0, x, y);
                }
            }
        }
        break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
        {
            SDL_Mouse *mouse = SDL_GetMouse();
            if (!mouse->relative_mode || mouse->relative_mode_warp) {
                if (GetMouseMessageSource() != SDL_MOUSE_EVENT_SOURCE_TOUCH &&
                    lParam != data->last_pointer_update) {
                    WIN_CheckWParamMouseButtons(wParam, data, 0);
                }
            }
        }
        break;

    case WM_INPUT:
        {
            SDL_Mouse *mouse = SDL_GetMouse();
            HRAWINPUT hRawInput = (HRAWINPUT)lParam;
            RAWINPUT inp;
            UINT size = sizeof(inp);

            /* We only use raw mouse input in relative mode */
            if (!mouse->relative_mode || mouse->relative_mode_warp) {
                break;
            }

            /* Relative mouse motion is delivered to the window with keyboard focus */
            if (data->window != SDL_GetKeyboardFocus()) {
                break;
            }

            GetRawInputData(hRawInput, RID_INPUT, &inp, &size, sizeof(RAWINPUTHEADER));

            /* Mouse data (ignoring synthetic mouse events generated for touchscreens) */
            if (inp.header.dwType == RIM_TYPEMOUSE) {
                SDL_MouseID mouseID;
                RAWMOUSE* rawmouse;
                if (SDL_GetNumTouchDevices() > 0 &&
                    (GetMouseMessageSource() == SDL_MOUSE_EVENT_SOURCE_TOUCH || (GetMessageExtraInfo() & 0x82) == 0x82)) {
                    break;
                }
                /* We do all of our mouse state checking against mouse ID 0
                 * We would only use the actual hDevice if we were tracking
                 * all mouse motion independently, and never using mouse ID 0.
                 */
                mouseID = 0; /* (SDL_MouseID)(uintptr_t)inp.header.hDevice; */
                rawmouse = &inp.data.mouse;

                if ((rawmouse->usFlags & 0x01) == MOUSE_MOVE_RELATIVE) {
                    SDL_SendMouseMotion(data->window, mouseID, 1, (int)rawmouse->lLastX, (int)rawmouse->lLastY);
                } else if (rawmouse->lLastX || rawmouse->lLastY) {
                    /* This is absolute motion, either using a tablet or mouse over RDP

                       Notes on how RDP appears to work, as of Windows 10 2004:
                        - SetCursorPos() calls are cached, with multiple calls coalesced into a single call that's sent to the RDP client. If the last call to SetCursorPos() has the same value as the last one that was sent to the client, it appears to be ignored and not sent. This means that we need to jitter the SetCursorPos() position slightly in order for the recentering to work correctly.
                        - User mouse motion is coalesced with SetCursorPos(), so the WM_INPUT positions we see will not necessarily match the positon we requested with SetCursorPos().
                        - SetCursorPos() outside of the bounds of the focus window appears not to do anything.
                        - SetCursorPos() while the cursor is NULL doesn't do anything

                       We handle this by creating a safe area within the application window, and when the mouse leaves that safe area, we warp back to the opposite side. Any single motion > 50% of the safe area is assumed to be a warp and ignored.
                    */
                    SDL_bool remote_desktop = GetSystemMetrics(SM_REMOTESESSION) ? SDL_TRUE : SDL_FALSE;
                    SDL_bool virtual_desktop = (rawmouse->usFlags & MOUSE_VIRTUAL_DESKTOP) ? SDL_TRUE : SDL_FALSE;
                    SDL_bool normalized_coordinates = ((rawmouse->usFlags & 0x40) == 0) ? SDL_TRUE : SDL_FALSE;
                    int w = GetSystemMetrics(virtual_desktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
                    int h = GetSystemMetrics(virtual_desktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);
                    int x = normalized_coordinates ? (int)(((float)rawmouse->lLastX / 65535.0f) * w) : (int)rawmouse->lLastX;
                    int y = normalized_coordinates ? (int)(((float)rawmouse->lLastY / 65535.0f) * h) : (int)rawmouse->lLastY;
                    int relX, relY;

                    /* Calculate relative motion */
                    if (data->last_raw_mouse_position.x == 0 && data->last_raw_mouse_position.y == 0) {
                        data->last_raw_mouse_position.x = x;
                        data->last_raw_mouse_position.y = y;
                    }
                    relX = (int)(x - data->last_raw_mouse_position.x);
                    relY = (int)(y - data->last_raw_mouse_position.y);

                    if (remote_desktop) {
                        if (!data->in_title_click && !data->focus_click_pending) {
                            static int wobble;
                            float floatX = (float)x / w;
                            float floatY = (float)y / h;

                            /* See if the mouse is at the edge of the screen, or in the RDP title bar area */
                            if (floatX <= 0.01f || floatX >= 0.99f || floatY <= 0.01f || floatY >= 0.99f || y < 32) {
                                /* Wobble the cursor position so it's not ignored if the last warp didn't have any effect */
                                RECT rect = data->cursor_clipped_rect;
                                int warpX = rect.left + ((rect.right - rect.left) / 2) + wobble;
                                int warpY = rect.top + ((rect.bottom - rect.top) / 2);

                                WIN_SetCursorPos(warpX, warpY);

                                ++wobble;
                                if (wobble > 1) {
                                    wobble = -1;
                                }
                            } else {
                                /* Send relative motion if we didn't warp last frame (had good position data)
                                   We also sometimes get large deltas due to coalesced mouse motion and warping,
                                   so ignore those.
                                 */
                                const int MAX_RELATIVE_MOTION = (h / 6);
                                if (SDL_abs(relX) < MAX_RELATIVE_MOTION &&
                                    SDL_abs(relY) < MAX_RELATIVE_MOTION) {
                                    SDL_SendMouseMotion(data->window, mouseID, 1, relX, relY);
                                }
                            }
                        }
                    } else {
                        const int MAXIMUM_TABLET_RELATIVE_MOTION = 32;
                        if (SDL_abs(relX) > MAXIMUM_TABLET_RELATIVE_MOTION ||
                            SDL_abs(relY) > MAXIMUM_TABLET_RELATIVE_MOTION) {
                            /* Ignore this motion, probably a pen lift and drop */
                        } else {
                            SDL_SendMouseMotion(data->window, mouseID, 1, relX, relY);
                        }
                    }

                    data->last_raw_mouse_position.x = x;
                    data->last_raw_mouse_position.y = y;
                }
                WIN_CheckRawMouseButtons(rawmouse->usButtonFlags, data, mouseID);
            }
        }
        break;

    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
        {
            short amount = GET_WHEEL_DELTA_WPARAM(wParam);
            float fAmount = (float) amount / WHEEL_DELTA;
            if (msg == WM_MOUSEWHEEL)
                SDL_SendMouseWheel(data->window, 0, 0.0f, fAmount, SDL_MOUSEWHEEL_NORMAL);
            else
                SDL_SendMouseWheel(data->window, 0, fAmount, 0.0f, SDL_MOUSEWHEEL_NORMAL);
        }
        break;

    case WM_MOUSELEAVE:
        if (!(data->window->flags & SDL_WINDOW_MOUSE_CAPTURE)) {
            if (SDL_GetMouseFocus() == data->window && !SDL_GetMouse()->relative_mode && !IsIconic(hwnd)) {
                SDL_Mouse *mouse;
                POINT cursorPos;
                GetCursorPos(&cursorPos);
                ScreenToClient(hwnd, &cursorPos);
                mouse = SDL_GetMouse();
                if (!mouse->was_touch_mouse_events) { /* we're not a touch handler causing a mouse leave? */
                    SDL_SendMouseMotion(data->window, 0, 0, cursorPos.x, cursorPos.y);
                } else { /* touch handling? */
                    mouse->was_touch_mouse_events = SDL_FALSE; /* not anymore */
                    if (mouse->touch_mouse_events) { /* convert touch to mouse events */
                        SDL_SendMouseMotion(data->window, SDL_TOUCH_MOUSEID, 0, cursorPos.x, cursorPos.y);
                    } else { /* normal handling */
                        SDL_SendMouseMotion(data->window, 0, 0, cursorPos.x, cursorPos.y);
                    }
               }
            }

            if (!SDL_GetMouse()->relative_mode) {
                /* When WM_MOUSELEAVE is fired we can be assured that the cursor has left the window */
                SDL_SetMouseFocus(NULL);
            }
        }

        /* Once we get WM_MOUSELEAVE we're guaranteed that the window is no longer tracked */
        data->mouse_tracked = SDL_FALSE;

        returnCode = 0;
        break;
#endif /*!defined(__XBOXONE__) && !defined(__XBOXSERIES__)*/

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        {
            SDL_Scancode code = WindowsScanCodeToSDLScanCode(lParam, wParam);
            const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

            /* Detect relevant keyboard shortcuts */
            if (keyboardState[SDL_SCANCODE_LALT] == SDL_PRESSED || keyboardState[SDL_SCANCODE_RALT] == SDL_PRESSED) {
                /* ALT+F4: Close window */
                if (code == SDL_SCANCODE_F4 && ShouldGenerateWindowCloseOnAltF4()) {
                    SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_CLOSE, 0, 0);
                }
            }

            if (code != SDL_SCANCODE_UNKNOWN) {
                SDL_SendKeyboardKey(SDL_PRESSED, code);
            }
        }

        returnCode = 0;
        break;

    case WM_SYSKEYUP:
    case WM_KEYUP:
        {
            SDL_Scancode code = WindowsScanCodeToSDLScanCode(lParam, wParam);
            const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

            if (code != SDL_SCANCODE_UNKNOWN) {
                if (code == SDL_SCANCODE_PRINTSCREEN &&
                    keyboardState[code] == SDL_RELEASED) {
                    SDL_SendKeyboardKey(SDL_PRESSED, code);
                }
                SDL_SendKeyboardKey(SDL_RELEASED, code);
            }
        }
        returnCode = 0;
        break;

    case WM_UNICHAR:
        if (wParam == UNICODE_NOCHAR) {
            returnCode = 1;
        } else {
            char text[5];
            if (WIN_ConvertUTF32toUTF8((UINT32)wParam, text)) {
                SDL_SendKeyboardText(text);
            }
            returnCode = 0;
        }
        break;

    case WM_CHAR:
        /* When a user enters a Unicode code point defined in the Basic Multilingual Plane, Windows sends a WM_CHAR
           message with the code point encoded as UTF-16. When a user enters a Unicode code point from a Supplementary
           Plane, Windows sends the code point in two separate WM_CHAR messages: The first message includes the UTF-16
           High Surrogate and the second the UTF-16 Low Surrogate. The High and Low Surrogates cannot be individually
           converted to valid UTF-8, therefore, we must save the High Surrogate from the first WM_CHAR message and
           concatenate it with the Low Surrogate from the second WM_CHAR message. At that point, we have a valid
           UTF-16 surrogate pair ready to re-encode as UTF-8. */
        if (IS_HIGH_SURROGATE(wParam)) {
            data->high_surrogate = (WCHAR)wParam;
        } else if (IS_SURROGATE_PAIR(data->high_surrogate, wParam)) {
            /* The code point is in a Supplementary Plane.
               Here wParam is the Low Surrogate. */
            char text[5];
            if (WIN_ConvertUTF16toUTF8((UINT32)data->high_surrogate, (UINT32)wParam, text)) {
                SDL_SendKeyboardText(text);
            }
            data->high_surrogate = 0;
        } else {
            /* The code point is in the Basic Multilingual Plane.
               It's numerically equal to UTF-32. */
            char text[5];
            if (WIN_ConvertUTF32toUTF8((UINT32)wParam, text)) {
                SDL_SendKeyboardText(text);
            }
        }
        returnCode = 0;
        break;

#if !defined(__XBOXONE__) && !defined(__XBOXSERIES__)
#ifdef WM_INPUTLANGCHANGE
    case WM_INPUTLANGCHANGE:
        {
            WIN_UpdateKeymap(SDL_TRUE);
        }
        returnCode = 1;
        break;
#endif /* WM_INPUTLANGCHANGE */

    case WM_NCLBUTTONDOWN:
        {
            data->in_title_click = SDL_TRUE;
        }
        break;

    case WM_CAPTURECHANGED:
        {
            data->in_title_click = SDL_FALSE;

            /* The mouse may have been released during a modal loop */
            WIN_CheckAsyncMouseRelease(data);
        }
        break;

#ifdef WM_GETMINMAXINFO
    case WM_GETMINMAXINFO:
        {
            MINMAXINFO *info;
            RECT size;
            int x, y;
            int w, h;
            int min_w, min_h;
            int max_w, max_h;
            BOOL constrain_max_size;

            if (SDL_IsShapedWindow(data->window)) {
                Win32_ResizeWindowShape(data->window);
            }

            /* If this is an expected size change, allow it */
            if (data->expected_resize) {
                break;
            }

            /* Get the current position of our window */
            GetWindowRect(hwnd, &size);
            x = size.left;
            y = size.top;

            /* Calculate current size of our window */
            SDL_GetWindowSize(data->window, &w, &h);
            SDL_GetWindowMinimumSize(data->window, &min_w, &min_h);
            SDL_GetWindowMaximumSize(data->window, &max_w, &max_h);

            /* Convert w, h, min_w, min_h, max_w, max_h from dpi-scaled points to pixels,
               treating them as coordinates within the client area. */
            WIN_ClientPointFromSDL(data->window, &w, &h);
            WIN_ClientPointFromSDL(data->window, &min_w, &min_h);
            WIN_ClientPointFromSDL(data->window, &max_w, &max_h);

            /* Store in min_w and min_h difference between current size and minimal
               size so we don't need to call AdjustWindowRectEx twice */
            min_w -= w;
            min_h -= h;
            if (max_w && max_h) {
                max_w -= w;
                max_h -= h;
                constrain_max_size = TRUE;
            } else {
                constrain_max_size = FALSE;
            }

            if (!(SDL_GetWindowFlags(data->window) & SDL_WINDOW_BORDERLESS)) {
                LONG style = GetWindowLong(hwnd, GWL_STYLE);
                /* DJM - according to the docs for GetMenu(), the
                   return value is undefined if hwnd is a child window.
                   Apparently it's too difficult for MS to check
                   inside their function, so I have to do it here.
                 */
                BOOL menu = (style & WS_CHILDWINDOW) ? FALSE : (GetMenu(hwnd) != NULL);
                UINT dpi;

                dpi = 96;
                size.top = 0;
                size.left = 0;
                size.bottom = h;
                size.right = w;

                if (WIN_IsPerMonitorV2DPIAware(SDL_GetVideoDevice())) {
                    dpi = data->videodata->GetDpiForWindow(hwnd);
                    data->videodata->AdjustWindowRectExForDpi(&size, style, menu, 0, dpi);
                } else {
                    AdjustWindowRectEx(&size, style, menu, 0);
                }
                w = size.right - size.left;
                h = size.bottom - size.top;
#ifdef HIGHDPI_DEBUG
                SDL_Log("WM_GETMINMAXINFO: max window size: %dx%d using dpi: %u", w, h, dpi);
#endif
            }

            /* Fix our size to the current size */
            info = (MINMAXINFO *) lParam;
            if (SDL_GetWindowFlags(data->window) & SDL_WINDOW_RESIZABLE) {
                if (SDL_GetWindowFlags(data->window) & SDL_WINDOW_BORDERLESS) {
                    int screenW = GetSystemMetrics(SM_CXSCREEN);
                    int screenH = GetSystemMetrics(SM_CYSCREEN);
                    info->ptMaxSize.x = SDL_max(w, screenW);
                    info->ptMaxSize.y = SDL_max(h, screenH);
                    info->ptMaxPosition.x = SDL_min(0, ((screenW - w) / 2));
                    info->ptMaxPosition.y = SDL_min(0, ((screenH - h) / 2));
                }
                info->ptMinTrackSize.x = w + min_w;
                info->ptMinTrackSize.y = h + min_h;
                if (constrain_max_size) {
                    info->ptMaxTrackSize.x = w + max_w;
                    info->ptMaxTrackSize.y = h + max_h;
                }
            } else {
                info->ptMaxSize.x = w;
                info->ptMaxSize.y = h;
                info->ptMaxPosition.x = x;
                info->ptMaxPosition.y = y;
                info->ptMinTrackSize.x = w;
                info->ptMinTrackSize.y = h;
                info->ptMaxTrackSize.x = w;
                info->ptMaxTrackSize.y = h;
            }
        }
        returnCode = 0;
        break;
#endif /* WM_GETMINMAXINFO */

    case WM_WINDOWPOSCHANGING:

        if (data->expected_resize) {
            returnCode = 0;
        }
        break;

    case WM_WINDOWPOSCHANGED:
        {
            RECT rect;
            int x, y;
            int w, h;

            if (data->initializing || data->in_border_change) {
                break;
            }

            /* When the window is minimized it's resized to the dock icon size, ignore this */
            if (IsIconic(hwnd)) {
                break;
            }

            if (!GetClientRect(hwnd, &rect) || IsRectEmpty(&rect)) {
                break;
            }
            ClientToScreen(hwnd, (LPPOINT) & rect);
            ClientToScreen(hwnd, (LPPOINT) & rect + 1);

            WIN_UpdateClipCursor(data->window);

            x = rect.left;
            y = rect.top;
            WIN_ScreenPointToSDL(&x, &y);

            SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_MOVED, x, y);

            /* Convert client area width/height from pixels to dpi-scaled points */
            w = rect.right - rect.left;
            h = rect.bottom - rect.top;
            WIN_ClientPointToSDL(data->window, &w, &h);

            SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_RESIZED, w, h);

#ifdef HIGHDPI_DEBUG
            SDL_Log("WM_WINDOWPOSCHANGED: Windows client rect (pixels): (%d, %d) (%d x %d)\tSDL client rect (points): (%d, %d) (%d x %d) cached dpi %d, windows reported dpi %d",
                rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                x, y, w, h, data->scaling_dpi, data->videodata->GetDpiForWindow ? data->videodata->GetDpiForWindow(data->hwnd) : 0);
#endif

            /* Forces a WM_PAINT event */
            InvalidateRect(hwnd, NULL, FALSE);

            WIN_UpdateWindowICCProfile(data->window, SDL_TRUE);
        }
        break;

    case WM_SIZE:
        {
            switch (wParam) {
            case SIZE_MAXIMIZED:
                SDL_SendWindowEvent(data->window,
                    SDL_WINDOWEVENT_RESTORED, 0, 0);
                SDL_SendWindowEvent(data->window,
                    SDL_WINDOWEVENT_MAXIMIZED, 0, 0);
                break;
            case SIZE_MINIMIZED:
                SDL_SendWindowEvent(data->window,
                    SDL_WINDOWEVENT_MINIMIZED, 0, 0);
                break;
            case SIZE_RESTORED:
                SDL_SendWindowEvent(data->window,
                    SDL_WINDOWEVENT_RESTORED, 0, 0);
                break;
            default:
                break;
            }
        }
        break;

    case WM_SETCURSOR:
        {
            Uint16 hittest;

            hittest = LOWORD(lParam);
            if (hittest == HTCLIENT) {
                SetCursor(SDL_cursor);
                returnCode = TRUE;
            } else if (!g_WindowFrameUsableWhileCursorHidden && !SDL_cursor) {
                SetCursor(NULL);
                returnCode = TRUE;
            }
        }
        break;

        /* We were occluded, refresh our display */
    case WM_PAINT:
        {
            RECT rect;
            if (GetUpdateRect(hwnd, &rect, FALSE)) {
                ValidateRect(hwnd, NULL);
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_EXPOSED, 0, 0);
            }
        }
        returnCode = 0;
        break;

        /* We'll do our own drawing, prevent flicker */
    case WM_ERASEBKGND:
        {
        }
        return (1);

    case WM_SYSCOMMAND:
        {
            if ((wParam & 0xFFF0) == SC_KEYMENU) {
                return (0);
            }

#if defined(SC_SCREENSAVE) || defined(SC_MONITORPOWER)
            /* Don't start the screensaver or blank the monitor in fullscreen apps */
            if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
                (wParam & 0xFFF0) == SC_MONITORPOWER) {
                if (SDL_GetVideoDevice()->suspend_screensaver) {
                    return (0);
                }
            }
#endif /* System has screensaver support */
        }
        break;

    case WM_CLOSE:
        {
            SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_CLOSE, 0, 0);
        }
        returnCode = 0;
        break;

    case WM_TOUCH:
        if (data->videodata->GetTouchInputInfo && data->videodata->CloseTouchInputHandle) {
            UINT i, num_inputs = LOWORD(wParam);
            SDL_bool isstack;
            PTOUCHINPUT inputs = SDL_small_alloc(TOUCHINPUT, num_inputs, &isstack);
            if (data->videodata->GetTouchInputInfo((HTOUCHINPUT)lParam, num_inputs, inputs, sizeof(TOUCHINPUT))) {
                RECT rect;
                float x, y;

                if (!GetClientRect(hwnd, &rect) ||
                    (rect.right == rect.left && rect.bottom == rect.top)) {
                    if (inputs) {
                        SDL_small_free(inputs, isstack);
                    }
                    break;
                }
                ClientToScreen(hwnd, (LPPOINT) & rect);
                ClientToScreen(hwnd, (LPPOINT) & rect + 1);
                rect.top *= 100;
                rect.left *= 100;
                rect.bottom *= 100;
                rect.right *= 100;

                for (i = 0; i < num_inputs; ++i) {
                    PTOUCHINPUT input = &inputs[i];

                    const SDL_TouchID touchId = (SDL_TouchID)((size_t)input->hSource);

                    /* TODO: Can we use GetRawInputDeviceInfo and HID info to
                       determine if this is a direct or indirect touch device?
                     */
                    if (SDL_AddTouch(touchId, SDL_TOUCH_DEVICE_DIRECT, "") < 0) {
                        continue;
                    }

                    /* Get the normalized coordinates for the window */
                    x = (float)(input->x - rect.left)/(rect.right - rect.left);
                    y = (float)(input->y - rect.top)/(rect.bottom - rect.top);

                    if (input->dwFlags & TOUCHEVENTF_DOWN) {
                        SDL_SendTouch(touchId, input->dwID, data->window, SDL_TRUE, x, y, 1.0f);
                    }
                    if (input->dwFlags & TOUCHEVENTF_MOVE) {
                        SDL_SendTouchMotion(touchId, input->dwID, data->window, x, y, 1.0f);
                    }
                    if (input->dwFlags & TOUCHEVENTF_UP) {
                        SDL_SendTouch(touchId, input->dwID, data->window, SDL_FALSE, x, y, 1.0f);
                    }
                }
            }
            SDL_small_free(inputs, isstack);

            data->videodata->CloseTouchInputHandle((HTOUCHINPUT)lParam);
            return 0;
        }
        break;

#if HAVE_TPCSHRD_H

    case WM_TABLET_QUERYSYSTEMGESTURESTATUS:
        /* See https://msdn.microsoft.com/en-us/library/windows/desktop/bb969148(v=vs.85).aspx .
         * If we're handling our own touches, we don't want any gestures.
         * Not all of these settings are documented.
         * The use of the undocumented ones was suggested by https://github.com/bjarkeck/GCGJ/blob/master/Monogame/Windows/WinFormsGameForm.cs . */
        return TABLET_DISABLE_PRESSANDHOLD |    /*  disables press and hold (right-click) gesture */
            TABLET_DISABLE_PENTAPFEEDBACK |     /*  disables UI feedback on pen up (waves) */
            TABLET_DISABLE_PENBARRELFEEDBACK |  /*  disables UI feedback on pen button down (circle) */
            TABLET_DISABLE_TOUCHUIFORCEON |
            TABLET_DISABLE_TOUCHUIFORCEOFF |
            TABLET_DISABLE_TOUCHSWITCH |
            TABLET_DISABLE_FLICKS |             /*  disables pen flicks (back, forward, drag down, drag up) */
            TABLET_DISABLE_SMOOTHSCROLLING |
            TABLET_DISABLE_FLICKFALLBACKKEYS;

#endif /* HAVE_TPCSHRD_H */

    case WM_DROPFILES:
        {
            UINT i;
            HDROP drop = (HDROP) wParam;
            UINT count = DragQueryFile(drop, 0xFFFFFFFF, NULL, 0);
            for (i = 0; i < count; ++i) {
                SDL_bool isstack;
                UINT size = DragQueryFile(drop, i, NULL, 0) + 1;
                LPTSTR buffer = SDL_small_alloc(TCHAR, size, &isstack);
                if (buffer) {
                    if (DragQueryFile(drop, i, buffer, size)) {
                        char *file = WIN_StringToUTF8(buffer);
                        SDL_SendDropFile(data->window, file);
                        SDL_free(file);
                    }
                    SDL_small_free(buffer, isstack);
                }
            }
            SDL_SendDropComplete(data->window);
            DragFinish(drop);
            return 0;
        }
        break;

    case WM_DISPLAYCHANGE:
        {
            // Reacquire displays if any were added or removed
            WIN_RefreshDisplays(SDL_GetVideoDevice());
        }
        break;

    case WM_NCCALCSIZE:
        {
            Uint32 window_flags = SDL_GetWindowFlags(data->window);
            if (wParam == TRUE && (window_flags & SDL_WINDOW_BORDERLESS) && !(window_flags & SDL_WINDOW_FULLSCREEN)) {
                /* When borderless, need to tell windows that the size of the non-client area is 0 */
                if (!(window_flags & SDL_WINDOW_RESIZABLE)) {
                    int w, h;
                    NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS *)lParam;
                    w = data->window->windowed.w;
                    h = data->window->windowed.h;
                    WIN_ClientPointFromSDL(data->window, &w, &h);
                    params->rgrc[0].right = params->rgrc[0].left + w;
                    params->rgrc[0].bottom = params->rgrc[0].top + h;
                }
                return 0;
            }
        }
        break;

    case WM_NCHITTEST:
        {
            SDL_Window *window = data->window;
            if (window->hit_test) {
                POINT winpoint;
                winpoint.x = GET_X_LPARAM(lParam);
                winpoint.y = GET_Y_LPARAM(lParam);
                if (ScreenToClient(hwnd, &winpoint)) {
                    SDL_Point point;
                    SDL_HitTestResult rc;
                    point.x = winpoint.x;
                    point.y = winpoint.y;
                    WIN_ClientPointToSDL(data->window, &point.x, &point.y);
                    rc = window->hit_test(window, &point, window->hit_test_data);
                    switch (rc) {
                        #define POST_HIT_TEST(ret) { SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_HIT_TEST, 0, 0); return ret; }
                        case SDL_HITTEST_DRAGGABLE: POST_HIT_TEST(HTCAPTION);
                        case SDL_HITTEST_RESIZE_TOPLEFT: POST_HIT_TEST(HTTOPLEFT);
                        case SDL_HITTEST_RESIZE_TOP: POST_HIT_TEST(HTTOP);
                        case SDL_HITTEST_RESIZE_TOPRIGHT: POST_HIT_TEST(HTTOPRIGHT);
                        case SDL_HITTEST_RESIZE_RIGHT: POST_HIT_TEST(HTRIGHT);
                        case SDL_HITTEST_RESIZE_BOTTOMRIGHT: POST_HIT_TEST(HTBOTTOMRIGHT);
                        case SDL_HITTEST_RESIZE_BOTTOM: POST_HIT_TEST(HTBOTTOM);
                        case SDL_HITTEST_RESIZE_BOTTOMLEFT: POST_HIT_TEST(HTBOTTOMLEFT);
                        case SDL_HITTEST_RESIZE_LEFT: POST_HIT_TEST(HTLEFT);
                        #undef POST_HIT_TEST
                        case SDL_HITTEST_NORMAL: return HTCLIENT;
                    }
                }
                /* If we didn't return, this will call DefWindowProc below. */
            }
        }
        break;

    case WM_GETDPISCALEDSIZE:
        /* Windows 10 Creators Update+ */
        /* Documented as only being sent to windows that are per-monitor V2 DPI aware.

           Experimentation shows it's only sent during interactive dragging, not in response to
           SetWindowPos. */
        if (data->videodata->GetDpiForWindow && data->videodata->AdjustWindowRectExForDpi) {
            /* Windows expects applications to scale their window rects linearly
               when dragging between monitors with different DPI's.
               e.g. a 100x100 window dragged to a 200% scaled monitor
               becomes 200x200.

               For SDL, we instead want the client size to scale linearly.
               This is not the same as the window rect scaling linearly,
               because Windows doesn't scale the non-client area (titlebar etc.)
               linearly. So, we need to handle this message to request custom
               scaling. */

            const int nextDPI = (int)wParam;
            const int prevDPI = (int)data->videodata->GetDpiForWindow(hwnd);
            SIZE *sizeInOut = (SIZE *)lParam;

            int frame_w, frame_h;
            int query_client_w_win, query_client_h_win;

            const DWORD style = GetWindowLong(hwnd, GWL_STYLE);
            const BOOL menu = (style & WS_CHILDWINDOW) ? FALSE : (GetMenu(hwnd) != NULL);

#ifdef HIGHDPI_DEBUG
            SDL_Log("WM_GETDPISCALEDSIZE: current DPI: %d potential DPI: %d input size: (%dx%d)",
                    prevDPI, nextDPI, sizeInOut->cx, sizeInOut->cy);
#endif

            /* Subtract the window frame size that would have been used at prevDPI */
            {
                RECT rect = {0};

                if (!(data->window->flags & SDL_WINDOW_BORDERLESS)) {
                    data->videodata->AdjustWindowRectExForDpi(&rect, style, menu, 0, prevDPI);
                }

                frame_w = -rect.left + rect.right;
                frame_h = -rect.top + rect.bottom;

                query_client_w_win = sizeInOut->cx - frame_w;
                query_client_h_win = sizeInOut->cy - frame_h;
            }

            /* Convert to new dpi if we are using scaling.
             * Otherwise leave as pixels.
             */
            if (data->videodata->dpi_scaling_enabled) {
                query_client_w_win = MulDiv(query_client_w_win, nextDPI, prevDPI);
                query_client_h_win = MulDiv(query_client_h_win, nextDPI, prevDPI);
            }

            /* Add the window frame size that would be used at nextDPI */
            {
                RECT rect = {0};
                rect.right = query_client_w_win;
                rect.bottom = query_client_h_win;

                if (!(data->window->flags & SDL_WINDOW_BORDERLESS)) {
                    data->videodata->AdjustWindowRectExForDpi(&rect, style, menu, 0, nextDPI);
                }

                /* This is supposed to control the suggested rect param of WM_DPICHANGED */
                sizeInOut->cx = rect.right - rect.left;
                sizeInOut->cy = rect.bottom - rect.top;
            }

#ifdef HIGHDPI_DEBUG
            SDL_Log("WM_GETDPISCALEDSIZE: output size: (%dx%d)", sizeInOut->cx, sizeInOut->cy);
#endif
            return TRUE;
        }
        break;

    case WM_DPICHANGED:
        /* Windows 8.1+ */
        {
            const int newDPI = HIWORD(wParam);
            RECT* const suggestedRect = (RECT*)lParam;
            int w, h;

#ifdef HIGHDPI_DEBUG
            SDL_Log("WM_DPICHANGED: to %d\tsuggested rect: (%d, %d), (%dx%d)\n", newDPI,
                suggestedRect->left, suggestedRect->top, suggestedRect->right - suggestedRect->left, suggestedRect->bottom - suggestedRect->top);
#endif

            if (data->expected_resize) {
                /* This DPI change is coming from an explicit SetWindowPos call within SDL.
                   Assume all call sites are calculating the DPI-aware frame correctly, so
                   we don't need to do any further adjustment. */

                if (data->videodata->dpi_scaling_enabled) {
                    /* Update the cached DPI value for this window */
                    data->scaling_dpi = newDPI;

                    /* Send a SDL_WINDOWEVENT_SIZE_CHANGED saying that the client size (in dpi-scaled points) is unchanged.
                       Renderers need to get this to know that the framebuffer size changed.

                       We clear the window size to force the event to be delivered, but what we really
                       want for SDL3 is a new event to notify that the DPI changed and then watch for
                       that in the renderer directly.
                     */
                    data->window->w = 0;
                    data->window->h = 0;
                    SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_SIZE_CHANGED, data->window->w, data->window->h);
                }

#ifdef HIGHDPI_DEBUG
                SDL_Log("WM_DPICHANGED: Doing nothing, assuming window is already sized correctly");
#endif
                return 0;
            }

            /* Interactive user-initiated resizing/movement */

            if (WIN_IsPerMonitorV2DPIAware(SDL_GetVideoDevice())) {
                /* WM_GETDPISCALEDSIZE should have been called prior, so we can trust the given
                   suggestedRect. */
                w = suggestedRect->right - suggestedRect->left;
                h = suggestedRect->bottom - suggestedRect->top;

#ifdef HIGHDPI_DEBUG
                SDL_Log("WM_DPICHANGED: using suggestedRect");
#endif
            } else {
                /* permonitor and earlier DPI awareness: calculate the new frame w/h such that
                   the client area size is maintained. */
                const DWORD style = GetWindowLong(hwnd, GWL_STYLE);
                const BOOL menu = (style & WS_CHILDWINDOW) ? FALSE : (GetMenu(hwnd) != NULL);

                RECT rect = {0};
                rect.right = data->window->w;
                rect.bottom = data->window->h;

                if (data->videodata->dpi_scaling_enabled) {
                    /* scale client size to from points to the new DPI */
                    rect.right = MulDiv(rect.right, newDPI, 96);
                    rect.bottom = MulDiv(rect.bottom, newDPI, 96);
                }

                if (!(data->window->flags & SDL_WINDOW_BORDERLESS)) {
                    AdjustWindowRectEx(&rect, style, menu, 0);
                }

                w = rect.right - rect.left;
                h = rect.bottom - rect.top;
            }

#ifdef HIGHDPI_DEBUG
            SDL_Log("WM_DPICHANGED: current SDL window size: (%dx%d)\tcalling SetWindowPos: (%d, %d), (%dx%d)\n",
                data->window->w, data->window->h,
                suggestedRect->left, suggestedRect->top, w, h);
#endif

            data->expected_resize = SDL_TRUE;
            SetWindowPos(hwnd,
                NULL,
                suggestedRect->left,
                suggestedRect->top,
                w,
                h,
                SWP_NOZORDER | SWP_NOACTIVATE);
            data->expected_resize = SDL_FALSE;

            if (data->videodata->dpi_scaling_enabled) {
                /* Update the cached DPI value for this window */
                data->scaling_dpi = newDPI;

                /* Send a SDL_WINDOWEVENT_SIZE_CHANGED saying that the client size (in dpi-scaled points) is unchanged.
                   Renderers need to get this to know that the framebuffer size changed. */
                SDL_SendWindowEvent(data->window, SDL_WINDOWEVENT_SIZE_CHANGED, data->window->w, data->window->h);
            }

            return 0;
        }
        break;
#endif /*!defined(__XBOXONE__) && !defined(__XBOXSERIES__)*/
    }

    /* If there's a window proc, assume it's going to handle messages */
    if (data->wndproc) {
        return CallWindowProc(data->wndproc, hwnd, msg, wParam, lParam);
    } else if (returnCode >= 0) {
        return returnCode;
    } else {
        return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
    }
}

