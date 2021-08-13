
#include "assets/font/pf_arma_five.ttf.c"

#define MAX_FONT_NAME 		100
#define DEFAULT_FONT_FAMILY "pf_arma_five"
#define DEFAULT_FONT_SIZE	16

#define BOLD_WEIGHT 	1.1
#define ITALIC_SHEAR 	0.207


typedef struct{
    void 	* character_data;   // ID handle of the glyph texture
    Vector2i size;    			// Size of glyph
    Vector2i bearing;  			// Offset from baseline to left/top of glyph
    GLuint 	 advance;    		// Horizontal offset to advance to next glyph
}TTFontCharacter;


// prototypes


#include "TTFont_GL.c"


//-----
TTFont * TTFont_New(FT_Face face,uint8_t font_size){

    TTFont *font=NEW(TTFont);
    memset(font,0,sizeof(TTFont));
    FT_Set_Pixel_Sizes(face, 0, font_size);
	// Load space character
	if (FT_Load_Char(face, ' ', FT_LOAD_RENDER)==0)
	{
		font->space_width=face->glyph->advance.x>>6; // 1.0/64
	}else{
		Log_Error("FREETYTPE: Failed to load Glyph");
	}

    font->font_size=font_size;
    font->ft_face=face;
    font->ascender=face->ascender>>6;
    font->characters=MapInt_New();



    return font;
}

// transform bold/italic
void TTFont_SetTransformation(TTFont * _this, float _weight, float _shear)
{
	FT_Matrix transform;
	transform.xx = (FT_Fixed)(_weight * 65536.0f);
	transform.xy = (FT_Fixed)(_shear * 65536.0f);
	transform.yx = (FT_Fixed)(0);
	transform.yy = (FT_Fixed)(65536.0f);

	FT_Set_Transform(_this->ft_face, &transform, NULL);

	_this->weight = _weight;
	_this->shear = _shear;
}

void TTFont_setStyle(TTFont * _this, uint8_t _style)
{
	TTFont_SetTransformation(_this,(_style & TTFONT_STYLE_BOLD)?(BOLD_WEIGHT):(1.0), (_style & TTFONT_STYLE_ITALIC)?(ITALIC_SHEAR):(0.0));
	_this->style = _style;
}

TTFontCharacter * TTFont_BuildChar(TTFont * _this, unsigned long c){

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		return TTFont_GL_BuildChar(_this,c);
		break;
	default:
		break;
	}

	return NULL;
}



TTFont * TTFont_NewFromFile(const char *ttf_file, size_t font_size){

	FT_Face face;
	TTFont *font=NULL;

	// Load font as face
	if (FT_New_Face(TTFontManager_GetFreeTypeHandler(), ttf_file, 0, &face)){
		Log_Error("FT_New_Memory_Face: Cannot open file %s",ttf_file);
		return NULL;
	}

    // Create new font with size...
    font=TTFont_New(face,font_size);

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_BuildChars(font,0,MAX_CHARACTER_VALUE);
		break;
	default:
		break;

	}


	return font;
}

TTFont * TTFont_NewFromMemory(const uint8_t *buffer, size_t buffer_len, size_t font_size){
	FT_Face face;
	TTFont *font=NULL;

	// Load font as face
	if (FT_New_Memory_Face(TTFontManager_GetFreeTypeHandler(), buffer, buffer_len, 0, &face)){
		Log_Error("FT_New_Memory_Face: Failed to load");
		return NULL;
	}

    // Create new font with size...
    font=TTFont_New(face,font_size);

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_BuildChars(font,0,MAX_CHARACTER_VALUE);
		break;
	default:
		break;

	}

	return font;
}




//-------------------------------------------------------------
//
// PRINT
//


void TTFont_RenderTextBegin(Color4f *color){
	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_RenderTextBegin(color);
		break;
	default:
		break;
	}
}



void TTFont_RenderTextEnd(void){
	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_RenderTextEnd();
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------------
//
//
void TTFont_Print(TTFont *_this,float x, float y, Color4f color, const char *in,...){

	if(_this==NULL) return;

	char out[1024]={0};
	ZG_VARGS(out,in);

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_Print(_this,x,y,color,out);
		break;
	default:
		break;
	}
}

void TTFont_WPrint(TTFont *_this,float x, float y, Color4f color,const wchar_t *in,...){

	if(_this==NULL) return;

	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_WPrint(_this,x,y,color,out);
		break;
	default:
		break;
	}
}
//
//
//----------------------------------------------------------------------------------------------
uint16_t TTFont_GetWidthBuiltInt(TTFont *_this, const void *text, size_t len, CharType fftont_text){
	void *ptr=(void *)text;
	uint32_t c=0;
	int width=0;

	if(_this == NULL) return 0;

	unsigned n=0;

	while((c=StrUtils_GetCharAndAdvance(&ptr,fftont_text))!=0 && (n < len))
	{
		TTFontCharacter *ch=(TTFontCharacter *)MapInt_Get(_this->characters,c);
		if(ch==NULL){ // build
			ch=TTFont_BuildChar(_this,c);

			if(ch==NULL){
				continue;
			}
		}

		width += (ch->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		n++;
	}

	return width;

}

uint16_t 		TTFont_GetWidth(TTFont *_this, const char *str){
	return TTFont_GetWidthBuiltInt(_this,str,strlen(str),CHAR_TYPE_CHAR);
}

uint16_t 		TTFont_WGetWidth(TTFont *_this, const wchar_t *str){
	return TTFont_GetWidthBuiltInt(_this,str,wcslen(str),CHAR_TYPE_WCHAR);
}

uint16_t 		TTFont_GetWidthN(TTFont *_this, const char *str, size_t len){
	return TTFont_GetWidthBuiltInt(_this,str,len,CHAR_TYPE_CHAR);

}
uint16_t 		TTFont_WGetWidthN(TTFont *_this, const wchar_t *str, size_t len){
	return TTFont_GetWidthBuiltInt(_this,str,len,CHAR_TYPE_WCHAR);
}


void	TTFont_Delete(TTFont *_this){
	if(_this ==NULL){
		return;
	}

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_Delete(_this);
		break;
	}

	// delete all element list using free (no destructors involved) ...
	MapInt_Delete(_this->characters);

	if(_this->font_data!=NULL){
		FREE(_this->font_data);
	}

	 FT_Done_Face(_this->ft_face);

	 FREE(_this);

}



