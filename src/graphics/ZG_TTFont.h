#ifndef __ZG_TTFONT_H__
#define __ZG_TTFONT_H__

#define ZG_MAX_CHARACTER_VALUE	128


typedef enum{
	ZG_TTFONT_STYLE_PLAIN=0x0
	,ZG_TTFONT_STYLE_BOLD=0x1
	,ZG_TTFONT_STYLE_ITALIC=0x2
}ZG_TTFontStyle;


typedef struct ZG_TTFont 			ZG_TTFont;


struct ZG_TTFont{
    void 		*data;
};


// static
ZG_TTFont * 		ZG_TTFont_NewFromFile(
		const char *ttf_file
);

ZG_TTFont * 		ZG_TTFont_GetEmbeddedFont(void);

ZG_TTFont * 		ZG_TTFont_NewFromMemory(
		const uint8_t *buffer
		, size_t buffer_len
);

// static
void			ZG_TTFont_Init(void);
ZG_TTFont * 	ZG_TTFont_GetEmbeddedFont(void);
void 			ZG_TTFont_SetFontResourcePath(const char * path);
const char * 	ZG_TTFont_GetFontResourcePath(void);
void			ZG_TTFont_DeInit(void);


// members
ZG_TTFont * 	ZG_TTFont_New(void);
void			ZG_TTFont_LoadFromFile(ZG_TTFont *_this,const char *ttf_file);
void 			ZG_TTFont_LoadFromMemory(ZG_TTFont *_this,const uint8_t *buffer, size_t buffer_len);
void	 		ZG_TTFont_SetStyle(ZG_TTFont *_this,ZG_TTFontStyle _style);
void	 		ZG_TTFont_SetFontSize(ZG_TTFont *_this,uint16_t _font_size);
uint16_t 		ZG_TTFont_GetFontSize(ZG_TTFont *_this);
int				ZG_TTFont_GetAscender(ZG_TTFont *_this);
int				ZG_TTFont_GetSpaceWidth(ZG_TTFont *_this);
void 			ZG_TTFont_RenderTextBegin(ZG_Color4f *color);
void 			ZG_TTFont_RenderTextEnd(void);
void 			ZG_TTFont_Print(ZG_TTFont *_this,float posx, float posy,ZG_Color4f _color, const char *str,...);
void 			ZG_TTFont_WPrint(ZG_TTFont *_this,float posx, float posy,ZG_Color4f _color, const wchar_t *str,...);


// Metrics
ZG_BoundingBox 	ZG_TTFont_GetBoundingBox(ZG_TTFont *_this, const char *_text);
ZG_BoundingBox 	ZG_TTFont_WGetBoundingBox(ZG_TTFont *_this, const wchar_t *_text);
ZG_BoundingBox 	ZG_TTFont_GetBoundingBoxN(ZG_TTFont *_this, const char *_text, size_t len);
ZG_BoundingBox 	ZG_TTFont_WGetBoundingBoxN(ZG_TTFont *_this, const wchar_t *_text, size_t len);
uint16_t 		ZG_TTFont_GetWidth(ZG_TTFont *_this, const char *str);
uint16_t 		ZG_TTFont_WGetWidth(ZG_TTFont *_this, const wchar_t *str);
uint16_t 		ZG_TTFont_GetWidthN(ZG_TTFont *_this, const char *str, size_t len);
uint16_t 		ZG_TTFont_WGetWidthN(ZG_TTFont *_this, const wchar_t *str, size_t len);
void 			ZG_TTFont_Delete(ZG_TTFont *_this);


#endif
