#ifndef __TTFONT_MANAGER_H__
#define __TTFONT_MANAGER_H__

typedef struct TTFontManager TTFontManager;

struct TTFontManager{
	void *data;
};

typedef struct{
	const char  	*font_name;
	uint8_t 		font_size;
}TTFontInfo;

// STATIC
void			TTFontManager_Init(void);
TTFontInfo 		TTFontManager_GetEmbeddedFontInfo(void);
FT_Library 		TTFontManager_GetFreeTypeHandler(void);
void			TTFontManager_DeInit(void);
TTFont * 		TTFontManager_GetEmbeddedFont(void);

// MEMBER
TTFontManager *	TTFontManager_New(void);

TTFontInfo 		TTFontManager_GetDefaultFontInfo(TTFontManager *_this);
TTFont * 		TTFontManager_GetFontFromFontInfo(TTFontManager *_this, TTFontInfo * font_info);
TTFont * 		TTFontManager_GetDefaultFont(TTFontManager *_this);
void 			TTFontManager_SetDefaultFont(TTFontManager *_this,TTFont * _font);
void 			TTFontManager_SetDefaultFontName(TTFontManager *_this,const char * default_font_name);
const char *	TTFontManager_GetDefaultFontName(TTFontManager *_this);
void 			TTFontManager_SetDefaultFontSize(TTFontManager *_this,uint8_t font_size);
uint8_t 		TTFontManager_GetDefaultFontSize(TTFontManager *_this);


TTFont * 		TTFontManager_NewFont(TTFontManager *_this);
void			TTFontManager_SetFontName(TTFontManager *_this, TTFont * _font);
/*TTFont * 		TTFontManager_GetFontFromMemory( TTFontManager *_this,const uint8_t * ptr, unsigned int ptr_len,uint8_t font_size);
TTFont * 		TTFontManager_GetFontFromFontInfo(TTFontManager *_this,TTFontInfo * font_info);*/
void 			TTFontManager_SetFontResourcePath(TTFontManager *_this,const char * path);
const char * 	TTFontManager_GetFontResourcePath(TTFontManager *_this);

void			TTFontManager_Delete(TTFontManager *_this);

#endif
