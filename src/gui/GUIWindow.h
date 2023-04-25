#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

struct GUIWindow{
	GUIWidget 	*widget;
	void 		*data;
};

GUIWindow 		*	GUIWindow_New(int x, int y, uint16_t width, uint16_t height, GUIWindowManager *_window_manager);
void 				GUIWindow_SetWidth(GUIWindow *_this,uint16_t _width);
void 				GUIWindow_SetHeight(GUIWindow *_this,uint16_t _height);
void 				CGUIWindow_SetWindowManager(GUIWindow *_this);
void 				GUIWindow_SetBackgroundColor3i(GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b);
void 				GUIWindow_SetBackgroundColorHexStr(GUIWindow * _this, const char * color);
void 				GUIWindow_SetCaption(GUIWindow * _this,const char *_text);
TextureManager 	*	GUIWindow_GetTextureManager(GUIWindow * _this);
TTFontManager	*	GUIWindow_GetTTFontManager(GUIWindow * _this);
void 				GUIWindow_Delete(GUIWindow * _this);

#endif
