#ifdef _WIN32

#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34

#define VK_A	0x41
#define VK_C	0x43
#define VK_R	0x52
#define VK_T	0x54

void ZG_Input_Win32_CheckKey(unsigned int code){

	ZG_KeyEvent key_event;
	memset(&key_event,0,sizeof(ZG_KeyEvent));
	//keyR[code]=false;
	uint32_t id_key=0;
	bool is_pressed = (GetAsyncKeyState(code) & 0x8000)== 0x8000;

	//if(GetAsyncKeyState(code)){ // is pressed ...

		//is_pressed = GetAsyncKeyState(code);

		switch(code){
			case VK_UP:  		id_key = ZG_KEY_ID_UP; break;
			case VK_DOWN:  		id_key = ZG_KEY_ID_DOWN; break;
			case VK_LEFT:  		id_key = ZG_KEY_ID_LEFT; break;
			case VK_RIGHT: 		id_key = ZG_KEY_ID_RIGHT; break;
			case VK_ESCAPE: 	id_key = ZG_KEY_ID_ESCAPE;  break;
			case VK_BACK:		id_key = ZG_KEY_ID_BACKSPACE;break;
			case VK_RETURN: 	id_key = ZG_KEY_ID_RETURN;break;
			case VK_HOME: 		id_key = ZG_KEY_ID_HOME;break;
			case VK_END: 		id_key = ZG_KEY_ID_END; break;
			case VK_1:			id_key = ZG_KEY_ID_1; break;
			case VK_2:			id_key = ZG_KEY_ID_2; break;
			case VK_3:			id_key = ZG_KEY_ID_3; break;
			case VK_4:			id_key = ZG_KEY_ID_4; break;
			case VK_A:			id_key = ZG_KEY_ID_A; break;
			case VK_C:			id_key = ZG_KEY_ID_C; break;
			case VK_R:			id_key = ZG_KEY_ID_R; break;
			case VK_T:			id_key = ZG_KEY_ID_T; break;
			case VK_F1: 		id_key = ZG_KEY_ID_F1;break;
			case VK_F2: 		id_key = ZG_KEY_ID_F2;break;
			case VK_F3: 		id_key = ZG_KEY_ID_F3;break;
			case VK_F4: 		id_key = ZG_KEY_ID_F4;break;
			case VK_F5: 		id_key = ZG_KEY_ID_F5;break;
			case VK_F6: 		id_key = ZG_KEY_ID_F6;break;
			case VK_F7: 		id_key = ZG_KEY_ID_F7;break;
			case VK_F8: 		id_key = ZG_KEY_ID_F8;break;
			case VK_F9: 		id_key = ZG_KEY_ID_F9;break;
			case VK_F10: 		id_key = ZG_KEY_ID_F10;break;
			case VK_F11: 		id_key = ZG_KEY_ID_F11;break;
			case VK_F12: 		id_key = ZG_KEY_ID_F12;break;
			case VK_TAB: 		id_key = ZG_KEY_ID_TAB;break;
			case VK_NUMPAD0:	id_key = ZG_KEY_ID_KP0;break;
			case VK_NUMPAD1:	id_key = ZG_KEY_ID_KP1;break;
			case VK_NUMPAD2:	id_key = ZG_KEY_ID_KP2;break;
			case VK_NUMPAD3:	id_key = ZG_KEY_ID_KP3;break;
			case VK_NUMPAD4:	id_key = ZG_KEY_ID_KP4;break;
			case VK_NUMPAD5:	id_key = ZG_KEY_ID_KP5;break;
			case VK_NUMPAD6:	id_key = ZG_KEY_ID_KP6;break;
			case VK_NUMPAD7:	id_key = ZG_KEY_ID_KP7;break;
			case VK_NUMPAD8:	id_key = ZG_KEY_ID_KP8;break;
			case VK_NUMPAD9:	id_key = ZG_KEY_ID_KP9;break;
			case VK_ADD:		id_key = ZG_KEY_ID_KP_PLUS;break;
			case VK_SUBTRACT:	id_key = ZG_KEY_ID_KP_MINUS;break;
			case VK_PAUSE: 		id_key = ZG_KEY_ID_PAUSE;break;

			/*default:
				if(ZG_KEY_ID_SPACE <= code && code <= ZG_KEY_ID_z) {
					 id_key =code;
				}*/
		}

	if(id_key==0)
		return;
	//}

	key_event.code_key=id_key;

	g_zg_key[id_key]=is_pressed;


	// event down or up state (one)...
	if(g_zg_key[id_key] != zg_old_state[id_key])
	{ // pressed or released ?

		if(is_pressed){ //Event->type == SDL_KEYDOWN) {
			g_zg_key_pressed[id_key]=true;
		}

		for(unsigned i = 0; i < g_zg_input_vars->on_key_up->list->count; i++){
			ZG_Callback * e=(ZG_Callback *)g_zg_input_vars->on_key_up->list->items[i];
			e->ptr_function(&key_event,e->user_data);
		}

	}

	// save current state ...
	zg_old_state[id_key] = g_zg_key[id_key];


}
#endif
