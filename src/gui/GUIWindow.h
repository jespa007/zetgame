#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

typedef struct GUIWindow GUIWindow;

struct GUIWindow{
	GUIWidget 	*widget;
	void 		*data;
};

GUIWindow * GUIWindow_New(int x, int y, uint16_t width, uint16_t height);
void 		GUIWindow_SetColorBackground3i(GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b);
void 		GUIWindow_Delete(GUIWindow * _this);

#endif
