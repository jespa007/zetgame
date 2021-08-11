#ifndef __TTFONT_H__
#define __TTFONT_H__

#define MAX_CHARACTER_VALUE	128
#define TTFONT_STYLE_BOLD		(0x1 << 0)
#define TTFONT_STYLE_ITALIC		(0x1 << 1)


typedef struct TTFont 			TTFont;


struct TTFont
{
	void *font_data;
	const char *font_filename;
	MapInt *characters;
    uint8_t font_size;
    uint16_t space_width; // in pixels
    uint32_t font_properties;
    FT_Face ft_face;
	uint32_t	ascender;
	float weight,shear;
	uint8_t 	style;
};

typedef struct{
	const char * 	font_name;
	uint8_t 		font_size;
}TTFontInfo;



// static
void			TTFont_Init(void);
void			TTFont_DeInit(void);

TTFontInfo 		TTFont_GetDefaultFontInfo(void);
TTFont * 		TTFont_GetDefaultFont(void);
void 			TTFont_SetDefaultFont(TTFont * _font);
void 			TTFont_SetDefaultFontName(const char * default_font_name);
const char *	TTFont_GetDefaultFontName(void);
void 			TTFont_SetDefaultFontSize(uint8_t font_size);
uint8_t 		TTFont_GetDefaultFontSize(void);


TTFont * 		TTFont_GetEmbeddedFont(void);
TTFont * 		TTFont_GetFontFromName(const char * font_name,uint8_t font_size);
TTFont * 		TTFont_GetFontFromMemory( const uint8_t * ptr, unsigned int ptr_len,uint8_t font_size);
TTFont * 		TTFont_GetFontFromFontInfo(TTFontInfo * font_info);
void 			TTFont_SetFontResourcePath(const char * path);
const char * 	TTFont_GetFontResourcePath(void);

// public
TTFont *	TTFont_Get(const char *ttf_file);
void 		TTFont_RenderTextBegin(Color4f *color);
void 		TTFont_RenderTextEnd(void);
void 		TTFont_Print(TTFont *_this,float posx, float posy,Color4f _color, const char *str,...);
void 		TTFont_WPrint(TTFont *_this,float posx, float posy,Color4f _color, const wchar_t *str,...);



uint16_t 		TTFont_GetWidth(TTFont *_this, const char *str);
uint16_t 		TTFont_WGetWidth(TTFont *_this, const wchar_t *str);
uint16_t 		TTFont_GetWidthN(TTFont *_this, const char *str, size_t len);
uint16_t 		TTFont_WGetWidthN(TTFont *_this, const wchar_t *str, size_t len);


#endif
