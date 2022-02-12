#include "zg_gui.h"

typedef struct{
	List				*windows;
	List				*labels;
	List				*buttons;
	List				*viewers;
	List				*frames;
	List				*textboxes;
}GUIWMWindowData;


typedef struct{
	MapString 			* 	windows;
	TextureManager   	*   texture_manager;
}GUIWindowManagerData;

void GUIWindowManager_OnDeleteGUIWMWindowData(MapStringNode *node){
	GUIWMWindowData *window_data=(GUIWMWindowData *)node->val;



	if(window_data != NULL){

		struct{
			List	*list;
			void (*delete)(void *_this);
		}widget_deallocate_collections[]={
				{&window_data->buttons,GUIButton_Delete}
				,{&window_data->labels,GUILabel_Delete}
				,{&window_data->viewers,GUIViewer_Delete}
				,{&window_data->textboxes,GUITextbox_Delete}
				,{&window_data->frames,GUIFrame_Delete}
				,{&window_data->windows,GUIWindow_Delete}
		};

		for(int j=0; j < ARRAY_SIZE(widget_deallocate_collections); j++){

			// deallocate all widgets
			for(int i=0; i < widget_deallocate_collections[j].list->count; i++){
				widget_deallocate_collections[j].delete(widget_deallocate_collections[j].list->items[i]);
			}
		}

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
GUIWindowManager *GUIWindowManager_New(TextureManager	* _texture_manager){
	GUIWindowManager *window_manager=ZG_NEW(GUIWindowManager);
	GUIWindowManagerData *data=ZG_NEW(GUIWindowManagerData);

	data->windows = MapString_New();//new std::map<std::string,TTFont *>();
	data->texture_manager = _texture_manager;

	data->windows->on_delete=GUIWindowManager_OnDeleteGUIWMWindowData;

	window_manager->data=data;

	return window_manager;
}

bool GUIWindowManager_NewTextbox(GUIWMWindowData *_window_data,GUIWidget *_parent,const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	GUITextbox *textbox=GUITextbox_New(0,0,10,10);
	List_Add(_window_data->textboxes,textbox);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,textbox->widget);
	}


	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}

bool GUIWindowManager_NewFrame(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	GUIFrame *frame=GUIFrame_New(0,0,10,10);
	List_Add(_window_data->frames,frame);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,frame->widget);
	}


	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}

bool GUIWindowManager_NewViewer(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	GUIViewer *viewer=GUIViewer_New(0,0,10,10);
	List_Add(_window_data->viewers,viewer);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,viewer->widget);
	}


	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}


bool GUIWindowManager_NewButton(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	GUIButton *button=GUIButton_New(0,0,10,10);
	List_Add(_window_data->buttons,button);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,button->widget);
	}


	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}

bool GUIWindowManager_NewLabel(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e ){
	XmlAttribute *attribute=e->attributes;
	GUILabel *label=GUILabel_New(0,0,10,10);
	List_Add(_window_data->labels,label);

	if(_parent != NULL){
		GUIWidget_AttachWidget(_parent,label->widget);
	}

	 if(attribute != NULL){
		 do{
			 Log_Debug("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}

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
}

bool GUIWindowManager_ProcessTag(GUIWMWindowData *_window_data,GUIWidget *_parent, const XmlElement *e){

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
	 }else if(STRCMP(e->name,==,"window")){
		 return GUIWindowManager_NewWindow(_window_data,_parent,e);
	 }else{
		Log_Error("unknow type '%s'",e->name);
	 }


	// process childs
	 if(children != NULL){
		 do{
			 GUIWindowManager_ProcessTag(_window_data,_parent,children);
			 children=children->next;
		 }while(children != children->parent->children);
	 }


	return true;
}

bool GUIWindowManager_LoadFromMemory(
		GUIWindowManager *_this
		,const char *_path
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
		xmlDocPerror(doc, stderr, "Error parsing standard input");
		ok=false;
		goto exit;
	}


	 GUIWMWindowData *window_data=ZG_NEW(GUIWMWindowData);
	 window_data->windows=List_New();
	 window_data->buttons=List_New();
	 window_data->labels=List_New();
	 window_data->viewers=List_New();
	 window_data->textboxes=List_New();
	 window_data->frames=List_New();


	 GUIWindowManager_ProcessTag(window_data,NULL,doc->root);

exit:

	freeDoc(doc);

	char *filename=Path_GetFilename(_path);

	MapString_SetValue(data->windows,filename,window_data);

	free(filename);

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
	char *_path=NULL;


	if((_xml_buf=File_Read(_json_tmx_file))!=NULL){
		ok=GUIWindowManager_LoadFromMemory(
				_this
				,_path=Path_GetDirectoryName(_json_tmx_file)
				,_xml_buf->ptr
				,_xml_buf->len
		);
	}


	if(_xml_buf) BufferByte_Delete(_xml_buf);
	if(_path) free(_path);


	return ok;
}

GUIWindow *GUIWindowManager_Get(GUIWindowManager *_this, const char *key){
	GUIWindowManagerData *data=_this->data;

	GUIWMWindowData *window_data= MapString_GetValue(data->windows,key,NULL);

	if(window_data != NULL){
		return window_data->main_window;
	}

	return NULL;
}


void  GUIWindowManager_Delete(GUIWindowManager *_this){
	GUIWindowManagerData 	*data=_this->data;

	MapString_Delete(data->windows);
	//MapString_Delete(data->textures);

	FREE(data);
	FREE(_this);
}
