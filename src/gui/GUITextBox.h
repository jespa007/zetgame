#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

typedef struct GUITextBox GUITextBox;

struct GUITextBox{
	GUIWidget *widget;
	void *data;
};


GUITextBox *	GUITextBox_New(int x, int y, uint16_t width, uint16_t height);
void 			GUITextBox_SetWidth(GUITextBox *_this,uint16_t _width);
void 			GUITextBox_SetHeight(GUITextBox *_this,uint16_t _height);
void 			GUITextBox_SetText(GUITextBox *_this, const char *_text,...);
void			GUITextBox_SetHorizontalAlignment(GUITextBox *_this,HorizontalAlignment _text_align);
void			GUITextBox_SetVerticalAlignment(GUITextBox *_this,VerticalAlignment _vertical_align);
void 			GUITextBox_SetBorderThickness(GUITextBox *_this,int _thickness);
void 			GUITextBox_SetBorderColorHtml(GUITextBox *_this,const char *_color_html);
void 			GUITextBox_SetFontSize(GUITextBox *_this, uint8_t _font_size);
uint16_t		GUITextBox_GetFontSize(GUITextBox *_this);
void 			GUITextBox_SetFontFile(GUITextBox *_this, const char *_font_name);
void 			GUITextBox_Delete(GUITextBox *_this);

#endif
