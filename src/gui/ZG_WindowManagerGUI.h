#ifndef __GUI_WINDOW_MANAGER_H__
#define __GUI_WINDOW_MANAGER_H__

struct GUIWindowManager{
	void *data;
};

GUIWindowManager 	*	GUIWindowManager_New(ZG_TextureManager	* _texture_manager/*, ZG_TTFontManager * _ttfont_manager*/);
bool 					GUIWindowManager_Load(GUIWindowManager *_this,const char *_xml_filename);
ZG_TextureManager		*	GUIWindowManager_GetTextureManager(GUIWindowManager *_this);
//ZG_TTFontManager		*	GUIWindowManager_GetTTFontManager(GUIWindowManager *_this);
GUIWindow 	  		*	GUIWindowManager_Get(GUIWindowManager *_this,const char *_wnd_id);

void GUIWindowManager_Delete(GUIWindowManager *_this);

#endif
