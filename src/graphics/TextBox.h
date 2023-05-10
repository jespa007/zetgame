#ifndef __TEXT_BOX_H__
#define __TEXT_BOX_H__

typedef enum{
	HORIZONTAL_ALIGNMENT_LEFT	=0,
	HORIZONTAL_ALIGNMENT_CENTER	=1,
	HORIZONTAL_ALIGNMENT_RIGHT	=2,
	HORIZONTAL_ALIGNMENT_JUSTIFY=3

}HorizontalAlignment;

typedef enum{
	VERTICAL_ALIGNMENT_TOP		=0,
	VERTICAL_ALIGNMENT_CENTER	=1,
	VERTICAL_ALIGNMENT_BOTTOM	=2,
}VerticalAlignment;

typedef struct TextBox TextBox;

struct TextBox{
	void *data; // internal data
};

// STATIC
HorizontalAlignment 	TextBox_ParseTextAlign(const char *_text);
VerticalAlignment 		TextBox_ParseVerticalAlignment(const char *_text);


TextBox 	*	TextBox_New(void);

void			TextBox_SetDimensions(TextBox *_this, uint16_t w, uint16_t h);
void	 		TextBox_SetBorderThickness(TextBox *_this, uint16_t _border_thickness);
void	 		TextBox_SetBorderColor4f(TextBox *_this, Color4f _border_color);
void	 		TextBox_SetWidth(TextBox *_this, uint16_t _width);
void	 		TextBox_SetHeight(TextBox *_this, uint16_t _height);
void	 		TextBox_SetHorizontalAlignment(TextBox *_this, HorizontalAlignment horizontal_alignment);
void	 		TextBox_SetVerticalAlignment(TextBox *_this, VerticalAlignment vertical_alignment);
void     		TextBox_SetText(TextBox *_this,const char *s, ...);
const char 	*	TextBox_GetText(TextBox *_this);
void     		TextBox_WSetText(TextBox *_this,const wchar_t *s, ...);
void     		TextBox_SetFontFile(TextBox *_this, const char *_font_file);
void     		TextBox_SetFontSize(TextBox *_this, uint16_t _font_file);
uint16_t   		TextBox_GetFontSize(TextBox *_this);
void	 		TextBox_Draw(TextBox *_this, Transform *transform, Color4f *color);
void	 		TextBox_Delete(TextBox *_this);

#endif
