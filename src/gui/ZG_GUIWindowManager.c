#include "@zg_gui.h"

typedef struct{
	ZG_MapString 			* 	windows;
	ZG_TextureManager   	*   texture_manager;
	ZG_TTFontManager   		*   ttfont_manager;
}ZG_GUIWindowManagerData;



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

	data->windows->on_delete=ZG_GUIWindowManager_OnDeleteGUIWMWindowData;

	window_manager->data=data;

	return window_manager;
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
			 ZG_LOG_DEBUG("[attribute] %s:%s", attribute->name, attribute->value);
			 attribute=attribute->next;
		 }while(attribute != attribute->parent->attributes);
		// process attributes
	}
	return true;
}*/



/**
 * ZG_GUIWindowManager_NewWindowFromFile from xml
 * @_this: ZG_GUIWindowManager object
 * @_wnd_id: window id
 * @_xml_file: xml file
  */
bool ZG_GUIWindowManager_NewWindowFromFile(ZG_GUIWindowManager *_this,const char *_wnd_id){

	bool exists=false;
	ZG_GUIWindowManagerData *data=_this->data;
	ZG_MapString_Get(data->windows,_wnd_id,&exists);

	if(exists){
		ZG_LOG_ERROR("Window '%s' already exists",_wnd_id);
		return false;
	}

	ZG_MapString_Set(data->windows,_wnd_id,window_data);

	return ok;
}

/**
 * ZG_GUIWindowManager_NewWindowFromFile from xml
 * @_this: ZG_GUIWindowManager object
 * @_wnd_id: window id
 * @_xml_file: xml file
  */
bool ZG_GUIWindowManager_NewWindowFromFile(ZG_GUIWindowManager *_this,const char *_wnd_id, const char *_xml_file){

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
