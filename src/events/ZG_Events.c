#include "@zg_events.h"

/* @{ */
typedef enum
{
    SDL_ADDEVENT,
    SDL_PEEKEVENT,
    SDL_GETEVENT
} SDL_eventaction;

static struct
{
    SDL_mutex *lock;
    SDL_atomic_t active;
    SDL_atomic_t count;
    int max_events_seen;
    SDL_EventEntry *head;
    SDL_EventEntry *tail;
    SDL_EventEntry *free;
    SDL_SysWMEntry *wmmsg_used;
    SDL_SysWMEntry *wmmsg_free;
} SDL_EventQ = { NULL, { 1 }, { 0 }, 0, NULL, NULL, NULL, NULL, NULL };

/* Lock the event queue, take a peep at it, and unlock it */
static int
SDL_PeepEventsInternal(SDL_Event * events, int numevents, SDL_eventaction action,
               Uint32 minType, Uint32 maxType, SDL_bool include_sentinel)
{
    int i, used, sentinels_expected = 0;

    /* Don't look after we've quit */
    if (!SDL_AtomicGet(&SDL_EventQ.active)) {
        /* We get a few spurious events at shutdown, so don't warn then */
        if (action == SDL_GETEVENT) {
            SDL_SetError("The event system has been shut down");
        }
        return (-1);
    }
    /* Lock the event queue */
    used = 0;
    if (!SDL_EventQ.lock || SDL_LockMutex(SDL_EventQ.lock) == 0) {
        if (action == SDL_ADDEVENT) {
            for (i = 0; i < numevents; ++i) {
                used += SDL_AddEvent(&events[i]);
            }
        } else {
            SDL_EventEntry *entry, *next;
            SDL_SysWMEntry *wmmsg, *wmmsg_next;
            Uint32 type;

            if (action == SDL_GETEVENT) {
                /* Clean out any used wmmsg data
                   FIXME: Do we want to retain the data for some period of time?
                 */
                for (wmmsg = SDL_EventQ.wmmsg_used; wmmsg; wmmsg = wmmsg_next) {
                    wmmsg_next = wmmsg->next;
                    wmmsg->next = SDL_EventQ.wmmsg_free;
                    SDL_EventQ.wmmsg_free = wmmsg;
                }
                SDL_EventQ.wmmsg_used = NULL;
            }

            for (entry = SDL_EventQ.head; entry && (!events || used < numevents); entry = next) {
                next = entry->next;
                type = entry->event.type;
                if (minType <= type && type <= maxType) {
                    if (events) {
                        events[used] = entry->event;
                        if (entry->event.type == SDL_SYSWMEVENT) {
                            /* We need to copy the wmmsg somewhere safe.
                               For now we'll guarantee it's valid at least until
                               the next call to SDL_PeepEvents()
                             */
                            if (SDL_EventQ.wmmsg_free) {
                                wmmsg = SDL_EventQ.wmmsg_free;
                                SDL_EventQ.wmmsg_free = wmmsg->next;
                            } else {
                                wmmsg = (SDL_SysWMEntry *)SDL_malloc(sizeof(*wmmsg));
                            }
                            wmmsg->msg = *entry->event.syswm.msg;
                            wmmsg->next = SDL_EventQ.wmmsg_used;
                            SDL_EventQ.wmmsg_used = wmmsg;
                            events[used].syswm.msg = &wmmsg->msg;
                        }

                        if (action == SDL_GETEVENT) {
                            SDL_CutEvent(entry);
                        }
                    }
                    if (type == SDL_POLLSENTINEL) {
                        /* Special handling for the sentinel event */
                        if (!include_sentinel) {
                            /* Skip it, we don't want to include it */
                            continue;
                        }
                        if (!events || action != SDL_GETEVENT) {
                            ++sentinels_expected;
                        }
                        if (SDL_AtomicGet(&SDL_sentinel_pending) > sentinels_expected) {
                            /* Skip it, there's another one pending */
                            continue;
                        }
                    }
                    ++used;
                }
            }
        }
        if (SDL_EventQ.lock) {
            SDL_UnlockMutex(SDL_EventQ.lock);
        }
    } else {
        return SDL_SetError("Couldn't lock event queue");
    }

    if (used > 0 && action == SDL_ADDEVENT) {
        SDL_SendWakeupEvent();
    }

    return (used);
}
int
SDL_PeepEvents(SDL_Event * events, int numevents, SDL_eventaction action,
               Uint32 minType, Uint32 maxType)
{
    return SDL_PeepEventsInternal(events, numevents, action, minType, maxType, SDL_FALSE);
}

