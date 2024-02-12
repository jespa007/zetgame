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
);

ZG_GUIWindow 		*	ZG_GUIWindow_NewFromFile(
	const char *_file_xml
	,ZG_TextureManager *_texture_manager
	,ZG_TTFontManager *_font_manager
);

void 					ZG_GUIWindow_SetWidth(ZG_GUIWindow *_this,uint16_t _width);
void 					ZG_GUIWindow_SetHeight(ZG_GUIWindow *_this,uint16_t _height);
void 					ZG_GUIWindow_SetBackgroundColor3i(ZG_GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b);
void 					ZG_GUIWindow_SetBackgroundColor4f(ZG_GUIWindow * _this, ZG_Color4f _color);
void 					ZG_GUIWindow_SetCaptionTitle(ZG_GUIWindow * _this,const char *_text);
void 					ZG_GUIWindow_SetWindowStyle(ZG_GUIWindow * _this, ZG_GUIWindowStyle _window_style);
ZG_TextureManager 	*	ZG_GUIWindow_GetTextureManager(ZG_GUIWindow * _this);

// Creates a new widget with optionally id to get it later (pass id=NULL if not set)
ZG_GUIButton		*	ZG_GUIWindow_NewGUIButton(ZG_GUIWindow * _this, const char *_id);
ZG_GUITextBox		* 	ZG_GUIWindow_NewGUITextBox(ZG_GUIWindow * _this, const char *_id);
ZG_GUITexture		* 	ZG_GUIWindow_NewGUITexture(ZG_GUIWindow * _this, const char *_id);
ZG_GUIFrame			* 	ZG_GUIWindow_NewGUIFrame(ZG_GUIWindow * _this, const char *_id);

// Add widget from previous allocated with optionally id to get it later (pass id=NULL if not set)
void					ZG_GUIWindow_AddGUIButton(ZG_GUIWindow * _this,ZG_GUIButton *_gui_button, const char *_id);
void					ZG_GUIWindow_AddGUITextBox(ZG_GUIWindow * _this,ZG_GUITextBox *_gui_textbox, const char *_id);
void					ZG_GUIWindow_AddGUITexture(ZG_GUIWindow * _this,ZG_GUITexture *_gui_texture, const char *_id);
void					ZG_GUIWindow_AddGUIFrame(ZG_GUIWindow * _this,ZG_GUIFrame *_gui_frame, const char *_id);


ZG_GUIButton		*	ZG_GUIWindow_GetGUIButton(ZG_GUIWindow * _this, const char *_id);
ZG_GUITextBox		* 	ZG_GUIWindow_GetGUITextBox(ZG_GUIWindow * _this, const char *_id);
ZG_GUITexture		* 	ZG_GUIWindow_GetGUITexture(ZG_GUIWindow * _this, const char *_id);
ZG_GUIFrame			* 	ZG_GUIWindow_GetGUIFrame(ZG_GUIWindow * _this, const char *_id);


//ZG_TTFontManager	*	ZG_GUIWindow_GetTTFontManager(ZG_GUIWindow * _this);
void 					ZG_GUIWindow_Delete(ZG_GUIWindow * _this);

#endif
