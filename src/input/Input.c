#include "zg_input.h"
#include "graphics/zg_graphics.h"


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

bool 	        	zg_key[ZGK_LAST]={0};
bool    	     	zg_keyR[ZGK_LAST]={0};
#ifdef _WIN32
bool 	        	old_state[ZGK_LAST]={0};
#endif


typedef struct{
	MapInt 	* on_key_up
				   ,*on_key_down;
	MapInt  *on_mouse_button_down
				  ,*on_mouse_motion
				  ,*on_mouse_button_up;
	MapInt * on_drop_file;
	MouseInfo  mouse_info;
	bool enable_mouse_events;
	uint32_t second_mouse_motion;
	int n_id_handle;


}InputVars;

InputVars * g_input_vars=NULL;

#include "Input_Win32.c"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Input_Init(void) {

	if(g_input_vars!=NULL){
		return;
	}

	g_input_vars=NEW(InputVars);

	g_input_vars->mouse_info=(MouseInfo){
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
	//memset(zg_keyR,0,sizeof(zg_keyR));
	//memset(zg_key,0,sizeof(zg_key));

	g_input_vars->on_key_up=MapInt_New();
	g_input_vars->on_key_down=MapInt_New();
	g_input_vars->on_mouse_button_down=MapInt_New();
	g_input_vars->on_mouse_motion=MapInt_New();
	g_input_vars->on_mouse_button_up=MapInt_New();
	g_input_vars->on_drop_file=MapInt_New();
	g_input_vars->enable_mouse_events=true;
	g_input_vars->second_mouse_motion = SDL_RegisterEvents(1);

}

MouseInfo Input_GetMouseInfo(void){
	return g_input_vars->mouse_info;
}

Vector2i  Input_GetMousePosition(void){
	return g_input_vars->mouse_info.position;
}

Vector2i  *Input_GetMousePositionPtr(void){
	return &g_input_vars->mouse_info.position;
}

uint32_t Input_GetSecondMouseMotionType(void){
	return g_input_vars->second_mouse_motion;
}
//------------------------------------------------------------------------------------------------
void Input_SetupCursors(void) {

}

//------------------------------------------------------------------------------------------------
void Input_EnableMouseEvents(bool _enable_mouse_events){
	g_input_vars->enable_mouse_events=_enable_mouse_events;
}


//------------------------------------------------------------------------------------------------
void Input_ShowCursor(bool _show_cursor){
	if(SDL_ShowCursor(_show_cursor)<0){
		Log_Error("SDL_ShowCursor:%s",SDL_GetError());
	}

}

//--------------------------------------------------------------------------------------------------------v
bool Input_IsKeyPressed(int id_key) {
	if(ZGK_FIRST < id_key && id_key < ZGK_LAST){
		return zg_key[id_key];
	}

	return false;
}

bool		Input_IsLeftButtonPressed(void){
	return g_input_vars->mouse_info.left_press;
}

//------------------------------------------------------------------------------------------------
// CALLBACKS
uint32_t Input_NewHandleId(void){
	return g_input_vars->n_id_handle++;
}

uint32_t 	Input_AddEventOnKeyUp(CallbackKeyEvent _callback){
	uint32_t idx_handle=Input_NewHandleId();
	CallbackKeyEvent *cf=NEW(CallbackKeyEvent);
	*cf=_callback;
	MapInt_Set(g_input_vars->on_key_up,idx_handle,cf);
	return idx_handle;
}

uint32_t 	Input_AddEventOnKeyDown(CallbackKeyEvent _callback){
	uint32_t idx_handle=Input_NewHandleId();
	CallbackKeyEvent *cf=NEW(CallbackKeyEvent);
	*cf=_callback;
	MapInt_Set(g_input_vars->on_key_down,idx_handle,cf);
	return idx_handle;
}

uint32_t 	Input_AddEventOnMouseButtonUp(CallbackMouseEvent _callback){
	uint32_t idx_handle=Input_NewHandleId();
	CallbackMouseEvent *cf=NEW(CallbackMouseEvent);
	*cf=_callback;
	MapInt_Set(g_input_vars->on_mouse_button_up,idx_handle,cf);
	return idx_handle;
}

uint32_t 	Input_AddEventOnMouseMotion(CallbackMouseEvent _callback){
	uint32_t idx_handle=Input_NewHandleId();
	CallbackMouseEvent *cf=NEW(CallbackMouseEvent);
	*cf=_callback;
	MapInt_Set(g_input_vars->on_mouse_motion,idx_handle,cf);
	return idx_handle;
}

uint32_t 	Input_AddEventOnMouseButtonDown(CallbackMouseEvent _callback){
	uint32_t idx_handle=Input_NewHandleId();
	CallbackMouseEvent *cf=NEW(CallbackMouseEvent);
	*cf=_callback;
	MapInt_Set(g_input_vars->on_mouse_button_down,idx_handle,cf);
	return idx_handle;
}

