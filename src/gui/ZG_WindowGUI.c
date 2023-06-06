#include "zg_gui.h"

#define DEFAULT_WINDOW_WIDTH 			150
#define DEFAULT_WINDOW_CONTENT_HEIHGT 	300


typedef struct{

	GUIFrame		*	frame_content,
					*	frame_caption;

	GUITextBox		* 	textbox_caption;
	GUIButton		*	button_close;

	bool				visible_caption;
	bool				start_dragging;
	ZG_Vector2i 			start_mouse_position;

	WindowStyle			window_style;

	GUIWindowManager	*window_manager;

}GUIWindowData;


void GUIWidget_AttachWidgetBase(GUIWidget *_this, GUIWidget *widget);
//void GUIWindow_OnMousePressDown();

void GUIWindow_OnMouseButtonUp(MouseEvent * mouse_event,void *gui_window);
void GUIWindow_OnMouseButtonDown(MouseEvent * mouse_event,void *gui_window);
void GUIWindow_OnMouseMotion(MouseEvent * mouse_event,void *gui_window);
void GUIWindow_OnSetWidth(void *gui_window,uint16_t width);
void GUIWindow_OnSetHeight(void *gui_window,uint16_t width);
void GUIWindow_AttachChild(void *gui_window, GUIWidget * widget_to_attach);

GUIWindow * GUIWindow_New(int x, int y, uint16_t _width, uint16_t _height, GUIWindowManager *_window_manager){

	GUIWindow *window = ZG_NEW(GUIWindow);
	GUIWindowData *data = ZG_NEW(GUIWindowData);
	window->data=data;

	data->visible_caption=true;

	window->widget=GUIWidget_New(x, y, _width, _height);
	window->widget->type=WIDGET_TYPE_WINDOW;
	window->widget->gui_ptr=window;

	// CAPTION
	data->frame_caption=GUIFrame_New(0,0,_width,DEFAULT_WINDOW_CAPTION_HEIGHT);
	GUIWidget_SetBackgroundColor4f(data->frame_caption->widget,ZG_Color4f_FromRGB(0,128,255));

	data->textbox_caption=GUITextBox_New(0,0,_width,DEFAULT_WINDOW_CAPTION_HEIGHT);
	ZG_TextBox_SetText(data->textbox_caption->textbox,"Window");


	data->button_close=GUIButton_New(
			_width-DEFAULT_WINDOW_CAPTION_HEIGHT
			,(DEFAULT_WINDOW_CAPTION_HEIGHT>>1)-((DEFAULT_WINDOW_CAPTION_HEIGHT*0.75f)*0.5f)
			,DEFAULT_WINDOW_CAPTION_HEIGHT*0.75
			,DEFAULT_WINDOW_CAPTION_HEIGHT*0.75
			);

	GUIButton_SetText(data->button_close,"");
	GUIButton_SetIcon(data->button_close,ZG_IconManager_GetIconDefault(DEFAULT_ICON_CLOSE_BIG));


	// CONTENT
	data->frame_content=GUIFrame_New(0,DEFAULT_WINDOW_CAPTION_HEIGHT,_width,_height);



	// SETUP CAPTION
	GUIWidget_AttachWidget(data->frame_caption->widget,data->textbox_caption->widget);
	GUIWidget_AttachWidget(data->frame_caption->widget,data->button_close->widget);

	// SETUP WINDOW
	GUIWidget_AttachWidget(window->widget,data->frame_caption->widget);
	GUIWidget_AttachWidget(window->widget,data->frame_content->widget);


	data->visible_caption=true;
	data->start_dragging=false;
	data->window_manager=_window_manager;
	data->window_style=WINDOW_STYLE_NORMAL;


	Input_AddEventOnMouseButtonDown((CallbackMouseEvent){
		.ptr_function=GUIWindow_OnMouseButtonDown
		,.user_data=window
	});

	Input_AddEventOnMouseButtonUp((CallbackMouseEvent){
		.ptr_function=GUIWindow_OnMouseButtonUp
		,.user_data=window
	});

	Input_AddEventOnMouseMotion((CallbackMouseEvent){
		.ptr_function=GUIWindow_OnMouseMotion
		,.user_data=window
	});

	GUIWidget_OnSetWidth(window->widget,(CallbackWidgetOnSetDimension){
	   .ptr_function=GUIWindow_OnSetWidth
	  ,.calling_widget=window
	});
	GUIWidget_OnSetHeight(window->widget,(CallbackWidgetOnSetDimension){
	   .ptr_function=GUIWindow_OnSetHeight
	  ,.calling_widget=window
	});

	GUIWidget_AttachWidgetFunctionOverride(window->widget,(CallbackWidgetAttachWidget){
	   .ptr_function=GUIWindow_AttachChild
	  ,.calling_widget=window
	});

	//GUIWindow_SetWidth(window,_width);
	//GUIWindow_SetHeight(window,_height);

	return window;

}