// define a queue of events
/* Run the system dependent event loops */
void
SDL_PumpEvents(void)
{
    SDL_VideoDevice *_this = SDL_GetVideoDevice();

    /* Release any keys held down from last frame */
    SDL_ReleaseAutoReleaseKeys();

    /* Get events from the video subsystem */
    if (_this) {
        _this->PumpEvents(_this);
    }

#if !SDL_JOYSTICK_DISABLED
    /* Check for joystick state change */
    if (SDL_update_joysticks) {
        SDL_JoystickUpdate();
    }
#endif

#if !SDL_SENSOR_DISABLED
    /* Check for sensor state change */
    if (SDL_update_sensors) {
        SDL_SensorUpdate();
    }
#endif

    SDL_SendPendingSignalEvents();  /* in case we had a signal handler fire, etc. */
}

/* Public functions */

int
SDL_PollEvent(SDL_Event * event)
{
    return SDL_WaitEventTimeout(event, 0);
}

int
SDL_WaitEvent(SDL_Event * event)
{
    return SDL_WaitEventTimeout(event, -1);
}

int
SDL_WaitEventTimeout(SDL_Event * event, int timeout)
{
    Uint32 expiration = 0;

    if (timeout > 0)
        expiration = SDL_GetTicks() + timeout;

    for (;;) {
        SDL_PumpEvents();
        switch (SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)) {
        case -1:
            return 0;
        case 0:
            if (timeout == 0) {
                /* Polling and no events, just return */
                return 0;
            }
            if (timeout > 0 && SDL_TICKS_PASSED(SDL_GetTicks(), expiration)) {
                /* Timeout expired and no events */
                return 0;
            }
            SDL_Delay(1);
            break;
        default:
            /* Has events */
            return 1;
        }
    }
}

/* Public functions */

void
SDL_StopEventLoop(void)
{
    const char *report = SDL_GetHint("SDL_EVENT_QUEUE_STATISTICS");
    int i;
    SDL_EventEntry *entry;
    SDL_SysWMEntry *wmmsg;

    if (SDL_EventQ.lock) {
        SDL_LockMutex(SDL_EventQ.lock);
    }

    SDL_AtomicSet(&SDL_EventQ.active, 0);

    if (report && SDL_atoi(report)) {
        SDL_Log("SDL EVENT QUEUE: Maximum events in-flight: %d\n",
                SDL_EventQ.max_events_seen);
    }

    /* Clean out EventQ */
    for (entry = SDL_EventQ.head; entry; ) {
        SDL_EventEntry *next = entry->next;
        SDL_free(entry);
        entry = next;
    }
    for (entry = SDL_EventQ.free; entry; ) {
        SDL_EventEntry *next = entry->next;
        SDL_free(entry);
        entry = next;
    }
    for (wmmsg = SDL_EventQ.wmmsg_used; wmmsg; ) {
        SDL_SysWMEntry *next = wmmsg->next;
        SDL_free(wmmsg);
        wmmsg = next;
    }
    for (wmmsg = SDL_EventQ.wmmsg_free; wmmsg; ) {
        SDL_SysWMEntry *next = wmmsg->next;
        SDL_free(wmmsg);
        wmmsg = next;
    }

    SDL_AtomicSet(&SDL_EventQ.count, 0);
    SDL_EventQ.max_events_seen = 0;
    SDL_EventQ.head = NULL;
    SDL_EventQ.tail = NULL;
    SDL_EventQ.free = NULL;
    SDL_EventQ.wmmsg_used = NULL;
    SDL_EventQ.wmmsg_free = NULL;
    SDL_AtomicSet(&SDL_sentinel_pending, 0);

    /* Clear disabled event state */
    for (i = 0; i < SDL_arraysize(SDL_disabled_events); ++i) {
        SDL_free(SDL_disabled_events[i]);
        SDL_disabled_events[i] = NULL;
    }

    if (SDL_event_watchers_lock) {
        SDL_DestroyMutex(SDL_event_watchers_lock);
        SDL_event_watchers_lock = NULL;
    }
    if (SDL_event_watchers) {
        SDL_free(SDL_event_watchers);
        SDL_event_watchers = NULL;
        SDL_event_watchers_count = 0;
    }
    SDL_zero(SDL_EventOK);

    if (SDL_EventQ.lock) {
        SDL_UnlockMutex(SDL_EventQ.lock);
        SDL_DestroyMutex(SDL_EventQ.lock);
        SDL_EventQ.lock = NULL;
    }
}

