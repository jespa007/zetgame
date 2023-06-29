
#include "assets/font/pf_arma_five.ttf.c"

#define ZG_MAX_FONT_NAME 		100
#define ZG_DEFAULT_FONT_FAMILY 	"pf_arma_five"
#define ZG_DEFAULT_FONT_SIZE	16

#define ZG_BOLD_WEIGHT 			1.1
#define ZG_ITALIC_SHEAR 		0.207

typedef struct{
	const char 	*	font_filename;
	ZG_MapInt 	*	characters;
    uint16_t 		font_size;
    int 			space_width; // in pixels
    uint32_t 		font_properties;
	int				ascender;
	float 			weight,shear;
	uint8_t 		style;
    ZG_Geometry 	*geometry;
    FT_Face 		ft_face;
    ZG_TTFontStyle	font_style;
    //int			max_bearing_y;
}ZG_TTFontData;

typedef struct{
    ZG_Vector2i size;    			// Size of glyph
    ZG_Vector2i bearing;  			// Offset from baseline to left/top of glyph
    int 	 advance_x;    		// Horizontal offset to advance to next glyph
    int 	 advance_y;    		// Horizontal offset to advance to next glyph
    //ZG_Rectangle4i	box;
    void 	* data;   			// ID handle of the glyph texture
}ZG_TTFontCharacter;

// prototypes

#include "ZG_TTFont_GL.c"

//-------
// GLOBAL
static ZG_TTFont 		*	g_font_embedded=NULL;
static FT_Library			g_ft_handler=NULL;
static char					g_font_resource_path[256]={0};

// Prototypes
void 					TTFont_RenderText(ZG_TTFont *_this,float _x3d, float _y3d,ZG_Color4f _color,const void *_text, ZG_CharType _char_type);

void 					TTFont_Unload(ZG_TTFont *_this);
void	 				ZG_TTFont_OnDeleteNode(ZG_MapIntNode *node);
ZG_TTFont 			* 		TTFont_NewEmpty(void);
ZG_TTFont 			*		ZG_TTFont_NewFromMemory(
	const uint8_t *buffer
	, size_t buffer_len
);


// STATIC
void	ZG_TTFont_Init(void){
	if(g_ft_handler == NULL){
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&g_ft_handler)){
			ZG_LOG_ERRORF("FREETYPE: Could not init FreeType Library");
		}
	}
}

ZG_TTFont * 		ZG_TTFont_GetEmbeddedFont(void){
	if(g_font_embedded == NULL){
		g_font_embedded=ZG_TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len);
	}
	return g_font_embedded;
}

void 			ZG_TTFont_SetFontResourcePath(const char * path){
	strcpy(g_font_resource_path,path);
}

const char * 	ZG_TTFont_GetFontResourcePath(void){
	return g_font_resource_path;
}

ZG_TTFont *ZG_TTFont_NewFromMemory(
		const uint8_t *buffer
		, size_t buffer_len
){
	ZG_TTFont *font=TTFont_NewEmpty();
	ZG_TTFont_LoadFromMemory(font, buffer,buffer_len);
	return font;
}

ZG_TTFont * ZG_TTFont_NewFromFile(
	const char *_filename
){
	ZG_TTFont *font=TTFont_NewEmpty();
	ZG_TTFont_LoadFromFile(font,_filename);
	return font;
}

void	ZG_TTFont_DeInit(void){
	// erase all loaded fonts...
	if(g_font_embedded != NULL){
		ZG_TTFont_Delete(g_font_embedded);
		g_font_embedded = NULL;
	}


	if(g_ft_handler != NULL){
		FT_Done_FreeType(g_ft_handler);
		g_ft_handler=NULL;
	}
}