void GUIWindow_SetWidth(GUIWindow *_this,uint16_t _width){
	GUIWindowData *data= _this->data;
	GUIWidget_SetWidth(data->frame_content->widget,_width);
	GUIWidget_SetWidth(data->frame_caption->widget,_width);
	GUIWidget_SetWidth(data->textbox_caption->widget,_width);
	GUIWidget_SetPositionX(data->button_close->widget,_width-DEFAULT_WINDOW_CAPTION_HEIGHT);
}

void GUIWindow_SetHeight(GUIWindow *_this, uint16_t _height){
	GUIWindowData *data= _this->data;
	GUIWidget_SetHeight(data->frame_content->widget,_height);
}

void GUIWindow_SetVisibleCaption(GUIWindow *_this, bool _v){
	GUIWindowData *data= _this->data;
	data->visible_caption=_v;

	ZG_Vector2i dim_content=GUIWidget_GetDimensions(data->frame_content->widget);
	ZG_Vector2i dim_caption=GUIWidget_GetDimensions(data->frame_caption->widget);


	if(_v){
		GUIWidget_SetPosition2i(_this->widget,0,DEFAULT_WINDOW_CAPTION_HEIGHT);
		GUIWidget_SetHeight(_this->widget,dim_content.y+dim_caption.y);
		GUIWidget_SetEnabled(data->button_close->widget,true);
	}
	else{
		GUIWidget_SetPosition2i(data->frame_content->widget,0,0);
		GUIWidget_SetHeight(data->frame_content->widget,dim_content.y+DEFAULT_WINDOW_CAPTION_HEIGHT);
		GUIWidget_SetEnabled(data->button_close->widget,false);
	}
}

void GUIWindow_SetBackgroundColor3i(GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b){
	GUIWindowData *data=_this->data;
	GUIWidget_SetBackgroundColor4f(data->frame_content->widget,ZG_Color4f_FromRGB(r,g,b));
}

void 		GUIWindow_SetBackgroundColor4f(GUIWindow * _this, ZG_Color4f color){
	GUIWindowData *data=_this->data;
	GUIWidget_SetBackgroundColor4f(data->frame_content->widget,color);
}

void 		GUIWindow_SetCaptionTitle(GUIWindow * _this, const char *_caption){
	GUIWindowData *data=_this->data;
	ZG_TextBox_SetText(data->textbox_caption->textbox,_caption);//frame_content->widget->background_color=Color4f_FromHexStr(color);
}

ZG_TextureManager 		*GUIWindow_GetTextureManager(GUIWindow * _this){

	if(_this == NULL){
		return NULL;
	}

	GUIWindowData *data=_this->data;
	if(data->window_manager){
		return GUIWindowManager_GetTextureManager(data->window_manager);//data->ttfont_manager;
	}
	return NULL;
}
/*
ZG_TTFontManager 		*GUIWindow_GetTTFontManager(GUIWindow * _this){
	GUIWindowData *data=_this->data;
	if(data->window_manager){
		return GUIWindowManager_GetTTFontManager(data->window_manager);//data->ttfont_manager;
	}
	return NULL;
}
*/