/* This function (and associated calls) may be called more than once */
int
SDL_StartEventLoop(void)
{
    /* We'll leave the event queue alone, since we might have gotten
       some important events at launch (like SDL_DROPFILE)

       FIXME: Does this introduce any other bugs with events at startup?
     */

    /* Create the lock and set ourselves active */
#if !SDL_THREADS_DISABLED
    if (!SDL_EventQ.lock) {
        SDL_EventQ.lock = SDL_CreateMutex();
        if (SDL_EventQ.lock == NULL) {
            return -1;
        }
    }

    if (!SDL_event_watchers_lock) {
        SDL_event_watchers_lock = SDL_CreateMutex();
        if (SDL_event_watchers_lock == NULL) {
            return -1;
        }
    }
#endif /* !SDL_THREADS_DISABLED */

    /* Process most event types */
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_TEXTEDITING, SDL_DISABLE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_DISABLE);
#if 0 /* Leave these events enabled so apps can respond to items being dragged onto them at startup */
    SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
    SDL_EventState(SDL_DROPTEXT, SDL_DISABLE);
#endif

    SDL_AtomicSet(&SDL_EventQ.active, 1);

    return 0;
}


int
SDL_PushEvent(SDL_Event * event)
{
    event->common.timestamp = SDL_GetTicks();

    if (SDL_EventOK.callback || SDL_event_watchers_count > 0) {
        if (!SDL_event_watchers_lock || SDL_LockMutex(SDL_event_watchers_lock) == 0) {
            if (SDL_EventOK.callback && !SDL_EventOK.callback(SDL_EventOK.userdata, event)) {
                if (SDL_event_watchers_lock) {
                    SDL_UnlockMutex(SDL_event_watchers_lock);
                }
                return 0;
            }

            if (SDL_event_watchers_count > 0) {
                /* Make sure we only dispatch the current watcher list */
                int i, event_watchers_count = SDL_event_watchers_count;

                SDL_event_watchers_dispatching = SDL_TRUE;
                for (i = 0; i < event_watchers_count; ++i) {
                    if (!SDL_event_watchers[i].removed) {
                        SDL_event_watchers[i].callback(SDL_event_watchers[i].userdata, event);
                    }
                }
                SDL_event_watchers_dispatching = SDL_FALSE;

                if (SDL_event_watchers_removed) {
                    for (i = SDL_event_watchers_count; i--; ) {
                        if (SDL_event_watchers[i].removed) {
                            --SDL_event_watchers_count;
                            if (i < SDL_event_watchers_count) {
                                SDL_memmove(&SDL_event_watchers[i], &SDL_event_watchers[i+1], (SDL_event_watchers_count - i) * sizeof(SDL_event_watchers[i]));
                            }
                        }
                    }
                    SDL_event_watchers_removed = SDL_FALSE;
                }
            }

            if (SDL_event_watchers_lock) {
                SDL_UnlockMutex(SDL_event_watchers_lock);
            }
        }
    }

    if (SDL_PeepEvents(event, 1, SDL_ADDEVENT, 0, 0) <= 0) {
        return -1;
    }

    SDL_GestureProcessEvent(event);

    return 1;
}

