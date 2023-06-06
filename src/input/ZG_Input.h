#ifndef __INPUT_H__
#define __INPUT_H__

#include "def_keys.h"


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
}KeyEvent,EventRepeatKey;

typedef struct{
	ZG_Vector2i position;
	bool  left_press;
	bool  right_press;
}MouseEvent;

typedef struct
{
	uint32_t type;        /**< ::SDL_DROPFILE */
	uint32_t timestamp;
	const char *file;         /**< The file name, which should be freed with SDL_free() */
} DropEvent;

typedef struct{
	ZG_Vector2i   	position,position_previous,position_diff;
	Vector2f    factor;
	bool    	motion,left_press,left_hold;
}MouseInfo;

// TESTCASE SDL CALLBACK
typedef void (*KeyEventCallback)(KeyEvent  * event, void * user_data);
typedef struct{
	KeyEventCallback ptr_function;
	void *user_data;
}CallbackKeyEvent;


typedef void (*MouseEventCallback)(MouseEvent  * event, void * user_data);
typedef struct{
	MouseEventCallback ptr_function;
	void *user_data;
}CallbackMouseEvent;


typedef void (*DropEventCallback)(DropEvent  * event, void * user_data);
typedef struct{
	DropEventCallback ptr_function;
	void *user_data;
}CallbackDropEvent;





//------------------------------------------------------------------------------------------


void 		Input_Init(void);
MouseInfo	Input_GetMouseInfo(void);
ZG_Vector2i  	Input_GetMousePosition(void);
ZG_Vector2i  *	Input_GetMousePositionPtr(void);
uint32_t 		Input_GetSecondMouseMotionType(void);
void		Input_EnableMouseEvents(bool _enable);
void 		Input_ShowCursor(bool _show_cursor);

uint32_t 		Input_AddEventOnKeyUp(CallbackKeyEvent _callback);
uint32_t 		Input_AddEventOnKeyDown(CallbackKeyEvent _callback);
uint32_t 		Input_AddEventOnMouseButtonUp(CallbackMouseEvent _callback);
uint32_t 		Input_AddEventOnMouseButtonDown(CallbackMouseEvent _callback);
uint32_t 		Input_AddEventOnMouseMotion(CallbackMouseEvent _callback);
uint32_t		Input_AddEventOnDropFile(CallbackDropEvent _callback);

void 		Input_RemoveEventOnKeyUp(uint32_t idx_handle);
void 		Input_RemoveEventOnKeyDown(uint32_t idx_handle);
void 		Input_RemoveEventOnMouseButtonUp(uint32_t idx_handle);
void 		Input_RemoveEventOnMouseMotion(uint32_t idx_handle);
void 		Input_RemoveEventOnMouseButtonDown(uint32_t idx_handle);
void		Input_RemoveEventOnDropFile(uint32_t idx_handle);

void 		Input_Update(void);
bool		Input_IsKeyPressed(int id_key);
bool		Input_IsLeftButtonPressed(void);
void 		Input_DeInit(void);
#endif
