#ifndef __ZG_TTFONT_MANAGER_H__
#define __ZG_TTFONT_MANAGER_H__


struct ZG_TTFontManager{
	void *data;
};

typedef struct{
	const char  	*font_name;
	uint8_t 		font_size;
}ZG_TTFontInfo;

// STATIC
void				ZG_TTFontManager_Init(void);
ZG_TTFontInfo 		ZG_TTFontManager_GetEmbeddedFontInfo(void);
FT_Library 			ZG_TTFontManager_GetFreeTypeHandler(void);
void				ZG_TTFontManager_DeInit(void);
ZG_TTFont * 		ZG_TTFontManager_GetEmbeddedFont(void);

// MEMBER
ZG_TTFontManager *	ZG_TTFontManager_New(void);
ZG_TTFont *			ZG_TTFontManager_GetFont(
	ZG_TTFontManager *_this
	,ZG_TTFont **_font
	,const char *_font_name
	,uint8_t	_font_size
	// more things
);

void				ZG_TTFontManager_Dereference(ZG_TTFontManager * _this, ZG_TTFont **_font);
/*
ZG_TTFontInfo 		ZG_TTFontManager_GetDefaultFontInfo(ZG_TTFontManager *_this);
ZG_TTFont * 		ZG_TTFontManager_GetFontFromFontInfo(ZG_TTFontManager *_this, ZG_TTFontInfo * font_info);
ZG_TTFont * 		ZG_TTFontManager_GetDefaultFont(ZG_TTFontManager *_this);
void 				ZG_TTFontManager_SetDefaultFont(ZG_TTFontManager *_this,ZG_TTFont * _font);
void 				ZG_TTFontManager_SetDefaultFontName(ZG_TTFontManager *_this,const char * default_font_name);
const char *		ZG_TTFontManager_GetDefaultFontName(ZG_TTFontManager *_this);
void 				ZG_TTFontManager_SetDefaultFontSize(ZG_TTFontManager *_this,uint8_t font_size);
uint8_t 			ZG_TTFontManager_GetDefaultFontSize(ZG_TTFontManager *_this);


ZG_TTFont * 		ZG_TTFontManager_NewFont(ZG_TTFontManager *_this);
void				ZG_TTFontManager_SetFontName(ZG_TTFontManager *_this, ZG_TTFont * _font);*/
/*ZG_TTFont * 		TTFontManager_GetFontFromMemory( ZG_TTFontManager *_this,const uint8_t * ptr, unsigned int ptr_len,uint8_t font_size);
ZG_TTFont * 		ZG_TTFontManager_GetFontFromFontInfo(ZG_TTFontManager *_this,ZG_TTFontInfo * font_info);*/
void 				ZG_TTFontManager_SetFontResourcePath(ZG_TTFontManager *_this,const char * path);
const char * 		ZG_TTFontManager_GetFontResourcePath(ZG_TTFontManager *_this);

void				ZG_TTFontManager_Delete(ZG_TTFontManager *_this);

#endif
