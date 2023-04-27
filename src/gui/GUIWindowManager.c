#include "zg_gui.h"

typedef struct{
	MapString 			* 	windows;
	TextureManager   	*   texture_manager;
	TTFontManager   	*   ttfont_manager;
}GUIWindowManagerData;

typedef struct{
	GUIWindow			*window;
	List				*labels;
	List				*buttons;
	List				*viewers;
	List				*frames;
	List				*textboxes;
	GUIWindowManagerData *gui_window_manager_data;
}GUIWMWindowData;

void GUIWindowManager_OnDeleteGUIWMWindowData(MapStringNode *node){
	GUIWMWindowData *window_data=(GUIWMWindowData *)node->val;

	if(window_data != NULL){

		struct{
			List	*list;
			void (*delete)(void *_this);
		}widget_deallocate_collections[]={
				{window_data->buttons,(void (*)(void *))GUIButton_Delete}
				,{window_data->labels,(void (*)(void *))GUILabel_Delete}
				,{window_data->viewers,(void (*)(void *))GUIViewer_Delete}
				,{window_data->textboxes,(void (*)(void *))GUITextbox_Delete}
				,{window_data->frames,(void (*)(void *))GUIFrame_Delete}
		};

		for(unsigned j=0; j < ARRAY_SIZE(widget_deallocate_collections); j++){

			// deallocate all widgets
			for(int i=0; i < widget_deallocate_collections[j].list->count; i++){
				widget_deallocate_collections[j].delete(widget_deallocate_collections[j].list->items[i]);
			}

			List_Delete(widget_deallocate_collections[j].list);
		}

		GUIWindow_Delete(window_data->window);

		// finally free window data
		free(window_data);
	}
}

void GUIWindowManager_OnDeleteTexture(MapStringNode *node){
	Texture *texture=(Texture *)node->val;

	if(texture != NULL){
		Texture_Delete(texture);
	}
}

// MEMBERS
GUIWindowManager *GUIWindowManager_New(
		TextureManager	* _texture_manager
		, TTFontManager * _ttfont_manager
){
	GUIWindowManager *window_manager=ZG_NEW(GUIWindowManager);
	GUIWindowManagerData *data=ZG_NEW(GUIWindowManagerData);

	data->windows = MapString_New();//new std::map<std::string,TTFont *>();
	data->texture_manager = _texture_manager;
	data->ttfont_manager = _ttfont_manager;

	data->windows->on_delete=GUIWindowManager_OnDeleteGUIWMWindowData;

	window_manager->data=data;

	return window_manager;
}

bool GUIWindowManager_NewTextbox(GUIWMWindowData *_window_data,GUIWidget *_parent,const XmlElement *e ){
	bool ok=true;
	XmlAttribute *attribute=e->attributes;
	GUITextbox *textbox=GUITextbox_New(0,0,10,10);
	int int_value=0;

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,textbox->widget);
	}


	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);

			 if(STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(STRCMP(attribute->name,==,"left")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionX(textbox->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"top")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionY(textbox->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"width")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetWidth(textbox->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"height")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetHeight(textbox->widget,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"font-size")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"font-name")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else{
				 Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 List_Add(_window_data->textboxes,textbox);
	return ok;
}

bool GUIWindowManager_NewFrame(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	GUIFrame *frame=GUIFrame_New(0,0,10,10);
	List_Add(_window_data->frames,frame);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,frame->widget);
	}

	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 if(STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(STRCMP(attribute->name,==,"left")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionX(frame->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"top")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionY(frame->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"width")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetWidth(frame->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"height")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetHeight(frame->widget,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"font-size")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"font-name")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else{
				 Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return ok;
}

bool GUIWindowManager_NewViewer(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	GUIViewer *viewer=GUIViewer_New(0,0,10,10);


	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,viewer->widget);
	}

	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 if(STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(STRCMP(attribute->name,==,"left")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionX(viewer->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"top")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionY(viewer->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"width")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetWidth(viewer->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"height")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetHeight(viewer->widget,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"font-size")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"font-name")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"src")){
				 GUIViewer_SetImage(viewer,attribute->value);
			 }else{
				 Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 List_Add(_window_data->viewers,viewer);

	return ok;
}


