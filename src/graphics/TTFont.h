#ifndef __TTFONT_H__
#define __TTFONT_H__

#define MAX_CHARACTER_VALUE	128
#define TTFONT_STYLE_BOLD		(0x1 << 0)
#define TTFONT_STYLE_ITALIC		(0x1 << 1)


typedef struct TTFont 			TTFont;


struct TTFont
{
	const char *font_filename;
	MapInt *	characters;
    uint8_t 	font_size;
    uint16_t 	space_width; // in pixels
    uint32_t 	font_properties;
	uint32_t	ascender;
	float 		weight,shear;
	uint8_t 	style;

    void 		*data;
};


// members
TTFont * 		TTFont_New(FT_Face face,uint8_t font_size);
TTFont * 		TTFont_NewFromFile(const char *ttf_file, size_t font_size);
TTFont * 		TTFont_NewFromMemory(const uint8_t *buffer, size_t buffer_len, size_t font_size);

TTFont *		TTFont_Get(const char *ttf_file);
void 			TTFont_RenderTextBegin(Color4f *color);
void 			TTFont_RenderTextEnd(void);
void 			TTFont_Print(TTFont *_this,float posx, float posy,Color4f _color, const char *str,...);
void 			TTFont_WPrint(TTFont *_this,float posx, float posy,Color4f _color, const wchar_t *str,...);



uint16_t 		TTFont_GetWidth(TTFont *_this, const char *str);
uint16_t 		TTFont_WGetWidth(TTFont *_this, const wchar_t *str);
uint16_t 		TTFont_GetWidthN(TTFont *_this, const char *str, size_t len);
uint16_t 		TTFont_WGetWidthN(TTFont *_this, const wchar_t *str, size_t len);
void 			TTFont_Delete(TTFont *_this);


#endif