uint32_t	Input_AddEventOnDropFile(CallbackDropEvent _callback){
	uint32_t idx_handle=Input_NewHandleId();
	CallbackDropEvent *cf=NEW(CallbackDropEvent);
	*cf=_callback;
	MapInt_Set(g_input_vars->on_drop_file,idx_handle,cf);
	return idx_handle;
}

void 		Input_RemoveEventOnKeyUp(uint32_t idx_handle){
	MapInt_EraseAndFreeItem(g_input_vars->on_key_up,idx_handle);
}

void 		Input_RemoveEventOnKeyDown(uint32_t idx_handle){
	MapInt_EraseAndFreeItem(g_input_vars->on_key_down,idx_handle);
}

void 		Input_RemoveEventOnMouseButtonUp(uint32_t idx_handle){
	MapInt_EraseAndFreeItem(g_input_vars->on_mouse_button_up,idx_handle);
}

void 		Input_RemoveEventOnMouseMotion(uint32_t idx_handle){
	MapInt_EraseAndFreeItem(g_input_vars->on_mouse_motion,idx_handle);
}

void 		Input_RemoveEventOnMouseButtonDown(uint32_t idx_handle){
	MapInt_EraseAndFreeItem(g_input_vars->on_mouse_button_down,idx_handle);
}

void		Input_RemoveEventOnDropFile(uint32_t idx_handle){
	MapInt_EraseAndFreeItem(g_input_vars->on_drop_file,idx_handle);
}

//------------------------------------------------------------------------------------------------
void Input_UpdateDropEvents(DropEvent *drop_event) {
	for(unsigned i = 0; i < g_input_vars->on_drop_file->list->count; i++){
			MapIntNode *node=g_input_vars->on_drop_file->list->items[i];
			CallbackDropEvent * e=(CallbackDropEvent *)node->val;
			((DropEventCallback)(e->ptr_function))(drop_event,e->user_data);
		}
}

void Input_UpdateMouseEvents(MapInt *mouse_events,MouseEvent *mouse_event) {

	for(unsigned i = 0; i < mouse_events->list->count; i++){
		MapIntNode *node=mouse_events->list->items[i];
		CallbackMouseEvent * e=(CallbackMouseEvent *)node->val;
		((MouseEventCallback)(e->ptr_function))(mouse_event,e->user_data);
	}

}

void Input_UpdateKeyEvents(MapInt *key_events,KeyEvent *key_event) {

	for(unsigned i = 0; i < key_events->list->count; i++){
		MapIntNode *node=key_events->list->items[i];
		CallbackKeyEvent * e=(CallbackKeyEvent *)node->val;
		((KeyEventCallback)(e->ptr_function))(key_event,e->user_data);
	}

}