bool GUIWindowManager_NewButton(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	GUIButton *button=GUIButton_New(0,0,10,10);


	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,button->widget);
	}


	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);

			 if(STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(STRCMP(attribute->name,==,"left")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionX(button->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"top")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionY(button->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"width")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetWidth(button->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"height")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetHeight(button->widget,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"font-size")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"font-name")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else{
				 Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 List_Add(_window_data->buttons,button);

	return ok;
}

bool GUIWindowManager_NewLabel(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	bool ok=true;
	int int_value=0;
	XmlAttribute *attribute=e->attributes;
	GUILabel *label=GUILabel_New(0,0,10,10);


	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,label->widget);
	}

	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);

			 if(STRCMP(attribute->name,==,"id")){
				 // set id
			 }else if(STRCMP(attribute->name,==,"left")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionX(label->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"top")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWidget_SetPositionY(label->widget,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"width")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUILabel_SetWidth(label,int_value);
				 }
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"height")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUILabel_SetHeight(label,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"font-size")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUILabel_SetFontSize(label,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"font-name")){
				 GUILabel_SetFontName(label,attribute->value);
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
				 //Textbox_SetFont(label->textbox,TTFontManager_GetFont(_window_data->gui_window_manager_data->ttfont_manager, attribute->value));
			 }else if(STRCMP(attribute->name,==,"text")){
				 GUILabel_SetText(label,attribute->value);
			 }else{
				 Log_Error("unexpected attribute '%s'",attribute->name);
				 ok=false;
			 }


			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}

	 List_Add(_window_data->labels,label);

	return ok;
}
/*
bool GUIWindowManager_NewWindow(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	GUIWindow *window=GUIWindow_New(0,0,10,10);
	List_Add(_window_data->windows,window);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,window->widget);
	}

	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}*/

bool GUIWindowManager_ProcessTag(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e){
	bool ok=true;
	 XmlElement *children=e->children;

	 Log_Debug("[tag]: %s", e->name);

	 // create widget from its tag name
	 if(STRCMP(e->name,==,"label")){
		 return GUIWindowManager_NewLabel(_window_data,_parent,e);
	 }else if(STRCMP(e->name,==,"button")){
		 return GUIWindowManager_NewButton(_window_data,_parent,e);
	 }else if(STRCMP(e->name,==,"img")){
		 return GUIWindowManager_NewViewer(_window_data,_parent,e);
	 }else if(STRCMP(e->name,==,"textbox")){
		 return GUIWindowManager_NewTextbox(_window_data,_parent,e);
	 }else{
		Log_Error("unexpected tag '%s'",e->name);
		ok=false;
	 }


	// process childs
	 if(children != NULL){
		 do{
			 ok&=GUIWindowManager_ProcessTag(_window_data,_parent,children);
			 children=children->next;
		 }while(children != children->parent->children);
	 }


	return ok;
}