int
SDL_EventsInit(void)
{
#if !SDL_JOYSTICK_DISABLED
    SDL_AddHintCallback(SDL_HINT_AUTO_UPDATE_JOYSTICKS, SDL_AutoUpdateJoysticksChanged, NULL);
#endif
#if !SDL_SENSOR_DISABLED
    SDL_AddHintCallback(SDL_HINT_AUTO_UPDATE_SENSORS, SDL_AutoUpdateSensorsChanged, NULL);
#endif
    SDL_AddHintCallback(SDL_HINT_EVENT_LOGGING, SDL_EventLoggingChanged, NULL);
    SDL_AddHintCallback(SDL_HINT_POLL_SENTINEL, SDL_PollSentinelChanged, NULL);
    if (SDL_StartEventLoop() < 0) {
        SDL_DelHintCallback(SDL_HINT_EVENT_LOGGING, SDL_EventLoggingChanged, NULL);
        return -1;
    }

    SDL_QuitInit();

    return 0;
}

void
SDL_EventsQuit(void)
{
    SDL_QuitQuit();
    SDL_StopEventLoop();
    SDL_DelHintCallback(SDL_HINT_POLL_SENTINEL, SDL_PollSentinelChanged, NULL);
    SDL_DelHintCallback(SDL_HINT_EVENT_LOGGING, SDL_EventLoggingChanged, NULL);
#if !SDL_JOYSTICK_DISABLED
    SDL_DelHintCallback(SDL_HINT_AUTO_UPDATE_JOYSTICKS, SDL_AutoUpdateJoysticksChanged, NULL);
#endif
#if !SDL_SENSOR_DISABLED
    SDL_DelHintCallback(SDL_HINT_AUTO_UPDATE_SENSORS, SDL_AutoUpdateSensorsChanged, NULL);
#endif
}

