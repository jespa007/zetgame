#ifndef __GUI_WINDOW_MANAGER_H__
#define __GUI_WINDOW_MANAGER_H__

struct GUIWindowManager{
	void *data;
};

GUIWindowManager 	*	GUIWindowManager_New(TextureManager	* _texture_manager, TTFontManager * _ttfont_manager);
bool 					GUIWindowManager_Load(GUIWindowManager *_this,const char *_xml_filename);
TextureManager		*	GUIWindowManager_GetTextureManager(GUIWindowManager *_this);
TTFontManager		*	GUIWindowManager_GetTTFontManager(GUIWindowManager *_this);
GUIWindow 	  		*	GUIWindowManager_Get(GUIWindowManager *_this,const char *_wnd_id);

void GUIWindowManager_Delete(GUIWindowManager *_this);

#endif