bool GUIWindowManager_LoadFromMemory(
		GUIWindowManager *_this
		,const char *_id
		,uint8_t *_xml_buf
		,size_t _xml_len
	){

	UNUSUED_PARAM(_xml_len);

	bool ok=false;
	GUIWindowManagerData *data=_this->data;

	// first read tilesets...
	XmlDoc *doc = parseDoc((char *)_xml_buf);

	 if (xmlDocError(doc) != XML_SUCCESS)
	 {
		/* unparsable XML was read from stdin */
		Log_ErrorF("Cannot parse xml");
		goto wm_load_from_memmory_exit;
	}

	 if(STRCMP(doc->root->name,!=,"window")){
			Log_Error("Expected first tag as 'window' but it was '%s'",doc->root->name);
			goto wm_load_from_memmory_exit;
	 }

	 GUIWMWindowData *window_data=ZG_NEW(GUIWMWindowData);


	 // setup gui elements
	 window_data->buttons=List_New();
	 window_data->labels=List_New();
	 window_data->viewers=List_New();
	 window_data->textboxes=List_New();
	 window_data->frames=List_New();
	 window_data->gui_window_manager_data=data;


	 window_data->window=GUIWindow_New(0,0,Graphics_GetWidth(),Graphics_GetHeight()-DEFAULT_WINDOW_CAPTION_HEIGHT,_this);
	 XmlAttribute *attribute=doc->root->attributes;
	 XmlElement *children=doc->root->children;

	 // setup window
	 if(attribute != NULL){
		 do{
			 int int_value=0;
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 if(STRCMP(attribute->name,==,"background-color")){
				 GUIWindow_SetBackgroundColorHexStr(window_data->window,attribute->value);
			 }else if(STRCMP(attribute->name,==,"window-style")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
			 }else if(STRCMP(attribute->name,==,"caption")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
				 }
			 }else if(STRCMP(attribute->name,==,"width")){
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWindow_SetWidth(window_data->window,int_value);
				 }
			 }else if(STRCMP(attribute->name,==,"height")){
				 //GUIWindow_SetWindowStyle(window_data->window->widget,attribute->value);
				 if(StrUtils_StrToInt(&int_value,attribute->value,10)){
					 GUIWindow_SetHeight(window_data->window,int_value);
				 }
			 }else{
				 Log_Error("unexpected attribute '%s'",attribute->name);
			 }

			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	 // process window childs
	 if(children != NULL){
		 do{
			 GUIWindowManager_ProcessTag(window_data,window_data->window->widget,children);
			 children=children->next;
		 }while(children != children->parent->children);
	 }

	 ok=true;

	MapString_SetValue(data->windows,_id,window_data);

wm_load_from_memmory_exit:

	freeDoc(doc);

	return ok;

}

/**
 * Load a set of GUIWindow from exported files from Aseprite
 * @_this: GUIWindowManager object
 * @_texture_filename: Texture filename (it can be .png,jpg,etc)
 * @_json_filename: Json file generated by Aseprite
 * @_extra_json_filename: Json file where it adds some extra information per frame (for instance collider)
 */
bool GUIWindowManager_Load(GUIWindowManager *_this,const char *_json_tmx_file){

	BufferByte *_xml_buf=NULL;bool ok=false;
	char *file=NULL;


	if((_xml_buf=File_Read(_json_tmx_file))!=NULL){
		ok=GUIWindowManager_LoadFromMemory(
				_this
				,file=Path_GetFilenameWithoutExtension(_json_tmx_file)
				,_xml_buf->ptr
				,_xml_buf->len
		);
	}


	if(_xml_buf) BufferByte_Delete(_xml_buf);
	if(file) free(file);


	return ok;
}

TextureManager		*	GUIWindowManager_GetTextureManager(GUIWindowManager *_this){
	GUIWindowManagerData *data=_this->data;
	return data->texture_manager;
}

TTFontManager		*	GUIWindowManager_GetTTFontManager(GUIWindowManager *_this){
	GUIWindowManagerData *data=_this->data;
	return data->ttfont_manager;

}


GUIWindow *GUIWindowManager_Get(GUIWindowManager *_this, const char *key){
	GUIWindowManagerData *data=_this->data;

	GUIWMWindowData *window_data= MapString_GetValue(data->windows,key,NULL);

	if(window_data != NULL){
		return window_data->window;
	}

	return NULL;
}


void  GUIWindowManager_Delete(GUIWindowManager *_this){
	GUIWindowManagerData 	*data=_this->data;

	MapString_Delete(data->windows);
	//MapString_Delete(data->textures);

	ZG_FREE(data);
	ZG_FREE(_this);
}
