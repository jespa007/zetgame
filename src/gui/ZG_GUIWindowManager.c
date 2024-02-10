#include "@zg_gui.h"

typedef struct{
	ZG_MapString 			* 	windows;
	ZG_TextureManager   	*   texture_manager;
	ZG_TTFontManager   		*   ttfont_manager;
}ZG_GUIWindowManagerData;


// MEMBERS
ZG_GUIWindowManager *ZG_GUIWindowManager_New(
	ZG_TextureManager	* _texture_manager
  , ZG_TTFontManager * _ttfont_manager
){
	ZG_GUIWindowManager *window_manager=ZG_NEW(ZG_GUIWindowManager);
	ZG_GUIWindowManagerData *data=ZG_NEW(ZG_GUIWindowManagerData);

	data->windows = ZG_MapString_New();//new std::map<std::string,ZG_TTFont *>();
	data->texture_manager = _texture_manager;
	data->ttfont_manager = _ttfont_manager;

	window_manager->data=data;

	return window_manager;
}


bool ZG_GUIWindow_ProcessTagFrame(ZG_GUIWindow *_window,ZG_GUIWidget *_parent, const XmlElement *e ){
	ZG_GUIWindowData *data=_window->data;
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUIFrame *frame= ZG_GUIWindow_NewFrame(_window);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,frame->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(ZG_STRCMP(attribute->name,==,"left")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionX(frame->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"top")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionY(frame->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetWidth(frame->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetHeight(frame->widget,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"font-size")){
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"font-file")){
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else{
				 ZG_LOG_ERROR("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return ok;
}

bool ZG_GUIWindow_ProcessTagTexture(ZG_GUIWindow *_window,ZG_GUIWidget *_parent, const XmlElement *e ){
	ZG_GUIWindowData *data=_window->data;
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUITexture *gui_texture=ZG_GUITexture_New(0,0,10,10);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_texture->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(ZG_STRCMP(attribute->name,==,"left")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionX(gui_texture->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"top")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionY(gui_texture->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetWidth(gui_texture->widget,int_value);
					 ZG_TextBox_SetWidth(gui_texture->textbox,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetHeight(gui_texture->widget,int_value);
					 ZG_TextBox_SetHeight(gui_texture->textbox,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"text")){
				 ZG_TextBox_SetText(gui_texture->textbox,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"horizontal-alignment")){
				 ZG_TextBox_SetHorizontalAlignment(gui_texture->textbox,ZG_TextBox_ParseTextAlign(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"vertical-alignment")){
				 ZG_TextBox_SetVerticalAlignment(gui_texture->textbox,ZG_TextBox_ParseVerticalAlignment(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"font-size")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_TextBox_SetFontSize(gui_texture->textbox,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"font-file")){
				 ZG_TextBox_SetFont(gui_texture->textbox,ZG_TTFontManager_GetFont(data->gui_window_manager_data->ttfont_manager, attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"color")){
				 ZG_GUIWidget_SetColor4f(gui_texture->widget,ZG_Color4f_FromHtml(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"source")){
				 ZG_GUITexture_SetTexture(gui_texture,attribute->value);
			 }else{
				 ZG_LOG_ERROR("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_List_Add(data->viewers,gui_texture);

	return ok;
}

bool ZG_GUIWindow_ProcessTagButton(ZG_GUIWindow *_window,ZG_GUIWidget *_parent, const XmlElement *e ){
	ZG_GUIWindowData *data=_window->data;
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUIButton *button=ZG_GUIButton_New(0,0,10,10);

	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,button->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);

			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(ZG_STRCMP(attribute->name,==,"left")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionX(button->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"top")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionY(button->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetWidth(button->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetHeight(button->widget,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"font-size")){
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"font-file")){
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else{
				 ZG_LOG_ERROR("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_List_Add(data->buttons,button);

	return ok;
}

bool ZG_GUIWindow_ProcessTagTextBox(ZG_GUIWindow *_window,ZG_GUIWidget *_parent, const XmlElement *e ){
	ZG_GUIWindowData *data=_window->data;
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUITextBox *gui_textbox=ZG_GUITextBox_New(0,0,10,10);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_textbox->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);

			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(ZG_STRCMP(attribute->name,==,"left")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionX(gui_textbox->widget,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"top")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionY(gui_textbox->widget,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetWidth(gui_textbox->widget,int_value);
					 ZG_TextBox_SetWidth(gui_textbox->textbox,int_value);

				 }
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetHeight(gui_textbox->widget,int_value);
					 ZG_TextBox_SetHeight(gui_textbox->textbox,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"font-size")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_TextBox_SetFontSize(gui_textbox->textbox,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"font-file")){
				 ZG_TextBox_SetFont(
					 gui_textbox->textbox
					 ,ZG_TTFontManager_GetFont(
							 data->gui_window_manager_data->ttfont_manager
							 ,attribute->value
					)
				);
			 }else if(ZG_STRCMP(attribute->name,==,"text")){
				 ZG_TextBox_SetText(gui_textbox->textbox,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"horizontal-alignment")){
				 ZG_TextBox_SetHorizontalAlignment(gui_textbox->textbox,ZG_TextBox_ParseTextAlign(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"vertical-alignment")){
				 ZG_TextBox_SetVerticalAlignment(gui_textbox->textbox,ZG_TextBox_ParseVerticalAlignment(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"border-thickness")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_TextBox_SetBorderThickness(gui_textbox->textbox,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"border-color")){
				 ZG_TextBox_SetBorderColor4f(gui_textbox->textbox,ZG_Color4f_FromHtml(attribute->value));
			 }else{
				 ZG_LOG_ERROR("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_List_Add(data->textboxes,gui_textbox);

	return ok;
}

bool ZG_GUIWindowManager_ProcessTag(ZG_GUIWindow *_window,ZG_GUIWidget *_parent, const XmlElement *e){
	ZG_GUIWindowData *data=_window->data;
	bool ok=true;
	 XmlElement *children=e->children;

	 ZG_LOG_DEBUG("[tag]: %s", e->name);

	 // create widget from its tag name
	 if(ZG_STRCMP(e->name,==,"textbox")){
		 return ZG_GUIWindowManager_NewTextBox(data,_parent,e);
	 }else if(ZG_STRCMP(e->name,==,"button")){
		 return ZG_GUIWindowManager_NewButton(data,_parent,e);
	 }else if(ZG_STRCMP(e->name,==,"image")){
		 return ZG_GUIWindowManager_NewTexture(data,_parent,e);
	 }else if(ZG_STRCMP(e->name,==,"textbox")){
		 return ZG_GUIWindowManager_NewTextBox(data,_parent,e);
	 }else{
		ZG_LOG_ERROR("unexpected tag '%s'",e->name);
		ok=false;
	 }

	// process childs
	 if(children != NULL){
		 do{
			 ok&=ZG_GUIWindowManager_ProcessTag(data,_parent,children);
			 children=children->next;
		 }while(children != children->parent->children);
	 }

	return ok;
}

/**
 * ZG_GUIWindowManager_NewWindowFromFile from xml
 * @_this: ZG_GUIWindowManager object
 * @_wnd_id: window id
 * @_xml_file: xml file
  */
ZG_GUIWindow *ZG_GUIWindowManager_NewWindowFromFile(ZG_GUIWindowManager *_this,const char *_wnd_id, const char *_xml_file){

	ZG_BufferByte *_xml_buf=NULL;bool ok=false;
	char *file=NULL;
	bool exists=false;
	ZG_GUIWindowManagerData *data=_this->data;
	ZG_MapString_Get(data->windows,_wnd_id,&exists);

	if(exists){
		ZG_LOG_ERROR("Window '%s' already exists",_wnd_id);
		return false;
	}

	if((_xml_buf=ZG_File_Read(_xml_file))!=NULL){
		ok=ZG_GUIWindowManager_LoadFromMemory(
				_this
				,_wnd_id
				,_xml_buf->ptr
				,_xml_buf->len
		);
	}

	if(_xml_buf) {
		ZG_BufferByte_Delete(_xml_buf);
	}


	return ok;
}

ZG_TextureManager		*	ZG_GUIWindowManager_GetTextureManager(ZG_GUIWindowManager *_this){
	ZG_GUIWindowManagerData *data=_this->data;
	return data->texture_manager;
}
/*
ZG_TTFontManager		*	ZG_GUIWindowManager_GetTTFontManager(ZG_GUIWindowManager *_this){
	ZG_GUIWindowManagerData *data=_this->data;
	return data->ttfont_manager;

}
*/

ZG_GUIWindow *ZG_GUIWindowManager_Get(ZG_GUIWindowManager *_this, const char *key){
	ZG_GUIWindowManagerData *data=_this->data;

	GUIWMWindowData *window_data= ZG_MapString_Get(data->windows,key,NULL);

	if(window_data != NULL){
		return window_data->window;
	}

	return NULL;
}


void  ZG_GUIWindowManager_Delete(ZG_GUIWindowManager *_this){
	ZG_GUIWindowManagerData 	*data=_this->data;

	ZG_MapString_Delete(data->windows);
	//ZG_MapString_Delete(data->textures);

	ZG_FREE(data);
	ZG_FREE(_this);
}
