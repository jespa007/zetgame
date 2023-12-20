#ifndef __ZG_TTFONT_MANAGER_H__
#define __ZG_TTFONT_MANAGER_H__

typedef struct ZG_TTFontManager ZG_TTFontManager;

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
	,const char *_font_name
);

void 				ZG_TTFontManager_SetFontResourcePath(ZG_TTFontManager *_this,const char * path);
const char * 		ZG_TTFontManager_GetFontResourcePath(ZG_TTFontManager *_this);

void				ZG_TTFontManager_Delete(ZG_TTFontManager *_this);

#endif
