#ifndef __ZG_INPUT_H__
#define __ZG_INPUT_H__

#include "ZG_Keys.h"


#define MOUSE_MOTION2	  	(SDL_USEREVENT+1)


typedef struct{
	uint32_t code_key;
	bool press_up;
	bool press_down;
	bool press_left;
	bool press_right;
	bool press_escape;
	bool press_return;
	bool press_backspace;
	bool press_home;
	bool press_end;
}ZG_KeyEvent,ZG_EventRepeatKey;

typedef struct{
	ZG_Vector2i position;
	bool  		left_press;
	bool  		right_press;
}ZG_MouseEvent;

typedef struct
{
	uint32_t type;        /**< ::SDL_DROPFILE */
	uint32_t timestamp;
	const char *file;         /**< The file name, which should be freed with SDL_free() */
} ZG_DropEvent;

typedef struct{
	ZG_Vector2i   	position,position_previous,position_diff;
	ZG_Vector2f    	factor;
	bool    		motion,left_press,left_hold;
}ZG_MouseInfo;

// TESTCASE SDL CALLBACK
typedef void (*ZG_KeyEventFunction)(ZG_KeyEvent  * event, void * user_data);
typedef struct{
	ZG_KeyEventFunction ptr_function;
	void *user_data;
}ZG_KeyEventCallback;


typedef void (*ZG_MouseEventFunction)(ZG_MouseEvent  * event, void * user_data);
typedef struct{
	ZG_MouseEventFunction ptr_function;
	void *user_data;
}ZG_MouseEventCallback;


typedef void (*ZG_DropEventFunction)(ZG_DropEvent  * event, void * user_data);
typedef struct{
	ZG_DropEventFunction ptr_function;
	void *user_data;
}ZG_DropEventCallback;


//------------------------------------------------------------------------------------------
void 			ZG_Input_Init(void);
ZG_MouseInfo	ZG_Input_GetMouseInfo(void);
ZG_Vector2i  	ZG_Input_GetMousePosition(void);
ZG_Vector2i  *	ZG_Input_GetMousePositionPtr(void);
uint32_t 		ZG_Input_GetSecondMouseMotionType(void);
void			ZG_Input_EnableMouseEvents(bool _enable);
void 			ZG_Input_ShowCursor(bool _show_cursor);

uint32_t 		ZG_Input_AddCallbackOnKeyUp(ZG_KeyEventCallback _callback);
uint32_t 		ZG_Input_AddCallbackOnKeyDown(ZG_KeyEventCallback _callback);
uint32_t 		ZG_Input_AddCallbackOnMouseButtonUp(ZG_MouseEventCallback _callback);
uint32_t 		ZG_Input_AddCallbackOnMouseButtonDown(ZG_MouseEventCallback _callback);
uint32_t 		ZG_Input_AddCallbackOnMouseMotion(ZG_MouseEventCallback _callback);
uint32_t		ZG_Input_AddCallbackOnDropFile(ZG_DropEventCallback _callback);

void 			ZG_Input_RemoveCallbackOnKeyUp(uint32_t idx_handle);
void 			ZG_Input_RemoveCallbackOnKeyDown(uint32_t idx_handle);
void 			ZG_Input_RemoveCallbackOnMouseButtonUp(uint32_t idx_handle);
void 			ZG_Input_RemoveCallbackOnMouseMotion(uint32_t idx_handle);
void 			ZG_Input_RemoveCallbackOnMouseButtonDown(uint32_t idx_handle);
void			ZG_Input_RemoveCallbackOnDropFile(uint32_t idx_handle);

void 			ZG_Input_Update(void);
bool			ZG_Input_IsKeyPressed(int id_key);
bool			ZG_Input_IsLeftButtonPressed(void);
void 			ZG_Input_DeInit(void);
#endif