//--------
// PRIVATE
ZG_TTFontCharacter *ZG_TTFont_BuildChar(ZG_TTFont *_this,unsigned long c){
	//GLuint VAO, VBO;
	ZG_TTFontCharacter *font_character=NULL;
	ZG_TTFontData *data=_this->data;
	FT_Face face=data->ft_face;

    // Load first 128 characters of ASCII set
	// Load character glyph
	if (FT_Load_Char(data->ft_face, c, FT_LOAD_RENDER))
	{
		ZG_LOG_ERRORF("FREETYTPE: Failed to load Glyph");
		return NULL;
	}

	// Now store character for later use
	font_character = ZG_NEW(ZG_TTFontCharacter);

	*font_character=(ZG_TTFontCharacter){
		.data=NULL
		,.size=ZG_Vector2i_New(face->glyph->bitmap.width, face->glyph->bitmap.rows)
		,.bearing=ZG_Vector2i_New(face->glyph->bitmap_left, face->glyph->bitmap_top)
		,.advance_x=face->glyph->advance.x
		,.advance_y=face->glyph->advance.y
		/*,.box=ZG_Rectangle4i_New(
				face->bbox.xMin>>6
				,face->bbox.yMin>>6
				,face->bbox.xMax>>6
				,face->bbox.yMax>>6

			)*/
	};
	//
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_TTFont_GL_BuildChar(font_character,face);
		break;
	default:
		break;
	}
    return font_character;
}

void TTFont_BuildChars(
	ZG_TTFont *_this
	,unsigned long char_ini
	, unsigned long char_end
){
    // Create new font with size...
	ZG_TTFontData *data=_this->data;

	ZG_MapInt_Clear(data->characters);

    FT_Set_Pixel_Sizes(data->ft_face, 0, data->font_size);
    if(FT_Set_Char_Size(
		data->ft_face
		, (FT_F26Dot6)(0)
		, (FT_F26Dot6)(data->font_size << 6)
		, 0
		, 0
	)!=0){
    	ZG_LOG_ERRORF("FREETYTPE: Can't set pizel size");
    }

   // TTFont_setStyle(_this,data->font_style);

	// Load space character
	if (FT_Load_Char(data->ft_face, ' ', FT_LOAD_RENDER)==0)
	{
		data->space_width=data->ft_face->glyph->advance.x>>6; // 1.0/64
	}else{
		ZG_LOG_ERRORF("FREETYTPE: Failed to load Glyph");
	}

	// main
    data->ascender=data->ft_face->ascender>>6;

	//int max_bearing_y=-1;
    for (unsigned long c = char_ini; c < char_end; c++)
    {
    	ZG_TTFontCharacter *font_character=ZG_TTFont_BuildChar(_this,c);
    	//max_bearing_y=MAX(max_bearing_y,font_character->bearing.y);
    	ZG_MapInt_Set(data->characters,c,font_character);
    }
    //data->max_bearing_y=max_bearing_y;
}

ZG_TTFont * TTFont_NewEmpty(void){
    ZG_TTFont *font=ZG_NEW(ZG_TTFont);
    ZG_TTFontData *data=ZG_NEW(ZG_TTFontData);
    font->data=data;

    data->characters=ZG_MapInt_New();
    data->characters->on_delete=ZG_TTFont_OnDeleteNode;
    data->font_size=ZG_DEFAULT_FONT_SIZE;

    // data
    data->geometry=ZG_Geometry_NewRectangle(ZG_GEOMETRY_PROPERTY_TEXTURE);

    // TTF load each char flipped vertically, we define inverse uv transform as usually
	float mesh_texture[]={
		   0.0f,  0.0f,   // bottom left
		   1.0f,  0.0f,   // bottom right
		   0.0f,  1.0f,   // top left
		   1.0f,  1.0f    // top right
	};
	ZG_Geometry_SetMeshTexture(data->geometry,mesh_texture,ZG_ARRAY_SIZE(mesh_texture));

	return font;
}

