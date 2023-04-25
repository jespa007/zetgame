#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

typedef struct GUILabel GUILabel;

struct GUILabel{
	GUIWidget *widget;
	void *data;
};


GUILabel *		GUILabel_New(int x, int y, uint16_t width, uint16_t height);
void 			GUILabel_SetText(GUILabel *_this, const char *_text,...);
void 			GUILabel_SetWidth(GUILabel *_this,uint16_t _width);
void 			GUILabel_SetHeight(GUILabel *_this,uint16_t _height);
void 			GUILabel_SetFontSize(GUILabel *_this, uint8_t _font_size);
uint8_t			GUILabel_GetFontSize(GUILabel *_this);
void 			GUILabel_SetFontName(GUILabel *_this, const char *_font_name);
const char *	GUILabel_GetFontName(GUILabel *_this);
void 			GUILabel_Delete(GUILabel *_this);

#endif
