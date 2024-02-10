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

	//
	ZG_MapString			*	buttons;
	ZG_MapString			*	textures;
	ZG_MapString			*	frames;
	ZG_MapString			*	textboxes;

}ZG_GUIWindowData;


void ZG_GUIWidget_AttachWidgetBase(ZG_GUIWidget *_this, ZG_GUIWidget *widget);
//void ZG_GUIWindow_OnMousePressDown();

void ZG_GUIWindow_OnMouseButtonUp(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnMouseButtonDown(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnMouseMotion(ZG_MouseEvent * mouse_event,void *gui_window);
void ZG_GUIWindow_OnSetWidth(void *gui_window,uint16_t width);
void ZG_GUIWindow_OnSetHeight(void *gui_window,uint16_t width);
void ZG_GUIWindow_AttachChild(void *gui_window, ZG_GUIWidget * widget_to_attach);





ZG_GUIWindow * ZG_GUIWindow_New(int x, int y, uint16_t _width, uint16_t _height){

	ZG_GUIWindow *window = ZG_NEW(ZG_GUIWindow);
	ZG_GUIWindowData *data = ZG_NEW(ZG_GUIWindowData);


	 data->buttons=ZG_MapString_New();
	 data->textboxes=ZG_MapString_New();
	 data->viewers=ZG_MapString_New();
	 data->frames=ZG_MapString_New();

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



ZG_GUIWindow ZG_GUIWindow_NewFromMemory(
		uint8_t *_xml_buf
		,size_t _xml_len
	){

	ZG_GUIWindowManagerData *data=_this->data;

	ZG_UNUSUED_PARAM(_xml_len);

	bool ok=false;


	// first read tilesets...
	XmlDoc *doc = parseDoc((char *)_xml_buf);

	 if (xmlDocError(doc) != XML_SUCCESS)
	 {
		/* unparsable XML was read from stdin */
		ZG_LOG_ERRORF("Cannot parse xml");
		goto wm_load_from_memmory_exit;
	}

	 if(ZG_STRCMP(doc->root->name,!=,"window")){
			ZG_LOG_ERROR("Expected first tag as 'window' but it was '%s'",doc->root->name);
			goto wm_load_from_memmory_exit;
	 }

	 //GUIWMWindowData *window_data=ZG_NEW(GUIWMWindowData);


	 // setup gui elements
	 window_data->buttons=ZG_List_New();
	 window_data->textboxes=ZG_List_New();
	 window_data->viewers=ZG_List_New();
	 window_data->frames=ZG_List_New();
	 //window_data->gui_window_manager_data=data;


	 window_data->window=ZG_GUIWindow_New(0,0,ZG_Graphics_GetWidth(),ZG_Graphics_GetHeight()-ZG_DEFAULT_WINDOW_CAPTION_HEIGHT,_this);
	 XmlAttribute *attribute=doc->root->attributes;
	 XmlElement *children=doc->root->children;

	 // setup window
	 if(attribute != NULL){
		 do{
			 int int_value=0;
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 if(ZG_STRCMP(attribute->name,==,"background-color")){
				 ZG_GUIWindow_SetBackgroundColor4f(window_data->window,ZG_Color4f_FromHtml(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"window-style")){
				 if(ZG_STRCMP(attribute->value,==,"none")){
					 ZG_GUIWindow_SetWindowStyle(window_data->window,ZG_GUI_WINDOW_STYLE_NONE);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"caption")){
				 ZG_GUIWindow_SetCaptionTitle(window_data->window,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWindow_SetWidth(window_data->window,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWindow_SetHeight(window_data->window,int_value);
				 }
			 }else{
				 ZG_LOG_ERROR("unexpected attribute '%s'",attribute->name);
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	 // process window childs
	 if(children != NULL){
		 do{
			 ZG_GUIWindowManager_ProcessTag(window_data,window_data->window->widget,children);
			 children=children->next;
		 }while(children != children->parent->children);
	 }

	 ok=true;

wm_load_from_memmory_exit:

	freeDoc(doc);

	return ok;

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
		return ZG_GUIWindow_GetTextureManager(data->window_manager);//data->ttfont_manager;
	}
	return NULL;
}
/*
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

 ZG_GUIButton		*	ZG_GUIWindow_NewButton(ZG_GUIWindow * _this){

 }

 ZG_GUITextBox		* 	ZG_GUIWindow_NewTextBox(ZG_GUIWindow * _this){

 }

 ZG_GUITexture		* 	ZG_GUIWindow_NewTexture(ZG_GUIWindow * _this){

 }

 ZG_GUIFrame			* 	ZG_GUIWindow_NewFrame(ZG_GUIWindow * _this){
	 ZG_GUIWindowData *data=_this->data;

	 ZG_GUIFrame *frame=new ZG_GUIFrame(0,0,0,0);
	 ZG_List_Add(data->frames,frame);
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