void Input_Update() {


	MouseInfo * mouse_info = &g_input_vars->mouse_info;
	// have a bad behaviour .... it's better to put own icon mouse...
	//resetCursor();
	SDL_Event event;
	KeyEvent key_event;
	MouseEvent mouse_event;
	DropEvent ev_drop;

	memset(zg_key,0,sizeof(zg_key));

	//left_mouse_button_repeat_pressed = false;
	mouse_info->motion = mouse_info->left_press =  false;

	uint32_t id_key;

	while  (SDL_PollEvent  (&event)) {

		memset(&key_event,0,sizeof(key_event));
		memset(&mouse_event,0,sizeof(mouse_event));

		if  (event.type  ==  SDL_QUIT)
			return ;


		if(event.type==g_input_vars->second_mouse_motion){

			SDL_MouseMotionEvent *mouse_motion_2=&event.motion;


			mouse_event.position=Vector2i_Mulv2f(Vector2i_New(
					mouse_motion_2->x
					,mouse_motion_2->y
			),Graphics_GetScale());

			//mouse_event.position.x = mouse_motion_2->x*Graphics_GetScaleX();
			//mouse_event.position.y = mouse_motion_2->y*Graphics_GetScaleY();
			mouse_info->position = mouse_event.position;


			Input_UpdateMouseEvents(g_input_vars->on_mouse_motion ,&mouse_event);

		}else{

			switch(event.type) {
			case SDL_DROPFILE:

				ev_drop.file = event.drop.file;
				ev_drop.timestamp = event.drop.timestamp;
				ev_drop.type = event.drop.type;

				Input_UpdateDropEvents(&ev_drop);

				break;
			case SDL_MOUSEMOTION:

				if(!g_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_info->motion = true;

				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,Graphics_GetScale());

				Input_UpdateMouseEvents(g_input_vars->on_mouse_motion ,&mouse_event);

				break;
			case SDL_MOUSEBUTTONDOWN:

				if(!g_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_event.left_press = true;
				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->left_press = true;
				mouse_info->left_hold = true;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,Graphics_GetScale());

				Input_UpdateMouseEvents(g_input_vars->on_mouse_button_down ,&mouse_event);

				break;
			case SDL_MOUSEBUTTONUP:

				if(!g_input_vars->enable_mouse_events){
					continue; // next loop
				}

				mouse_event.left_press = false;
				mouse_event.position.x = event.button.x;
				mouse_event.position.y = event.button.y;

				mouse_info->left_press=false;
				mouse_info->left_hold =false;

				mouse_info->position = Vector2i_Mulv2f(mouse_event.position,Graphics_GetScale());

				Input_UpdateMouseEvents(g_input_vars->on_mouse_button_up ,&mouse_event);
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				id_key = event.key.keysym.sym;

				//Log_Info("UnPress");

				if(SDLK_UP ==event.key.keysym.sym) {key_event.code_key =id_key = ZGK_UP; key_event.press_up = true;}
				else if(SDLK_DOWN ==event.key.keysym.sym) {key_event.code_key =id_key = ZGK_DOWN; key_event.press_down = true;}
				else if(SDLK_LEFT ==event.key.keysym.sym) {key_event.code_key =id_key = ZGK_LEFT; key_event.press_left = true;}
				else if(SDLK_RIGHT ==event.key.keysym.sym) {key_event.code_key =id_key = ZGK_RIGHT; key_event.press_right = true;}
				else if(SDLK_ESCAPE ==event.key.keysym.sym) {key_event.press_escape = true;}
				else if(SDLK_BACKSPACE ==event.key.keysym.sym) {key_event.press_backspace = true;}
				else if(SDLK_RETURN ==event.key.keysym.sym) {key_event.press_return = true;}
				else if(SDLK_HOME ==event.key.keysym.sym) {key_event.press_home = true;}
				else if(SDLK_END ==event.key.keysym.sym) {key_event.press_end = true;}
				else {
					if(ZGK_SPACE <= event.key.keysym.sym && event.key.keysym.sym <= ZGK_z) {
						key_event.code_key = event.key.keysym.sym;
					}
					else if(SDLK_F1 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F1;
					else if(SDLK_F2 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F2;
					else if(SDLK_F3 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F3;
					else if(SDLK_F4 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F4;
					else if(SDLK_F5 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F5;
					else if(SDLK_F6 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F6;
					else if(SDLK_F7 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F7;
					else if(SDLK_F8 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F8;
					else if(SDLK_F9 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F9;
					else if(SDLK_F10 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F10;
					else if(SDLK_F11 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F11;
					else if(SDLK_F12 ==event.key.keysym.sym)
						key_event.code_key =  id_key = ZGK_F12;
					else if(SDLK_KP_PLUS ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZGK_KP_PLUS;
					}else if(SDLK_KP_MINUS ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZGK_KP_MINUS;
					}else if(SDLK_PAUSE ==event.key.keysym.sym) {
						key_event.code_key = id_key = ZGK_PAUSE;
					}
				}
				if(id_key >= ZGK_LAST) {
					Log_Error("unhandled id key %i", id_key);
				}
				else{
					if(id_key) {
						zg_keyR[id_key]=false;

						if(event.type == SDL_KEYDOWN) {
							zg_key[id_key]=true;
							zg_keyR[id_key]=true;
						}

						if(event.type == SDL_KEYUP){
							Input_UpdateKeyEvents(g_input_vars->on_key_up,&key_event);
						}
						else{ // is key down...
							Input_UpdateKeyEvents(g_input_vars->on_key_down,&key_event);
						}
					}
				}
				break;
			default:
				//Log_Info("unknow event %i",event.type);
				break;
			}
		}
	}

    //mouse_info->pos_diff.x = mouse_info->pos.x-mouse_info->pos_old.x;//mouse_info.pos_diff.x
	//mouse_info->pos_diff.y = mouse_info->pos.y-mouse_info->pos_old.y;

#if defined(USING_ASYNC_SPACE_ESC_KEY)
#ifdef WIN32
	// check generic keys ...
	//for(int i=8; i<190; i++){
	if(id_key==0){ // check only escape/space keys...

		// required global detect keys...
		Input_Win32_CheckKey(VK_PAUSE);
		Input_Win32_CheckKey(VK_SPACE);

		if(Graphics_GetNumMonitors() == 1){
			Input_Win32_CheckKey(VK_ESCAPE);
			Input_Win32_CheckKey(VK_C);
			Input_Win32_CheckKey(VK_A);
			Input_Win32_CheckKey(VK_R);
			Input_Win32_CheckKey(VK_T);
			Input_Win32_CheckKey(VK_1);
			Input_Win32_CheckKey(VK_2);
			Input_Win32_CheckKey(VK_3);
			Input_Win32_CheckKey(VK_4);
		}
	}
	//}

#endif
#endif

}
//------------------------------------------------------------------------------------------------
void Input_DeInit(void){

	MapInt_DeleteAndFreeAllItems(g_input_vars->on_key_up);
	MapInt_DeleteAndFreeAllItems(g_input_vars->on_key_down);
	MapInt_DeleteAndFreeAllItems(g_input_vars->on_mouse_button_down);
	MapInt_DeleteAndFreeAllItems(g_input_vars->on_mouse_motion);
	MapInt_DeleteAndFreeAllItems(g_input_vars->on_mouse_button_up);
	MapInt_DeleteAndFreeAllItems(g_input_vars->on_drop_file);

	FREE(g_input_vars);
	g_input_vars=NULL;
}


