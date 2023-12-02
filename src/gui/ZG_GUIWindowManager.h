#ifndef __GUI_WINDOW_MANAGER_H__
#define __GUI_WINDOW_MANAGER_H__

struct ZG_GUIWindowManager{
	void *data;
};

ZG_GUIWindowManager 	*	ZG_GUIWindowManager_New(ZG_TextureManager	* _texture_manager, ZG_TTFontManager * _ttfont_manager);
bool 						ZG_GUIWindowManager_Load(ZG_GUIWindowManager *_this,const char *_xml_filename);
ZG_TextureManager		*	ZG_GUIWindowManager_GetTextureManager(ZG_GUIWindowManager *_this);
//ZG_TTFontManager		*	ZG_GUIWindowManager_GetTTFontManager(ZG_GUIWindowManager *_this);
ZG_GUIWindow 	  		*	ZG_GUIWindowManager_Get(ZG_GUIWindowManager *_this,const char *_wnd_id);

void ZG_GUIWindowManager_Delete(ZG_GUIWindowManager *_this);

#endif
