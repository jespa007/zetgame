#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

typedef struct GUITextBox GUITextBox;

struct GUITextBox{
	GUIWidget *widget;
	void *data;
};


GUITextBox *		GUITextBox_New(int x, int y, uint16_t width, uint16_t height);
void 			GUITextBox_SetText(GUITextBox *_this, const char *_text,...);
void 			GUITextBox_SetWidth(GUITextBox *_this,uint16_t _width);
void 			GUITextBox_SetHeight(GUITextBox *_this,uint16_t _height);
void			GUITextBox_SetTextAlign(GUITextBox *_this,TextAlign _text_align);
void			GUITextBox_SetVerticalAlign(GUITextBox *_this,VerticalAlign _vertical_align);
void 			GUITextBox_SetFontSize(GUITextBox *_this, uint8_t _font_size);
uint8_t			GUITextBox_GetFontSize(GUITextBox *_this);
void 			GUITextBox_SetFontName(GUITextBox *_this, const char *_font_name);
const char *	GUITextBox_GetFontName(GUITextBox *_this);
void 			GUITextBox_Delete(GUITextBox *_this);

#endif
