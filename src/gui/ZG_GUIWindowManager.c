#include "_zg_gui_.h"

typedef struct{
	ZG_MapString 			* 	windows;
	ZG_TextureManager   	*   texture_manager;
	//ZG_TTFontManager   	*   ttfont_manager;
}ZG_GUIWindowManagerData;

typedef struct{
	ZG_GUIWindow			*window;
	ZG_List				*buttons;
	ZG_List				*viewers;
	ZG_List				*frames;
	ZG_List				*textboxes;
	ZG_GUIWindowManagerData *gui_window_manager_data;
}GUIWMWindowData;

void ZG_GUIWindowManager_OnDeleteGUIWMWindowData(ZG_MapStringNode *node){
	GUIWMWindowData *window_data=(GUIWMWindowData *)node->val;

	if(window_data != NULL){

		struct{
			ZG_List	*list;
			void (*delete_callback)(void *_this);
		}widget_deallocate_collections[]={
				{window_data->buttons,(void (*)(void *))ZG_GUIButton_Delete}
				,{window_data->textboxes,(void (*)(void *))ZG_GUITextBox_Delete}
				,{window_data->viewers,(void (*)(void *))ZG_GUITexture_Delete}
				,{window_data->frames,(void (*)(void *))ZG_GUIFrame_Delete}
		};

		for(unsigned j=0; j < ZG_ARRAY_SIZE(widget_deallocate_collections); j++){

			// deallocate all widgets
			for(int i=0; i < widget_deallocate_collections[j].list->count; i++){
				widget_deallocate_collections[j].delete_callback(widget_deallocate_collections[j].list->items[i]);
			}

			ZG_List_Delete(widget_deallocate_collections[j].list);
		}

		ZG_GUIWindow_Delete(window_data->window);

		// finally free window data
		free(window_data);
	}
}

void ZG_GUIWindowManager_OnDeleteTexture(ZG_MapStringNode *node){
	ZG_Texture *texture=(ZG_Texture *)node->val;

	if(texture != NULL){
		ZG_Texture_Delete(texture);
	}
}

// MEMBERS
ZG_GUIWindowManager *ZG_GUIWindowManager_New(
		ZG_TextureManager	* _texture_manager
		//, ZG_TTFontManager * _ttfont_manager
){
	ZG_GUIWindowManager *window_manager=ZG_NEW(ZG_GUIWindowManager);
	ZG_GUIWindowManagerData *data=ZG_NEW(ZG_GUIWindowManagerData);

	data->windows = ZG_MapString_New();//new std::map<std::string,ZG_TTFont *>();
	data->texture_manager = _texture_manager;
	//data->ttfont_manager = _ttfont_manager;

	data->windows->on_delete=ZG_GUIWindowManager_OnDeleteGUIWMWindowData;

	window_manager->data=data;

	return window_manager;
}


bool ZG_GUIWindowManager_NewFrame(GUIWMWindowData *_window_data,ZG_GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUIFrame *frame=ZG_GUIFrame_New(0,0,10,10);
	ZG_List_Add(_window_data->frames,frame);

	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,frame->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
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
				 ZG_Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return ok;
}

bool ZG_GUIWindowManager_NewTexture(GUIWMWindowData *_window_data,ZG_GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUITexture *gui_texture=ZG_GUITexture_New(0,0,10,10);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_texture->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
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
				 ZG_TextBox_SetFontFile(gui_texture->textbox,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"color")){
				 ZG_GUIWidget_SetColor4f(gui_texture->widget,ZG_Color4f_FromHtml(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"source")){
				 ZG_GUITexture_SetTexture(gui_texture,attribute->value);
			 }else{
				 ZG_Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_List_Add(_window_data->viewers,gui_texture);

	return ok;
}


bool ZG_GUIWindowManager_NewButton(GUIWMWindowData *_window_data,ZG_GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUIButton *button=ZG_GUIButton_New(0,0,10,10);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,button->widget);
	}


	 if(attribute != NULL){
		 do{
			 ZG_Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);

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
				 ZG_Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_List_Add(_window_data->buttons,button);

	return ok;
}

bool ZG_GUIWindowManager_NewTextBox(GUIWMWindowData *_window_data,ZG_GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	ZG_GUITextBox *gui_textbox=ZG_GUITextBox_New(0,0,10,10);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_textbox->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);

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
				 ZG_TextBox_SetFontFile(gui_textbox->textbox,attribute->value);
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
				 ZG_Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_List_Add(_window_data->textboxes,gui_textbox);

	return ok;
}
/*
bool ZG_GUIWindowManager_NewWindow(GUIWMWindowData *_window_data,ZG_GUIWidget *_parent, const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	ZG_GUIWindow *window=ZG_GUIWindow_New(0,0,10,10);
	ZG_List_Add(_window_data->windows,window);

	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,window->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}*/

