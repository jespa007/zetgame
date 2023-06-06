#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

#define DEFAULT_WINDOW_CAPTION_HEIGHT 	32

typedef enum{
	WINDOW_STYLE_NONE=0
	,WINDOW_STYLE_NORMAL
}WindowStyle;

struct GUIWindow{
	GUIWidget 	*widget;
	void 		*data;
};

GUIWindow 		*	GUIWindow_New(int x, int y, uint16_t width, uint16_t height, GUIWindowManager *_window_manager);
void 				GUIWindow_SetWidth(GUIWindow *_this,uint16_t _width);
void 				GUIWindow_SetHeight(GUIWindow *_this,uint16_t _height);
void 				GUIWindow_SetBackgroundColor3i(GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b);
void 				GUIWindow_SetBackgroundColor4f(GUIWindow * _this, ZG_Color4f _color);
void 				GUIWindow_SetCaptionTitle(GUIWindow * _this,const char *_text);
void 				GUIWindow_SetWindowStyle(GUIWindow * _this, WindowStyle _window_style);
ZG_TextureManager 	*	GUIWindow_GetTextureManager(GUIWindow * _this);
//ZG_TTFontManager	*	GUIWindow_GetTTFontManager(GUIWindow * _this);
void 				GUIWindow_Delete(GUIWindow * _this);

#endif
