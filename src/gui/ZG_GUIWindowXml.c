bool ZG_GUIWindow_ProcessTagFrame(
		ZG_GUIWindow *_window
		,ZG_GUIWidget *_parent
		, const XmlElement *e
){
	bool ok=true;
	int int_value=0;
	const char *id=NULL;
	XmlAttribute *attribute=e->attributes;
	// TODO set id
	ZG_GUIFrame *gui_frame= ZG_GUIFrame_New(0,0,0,0);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_frame->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
				 id=attribute->value;
			 }else if(ZG_STRCMP(attribute->name,==,"left")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionX(gui_frame->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"top")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetPositionY(gui_frame->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetWidth(gui_frame->widget,int_value);
				 }
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWidget_SetHeight(gui_frame->widget,int_value);
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


	 ZG_GUIWindow_AddGUIFrame(_window,gui_frame,id);

	return ok;
}

bool ZG_GUIWindow_ProcessTagTexture(
		ZG_GUIWindow *_window
		,ZG_GUIWidget *_parent
		, const XmlElement *e
		,ZG_TextureManager *_texture_manager
		,ZG_TTFontManager *_ttfont_manager
){
	bool ok=true;
	int int_value=0;
	const char *id=NULL;
	XmlAttribute *attribute=e->attributes;

	// TODO set id
	ZG_GUITexture *gui_texture=ZG_GUITexture_New(0,0,0,0);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_texture->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
				 id=attribute->value;
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
				 ZG_TextBox_SetFont(gui_texture->textbox,ZG_TTFontManager_GetFont(_ttfont_manager, attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"color")){
				 ZG_GUIWidget_SetColor4f(gui_texture->widget,ZG_Color4f_FromHtml(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"source")){
				 ZG_GUITexture_SetTexture(gui_texture,ZG_TextureManager_GetTexture(_texture_manager,attribute->value));
			 }else{
				 ZG_LOG_ERROR("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 ZG_GUIWindow_AddGUITexture(_window,gui_texture,id);

	return ok;
}

bool ZG_GUIWindow_ProcessTagButton(
		ZG_GUIWindow *_window
		,ZG_GUIWidget *_parent
		, const XmlElement *e
		,ZG_TextureManager *_texture_manager
		,ZG_TTFontManager *_ttfont_manager
){
	bool ok=true;
	int int_value=0;
	const char *id=NULL;
	XmlAttribute *attribute=e->attributes;

	ZG_GUIButton *button=ZG_GUIButton_New(0,0,0,0);

	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,button->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);

			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
				 id=attribute->value;
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

	 ZG_GUIWindow_AddGUIButton(_window,button,id);

	return ok;
}

bool ZG_GUIWindow_ProcessTagTextBox(
	ZG_GUIWindow *_window
	,ZG_GUIWidget *_parent
	, const XmlElement *e
	,ZG_TTFontManager *_ttfont_manager
){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	const char *id=NULL;
	ZG_GUITextBox *gui_textbox=ZG_GUITextBox_New(0,0,0,0);


	if(_parent != NULL){
		ZG_GUIWidget_AttachWidget(_parent,gui_textbox->widget);
	}

	 if(attribute != NULL){
		 do{
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);

			 if(ZG_STRCMP(attribute->name,==,"id")){
				 // set id
				 id=attribute->value;
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
						 _ttfont_manager
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

	 ZG_GUIWindow_AddGUITextBox(_window,gui_textbox,id);

	return ok;
}

bool ZG_GUIWindow_ProcessTag(
	ZG_GUIWindow *_window
	,ZG_GUIWidget *_parent
	,const XmlElement *e
	,ZG_TextureManager *_texture_manager
	,ZG_TTFontManager *_ttfont_manager
){
	bool ok=true;
	 XmlElement *children=e->children;

	 ZG_LOG_DEBUG("[tag]: %s", e->name);

	 // create widget from its tag name
	 if(ZG_STRCMP(e->name,==,"textbox")){
		 return ZG_GUIWindow_ProcessTagTextBox(
				 _window
				 ,_parent
				 ,e
				,_ttfont_manager
		);
	 }else if(ZG_STRCMP(e->name,==,"button")){
		 return ZG_GUIWindow_ProcessTagButton(
				 _window
				 ,_parent
				 ,e
				,_texture_manager
				,_ttfont_manager
		);
	 }else if(ZG_STRCMP(e->name,==,"image")){
		 return ZG_GUIWindow_ProcessTagTexture(
				 _window
				 ,_parent
				 ,e
				,_texture_manager
				,_ttfont_manager
		);
	 }else if(ZG_STRCMP(e->name,==,"textbox")){
		 return ZG_GUIWindow_ProcessTagTextBox(
				 _window
				 ,_parent
				 ,e
				,_ttfont_manager
		);
	 }else{
		ZG_LOG_ERROR("unexpected tag '%s'",e->name);
		ok=false;
	 }

	// process childs
	 if(children != NULL){
		 do{
			 ok&=ZG_GUIWindow_ProcessTag(
					 _window
					 ,_parent
					 ,children
					,_texture_manager
					,_ttfont_manager
			);
			 children=children->next;
		 }while(children != children->parent->children);
	 }

	return ok;
}

ZG_GUIWindow *ZG_GUIWindow_NewFromXmlMemory(
		uint8_t *_xml_buf
		,size_t _xml_len
		,ZG_TextureManager *_texture_manager
		,ZG_TTFontManager *_ttfont_manager
	){

	ZG_UNUSUED_PARAM(_xml_len);

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


	 ZG_GUIWindow *window=ZG_GUIWindow_New(0,0,ZG_Graphics_GetWidth(),ZG_Graphics_GetHeight()-ZG_DEFAULT_WINDOW_CAPTION_HEIGHT);
	 XmlAttribute *attribute=doc->root->attributes;
	 XmlElement *children=doc->root->children;

	 // setup window
	 if(attribute != NULL){
		 do{
			 int int_value=0;
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 if(ZG_STRCMP(attribute->name,==,"background-color")){
				 ZG_GUIWindow_SetBackgroundColor4f(window,ZG_Color4f_FromHtml(attribute->value));
			 }else if(ZG_STRCMP(attribute->name,==,"window-style")){
				 if(ZG_STRCMP(attribute->value,==,"none")){
					 ZG_GUIWindow_SetWindowStyle(window,ZG_GUI_WINDOW_STYLE_NONE);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"caption")){
				 ZG_GUIWindow_SetCaptionTitle(window,attribute->value);
			 }else if(ZG_STRCMP(attribute->name,==,"width")){
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWindow_SetWidth(window,int_value);
				 }
			 }else if(ZG_STRCMP(attribute->name,==,"height")){
				 //ZG_GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
				 if(ZG_String_StringToInt(&int_value,attribute->value,10)){
					 ZG_GUIWindow_SetHeight(window,int_value);
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
			 ZG_GUIWindow_ProcessTag(
					 window
					 ,window->widget
					 ,children
					,_texture_manager
					,_ttfont_manager
				);
			 children=children->next;
		 }while(children != children->parent->children);
	 }


wm_load_from_memmory_exit:

	freeDoc(doc);

	return window;

}

/**
 * ZG_GUIWindowManager_NewWindowFromFile from xml
 * @_this: ZG_GUIWindowManager object
 * @_wnd_id: window id
 * @_xml_file: xml file
  */
ZG_GUIWindow *ZG_GUIWindow_NewFromXmlFile(
	const char *_xml_file
	,ZG_TextureManager *_texture_manager
	,ZG_TTFontManager *_ttfont_manager
){

	ZG_BufferByte *_xml_buf=NULL;
	ZG_GUIWindow *window=NULL;

	if((_xml_buf=ZG_File_Read(_xml_file))!=NULL){
		window=ZG_GUIWindow_NewFromXmlMemory(
				_xml_buf->ptr
				,_xml_buf->len
				,_texture_manager
				,_ttfont_manager
		);
	}

	if(_xml_buf) {
		ZG_BufferByte_Delete(_xml_buf);
	}


	return window;
}
