#ifndef __TEXT_BOX_H__
#define __TEXT_BOX_H__

typedef enum{
	TEXT_ALIGN_LEFT		=0,
	TEXT_ALIGN_CENTER		=1,
	TEXT_ALIGN_RIGHT		=2,
	TEXT_ALIGN_JUSTIFY	=3

}TextAlign;

typedef enum{
	VERTICAL_ALIGN_TOP	=0,
	VERTICAL_ALIGN_CENTER	=1,
	VERTICAL_ALIGN_BOTTOM	=2,
}VerticalAlign;

typedef struct TextBox TextBox;

struct TextBox{
	void *data; // internal data
};

TextBox *Textbox_New(void);

void	 Textbox_SetDimensions(TextBox *_this, uint16_t w, uint16_t h);
void	 Textbox_SetTextAlign(TextBox *_this, TextAlign text_align);
void	 Textbox_SetVerticalAlign(TextBox *_this, VerticalAlign vertical_align);
void     Textbox_SetText(TextBox *_this,const char *s, ...);
void     Textbox_WSetText(TextBox *_this,const wchar_t *s, ...);
void     Textbox_SetFont(TextBox *_this, TTFont *font);
void	 Textbox_Draw(TextBox *_this, Transform *transform, Color4f *color);
void	 Textbox_Delete(TextBox *_this);

#endif