void ZG_Events_Update(void){
	ZG_MouseInfo * mouse_info = &g_zg_input_vars->mouse_info;
	// have a bad behaviour .... it's better to put own icon mouse...
	//resetCursor();
	SDL_Event event;
	ZG_KeyEvent key_event;
	ZG_MouseEvent mouse_event;
	ZG_DropEvent ev_drop;

	memset(g_zg_key_pressed,0,sizeof(g_zg_key_pressed));

	//left_mouse_button_repeat_pressed = false;
	mouse_info->motion = mouse_info->left_press =  false;

	uint32_t id_key;

	while  (SDL_PollEvent  (&event)) {

		memset(&key_event,0,sizeof(key_event));
		memset(&mouse_event,0,sizeof(mouse_event));

		if  (event.type  ==  SDL_QUIT)
			return ;


		if(event.type==g_zg_input_vars->second_mouse_motion){

			SDL_MouseMotionEvent *mouse_motion_2=&event.motion;


			mouse_event.position=Vector2i_Mulv2f(ZG_Vector2i_New(
					mouse_motion_2->x
					,mouse_motion_2->y
			),ZG_Graphics_GetScale());

			//mouse_event.position.x = mouse_motion_2->x*Graphics_GetScaleX();
			//mouse_event.position.y = mouse_motion_2->y*Graphics_GetScaleY();
			mouse_info->position = mouse_event.position;


			ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_motion_callbacks ,&mouse_event);

		}else{

			switch(event.type) {
			case SDL_DROPFILE:

				ev_drop.file = event.drop.file;
				ev_drop.timestamp = event.drop.timestamp;
				ev_drop.type = event.drop.type;

				ZG_Input_UpdateDropEvents(&ev_drop);

				break;
			case SDL_MOUSEMOTION:


				if(!g_zg_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_info->motion = true;

				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,ZG_Graphics_GetScale());

				ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_motion_callbacks ,&mouse_event);

				break;
			case SDL_MOUSEBUTTONDOWN:

				if(!g_zg_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_event.left_press = true;
				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->left_press = true;
				mouse_info->left_hold = true;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,ZG_Graphics_GetScale());

				ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_button_down_callbacks ,&mouse_event);

				break;
			case SDL_MOUSEBUTTONUP:

				if(!g_zg_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_event.left_press = false;
				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->left_press=false;
				mouse_info->left_hold =false;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,ZG_Graphics_GetScale());

				ZG_Input_UpdateMouseEvents(g_zg_input_vars->on_mouse_button_up_callbacks ,&mouse_event);
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				id_key = event.key.keysym.sym;

				//ZG_LOG_INFO("UnPress");

				if(SDLK_UP ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_UP; key_event.press_up = true;}
				else if(SDLK_DOWN ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_DOWN; key_event.press_down = true;}
				else if(SDLK_LEFT ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_LEFT; key_event.press_left = true;}
				else if(SDLK_RIGHT ==event.key.keysym.sym) {key_event.code_key =id_key = ZG_KEY_ID_RIGHT; key_event.press_right = true;}
				else if(SDLK_ESCAPE ==event.key.keysym.sym) {key_event.press_escape = true;}
				else if(SDLK_BACKSPACE ==event.key.keysym.sym) {key_event.press_backspace = true;}
				else if(SDLK_RETURN ==event.key.keysym.sym) {key_event.press_return = true;}
				else if(SDLK_HOME ==event.key.keysym.sym) {key_event.press_home = true;}
				else if(SDLK_END ==event.key.keysym.sym) {key_event.press_end = true;}
				else {
					if(ZG_KEY_ID_SPACE <= event.key.keysym.sym && event.key.keysym.sym <= ZG_KEY_ID_Z) {
						key_event.code_key = event.key.keysym.sym;
					}
					else if(SDLK_F1 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F1;
					else if(SDLK_F2 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F2;
					else if(SDLK_F3 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F3;
					else if(SDLK_F4 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F4;
					else if(SDLK_F5 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F5;
					else if(SDLK_F6 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F6;
					else if(SDLK_F7 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F7;
					else if(SDLK_F8 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F8;
					else if(SDLK_F9 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F9;
					else if(SDLK_F10 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F10;
					else if(SDLK_F11 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F11;
					else if(SDLK_F12 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZG_KEY_ID_F12;
					else if(SDLK_KP_PLUS ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZG_KEY_ID_KP_PLUS;
					}else if(SDLK_KP_MINUS ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZG_KEY_ID_KP_MINUS;
					}else if(SDLK_PAUSE ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZG_KEY_ID_PAUSE;
					}
				}
				if(id_key >= ZG_KEY_ID_LAST) {
					ZG_LOG_ERROR("unhandled id key %i", id_key);
				}
				else{
					if(id_key) {
						g_zg_key[id_key]=false;

						if(event.type == SDL_KEYDOWN) {
							g_zg_key_pressed[id_key]=true;
							g_zg_key[id_key]=true;
						}

						if(event.type == SDL_KEYUP){
							ZG_Input_UpdateKeyEvents(g_zg_input_vars->on_key_up_callbacks,&key_event);
						}
						else{ // is key down...
							ZG_Input_UpdateKeyEvents(g_zg_input_vars->on_key_down_callbacks,&key_event);
						}
					}
				}
				break;
			default:
				//ZG_LOG_INFO("unknow event %i",event.type);
				break;
			}
		}
	}

    //mouse_info->pos_diff.x = mouse_info->pos.x-mouse_info->pos_old.x;//mouse_info.pos_diff.x
	//mouse_info->pos_diff.y = mouse_info->pos.y-mouse_info->pos_old.y;

#if defined(ZG_USING_ASYNC_SPACE_ESC_KEY)
#ifdef WIN32
	// check generic keys ...
	//for(int i=8; i<190; i++){
	if(id_key==0){ // check only escape/space keys...

		// required global detect keys...
		ZG_Input_Win32_CheckKey(VK_PAUSE);
		ZG_Input_Win32_CheckKey(VK_SPACE);

		if(Graphics_GetNumMonitors() == 1){
			ZG_Input_Win32_CheckKey(VK_ESCAPE);
			ZG_Input_Win32_CheckKey(VK_C);
			ZG_Input_Win32_CheckKey(VK_A);
			ZG_Input_Win32_CheckKey(VK_R);
			ZG_Input_Win32_CheckKey(VK_T);
			ZG_Input_Win32_CheckKey(VK_1);
			ZG_Input_Win32_CheckKey(VK_2);
			ZG_Input_Win32_CheckKey(VK_3);
			ZG_Input_Win32_CheckKey(VK_4);
		}
	}
	//}

#endif
#endif
}
