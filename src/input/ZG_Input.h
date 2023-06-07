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
}ZG_KeyEvent,EventRepeatKey;

typedef struct{
	ZG_Vector2i position;
	bool  left_press;
	bool  right_press;
}ZG_MouseEvent;

typedef struct
{
	uint32_t type;        /**< ::SDL_DROPFILE */
	uint32_t timestamp;
	const char *file;         /**< The file name, which should be freed with SDL_free() */
} ZG_DropEvent;

typedef struct{
	ZG_Vector2i   	position,position_previous,position_diff;
	ZG_Vector2f    factor;
	bool    	motion,left_press,left_hold;
}ZG_MouseInfo;

// TESTCASE SDL CALLBACK
typedef void (*ZG_KeyEventCallback)(ZG_KeyEvent  * event, void * user_data);
typedef struct{
	ZG_KeyEventCallback ptr_function;
	void *user_data;
}ZG_CallbackKeyEvent;


typedef void (*ZG_MouseEventCallback)(ZG_MouseEvent  * event, void * user_data);
typedef struct{
	ZG_MouseEventCallback ptr_function;
	void *user_data;
}ZG_CallbackMouseEvent;


typedef void (*ZG_DropEventCallback)(ZG_DropEvent  * event, void * user_data);
typedef struct{
	ZG_DropEventCallback ptr_function;
	void *user_data;
}ZG_CallbackDropEvent;





//------------------------------------------------------------------------------------------


void 			ZG_Input_Init(void);
ZG_MouseInfo	ZG_Input_GetMouseInfo(void);
ZG_Vector2i  	ZG_Input_GetMousePosition(void);
ZG_Vector2i  *	ZG_Input_GetMousePositionPtr(void);
uint32_t 		ZG_Input_GetSecondMouseMotionType(void);
void			ZG_Input_EnableMouseEvents(bool _enable);
void 			ZG_Input_ShowCursor(bool _show_cursor);

uint32_t 		ZG_Input_AddEventOnKeyUp(ZG_CallbackKeyEvent _callback);
uint32_t 		ZG_Input_AddEventOnKeyDown(ZG_CallbackKeyEvent _callback);
uint32_t 		ZG_Input_AddEventOnMouseButtonUp(ZG_CallbackMouseEvent _callback);
uint32_t 		ZG_Input_AddEventOnMouseButtonDown(ZG_CallbackMouseEvent _callback);
uint32_t 		ZG_Input_AddEventOnMouseMotion(ZG_CallbackMouseEvent _callback);
uint32_t		ZG_Input_AddEventOnDropFile(ZG_CallbackDropEvent _callback);

void 			ZG_Input_RemoveEventOnKeyUp(uint32_t idx_handle);
void 			ZG_Input_RemoveEventOnKeyDown(uint32_t idx_handle);
void 			ZG_Input_RemoveEventOnMouseButtonUp(uint32_t idx_handle);
void 			ZG_Input_RemoveEventOnMouseMotion(uint32_t idx_handle);
void 			ZG_Input_RemoveEventOnMouseButtonDown(uint32_t idx_handle);
void			ZG_Input_RemoveEventOnDropFile(uint32_t idx_handle);

void 			ZG_Input_Update(void);
bool			ZG_Input_IsKeyPressed(int id_key);
bool			ZG_Input_IsLeftButtonPressed(void);
void 			ZG_Input_DeInit(void);
#endif
