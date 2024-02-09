#ifndef __ZG_GUI_WINDOW_H__
#define __ZG_GUI_WINDOW_H__

#define ZG_DEFAULT_WINDOW_CAPTION_HEIGHT 	32

typedef enum{
	ZG_GUI_WINDOW_STYLE_NONE=0
	,ZG_GUI_WINDOW_STYLE_NORMAL
}ZG_GUIWindowStyle;

struct ZG_GUIWindow{
	ZG_GUIWidget 	*widget;
	void 		*data;
};

ZG_GUIWindow 		*	ZG_GUIWindow_New(
		  int x
		, int y
		, uint16_t width
		, uint16_t height
		, ZG_TextureManager *_texture_manager
		, ZG_TTFontManager *_ttf_font_manager
);
void 					ZG_GUIWindow_SetWidth(ZG_GUIWindow *_this,uint16_t _width);
void 					ZG_GUIWindow_SetHeight(ZG_GUIWindow *_this,uint16_t _height);
void 					ZG_GUIWindow_SetBackgroundColor3i(ZG_GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b);
void 					ZG_GUIWindow_SetBackgroundColor4f(ZG_GUIWindow * _this, ZG_Color4f _color);
void 					ZG_GUIWindow_SetCaptionTitle(ZG_GUIWindow * _this,const char *_text);
void 					ZG_GUIWindow_SetWindowStyle(ZG_GUIWindow * _this, ZG_GUIWindowStyle _window_style);
ZG_TextureManager 	*	ZG_GUIWindow_GetTextureManager(ZG_GUIWindow * _this);

int						ZG_GUIWindow_NewButton(ZG_GUIWindow * _this);
int						ZG_GUIWindow_NewTextBox(ZG_GUIWindow * _this);
int						ZG_GUIWindow_NewTexture(ZG_GUIWindow * _this);
int						ZG_GUIWindow_NewFrame(ZG_GUIWindow * _this);

//ZG_TTFontManager	*	ZG_GUIWindow_GetTTFontManager(ZG_GUIWindow * _this);
void 					ZG_GUIWindow_Delete(ZG_GUIWindow * _this);

#endif
