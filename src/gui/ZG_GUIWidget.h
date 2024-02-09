#ifndef __ZG_GUI_WIDGET_H__
#define __ZG_GUI_WIDGET_H__


//#define GUI_NO_ICON 			GUIIconManager_GetIcon(NULL,0)
//#define GUI_DEFAULT_ICON(x)		GUIIconManager_GetIcon("", x)

#define ZG_GUI_GET_RELATIVE_X(widget) (widget->local_position.x + widget->position_offset.x)
#define ZG_GUI_GET_RELATIVE_Y(widget) (widget->local_position.y + widget->position_offset.y)

typedef enum{
	ZG_GUI_WIDGET_TYPE_UNKNOWN=0
	,ZG_GUI_WIDGET_TYPE_WIDGET
	,ZG_GUI_WIDGET_TYPE_FRAME
	,ZG_GUI_WIDGET_TYPE_LABEL
	,ZG_GUI_WIDGET_TYPE_TEXTBOX
	,ZG_GUI_WIDGET_TYPE_BUTTON
	,ZG_GUI_WIDGET_TYPE_VIEWER
	,ZG_GUI_WIDGET_TYPE_WINDOW
}ZG_GUIWidgetType;

typedef enum{
	ZG_GUI_WIDGET_LOCAL_POSITION=0,
	ZG_GUI_WIDGET_ABSOLUTE_POSITION,
	ZG_GUI_WIDGET_SCREEN_POSITION
}ZG_GUIWidgetPosition;

typedef struct ZG_GUIWidget ZG_GUIWidget;
typedef struct ZG_GUIWindow ZG_GUIWindow;
typedef struct ZG_GUIWindowManager ZG_GUIWindowManager;


typedef struct{
	void (*ptr_function)(void *_this, uint16_t new_size);
	void * calling_widget;
}ZG_CallbackWidgetOnSetDimension;

typedef struct{
	void (*ptr_function)(void *_this);
	void * calling_widget;
}ZG_CallbackWidgetUpdate;

typedef struct{
	void (*ptr_function)(void *_this,ZG_GUIWidget *widget_to_attach);
	void * calling_widget;
}ZG_CallbackWidgetAttachWidget;


struct ZG_GUIWidget{
	ZG_GUIWidgetType			type;
	void				*gui_ptr;
	void 				*data;
};

ZG_GUIWidget * 		ZG_GUIWidget_New(int x, int y, uint16_t width, uint16_t height);
void 				ZG_GUIWidget_SetDrawFunction(ZG_GUIWidget *_this,ZG_CallbackWidgetUpdate draw);
void 				ZG_GUIWidget_SetPostUpdateFunction(ZG_GUIWidget *_this,ZG_CallbackWidgetUpdate post_update);
void 				ZG_GUIWidget_UpdateFunctionOverride(ZG_GUIWidget *_this,ZG_CallbackWidgetUpdate update);
void 				ZG_GUIWidget_OnSetHeight(ZG_GUIWidget *_this,ZG_CallbackWidgetOnSetDimension on_set_height);
void 				ZG_GUIWidget_OnSetWidth(ZG_GUIWidget *_this,ZG_CallbackWidgetOnSetDimension on_set_width);
void 				ZG_GUIWidget_AttachWidgetFunctionOverride(ZG_GUIWidget *_this,ZG_CallbackWidgetAttachWidget on_set_width);

void 				ZG_GUIWidget_SetPosition2i(ZG_GUIWidget *_this,int x, int y);
void 				ZG_GUIWidget_SetPositionX(ZG_GUIWidget *_this,int x);
void 				ZG_GUIWidget_SetPositionY(ZG_GUIWidget *_this,int y);
void 				ZG_GUIWidget_SetWidth(ZG_GUIWidget *_this,uint16_t width);
void 				ZG_GUIWidget_SetHeight(ZG_GUIWidget *_this,uint16_t height);
void 				ZG_GUIWidget_SetDimensions(ZG_GUIWidget *_this,uint16_t width, uint16_t height);
ZG_Color4f 			ZG_GUIWidget_GetBackgroundColor4f(ZG_GUIWidget *_this);
void 				ZG_GUIWidget_SetBackgroundColor4f(ZG_GUIWidget *_this,ZG_Color4f _color);
void 				ZG_GUIWidget_SetColor4f(ZG_GUIWidget *_this,ZG_Color4f _color);
ZG_Color4f 			ZG_GUIWidget_GetColor4f(ZG_GUIWidget *_this);
float 				ZG_GUIWidget_GetOpacity(ZG_GUIWidget *_this);
void 				ZG_GUIWidget_SetOpacity(ZG_GUIWidget *_this,float  _opacity);

void 				ZG_GUIWidget_SetEnabled(ZG_GUIWidget *_this, bool _enabled);
bool 				ZG_GUIWidget_IsEnabled(ZG_GUIWidget *_this);
ZG_Vector2i 		ZG_GUIWidget_GetDimensions(ZG_GUIWidget *_this);

bool				ZG_GUIWidget_IsPointCollision(ZG_GUIWidget *_this,ZG_Vector2i point);

void 				ZG_GUIWidget_Update(ZG_GUIWidget *_this);

void 				ZG_GUIWidget_Draw(ZG_GUIWidget *_this);

ZG_GUIWindow 		*ZG_GUIWidget_GetWindow(ZG_GUIWidget *_this);
//ZG_TTFontManager 	*ZG_GUIWidget_GetTTFontManager(ZG_GUIWidget *_this);

void 				ZG_GUIWidget_AttachWidget(ZG_GUIWidget *_this, ZG_GUIWidget *widget);
ZG_GUIWidget * 		ZG_GUIWidget_GetParent(ZG_GUIWidget *_this);
ZG_Vector2i 		ZG_GUIWidget_GetPosition(ZG_GUIWidget *_this,ZG_GUIWidgetPosition widget_pos);
void 				ZG_GUIWidget_Delete(ZG_GUIWidget  *_this);



#endif
