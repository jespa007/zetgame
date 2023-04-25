#ifndef __GUI_WIDGET_H__
#define __GUI_WIDGET_H__


//#define GUI_NO_ICON 			GUIIconManager_GetIcon(NULL,0)
//#define GUI_DEFAULT_ICON(x)		GUIIconManager_GetIcon("", x)

#define GUI_GET_RELATIVE_X(widget) (widget->position_local.x + widget->position_offset.x)
#define GUI_GET_RELATIVE_Y(widget) (widget->position_local.y + widget->position_offset.y)

typedef enum{
	WIDGET_TYPE_UNKNOWN=0
	,WIDGET_TYPE_WIDGET
	,WIDGET_TYPE_FRAME
	,WIDGET_TYPE_LABEL
	,WIDGET_TYPE_TEXTBOX
	,WIDGET_TYPE_BUTTON
	,WIDGET_TYPE_VIEWER
	,WIDGET_TYPE_WINDOW
}WidgetType;

typedef enum{
	WIDGET_POSITION_LOCAL=0,
	WIDGET_POSITION_WORLD,
	WIDGET_POSITION_SCREEN
}WidgetPosition;

typedef struct GUIWidget GUIWidget;
typedef struct GUIWindow GUIWindow;
typedef struct GUIWindowManager GUIWindowManager;


typedef struct{
	void (*ptr_function)(void *_this, uint16_t new_size);
	void * calling_widget;
}CallbackWidgetOnSetDimension;

typedef struct{
	void (*ptr_function)(void *_this);
	void * calling_widget;
}CallbackWidgetUpdate;

typedef struct{
	void (*ptr_function)(void *_this,GUIWidget *widget_to_attach);
	void * calling_widget;
}CallbackWidgetAttachWidget;


struct GUIWidget{
	WidgetType			type;
	Color4f				color,background_color;
	float 				opacity;
	bool 				is_enabled;
	void 				*data;
};

GUIWidget * GUIWidget_New(int x, int y, uint16_t width, uint16_t height);
void GUIWidget_SetDrawFunction(GUIWidget *_this,CallbackWidgetUpdate draw);
void GUIWidget_SetPostUpdateFunction(GUIWidget *_this,CallbackWidgetUpdate post_update);
void GUIWidget_UpdateFunctionOverride(GUIWidget *_this,CallbackWidgetUpdate update);
void GUIWidget_OnSetHeight(GUIWidget *_this,CallbackWidgetOnSetDimension on_set_height);
void GUIWidget_OnSetWidth(GUIWidget *_this,CallbackWidgetOnSetDimension on_set_width);
void GUIWidget_AttachWidgetFunctionOverride(GUIWidget *_this,CallbackWidgetAttachWidget on_set_width);

void GUIWidget_SetPosition2i(GUIWidget *_this,int x, int y);
void GUIWidget_SetPositionX(GUIWidget *_this,int x);
void GUIWidget_SetPositionY(GUIWidget *_this,int y);
void GUIWidget_SetWidth(GUIWidget *_this,uint16_t width);
void GUIWidget_SetHeight(GUIWidget *_this,uint16_t height);
void GUIWidget_SetDimensions(GUIWidget *_this,uint16_t width, uint16_t height);
Vector2i GUIWidget_GetDimensions(GUIWidget *_this);

bool	GUIWidget_IsPointCollision(GUIWidget *_this,Vector2i point);

void GUIWidget_Update(GUIWidget *_this);

void GUIWidget_Draw(GUIWidget *_this);

GUIWindow 		*GUIWidget_GetWindow(GUIWidget *_this);
TTFontManager 	*GUIWidget_GetTTFontManager(GUIWidget *_this);
TextureManager 	*GUIWidget_GetTextureManager(GUIWidget *_this);

void GUIWidget_AttachWidget(GUIWidget *_this, GUIWidget *widget);
GUIWidget * GUIWidget_GetParent(GUIWidget *_this);
Vector2i GUIWidget_GetPosition(GUIWidget *_this,WidgetPosition widget_pos);
void GUIWidget_Delete(GUIWidget  *_this);



#endif