bool ZG_GUIWindowManager_ProcessTag(GUIWMWindowData *_window_data,ZG_GUIWidget *_parent, const XmlElement *e){
	bool ok=true;
	 XmlElement *children=e->children;

	 ZG_Log_Debug("[tag]: %s", e->name);

	 // create widget from its tag name
	 if(ZG_STRCMP(e->name,==,"textbox")){
		 return ZG_GUIWindowManager_NewTextBox(_window_data,_parent,e);
	 }else if(ZG_STRCMP(e->name,==,"button")){
		 return ZG_GUIWindowManager_NewButton(_window_data,_parent,e);
	 }else if(ZG_STRCMP(e->name,==,"texture")){
		 return ZG_GUIWindowManager_NewTexture(_window_data,_parent,e);
	 }else if(ZG_STRCMP(e->name,==,"textbox")){
		 return ZG_GUIWindowManager_NewTextBox(_window_data,_parent,e);
	 }else{
		ZG_Log_Error("unexpected tag '%s'",e->name);
		ok=false;
	 }


	// process childs
	 if(children != NULL){
		 do{
			 ok&=ZG_GUIWindowManager_ProcessTag(_window_data,_parent,children);
			 children=children->next;
		 }while(children != children->parent->children);
	 }


	return ok;
}

bool ZG_GUIWindowManager_LoadFromMemory(
		ZG_GUIWindowManager *_this
		,const char *_id
		,uint8_t *_xml_buf
		,size_t _xml_len
	){

	ZG_UNUSUED_PARAM(_xml_len);

	bool ok=false;
	ZG_GUIWindowManagerData *data=_this->data;

	// first read tilesets...
	XmlDoc *doc = parseDoc((char *)_xml_buf);

	 if (xmlDocError(doc) != XML_SUCCESS)
	 {
		/* unparsable XML was read from stdin */
		ZG_Log_ErrorF("Cannot parse xml");
		goto wm_load_from_memmory_exit;
	}

	 if(ZG_STRCMP(doc->root->name,!=,"window")){
			ZG_Log_Error("Expected first tag as 'window' but it was '%s'",doc->root->name);
			goto wm_load_from_memmory_exit;
	 }

	 GUIWMWindowData *window_data=ZG_NEW(GUIWMWindowData);


	 // setup gui elements
	 window_data->buttons=ZG_List_New();
	 window_data->textboxes=ZG_List_New();
	 window_data->viewers=ZG_List_New();
	 window_data->frames=ZG_List_New();
	 window_data->gui_window_manager_data=data;


	 window_data->window=ZG_GUIWindow_New(0,0,ZG_Graphics_GetWidth(),ZG_Graphics_GetHeight()-ZG_DEFAULT_WINDOW_CAPTION_HEIGHT,_this);
	 XmlAttribute *attribute=doc->root->attributes;
	 XmlElement *children=doc->root->children;

	 // setup window
	 if(attribute != NULL){
		 do{
			 int int_value=0;
			 ZG_Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
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
				 ZG_Log_Error("unexpected attribute '%s'",attribute->name);
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

	ZG_MapString_SetValue(data->windows,_id,window_data);

wm_load_from_memmory_exit:

	freeDoc(doc);

	return ok;

}

/**
 * Load a set of ZG_GUIWindow from exported files from Aseprite
 * @_this: ZG_GUIWindowManager object
 * @_texture_filename: ZG_Texture filename (it can be .png,jpg,etc)
 * @_json_filename: Json file generated by Aseprite
 * @_extra_json_filename: Json file where it adds some extra information per frame (for instance collider)
 */
bool ZG_GUIWindowManager_Load(ZG_GUIWindowManager *_this,const char *_json_tmx_file){

	ZG_BufferByte *_xml_buf=NULL;bool ok=false;
	char *file=NULL;


	if((_xml_buf=ZG_File_Read(_json_tmx_file))!=NULL){
		ok=ZG_GUIWindowManager_LoadFromMemory(
				_this
				,file=ZG_Path_GetFilenameWithoutExtension(_json_tmx_file)
				,_xml_buf->ptr
				,_xml_buf->len
		);
	}


	if(_xml_buf) ZG_BufferByte_Delete(_xml_buf);
	if(file) free(file);


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

	GUIWMWindowData *window_data= ZG_MapString_GetValue(data->windows,key,NULL);

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