// transform bold/italic
void TTFont_SetTransformation(ZG_TTFont * _this, float _weight, float _shear)
{
	ZG_TTFontData *data=_this->data;

	FT_Matrix transform;
	transform.xx = (FT_Fixed)(_weight * 65536.0f);
	transform.xy = (FT_Fixed)(_shear * 65536.0f);
	transform.yx = (FT_Fixed)(0);
	transform.yy = (FT_Fixed)(65536.0f);

	FT_Set_Transform(data->ft_face, &transform, NULL);

	data->weight = _weight;
	data->shear = _shear;
}

void ZG_TTFont_SetStyle(ZG_TTFont * _this, ZG_TTFontStyle _style){
	ZG_TTFontData *data=_this->data;
	TTFont_SetTransformation(_this,(_style & ZG_TTFONT_STYLE_BOLD)?(ZG_BOLD_WEIGHT):(1.0), (_style & ZG_TTFONT_STYLE_ITALIC)?(ZG_ITALIC_SHEAR):(0.0));
	data->style = _style;
}

//---------------
// PUBLIC MEMBERS
ZG_TTFont * ZG_TTFont_New(void){
	return ZG_TTFont_NewFromMemory(
		pf_arma_five_ttf
		,pf_arma_five_ttf_len
	);
}

void ZG_TTFont_LoadFromMemory(
		ZG_TTFont *_this
		,const uint8_t *_buffer
		, size_t _buffer_len
){
	ZG_TTFontData *data=_this->data;

	TTFont_Unload(_this);

	// Load font as face
	if (FT_New_Memory_Face(g_ft_handler, _buffer, _buffer_len, 0, &data->ft_face)){
		ZG_LOG_ERRORF("FT_New_Memory_Face: Failed to load");
		return;
	}

	TTFont_BuildChars(
		_this
		,0
		,ZG_MAX_CHARACTER_VALUE
	);
}

void ZG_TTFont_LoadFromFile(
		ZG_TTFont *_this
		,const char *_filename
){
	ZG_BufferByte *buffer= NULL;
	char filename[ZG_PATH_MAX]={0};
	bool file_exists=false;

	strcpy(filename,_filename);

	if((file_exists=ZG_File_Exists(filename)) == false){
		sprintf(filename,"%s/%s",g_font_resource_path,_filename);
		file_exists=ZG_File_Exists(filename);
	}

	if(file_exists){
		buffer=ZG_FileSystem_ReadFile(filename);

		ZG_TTFont_LoadFromMemory(
				_this
				,buffer->ptr
				,buffer->len
		);
		ZG_BufferByte_Delete(buffer);

	}else{
		ZG_LOG_ERROR("File '%s' not exist",_filename);
	}
}

int				ZG_TTFont_GetAscender(ZG_TTFont *_this){
	ZG_TTFontData *data=_this->data;
	return data->ascender;
}

int				ZG_TTFont_GetSpaceWidth(ZG_TTFont *_this){
	ZG_TTFontData *data=_this->data;
	return data->space_width;
}


void	 		ZG_TTFont_SetFontSize(ZG_TTFont *_this,uint16_t _font_size){

	ZG_TTFontData *data=_this->data;
	data->font_size=_font_size;

	TTFont_BuildChars(
		_this
		,0
		,ZG_MAX_CHARACTER_VALUE
	);
}

uint16_t 		ZG_TTFont_GetFontSize(ZG_TTFont *_this){
	ZG_TTFontData *data=_this->data;
	return data->font_size;
}
//-------------------------------------------------------------
//
// PRINT
//

void ZG_TTFont_RenderTextBegin(ZG_Color4f *color){
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_TTFont_GL_RenderTextBegin(color);
		break;
	default:
		break;
	}
}

void ZG_TTFont_RenderTextEnd(void){
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_TTFont_GL_RenderTextEnd();
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------------
//
//
void TTFont_DrawCharacter(ZG_TTFontCharacter *_ch){
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_TTFont_GL_DrawCharacter(_ch);
		break;
	default:
		break;
	}
}

