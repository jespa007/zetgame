#include "@zg_events.h"

// define a queue of events

bool ZG_Events_PollEvent(void){

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
