#include "@zg_input.h"
#include "graphics/@zg_graphics.h"


/*
typedef struct{
	KeyListener event_callback;
	void * user_data;
}BindKeyListener;

typedef struct{
	MouseListener event_callback;
	void *  user_data;
}BindMouseListener;

typedef struct{
	DropListener event_callback;
	void *  user_data;
}BindDropListener;*/

bool 	        	g_zg_key_pressed[ZG_KEY_ID_LAST]={0};
bool    	     	g_zg_key[ZG_KEY_ID_LAST]={0};
#ifdef _WIN32
bool 	        	zg_old_state[ZG_KEY_ID_LAST]={0};
#endif


typedef struct{
	ZG_MapInt 	* on_key_up_callbacks
				   ,*on_key_down_callbacks;
	ZG_MapInt  *on_mouse_button_down_callbacks
				  ,*on_mouse_motion_callbacks
				  ,*on_mouse_button_up_callbacks;
	ZG_MapInt * on_drop_file_callbacks;
	ZG_MouseInfo  mouse_info;
	bool enable_mouse_events;
	uint32_t second_mouse_motion;
	int n_id_handle;


}ZG_InputVars;

ZG_InputVars * g_zg_input_vars=NULL;

#include "ZG_Input_Win32.c"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ZG_Input_Init(void) {

	if(g_zg_input_vars!=NULL){
		return;
	}

	g_zg_input_vars=ZG_NEW(ZG_InputVars);

	g_zg_input_vars->mouse_info=(ZG_MouseInfo){
			.left_press = false
			,.left_hold = false
			,.position_diff.x=0
			,.position_diff.y=0

			,.position.x=0
			,.position.y=0
			,.position_previous.x=0
			,.position_previous.y=0
			,.factor.x=1
			,.factor.y=1
	};

	/*mouse_info=;*/
	//memset(g_zg_key,0,sizeof(g_zg_key));
	//memset(g_zg_key_pressed,0,sizeof(g_zg_key_pressed));

	g_zg_input_vars->on_key_up_callbacks=ZG_MapInt_New();
	g_zg_input_vars->on_key_down_callbacks=ZG_MapInt_New();
	g_zg_input_vars->on_mouse_button_down_callbacks=ZG_MapInt_New();
	g_zg_input_vars->on_mouse_motion_callbacks=ZG_MapInt_New();
	g_zg_input_vars->on_mouse_button_up_callbacks=ZG_MapInt_New();
	g_zg_input_vars->on_drop_file_callbacks=ZG_MapInt_New();
	g_zg_input_vars->enable_mouse_events=true;
	g_zg_input_vars->second_mouse_motion = SDL_RegisterEvents(1);

}

ZG_MouseInfo ZG_Input_GetMouseInfo(void){
	return g_zg_input_vars->mouse_info;
}

ZG_Vector2i  ZG_Input_GetMousePosition(void){
	return g_zg_input_vars->mouse_info.position;
}

ZG_Vector2i  *ZG_Input_GetMousePositionPtr(void){
	return &g_zg_input_vars->mouse_info.position;
}

uint32_t ZG_Input_GetSecondMouseMotionType(void){
	return g_zg_input_vars->second_mouse_motion;
}
//------------------------------------------------------------------------------------------------
void ZG_Input_SetupCursors(void) {

}

//------------------------------------------------------------------------------------------------
void ZG_Input_EnableMouseEvents(bool _enable_mouse_events){
	g_zg_input_vars->enable_mouse_events=_enable_mouse_events;
}


//------------------------------------------------------------------------------------------------
void ZG_Input_ShowCursor(bool _show_cursor){
	if(SDL_ShowCursor(_show_cursor)<0){
		ZG_LOG_ERROR("SDL_ShowCursor:%s",SDL_GetError());
	}

}

//--------------------------------------------------------------------------------------------------------v
bool ZG_Input_IsKeyPressed(int id_key) {
	if(ZG_KEY_ID_FIRST < id_key && id_key < ZG_KEY_ID_LAST){
		return g_zg_key_pressed[id_key];
	}

	return false;
}

bool		ZG_Input_IsLeftButtonPressed(void){
	return g_zg_input_vars->mouse_info.left_press;
}

//------------------------------------------------------------------------------------------------
// CALLBACKS
uint32_t ZG_Input_NewHandleId(void){
	return g_zg_input_vars->n_id_handle++;
}

uint32_t 	ZG_Input_AddCallbackOnKeyUp(ZG_KeyEventCallback _callback){
	uint32_t idx_handle=ZG_Input_NewHandleId();
	ZG_KeyEventCallback *cf=ZG_NEW(ZG_KeyEventCallback);
	*cf=_callback;
	ZG_MapInt_Set(g_zg_input_vars->on_key_up_callbacks,idx_handle,cf);
	return idx_handle;
}

uint32_t 	ZG_Input_AddCallbackOnKeyDown(ZG_KeyEventCallback _callback){
	uint32_t idx_handle=ZG_Input_NewHandleId();
	ZG_KeyEventCallback *cf=ZG_NEW(ZG_KeyEventCallback);
	*cf=_callback;
	ZG_MapInt_Set(g_zg_input_vars->on_key_down_callbacks,idx_handle,cf);
	return idx_handle;
}