ZG_BoundingBox TTFont_GetBoundingBoxInternal(ZG_TTFont *_this, const void *_text, size_t len, ZG_CharType _char_type){
	ZG_TTFontData *data=_this->data;
	ZG_BoundingBox bb=ZG_BoundingBox_New4f(
			 FLT_MAX
			,FLT_MAX
			,-FLT_MAX
			,-FLT_MAX
	);

	void *ptr=(void *)_text;
	unsigned long c=0;
	unsigned n=0;
	int x=0;

	while((c=ZG_String_GetCharAndAdvance(&ptr,_char_type))!=0 && (n < len)){
		ZG_TTFontCharacter *ch=(ZG_TTFontCharacter *)ZG_MapInt_Get(data->characters,c);
		if(ch==NULL){ // build
			ch=ZG_TTFont_BuildChar(_this,c);

			if(ch==NULL){
				continue;
			}
		}

		if(x==0){
			bb.minx=ch->bearing.x;
		}
		bb.miny=MIN(bb.miny,-ch->bearing.y);
		bb.maxx=MAX(bb.maxx,x+ch->bearing.x+ch->size.x);
		bb.maxy=MAX(bb.maxy,ch->size.y-ch->bearing.y);

		x+=ch->advance_x >> 6;
		n++;
	}

	return bb;
}

ZG_BoundingBox ZG_TTFont_GetBoundingBox(ZG_TTFont *_this, const char *_text){
	return TTFont_GetBoundingBoxInternal(_this,_text,strlen(_text),ZG_CHAR_TYPE_CHAR);
}

ZG_BoundingBox ZG_TTFont_WGetBoundingBox(ZG_TTFont *_this, const wchar_t *_text){
	return TTFont_GetBoundingBoxInternal(_this,_text,wcslen(_text),ZG_CHAR_TYPE_WCHAR);
}

ZG_BoundingBox 	ZG_TTFont_GetBoundingBoxN(ZG_TTFont *_this, const char *_text, size_t len){
	return TTFont_GetBoundingBoxInternal(_this,_text,len,ZG_CHAR_TYPE_CHAR);
}

ZG_BoundingBox 	ZG_TTFont_WGetBoundingBoxN(ZG_TTFont *_this, const wchar_t *_text, size_t len){
	return TTFont_GetBoundingBoxInternal(_this,_text,len,ZG_CHAR_TYPE_WCHAR);
}

void ZG_TTFont_Print(ZG_TTFont *_this,float _x, float _y, ZG_Color4f _color, const char *in,...){

	if(_this==NULL) return;

	char out[1024]={0};
	ZG_VARGS(out,in);

	TTFont_RenderText(_this,_x,_y,_color,out,ZG_CHAR_TYPE_CHAR);

}