void 				GUIWindow_SetWindowStyle(GUIWindow * _this, WindowStyle _window_style){
	GUIWindowData *data=_this->data;
	switch(data->window_style=_window_style){
	case WINDOW_STYLE_NONE:
		GUIWindow_SetVisibleCaption(_this,false);
		break;
	case WINDOW_STYLE_NORMAL:
		GUIWindow_SetVisibleCaption(_this,true);
		break;
	}
}

void GUIWindow_OnSetWidth(void *gui_window, uint16_t width){

	UNUSUED_PARAM(width);

	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	ZG_Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);
	ZG_Vector2i caption_dimensions=GUIWidget_GetDimensions(data->frame_caption->widget);


	GUIWidget_SetWidth(data->frame_caption->widget,dimensions.x);
	GUIWidget_SetWidth(data->frame_content->widget,dimensions.x);

	GUIWidget_SetPosition2i(data->button_close->widget,dimensions.x -(DEFAULT_WINDOW_CAPTION_HEIGHT),(caption_dimensions.y>>1)-(DEFAULT_WINDOW_CAPTION_HEIGHT*0.75)*0.5);

}

// the attach child is overrided to add in frame_content...
void GUIWindow_AttachChild(void *gui_window, GUIWidget * widget_to_attach){
	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	GUIWidget_AttachWidgetBase(data->frame_content->widget,widget_to_attach);
}

void GUIWindow_OnSetHeight(void *gui_window,uint16_t height){
	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	GUIWidget_SetHeight(data->frame_content->widget,height);
}


void GUIWindow_OnMouseButtonDown(MouseEvent * mouse_event, void *gui_window){

	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	if(!data->visible_caption){
		return;
	}

	//mouse_position=Input_GetMousePosition();

	if(GUIWidget_IsPointCollision(data->button_close->widget,mouse_event->position))
	{
	//	ZG_Log_Info("button collision");
		// collision with button (ignore it!)
		return;
	}

	if(GUIWidget_IsPointCollision(data->frame_caption->widget,mouse_event->position))
	{
		if(Input_IsLeftButtonPressed()) {
			ZG_Log_InfoF("start dragging");
			data->start_dragging = true;
			data->start_mouse_position=Input_GetMousePosition();
		}
	 }
}

void  GUIWindow_OnMouseMotion(MouseEvent * event, void *gui_window){

	UNUSUED_PARAM(event);

	 GUIWindow *_this=gui_window;
	 GUIWindowData *data=_this->data;

	if(!data->visible_caption){
		return;
	}


	if(data->start_dragging) {

		//ZG_Log_Info("moving !");

		ZG_Vector2i position = Vector2i_Add(
			GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_LOCAL)
			,Vector2i_Sub(Input_GetMousePosition(),data->start_mouse_position)
		);

		GUIWidget_SetPosition2i(_this->widget,position.x,position.y);


		data->start_mouse_position=Input_GetMousePosition();

	}

	return;
}

 void GUIWindow_OnMouseButtonUp(MouseEvent * event, void *gui_window){
	 UNUSUED_PARAM(event);

	 GUIWindow *_this=gui_window;
	 GUIWindowData *data=_this->data;

	// ZG_Log_Info("mouse up");

	if(!data->visible_caption){
		return;
	}

	if(data->start_dragging) {

		data->start_dragging = false;
	}
}

void GUIWindow_Delete(GUIWindow *_this) {
	if(_this == NULL) return;
	GUIWindowData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	GUIFrame_Delete(data->frame_caption);
	GUIFrame_Delete(data->frame_content);
	GUITextBox_Delete(data->textbox_caption);
	GUIButton_Delete(data->button_close);

	ZG_FREE(data);
	ZG_FREE(_this);
}

