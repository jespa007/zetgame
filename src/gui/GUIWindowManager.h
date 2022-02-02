#ifndef __GUI_WINDOW_MANAGER_H__
#define __GUI_WINDOW_MANAGER_H__

typedef struct GUIWindowManager GUIWindowManager;

struct GUIWindowManager{
	void *data;
};

GUIWindowManager 	*	GUIWindowManager_New(TextureManager	* _texture_manager);
bool 					GUIWindowManager_Load(GUIWindowManager *_this,const char *_xml_filename);
GUIWindow 	  		*	GUIWindowManager_Get(GUIWindowManager *_this,const char *_wnd_id);

void GUIWindowManager_Delete(GUIWindowManager *_this);

#endif