void ZG_TTFont_WPrint(ZG_TTFont *_this,float _x, float _y, ZG_Color4f _color,const wchar_t *in,...){

	if(_this==NULL) return;

	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	TTFont_RenderText(_this,_x,_y,_color,out,ZG_CHAR_TYPE_WCHAR);
}
//
//
//----------------------------------------------------------------------------------------------
uint16_t TTFont_GetWidthBuiltInt(ZG_TTFont *_this, const void *text, size_t len, ZG_CharType fftont_text){
	ZG_TTFontData *data=_this->data;
	void *ptr=(void *)text;
	uint32_t c=0;
	int width=0;

	if(_this == NULL) return 0;

	unsigned n=0;

	while((c=ZG_String_GetCharAndAdvance(&ptr,fftont_text))!=0 && (n < len))
	{
		ZG_TTFontCharacter *ch=(ZG_TTFontCharacter *)ZG_MapInt_Get(data->characters,c);
		if(ch==NULL){ // build
			ch=ZG_TTFont_BuildChar(_this,c);

			if(ch==NULL){
				continue;
			}
		}

		width += (ch->advance_x >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		n++;
	}
	return width;
}

uint16_t 		ZG_TTFont_GetWidth(ZG_TTFont *_this, const char *_text){
	return TTFont_GetWidthBuiltInt(_this,_text,strlen(_text),ZG_CHAR_TYPE_CHAR);
}

uint16_t 		ZG_TTFont_WGetWidth(ZG_TTFont *_this, const wchar_t *_text){
	return TTFont_GetWidthBuiltInt(_this,_text,wcslen(_text),ZG_CHAR_TYPE_WCHAR);
}

uint16_t 		ZG_TTFont_GetWidthN(ZG_TTFont *_this, const char *_text, size_t _len){
	return TTFont_GetWidthBuiltInt(_this,_text,_len,ZG_CHAR_TYPE_CHAR);

}
uint16_t 		ZG_TTFont_WGetWidthN(ZG_TTFont *_this, const wchar_t *_text, size_t _len){
	return TTFont_GetWidthBuiltInt(_this,_text,_len,ZG_CHAR_TYPE_WCHAR);
}

void TTFont_RenderText(ZG_TTFont *_this,float _x3d, float _y3d,ZG_Color4f _color,const void *_text, ZG_CharType _char_type){
	ZG_TTFontData *data=_this->data;

	if(_this == NULL) return;

	void *ptr=(void *)_text;
	unsigned long c=0;


	ZG_Graphics_SetColor4f(_color.r,_color.g, _color.b,1);

	while((c=ZG_String_GetCharAndAdvance(&ptr,_char_type))!=0)
	{
		ZG_TTFontCharacter *ch=(ZG_TTFontCharacter *)ZG_MapInt_Get(data->characters,c);
		if(ch==NULL){ // build
			ch=ZG_TTFont_BuildChar(_this,c);

			if(ch==NULL){
				continue;
			}
		}

		//int end_y=ch->size.y-ch->bearing.y+data->ascender;
		// calcule p1 and p2 center
		ZG_Vector3f p1_3d=ZG_ViewPort_ScreenToWorldDimension2i(ch->bearing.x,-ch->bearing.y);
		ZG_Vector3f p2_3d=ZG_ViewPort_ScreenToWorldDimension2i(ch->bearing.x+ch->size.x,ch->size.y-ch->bearing.y);

		const float mesh_vertex []={
				_x3d+p1_3d.x, _y3d-p1_3d.y,0,  // bottom left
				_x3d+p2_3d.x, _y3d-p1_3d.y,0,  // bottom right
				_x3d+p1_3d.x, _y3d-p2_3d.y,0,   // top left
				_x3d+p2_3d.x, _y3d-p2_3d.y,0    // top right
		};

		TTFont_DrawCharacter(ch);

		ZG_Geometry_SetMeshVertex(data->geometry,mesh_vertex,ZG_ARRAY_SIZE(mesh_vertex));
		ZG_Geometry_Draw(data->geometry);

		_x3d += ZG_ViewPort_ScreenToWorldWidth(ch->advance_x >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
}

void	ZG_TTFont_OnDeleteNode(ZG_MapIntNode *node){
	ZG_TTFontCharacter * _font_character = node->val;
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_TTFont_GL_DeleteChar(_font_character);
		break;
	}

	ZG_FREE(_font_character);
}

void	TTFont_Unload(ZG_TTFont *_this){

	ZG_TTFontData *data=_this->data;

	ZG_MapInt_Clear(data->characters);

	if(data->ft_face != NULL){
		FT_Done_Face(data->ft_face);
		data->ft_face=NULL;
	}
}

void	ZG_TTFont_Delete(ZG_TTFont *_this){

	TTFont_Unload(_this);
	ZG_TTFontData *data=_this->data;
	ZG_MapInt_Delete(data->characters);
	ZG_Geometry_Delete(data->geometry);
	ZG_FREE(_this);
	ZG_FREE(data);

}
