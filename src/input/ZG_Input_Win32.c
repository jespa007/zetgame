#ifdef _WIN32

#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34

#define VK_A	0x41
#define VK_C	0x43
#define VK_R	0x52
#define VK_T	0x54

void Input_Win32_CheckKey(unsigned int code){

	KeyEvent key_event;
	memset(&key_event,0,sizeof(KeyEvent));
	//keyR[code]=false;
	uint32_t id_key=0;
	bool is_pressed = (GetAsyncKeyState(code) & 0x8000)== 0x8000;

	//if(GetAsyncKeyState(code)){ // is pressed ...

		//is_pressed = GetAsyncKeyState(code);

		switch(code){
			case VK_UP:  		id_key = ZGK_UP; break;
			case VK_DOWN:  		id_key = ZGK_DOWN; break;
			case VK_LEFT:  		id_key = ZGK_LEFT; break;
			case VK_RIGHT: 		id_key = ZGK_RIGHT; break;
			case VK_ESCAPE: 	id_key = ZGK_ESCAPE;  break;
			case VK_BACK:		id_key = ZGK_BACKSPACE;break;
			case VK_RETURN: 	id_key = ZGK_RETURN;break;
			case VK_HOME: 		id_key = ZGK_HOME;break;
			case VK_END: 		id_key = ZGK_END; break;
			case VK_1:			id_key = ZGK_1; break;
			case VK_2:			id_key = ZGK_2; break;
			case VK_3:			id_key = ZGK_3; break;
			case VK_4:			id_key = ZGK_4; break;
			case VK_A:			id_key = ZGK_a; break;
			case VK_C:			id_key = ZGK_c; break;
			case VK_R:			id_key = ZGK_r; break;
			case VK_T:			id_key = ZGK_t; break;
			case VK_F1: 		id_key = ZGK_F1;break;
			case VK_F2: 		id_key = ZGK_F2;break;
			case VK_F3: 		id_key = ZGK_F3;break;
			case VK_F4: 		id_key = ZGK_F4;break;
			case VK_F5: 		id_key = ZGK_F5;break;
			case VK_F6: 		id_key = ZGK_F6;break;
			case VK_F7: 		id_key = ZGK_F7;break;
			case VK_F8: 		id_key = ZGK_F8;break;
			case VK_F9: 		id_key = ZGK_F9;break;
			case VK_F10: 		id_key = ZGK_F10;break;
			case VK_F11: 		id_key = ZGK_F11;break;
			case VK_F12: 		id_key = ZGK_F12;break;
			case VK_TAB: 		id_key = ZGK_TAB;break;
			case VK_NUMPAD0:	id_key = ZGK_KP0;break;
			case VK_NUMPAD1:	id_key = ZGK_KP1;break;
			case VK_NUMPAD2:	id_key = ZGK_KP2;break;
			case VK_NUMPAD3:	id_key = ZGK_KP3;break;
			case VK_NUMPAD4:	id_key = ZGK_KP4;break;
			case VK_NUMPAD5:	id_key = ZGK_KP5;break;
			case VK_NUMPAD6:	id_key = ZGK_KP6;break;
			case VK_NUMPAD7:	id_key = ZGK_KP7;break;
			case VK_NUMPAD8:	id_key = ZGK_KP8;break;
			case VK_NUMPAD9:	id_key = ZGK_KP9;break;
			case VK_ADD:		id_key = ZGK_KP_PLUS;break;
			case VK_SUBTRACT:	id_key = ZGK_KP_MINUS;break;
			case VK_PAUSE: 		id_key = ZGK_PAUSE;break;

			/*default:
				if(ZGK_SPACE <= code && code <= ZGK_z) {
					 id_key =code;
				}*/
		}

	if(id_key==0)
		return;
	//}

	key_event.code_key=id_key;

	if(is_pressed){
		zg_keyR[id_key]=true;
	}else{
		zg_keyR[id_key]=false;
	}


	// event down or up state (one)...
	if(zg_keyR[id_key] != old_state[id_key])
	{ // pressed or released ?

		if(is_pressed){ //Event->type == SDL_KEYDOWN) {
			zg_key[id_key]=true;
		}

		for(unsigned i = 0; i < g_input_vars->on_key_up->list->count; i++){
			Callback * e=(Callback *)g_input_vars->on_key_up->list->items[i];
			e->ptr_function(&key_event,e->user_data);
		}

	}

	// save current state ...
	old_state[id_key] = zg_keyR[id_key];


}
#endif
