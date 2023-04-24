#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

typedef struct GUIWindow GUIWindow;

struct GUIWindow{
	GUIWidget 	*widget;
	void 		*data;
};

GUIWindow * GUIWindow_New(int x, int y, uint16_t width, uint16_t height);
void 		GUIWindow_SetBackgroundColor3i(GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b);
void 		GUIWindow_SetBackgroundColorHexStr(GUIWindow * _this, const char * color);
void 		GUIWindow_SetCaption(GUIWindow * _this,const char *_text);
void 		GUIWindow_Delete(GUIWindow * _this);

#endif
