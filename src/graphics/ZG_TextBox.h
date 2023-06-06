#ifndef __ZG_TEXT_BOX_H__
#define __ZG_TEXT_BOX_H__

typedef enum{
	ZG_HORIZONTAL_ALIGNMENT_LEFT	=0,
	ZG_HORIZONTAL_ALIGNMENT_CENTER	=1,
	ZG_HORIZONTAL_ALIGNMENT_RIGHT	=2,
	ZG_HORIZONTAL_ALIGNMENT_JUSTIFY=3

}ZG_HorizontalAlignment;

typedef enum{
	ZG_VERTICAL_ALIGNMENT_TOP		=0,
	ZG_VERTICAL_ALIGNMENT_CENTER	=1,
	VERTICAL_ALIGNMENT_BOTTOM	=2,
}ZG_VerticalAlignment;

typedef struct ZG_TextBox ZG_TextBox;

struct ZG_TextBox{
	void *data; // internal data
};

// STATIC
ZG_HorizontalAlignment 	ZG_TextBox_ParseTextAlign(const char *_text);
ZG_VerticalAlignment 		ZG_TextBox_ParseVerticalAlignment(const char *_text);


ZG_TextBox 	*	ZG_TextBox_New(void);

void			ZG_TextBox_SetDimensions(ZG_TextBox *_this, uint16_t w, uint16_t h);
void	 		ZG_TextBox_SetBorderThickness(ZG_TextBox *_this, uint16_t _border_thickness);
void	 		ZG_TextBox_SetBorderColor4f(ZG_TextBox *_this, ZG_Color4f _border_color);
void	 		ZG_TextBox_SetWidth(ZG_TextBox *_this, uint16_t _width);
void	 		ZG_TextBox_SetHeight(ZG_TextBox *_this, uint16_t _height);
void	 		ZG_TextBox_SetHorizontalAlignment(ZG_TextBox *_this, ZG_HorizontalAlignment horizontal_alignment);
void	 		ZG_TextBox_SetVerticalAlignment(ZG_TextBox *_this, ZG_VerticalAlignment vertical_alignment);
void     		ZG_TextBox_SetText(ZG_TextBox *_this,const char *s, ...);
const char 	*	ZG_TextBox_GetText(ZG_TextBox *_this);
void     		ZG_TextBox_WSetText(ZG_TextBox *_this,const wchar_t *s, ...);
void     		ZG_TextBox_SetFontFile(ZG_TextBox *_this, const char *_font_file);
void     		ZG_TextBox_SetFontSize(ZG_TextBox *_this, uint16_t _font_file);
uint16_t   		ZG_TextBox_GetFontSize(ZG_TextBox *_this);
void	 		ZG_TextBox_Draw(ZG_TextBox *_this, ZG_Transform *transform, ZG_Color4f *color);
void	 		ZG_TextBox_Delete(ZG_TextBox *_this);

#endif