uint32_t 	ZG_Input_AddCallbackOnMouseButtonUp(ZG_MouseEventCallback _callback){
	uint32_t idx_handle=ZG_Input_NewHandleId();
	ZG_MouseEventCallback *cf=ZG_NEW(ZG_MouseEventCallback);
	*cf=_callback;
	ZG_MapInt_Set(g_zg_input_vars->on_mouse_button_up_callbacks,idx_handle,cf);
	return idx_handle;
}

uint32_t 	ZG_Input_AddCallbackOnMouseMotion(ZG_MouseEventCallback _callback){
	uint32_t idx_handle=ZG_Input_NewHandleId();
	ZG_MouseEventCallback *cf=ZG_NEW(ZG_MouseEventCallback);
	*cf=_callback;
	ZG_MapInt_Set(g_zg_input_vars->on_mouse_motion_callbacks,idx_handle,cf);
	return idx_handle;
}

uint32_t 	ZG_Input_AddCallbackOnMouseButtonDown(ZG_MouseEventCallback _callback){
	uint32_t idx_handle=ZG_Input_NewHandleId();
	ZG_MouseEventCallback *cf=ZG_NEW(ZG_MouseEventCallback);
	*cf=_callback;
	ZG_MapInt_Set(g_zg_input_vars->on_mouse_button_down_callbacks,idx_handle,cf);
	return idx_handle;
}

uint32_t	ZG_Input_AddCallbackOnDropFile(ZG_DropEventCallback _callback){
	uint32_t idx_handle=ZG_Input_NewHandleId();
	ZG_DropEventCallback *cf=ZG_NEW(ZG_DropEventCallback);
	*cf=_callback;
	ZG_MapInt_Set(g_zg_input_vars->on_drop_file_callbacks,idx_handle,cf);
	return idx_handle;
}

void 		ZG_Input_RemoveCallbackOnKeyUp(uint32_t idx_handle){
	ZG_MapInt_EraseAndFreeItem(g_zg_input_vars->on_key_up_callbacks,idx_handle);
}

void 		ZG_Input_RemoveCallbackOnKeyDown(uint32_t idx_handle){
	ZG_MapInt_EraseAndFreeItem(g_zg_input_vars->on_key_down_callbacks,idx_handle);
}

void 		ZG_Input_RemoveCallbackOnMouseButtonUp(uint32_t idx_handle){
	ZG_MapInt_EraseAndFreeItem(g_zg_input_vars->on_mouse_button_up_callbacks,idx_handle);
}

void 		ZG_Input_RemoveCallbackOnMouseMotion(uint32_t idx_handle){
	ZG_MapInt_EraseAndFreeItem(g_zg_input_vars->on_mouse_motion_callbacks,idx_handle);
}

void 		ZG_Input_RemoveCallbackOnMouseButtonDown(uint32_t idx_handle){
	ZG_MapInt_EraseAndFreeItem(g_zg_input_vars->on_mouse_button_down_callbacks,idx_handle);
}

void		ZG_Input_RemoveCallbackOnDropFile(uint32_t idx_handle){
	ZG_MapInt_EraseAndFreeItem(g_zg_input_vars->on_drop_file_callbacks,idx_handle);
}

//------------------------------------------------------------------------------------------------
void ZG_Input_UpdateDropEvents(ZG_DropEvent *drop_event) {
	for(unsigned i = 0; i < g_zg_input_vars->on_drop_file_callbacks->list->count; i++){
			ZG_MapIntNode *node=g_zg_input_vars->on_drop_file_callbacks->list->items[i];
			ZG_DropEventCallback * e=(ZG_DropEventCallback *)node->val;
			((ZG_DropEventFunction)(e->ptr_function))(drop_event,e->user_data);
		}
}

void ZG_Input_UpdateMouseEvents(ZG_MapInt *mouse_events,ZG_MouseEvent *mouse_event) {

	for(unsigned i = 0; i < mouse_events->list->count; i++){
		ZG_MapIntNode *node=mouse_events->list->items[i];
		ZG_MouseEventCallback * e=(ZG_MouseEventCallback *)node->val;
		((ZG_MouseEventFunction)(e->ptr_function))(mouse_event,e->user_data);
	}

}

void ZG_Input_UpdateKeyEvents(ZG_MapInt *key_events,ZG_KeyEvent *key_event) {

	for(unsigned i = 0; i < key_events->list->count; i++){
		ZG_MapIntNode *node=key_events->list->items[i];
		ZG_KeyEventCallback * e=(ZG_KeyEventCallback *)node->val;
		((ZG_KeyEventFunction)(e->ptr_function))(key_event,e->user_data);
	}

}

void ZG_Input_Update() {


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
//------------------------------------------------------------------------------------------------
void ZG_Input_DeInit(void){

	ZG_MapInt_DeleteAndFreeAllItems(g_zg_input_vars->on_key_up_callbacks);
	ZG_MapInt_DeleteAndFreeAllItems(g_zg_input_vars->on_key_down_callbacks);
	ZG_MapInt_DeleteAndFreeAllItems(g_zg_input_vars->on_mouse_button_down_callbacks);
	ZG_MapInt_DeleteAndFreeAllItems(g_zg_input_vars->on_mouse_motion_callbacks);
	ZG_MapInt_DeleteAndFreeAllItems(g_zg_input_vars->on_mouse_button_up_callbacks);
	ZG_MapInt_DeleteAndFreeAllItems(g_zg_input_vars->on_drop_file_callbacks);

	ZG_FREE(g_zg_input_vars);
	g_zg_input_vars=NULL;
}


