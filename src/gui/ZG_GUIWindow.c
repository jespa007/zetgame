#include "@zg_gui.h"

#define ZG_DEFAULT_WINDOW_WIDTH 			150
#define ZG_DEFAULT_WINDOW_CONTENT_HEIHGT 	300


typedef struct{

	ZG_GUIFrame		*	frame_content,
					*	frame_caption;

	ZG_GUITextBox		* 	textbox_caption;
	ZG_GUIButton		*	button_close;

	bool				visible_caption;
	bool				start_dragging;
	ZG_Vector2i 			start_mouse_position;

	ZG_GUIWindowStyle			window_style;

	ZG_GUIWindowManager	*window_manager;

}ZG_GUIWindowData;


void ZG_GUIWidget_AttachWidgetBase(ZG_GUIWidget *_this, ZG_GUIWidget *widget);
//void ZG_GUIWindow_OnMousePressDown();

void ZG_GUIWindow_OnMouseButtonUp(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnMouseButtonDown(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnMouseMotion(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnSetWidth(void *gui_window,uint16_t width);
void ZG_GUIWindow_OnSetHeight(void *gui_window,uint16_t width);
void ZG_GUIWindow_AttachChild(void *gui_window, ZG_GUIWidget * widget_to_attach);

ZG_GUIWindow * ZG_GUIWindow_New(int x, int y, uint16_t _width, uint16_t _height, ZG_GUIWindowManager *_window_manager){

	ZG_GUIWindow *window = ZG_NEW(ZG_GUIWindow);
	ZG_GUIWindowData *data = ZG_NEW(ZG_GUIWindowData);
	window->data=data;

	data->visible_caption=true;

	window->widget=ZG_GUIWidget_New(x, y, _width, _height);
	window->widget->type=ZG_GUI_WIDGET_TYPE_WINDOW;
	window->widget->gui_ptr=window;

	// CAPTION
	data->frame_caption=ZG_GUIFrame_New(0,0,_width,ZG_DEFAULT_WINDOW_CAPTION_HEIGHT);
	ZG_GUIWidget_SetBackgroundColor4f(data->frame_caption->widget,ZG_Color4f_FromRGB(0,128,255));

	data->textbox_caption=ZG_GUITextBox_New(0,0,_width,ZG_DEFAULT_WINDOW_CAPTION_HEIGHT);
	ZG_TextBox_SetText(data->textbox_caption->textbox,"Window");


	data->button_close=ZG_GUIButton_New(
			_width-ZG_DEFAULT_WINDOW_CAPTION_HEIGHT
			,(ZG_DEFAULT_WINDOW_CAPTION_HEIGHT>>1)-((ZG_DEFAULT_WINDOW_CAPTION_HEIGHT*0.75f)*0.5f)
			,ZG_DEFAULT_WINDOW_CAPTION_HEIGHT*0.75
			,ZG_DEFAULT_WINDOW_CAPTION_HEIGHT*0.75
			);

	ZG_GUIButton_SetText(data->button_close,"");
	ZG_GUIButton_SetIcon(data->button_close,ZG_IconManager_GetIconDefault(ZG_DEFAULT_ICON_CLOSE_BIG));


	// CONTENT
	data->frame_content=ZG_GUIFrame_New(0,ZG_DEFAULT_WINDOW_CAPTION_HEIGHT,_width,_height);



	// SETUP CAPTION
	ZG_GUIWidget_AttachWidget(data->frame_caption->widget,data->textbox_caption->widget);
	ZG_GUIWidget_AttachWidget(data->frame_caption->widget,data->button_close->widget);

	// SETUP WINDOW
	ZG_GUIWidget_AttachWidget(window->widget,data->frame_caption->widget);
	ZG_GUIWidget_AttachWidget(window->widget,data->frame_content->widget);


	data->visible_caption=true;
	data->start_dragging=false;
	data->window_manager=_window_manager;
	data->window_style=ZG_GUI_WINDOW_STYLE_NORMAL;


	ZG_Input_AddEventOnMouseButtonDown((ZG_CallbackMouseEvent){
		.ptr_function=ZG_GUIWindow_OnMouseButtonDown
		,.user_data=window
	});

	ZG_Input_AddEventOnMouseButtonUp((ZG_CallbackMouseEvent){
		.ptr_function=ZG_GUIWindow_OnMouseButtonUp
		,.user_data=window
	});

	ZG_Input_AddEventOnMouseMotion((ZG_CallbackMouseEvent){
		.ptr_function=ZG_GUIWindow_OnMouseMotion
		,.user_data=window
	});

	ZG_GUIWidget_OnSetWidth(window->widget,(ZG_CallbackWidgetOnSetDimension){
	   .ptr_function=ZG_GUIWindow_OnSetWidth
	  ,.calling_widget=window
	});
	ZG_GUIWidget_OnSetHeight(window->widget,(ZG_CallbackWidgetOnSetDimension){
	   .ptr_function=ZG_GUIWindow_OnSetHeight
	  ,.calling_widget=window
	});

	ZG_GUIWidget_AttachWidgetFunctionOverride(window->widget,(ZG_CallbackWidgetAttachWidget){
	   .ptr_function=ZG_GUIWindow_AttachChild
	  ,.calling_widget=window
	});

	//ZG_GUIWindow_SetWidth(window,_width);
	//ZG_GUIWindow_SetHeight(window,_height);

	return window;

}

void ZG_GUIWindow_SetWidth(ZG_GUIWindow *_this,uint16_t _width){
	ZG_GUIWindowData *data= _this->data;
	ZG_GUIWidget_SetWidth(data->frame_content->widget,_width);
	ZG_GUIWidget_SetWidth(data->frame_caption->widget,_width);
	ZG_GUIWidget_SetWidth(data->textbox_caption->widget,_width);
	ZG_GUIWidget_SetPositionX(data->button_close->widget,_width-ZG_DEFAULT_WINDOW_CAPTION_HEIGHT);
}

void ZG_GUIWindow_SetHeight(ZG_GUIWindow *_this, uint16_t _height){
	ZG_GUIWindowData *data= _this->data;
	ZG_GUIWidget_SetHeight(data->frame_content->widget,_height);
}

void ZG_GUIWindow_SetVisibleCaption(ZG_GUIWindow *_this, bool _v){
	ZG_GUIWindowData *data= _this->data;
	data->visible_caption=_v;

	ZG_Vector2i dim_content=ZG_GUIWidget_GetDimensions(data->frame_content->widget);
	ZG_Vector2i dim_caption=ZG_GUIWidget_GetDimensions(data->frame_caption->widget);


	if(_v){
		ZG_GUIWidget_SetPosition2i(_this->widget,0,ZG_DEFAULT_WINDOW_CAPTION_HEIGHT);
		ZG_GUIWidget_SetHeight(_this->widget,dim_content.y+dim_caption.y);
		ZG_GUIWidget_SetEnabled(data->button_close->widget,true);
	}
	else{
		ZG_GUIWidget_SetPosition2i(data->frame_content->widget,0,0);
		ZG_GUIWidget_SetHeight(data->frame_content->widget,dim_content.y+ZG_DEFAULT_WINDOW_CAPTION_HEIGHT);
		ZG_GUIWidget_SetEnabled(data->button_close->widget,false);
	}
}

void ZG_GUIWindow_SetBackgroundColor3i(ZG_GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b){
	ZG_GUIWindowData *data=_this->data;
	ZG_GUIWidget_SetBackgroundColor4f(data->frame_content->widget,ZG_Color4f_FromRGB(r,g,b));
}

void 		ZG_GUIWindow_SetBackgroundColor4f(ZG_GUIWindow * _this, ZG_Color4f color){
	ZG_GUIWindowData *data=_this->data;
	ZG_GUIWidget_SetBackgroundColor4f(data->frame_content->widget,color);
}

void 		ZG_GUIWindow_SetCaptionTitle(ZG_GUIWindow * _this, const char *_caption){
	ZG_GUIWindowData *data=_this->data;
	ZG_TextBox_SetText(data->textbox_caption->textbox,_caption);//frame_content->widget->background_color=Color4f_FromHexStr(color);
}

ZG_TextureManager 		*ZG_GUIWindow_GetTextureManager(ZG_GUIWindow * _this){

	if(_this == NULL){
		return NULL;
	}

	ZG_GUIWindowData *data=_this->data;
	if(data->window_manager){
		return ZG_GUIWindowManager_GetTextureManager(data->window_manager);//data->ttfont_manager;
	}
	return NULL;
}
/*
ZG_TTFontManager 		*ZG_GUIWindow_GetTTFontManager(ZG_GUIWindow * _this){
	ZG_GUIWindowData *data=_this->data;
	if(data->window_manager){
		return ZG_GUIWindowManager_GetTTFontManager(data->window_manager);//data->ttfont_manager;
	}
	return NULL;
}
*/

void 				ZG_GUIWindow_SetWindowStyle(ZG_GUIWindow * _this, ZG_GUIWindowStyle _window_style){
	ZG_GUIWindowData *data=_this->data;
	switch(data->window_style=_window_style){
	case ZG_GUI_WINDOW_STYLE_NONE:
		ZG_GUIWindow_SetVisibleCaption(_this,false);
		break;
	case ZG_GUI_WINDOW_STYLE_NORMAL:
		ZG_GUIWindow_SetVisibleCaption(_this,true);
		break;
	}
}

void ZG_GUIWindow_OnSetWidth(void *gui_window, uint16_t width){

	ZG_UNUSUED_PARAM(width);

	ZG_GUIWindow *_this=gui_window;
	ZG_GUIWindowData *data=_this->data;

	ZG_Vector2i dimensions=ZG_GUIWidget_GetDimensions(_this->widget);
	ZG_Vector2i caption_dimensions=ZG_GUIWidget_GetDimensions(data->frame_caption->widget);


	ZG_GUIWidget_SetWidth(data->frame_caption->widget,dimensions.x);
	ZG_GUIWidget_SetWidth(data->frame_content->widget,dimensions.x);

	ZG_GUIWidget_SetPosition2i(data->button_close->widget,dimensions.x -(ZG_DEFAULT_WINDOW_CAPTION_HEIGHT),(caption_dimensions.y>>1)-(ZG_DEFAULT_WINDOW_CAPTION_HEIGHT*0.75)*0.5);

}

// the attach child is overrided to add in frame_content...
void ZG_GUIWindow_AttachChild(void *gui_window, ZG_GUIWidget * widget_to_attach){
	ZG_GUIWindow *_this=gui_window;
	ZG_GUIWindowData *data=_this->data;

	ZG_GUIWidget_AttachWidgetBase(data->frame_content->widget,widget_to_attach);
}

void ZG_GUIWindow_OnSetHeight(void *gui_window,uint16_t height){
	ZG_GUIWindow *_this=gui_window;
	ZG_GUIWindowData *data=_this->data;

	ZG_GUIWidget_SetHeight(data->frame_content->widget,height);
}


void ZG_GUIWindow_OnMouseButtonDown(ZG_MouseEvent * mouse_event, void *gui_window){

	ZG_GUIWindow *_this=gui_window;
	ZG_GUIWindowData *data=_this->data;

	if(!data->visible_caption){
		return;
	}

	//mouse_position=ZG_Input_GetMousePosition();

	if(ZG_GUIWidget_IsPointCollision(data->button_close->widget,mouse_event->position))
	{
	//	ZG_LOG_INFO("button collision");
		// collision with button (ignore it!)
		return;
	}

	if(ZG_GUIWidget_IsPointCollision(data->frame_caption->widget,mouse_event->position))
	{
		if(ZG_Input_IsLeftButtonPressed()) {
			ZG_LOG_INFOF("start dragging");
			data->start_dragging = true;
			data->start_mouse_position=ZG_Input_GetMousePosition();
		}
	 }
}

void  ZG_GUIWindow_OnMouseMotion(ZG_MouseEvent * event, void *gui_window){

	ZG_UNUSUED_PARAM(event);

	 ZG_GUIWindow *_this=gui_window;
	 ZG_GUIWindowData *data=_this->data;

	if(!data->visible_caption){
		return;
	}


	if(data->start_dragging) {

		//ZG_LOG_INFO("moving !");

		ZG_Vector2i position = ZG_Vector2i_Add(
			ZG_GUIWidget_GetPosition(_this->widget,ZG_GUI_WIDGET_POSITION_LOCAL)
			,ZG_Vector2i_Sub(ZG_Input_GetMousePosition(),data->start_mouse_position)
		);

		ZG_GUIWidget_SetPosition2i(_this->widget,position.x,position.y);


		data->start_mouse_position=ZG_Input_GetMousePosition();

	}

	return;
}

 void ZG_GUIWindow_OnMouseButtonUp(ZG_MouseEvent * event, void *gui_window){
	 ZG_UNUSUED_PARAM(event);

	 ZG_GUIWindow *_this=gui_window;
	 ZG_GUIWindowData *data=_this->data;

	// ZG_LOG_INFO("mouse up");

	if(!data->visible_caption){
		return;
	}

	if(data->start_dragging) {

		data->start_dragging = false;
	}
}

void ZG_GUIWindow_Delete(ZG_GUIWindow *_this) {
	if(_this == NULL) return;
	ZG_GUIWindowData *data=_this->data;

	ZG_GUIWidget_Delete(_this->widget);
	ZG_GUIFrame_Delete(data->frame_caption);
	ZG_GUIFrame_Delete(data->frame_content);
	ZG_GUITextBox_Delete(data->textbox_caption);
	ZG_GUIButton_Delete(data->button_close);

	ZG_FREE(data);
	ZG_FREE(_this);
}

