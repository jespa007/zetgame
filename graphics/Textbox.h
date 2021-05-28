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

typedef struct Textbox Textbox;

struct Textbox{
	void *data; // internal data
};

Textbox *Textbox_New(void);

void	 Textbox_SetDimensions(Textbox *_this, uint16_t w, uint16_t h);
void	 Textbox_SetTextAlign(Textbox *_this, TextAlign text_align);
void	 Textbox_SetVerticalAlign(Textbox *_this, VerticalAlign vertical_align);
void     Textbox_SetText(Textbox *_this,const char *s, ...);
void     Textbox_WSetText(Textbox *_this,const wchar_t *s, ...);
void     Textbox_SetFont(Textbox *_this, TTFont *font);
void	 Textbox_Draw(Textbox *_this, Transform *transform, Color4f *color);
void	 Textbox_Delete(Textbox *_this);

#endif
