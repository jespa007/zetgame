#include "@zg_gui.h"

#define ZG_DEFAULT_WINDOW_WIDTH 			150
#define ZG_DEFAULT_WINDOW_CONTENT_HEIHGT 	300


typedef struct{
	ZG_List *list;
	ZG_MapString *ids;
}ZG_GUIWindowWidgetContainer;

typedef struct{

	ZG_GUIFrame			*	frame_content,
						*	frame_caption;

	ZG_GUITextBox		* 	textbox_caption;
	ZG_GUIButton		*	button_close;

	bool					visible_caption;
	bool					start_dragging;
	ZG_Vector2i 			start_mouse_position;

	ZG_GUIWindowStyle		window_style;

	//
	ZG_GUIWindowWidgetContainer gui_buttons,gui_textures,gui_frames,gui_textboxes;

}ZG_GUIWindowData;


void ZG_GUIWidget_AttachWidgetBase(ZG_GUIWidget *_this, ZG_GUIWidget *widget);
//void ZG_GUIWindow_OnMousePressDown();

void ZG_GUIWindow_OnMouseButtonUp(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnMouseButtonDown(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnMouseMotion(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnSetWidth(void *gui_window,uint16_t width);
void ZG_GUIWindow_OnSetHeight(void *gui_window,uint16_t width);
void ZG_GUIWindow_AttachChild(void *gui_window, ZG_GUIWidget * widget_to_attach);

#include "ZG_GUIWindowXml.c"

ZG_GUIWindow * ZG_GUIWindow_New(int x, int y, uint16_t _width, uint16_t _height){

	ZG_GUIWindow *window = ZG_NEW(ZG_GUIWindow);
	ZG_GUIWindowData *data = ZG_NEW(ZG_GUIWindowData);

	// list of widget container to ALLOCATE
	struct{
		ZG_GUIWindowWidgetContainer	widgets;
	}widget_allocate_containers[]={
		{data->gui_buttons}
		,{data->gui_textboxes}
		,{data->gui_textures}
		,{data->gui_frames}
	};

	for(unsigned j=0; j < ZG_ARRAY_SIZE(widget_allocate_containers); j++){
		widget_allocate_containers[j].widgets.ids=ZG_MapString_New();
		widget_allocate_containers[j].widgets.list=ZG_List_New();
	}

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
	data->window_style=ZG_GUI_WINDOW_STYLE_NORMAL;

	ZG_Input_AddCallbackOnMouseButtonDown((ZG_MouseEventCallback){
		.ptr_function=ZG_GUIWindow_OnMouseButtonDown
		,.user_data=window
	});

	ZG_Input_AddCallbackOnMouseButtonUp((ZG_MouseEventCallback){
		.ptr_function=ZG_GUIWindow_OnMouseButtonUp
		,.user_data=window
	});

	ZG_Input_AddCallbackOnMouseMotion((ZG_MouseEventCallback){
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
/*
ZG_TextureManager 		*ZG_GUIWindow_GetTextureManager(ZG_GUIWindow * _this){

	if(_this == NULL){
		return NULL;
	}

	ZG_GUIWindowData *data=_this->data;
	if(data->window_manager){
		return ZG_GUIWindow_GetTextureManager(data->window_manager);//data->ttfont_manager;
	}
	return NULL;
}

ZG_TTFontManager 		*ZG_GUIWindow_GetTTFontManager(ZG_GUIWindow * _this){
	ZG_GUIWindowData *data=_this->data;
	if(data->window_manager){
		return ZG_GUIWindow_GetTTFontManager(data->window_manager);//data->ttfont_manager;
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

	ZG_GUIWidget_AttachWidget(data->frame_content->widget,widget_to_attach);
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
			ZG_GUIWidget_GetPosition(_this->widget,ZG_GUI_WIDGET_LOCAL_POSITION)
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

 ZG_GUIButton		*	ZG_GUIWindow_NewButton(ZG_GUIWindow * _this, const char *_id){
	 ZG_GUIButton *gui_button=ZG_GUIButton_New(0,0,0,0);
	 ZG_GUIWindow_AddGUIButton(_this,gui_button,_id);
	 return gui_button;
 }

 ZG_GUITextBox		* 	ZG_GUIWindow_NewTextBox(ZG_GUIWindow * _this, const char *_id){
	 ZG_GUITextBox *gui_textbox=ZG_GUITextBox_New(0,0,0,0);
	 ZG_GUIWindow_AddGUITextBox(_this,gui_textbox,_id);
	 return gui_textbox;
 }

 ZG_GUITexture		* 	ZG_GUIWindow_NewGUITexture(ZG_GUIWindow * _this, const char *_id){
	 ZG_GUITexture *gui_texture=ZG_GUITexture_New(0,0,0,0);
	 ZG_GUIWindow_AddGUITexture(_this,gui_texture,_id);
	 return gui_texture;
 }

 ZG_GUIFrame			* 	ZG_GUIWindow_NewGUIFrame(ZG_GUIWindow * _this, const char *_id){
	 ZG_GUIFrame *gui_frame=ZG_GUIFrame_New(0,0,0,0);
	 ZG_GUIWindow_AddGUIFrame(_this,gui_frame,_id);
	 return gui_frame;
}

//----


 void 	ZG_GUIWindow_AddGUIWidget(ZG_GUIWindowWidgetContainer _window_gui_widget_container,void *_gui_widget, const char *_id){

 	 size_t idx_pos=ZG_List_Count(_window_gui_widget_container.list);

 	 ZG_List_Add(_window_gui_widget_container.list, _gui_widget);

 	 if(_id!=NULL){
 		 ZG_MapString_Set(_window_gui_widget_container.ids, _id, (void *)idx_pos);
 	 }
 }


void	ZG_GUIWindow_AddGUIButton(ZG_GUIWindow * _this,ZG_GUIButton *_gui_button, const char *_id){
	ZG_GUIWindowData *data=_this->data;
	ZG_GUIWindow_AddGUIWidget(data->gui_buttons,_gui_button,_id);
}

void 	ZG_GUIWindow_AddGUITextBox(ZG_GUIWindow * _this,ZG_GUITextBox *_gui_textbox, const char *_id){
	ZG_GUIWindowData *data=_this->data;
	ZG_GUIWindow_AddGUIWidget(data->gui_textboxes,_gui_textbox,_id);
}

void 	ZG_GUIWindow_AddGUITexture(ZG_GUIWindow * _this,ZG_GUITexture *_gui_texture, const char *_id){
	ZG_GUIWindowData *data=_this->data;
	ZG_GUIWindow_AddGUIWidget(data->gui_textures,_gui_texture,_id);
}

void 	ZG_GUIWindow_AddGUIFrame(ZG_GUIWindow * _this,ZG_GUIFrame *_gui_frame, const char *_id){
	ZG_GUIWindowData *data=_this->data;
	ZG_GUIWindow_AddGUIWidget(data->gui_textures,_gui_frame,_id);
}


ZG_GUIButton		*	ZG_GUIWindow_GetGUIButton(ZG_GUIWindow * _this, const char *_id){
	bool exists=false;
	ZG_GUIWindowData *data=_this->data;
	size_t pos_idx=(size_t)ZG_MapString_Get(data->gui_buttons.ids,_id,&exists);

	if(exists == false){
		ZG_LOG_ERROR("ZG_GUIWindow_GetGUIButton : GUIButton '%s' does not exist",_id);
		return NULL;
	}

	return ZG_List_Get(data->gui_buttons.list,pos_idx);


}

ZG_GUITextBox		* 	ZG_GUIWindow_GetGUITextBox(ZG_GUIWindow * _this, const char *_id){
	bool exists=false;
	ZG_GUIWindowData *data=_this->data;
	size_t pos_idx=(size_t)ZG_MapString_Get(data->gui_textboxes.ids,_id,&exists);

	if(exists == false){
		ZG_LOG_ERROR("ZG_GUIWindow_GetGUITextBox : GUITextBox '%s' does not exist",_id);
		return NULL;
	}

	return ZG_List_Get(data->gui_textboxes.list,pos_idx);
}

ZG_GUITexture		* 	ZG_GUIWindow_GetGUITexture(ZG_GUIWindow * _this, const char *_id){
	bool exists=false;
	ZG_GUIWindowData *data=_this->data;
	size_t pos_idx=(size_t)ZG_MapString_Get(data->gui_textures.ids,_id,&exists);

	if(exists == false){
		ZG_LOG_ERROR("ZG_GUIWindow_GetGUITexture : GUITexture '%s' does not exist",_id);
		return NULL;
	}

	return ZG_List_Get(data->gui_textures.list,pos_idx);
}

ZG_GUIFrame			* 	ZG_GUIWindow_GetGUIFrame(ZG_GUIWindow * _this, const char *_id){
	bool exists=false;
	ZG_GUIWindowData *data=_this->data;
	size_t pos_idx=(size_t)ZG_MapString_Get(data->gui_frames.ids,_id,&exists);

	if(exists == false){
		ZG_LOG_ERROR("ZG_GUIWindow_GetGUITexture : GUIFrame '%s' does not exist",_id);
		return NULL;
	}

	return ZG_List_Get(data->gui_frames.list,pos_idx);
}


void ZG_GUIWindow_Delete(ZG_GUIWindow *_this) {
	if(_this == NULL) return;
	ZG_GUIWindowData *data=_this->data;

	// erase window components
	ZG_GUIWidget_Delete(_this->widget);
	ZG_GUIFrame_Delete(data->frame_caption);
	ZG_GUIFrame_Delete(data->frame_content);
	ZG_GUITextBox_Delete(data->textbox_caption);
	ZG_GUIButton_Delete(data->button_close);

	struct{
		ZG_GUIWindowWidgetContainer	widgets;
	}widget_allocate_containers[]={
		{data->gui_buttons}
		,{data->gui_textboxes}
		,{data->gui_textures}
		,{data->gui_frames}
	};

	for(unsigned j=0; j < ZG_ARRAY_SIZE(widget_allocate_containers); j++){
		widget_allocate_containers[j].widgets.ids=ZG_MapString_New();
		widget_allocate_containers[j].widgets.list=ZG_List_New();
	}

	// list of widget container to DEALLOCATE
	struct{
		ZG_GUIWindowWidgetContainer	widget_containers;
		void (*delete_callback)(void *_this);
	}widget_deallocate_collections[]={
		{data->gui_buttons,(void (*)(void *))ZG_GUIButton_Delete}
		,{data->gui_textboxes,(void (*)(void *))ZG_GUITextBox_Delete}
		,{data->gui_textures,(void (*)(void *))ZG_GUITexture_Delete}
		,{data->gui_frames,(void (*)(void *))ZG_GUIFrame_Delete}
	};

	for(unsigned j=0; j < ZG_ARRAY_SIZE(widget_deallocate_collections); j++){
		ZG_List *list_widgets=widget_deallocate_collections[j].widget_containers.list;
		void (*delete_callback)(void *_this) = widget_deallocate_collections[j].delete_callback;

		for(unsigned i = 0; ZG_List_Count(list_widgets);i++){

			void *widget=ZG_List_Get(list_widgets,i);
			// deallocate all widgets
			delete_callback(widget);
		}

		ZG_MapString_Delete(widget_deallocate_collections[j].widget_containers.ids);
		ZG_List_Delete(widget_deallocate_collections[j].widget_containers.list);
	}


	ZG_FREE(data);
	ZG_FREE(_this);
}

