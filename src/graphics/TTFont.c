
#include "assets/font/pf_arma_five.ttf.c"

#define MAX_FONT_NAME 		100
#define DEFAULT_FONT_FAMILY "pf_arma_five"
#define DEFAULT_FONT_SIZE	16

#define BOLD_WEIGHT 		1.1
#define ITALIC_SHEAR 		0.207


typedef struct{
	const char *font_filename;
	MapInt *	characters;
    uint16_t 	font_size;
    int 		space_width; // in pixels
    uint32_t 	font_properties;
	int			ascender;
	float 		weight,shear;
	uint8_t 	style;
    Geometry 	*geometry;
    FT_Face 	ft_face;
    TTFontStyle	font_style;
    //int			max_bearing_y;
}TTFontData;

typedef struct{
    Vector2i size;    			// Size of glyph
    Vector2i bearing;  			// Offset from baseline to left/top of glyph
    int 	 advance_x;    		// Horizontal offset to advance to next glyph
    int 	 advance_y;    		// Horizontal offset to advance to next glyph
    //Rectangle4i	box;
    void 	* data;   			// ID handle of the glyph texture
}TTFontCharacter;


// prototypes

#include "TTFont_GL.c"


//-------
// GLOBAL
static TTFont 		*	g_font_embedded=NULL;
static FT_Library		g_ft_handler=NULL;
static char				g_font_resource_path[256]={0};


// Prototypes
void 					TTFont_RenderText(TTFont *_this,float _x3d, float _y3d,Color4f _color,const void *_text, CharType _char_type);

void 					TTFont_Unload(TTFont *_this);
void	 				TTFont_OnDeleteNode(MapIntNode *node);
TTFont 			* 		TTFont_NewEmpty(void);
TTFont 			*		TTFont_NewFromMemory(
	const uint8_t *buffer
	, size_t buffer_len
);


// STATIC
void	TTFont_Init(void){
	if(g_ft_handler == NULL){
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&g_ft_handler)){
			Log_ErrorF("FREETYPE: Could not init FreeType Library");
		}
	}
}

TTFont * 		TTFont_GetEmbeddedFont(void){
	if(g_font_embedded == NULL){
		g_font_embedded=TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len);
	}
	return g_font_embedded;
}

void 			TTFont_SetFontResourcePath(const char * path){
	strcpy(g_font_resource_path,path);
}

const char * 	TTFont_GetFontResourcePath(void){
	return g_font_resource_path;
}

TTFont *TTFont_NewFromMemory(
		const uint8_t *buffer
		, size_t buffer_len
){
	TTFont *font=TTFont_NewEmpty();
	TTFont_LoadFromMemory(font, buffer,buffer_len);
	return font;
}


TTFont * TTFont_NewFromFile(
	const char *_filename
){
	TTFont *font=TTFont_NewEmpty();
	TTFont_LoadFromFile(font,_filename);
	return font;
}


void	TTFont_DeInit(void){
	// erase all loaded fonts...
	if(g_font_embedded != NULL){
		TTFont_Delete(g_font_embedded);
		g_font_embedded = NULL;
	}


	if(g_ft_handler != NULL){
		FT_Done_FreeType(g_ft_handler);
		g_ft_handler=NULL;
	}

}

//--------
// PRIVATE
TTFontCharacter *TTFont_BuildChar(TTFont *_this,unsigned long c){
	//GLuint VAO, VBO;
	TTFontCharacter *font_character=NULL;
	TTFontData *data=_this->data;
	FT_Face face=data->ft_face;


    // Load first 128 characters of ASCII set
	// Load character glyph
	if (FT_Load_Char(data->ft_face, c, FT_LOAD_RENDER))
	{
		Log_ErrorF("FREETYTPE: Failed to load Glyph");
		return NULL;
	}

	// Now store character for later use
	font_character = ZG_NEW(TTFontCharacter);

	*font_character=(TTFontCharacter){
		.data=NULL
		,.size=Vector2i_New(face->glyph->bitmap.width, face->glyph->bitmap.rows)
		,.bearing=Vector2i_New(face->glyph->bitmap_left, face->glyph->bitmap_top)
		,.advance_x=face->glyph->advance.x
		,.advance_y=face->glyph->advance.y
		/*,.box=Rectangle4i_New(
				face->bbox.xMin>>6
				,face->bbox.yMin>>6
				,face->bbox.xMax>>6
				,face->bbox.yMax>>6

			)*/
	};


	//
	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_BuildChar(font_character,face);
		break;
	default:
		break;
	}

    return font_character;
}

void TTFont_BuildChars(
	TTFont *_this
	,unsigned long char_ini
	, unsigned long char_end
){
    // Create new font with size...
	TTFontData *data=_this->data;

	MapInt_Clear(data->characters);

    FT_Set_Pixel_Sizes(data->ft_face, 0, data->font_size);
    if(FT_Set_Char_Size(
		data->ft_face
		, (FT_F26Dot6)(0)
		, (FT_F26Dot6)(data->font_size << 6)
		, 0
		, 0
	)!=0){
    	Log_ErrorF("FREETYTPE: Can't set pizel size");
    }

   // TTFont_setStyle(_this,data->font_style);

	// Load space character
	if (FT_Load_Char(data->ft_face, ' ', FT_LOAD_RENDER)==0)
	{
		data->space_width=data->ft_face->glyph->advance.x>>6; // 1.0/64
	}else{
		Log_ErrorF("FREETYTPE: Failed to load Glyph");
	}

	// main
    data->ascender=data->ft_face->ascender>>6;

	//int max_bearing_y=-1;
    for (unsigned long c = char_ini; c < char_end; c++)
    {
    	TTFontCharacter *font_character=TTFont_BuildChar(_this,c);
    	//max_bearing_y=MAX(max_bearing_y,font_character->bearing.y);
    	MapInt_Set(data->characters,c,font_character);
    }
    //data->max_bearing_y=max_bearing_y;
}


TTFont * TTFont_NewEmpty(void){
    TTFont *font=ZG_NEW(TTFont);
    TTFontData *data=ZG_NEW(TTFontData);
    font->data=data;

    data->characters=MapInt_New();
    data->characters->on_delete=TTFont_OnDeleteNode;
    data->font_size=DEFAULT_FONT_SIZE;

    // data
    data->geometry=Geometry_NewRectangleFilled(GEOMETRY_PROPERTY_TEXTURE);

    // TTF load each char flipped vertically, we define inverse uv transform as usually
	float mesh_texture[]={
		   0.0f,  0.0f,   // bottom left
		   1.0f,  0.0f,   // bottom right
		   0.0f,  1.0f,   // top left
		   1.0f,  1.0f    // top right
	};
	Geometry_SetMeshTexture(data->geometry,mesh_texture,ARRAY_SIZE(mesh_texture));

	return font;
}


// transform bold/italic
void TTFont_SetTransformation(TTFont * _this, float _weight, float _shear)
{
	TTFontData *data=_this->data;

	FT_Matrix transform;
	transform.xx = (FT_Fixed)(_weight * 65536.0f);
	transform.xy = (FT_Fixed)(_shear * 65536.0f);
	transform.yx = (FT_Fixed)(0);
	transform.yy = (FT_Fixed)(65536.0f);

	FT_Set_Transform(data->ft_face, &transform, NULL);

	data->weight = _weight;
	data->shear = _shear;
}

void TTFont_SetStyle(TTFont * _this, TTFontStyle _style){
	TTFontData *data=_this->data;
	TTFont_SetTransformation(_this,(_style & TTFONT_STYLE_BOLD)?(BOLD_WEIGHT):(1.0), (_style & TTFONT_STYLE_ITALIC)?(ITALIC_SHEAR):(0.0));
	data->style = _style;
}

//---------------
// PUBLIC MEMBERS
TTFont * TTFont_New(void){
	return TTFont_NewFromMemory(
		pf_arma_five_ttf
		,pf_arma_five_ttf_len
	);
}


void TTFont_LoadFromMemory(
		TTFont *_this
		,const uint8_t *_buffer
		, size_t _buffer_len
){
	TTFontData *data=_this->data;

	TTFont_Unload(_this);

	// Load font as face
	if (FT_New_Memory_Face(g_ft_handler, _buffer, _buffer_len, 0, &data->ft_face)){
		Log_ErrorF("FT_New_Memory_Face: Failed to load");
		return;
	}

	TTFont_BuildChars(
		_this
		,0
		,MAX_CHARACTER_VALUE
	);
}

void TTFont_LoadFromFile(
		TTFont *_this
		,const char *_filename
){
	BufferByte *buffer= NULL;
	char filename[PATH_MAX]={0};
	bool file_exists=false;

	strcpy(filename,_filename);

	if((file_exists=File_Exists(filename)) == false){
		sprintf(filename,"%s/%s",g_font_resource_path,_filename);
		file_exists=File_Exists(filename);
	}

	if(file_exists){
		buffer=FileSystem_ReadFile(filename);

		TTFont_LoadFromMemory(
				_this
				,buffer->ptr
				,buffer->len
		);
		BufferByte_Delete(buffer);

	}else{
		Log_Error("File '%s' not exist",_filename);
	}
}

int				TTFont_GetAscender(TTFont *_this){
	TTFontData *data=_this->data;
	return data->ascender;
}

int				TTFont_GetSpaceWidth(TTFont *_this){
	TTFontData *data=_this->data;
	return data->space_width;
}


int TTFont_GetFontWidth(TTFont *_this){
	TTFontData *data=_this->data;
	return data->space_width;
}

void	 		TTFont_SetFontSize(TTFont *_this,uint16_t _font_size){

	TTFontData *data=_this->data;
	data->font_size=_font_size;

	TTFont_BuildChars(
		_this
		,0
		,MAX_CHARACTER_VALUE
	);
}

uint16_t 		TTFont_GetFontSize(TTFont *_this){
	TTFontData *data=_this->data;
	return data->font_size;
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
void TTFont_DrawCharacter(TTFontCharacter *_ch){
	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_DrawCharacter(_ch);
		break;
	default:
		break;
	}
}

BoundingBox TTFont_GetBoundingBoxInternal(TTFont *_this, const void *_text, size_t len, CharType _char_type){
	TTFontData *data=_this->data;
	BoundingBox bb=BoundingBox_New4f(
			 FLT_MAX
			,FLT_MAX
			,-FLT_MAX
			,-FLT_MAX
	);

	void *ptr=(void *)_text;
	unsigned long c=0;
	unsigned n=0;
	int x=0;

	while((c=StrUtils_GetCharAndAdvance(&ptr,_char_type))!=0 && (n < len)){
		TTFontCharacter *ch=(TTFontCharacter *)MapInt_Get(data->characters,c);
		if(ch==NULL){ // build
			ch=TTFont_BuildChar(_this,c);

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
		//max,Vector3f p1_3d=ViewPort_ScreenToWorldDimension2i(ch->bearing.x,-ch->bearing.y);
		//Vector3f p2_3d=ViewPort_ScreenToWorldDimension2i(ch->bearing.x+ch->size.x,ch->size.y-ch->bearing.y);

		x+=ch->advance_x >> 6;
		n++;
	}

	return bb;
}

BoundingBox TTFont_GetBoundingBox(TTFont *_this, const char *_text){
	return TTFont_GetBoundingBoxInternal(_this,_text,strlen(_text),CHAR_TYPE_CHAR);
}

BoundingBox TTFont_WGetBoundingBox(TTFont *_this, const wchar_t *_text){
	return TTFont_GetBoundingBoxInternal(_this,_text,wcslen(_text),CHAR_TYPE_WCHAR);
}

BoundingBox 	TTFont_GetBoundingBoxN(TTFont *_this, const char *_text, size_t len){
	return TTFont_GetBoundingBoxInternal(_this,_text,len,CHAR_TYPE_CHAR);
}

BoundingBox 	TTFont_WGetBoundingBoxN(TTFont *_this, const wchar_t *_text, size_t len){
	return TTFont_GetBoundingBoxInternal(_this,_text,len,CHAR_TYPE_WCHAR);
}


/*
void TTFont_GL_Print(TTFont *_this,float x, float y,Color4f color,const char *str){
	TTFont_RenderText(_this,x,y,color,str,CHAR_TYPE_CHAR);
}

void TTFont_GL_WPrint(TTFont *_this,float x, float y,Color4f color,const wchar_t *str){
	TTFont_RenderText(_this,x,y,color,str,CHAR_TYPE_WCHAR);
}*/

void TTFont_Print(TTFont *_this,float _x, float _y, Color4f _color, const char *in,...){

	if(_this==NULL) return;

	char out[1024]={0};
	ZG_VARGS(out,in);

	TTFont_RenderText(_this,_x,_y,_color,out,CHAR_TYPE_CHAR);

}

void TTFont_WPrint(TTFont *_this,float _x, float _y, Color4f _color,const wchar_t *in,...){

	if(_this==NULL) return;

	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	TTFont_RenderText(_this,_x,_y,_color,out,CHAR_TYPE_WCHAR);
}
//
//
//----------------------------------------------------------------------------------------------
uint16_t TTFont_GetWidthBuiltInt(TTFont *_this, const void *text, size_t len, CharType fftont_text){
	TTFontData *data=_this->data;
	void *ptr=(void *)text;
	uint32_t c=0;
	int width=0;

	if(_this == NULL) return 0;

	unsigned n=0;

	while((c=StrUtils_GetCharAndAdvance(&ptr,fftont_text))!=0 && (n < len))
	{
		TTFontCharacter *ch=(TTFontCharacter *)MapInt_Get(data->characters,c);
		if(ch==NULL){ // build
			ch=TTFont_BuildChar(_this,c);

			if(ch==NULL){
				continue;
			}
		}

		width += (ch->advance_x >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		n++;
	}

	return width;

}

uint16_t 		TTFont_GetWidth(TTFont *_this, const char *_text){
	return TTFont_GetWidthBuiltInt(_this,_text,strlen(_text),CHAR_TYPE_CHAR);
}

uint16_t 		TTFont_WGetWidth(TTFont *_this, const wchar_t *_text){
	return TTFont_GetWidthBuiltInt(_this,_text,wcslen(_text),CHAR_TYPE_WCHAR);
}

uint16_t 		TTFont_GetWidthN(TTFont *_this, const char *_text, size_t _len){
	return TTFont_GetWidthBuiltInt(_this,_text,_len,CHAR_TYPE_CHAR);

}
uint16_t 		TTFont_WGetWidthN(TTFont *_this, const wchar_t *_text, size_t _len){
	return TTFont_GetWidthBuiltInt(_this,_text,_len,CHAR_TYPE_WCHAR);
}

void TTFont_RenderText(TTFont *_this,float _x3d, float _y3d,Color4f _color,const void *_text, CharType _char_type){
	TTFontData *data=_this->data;

	if(_this == NULL) return;

	void *ptr=(void *)_text;
	unsigned long c=0;


	Graphics_SetColor4f(_color.r,_color.g, _color.b,1);

	while((c=StrUtils_GetCharAndAdvance(&ptr,_char_type))!=0)
	{
		TTFontCharacter *ch=(TTFontCharacter *)MapInt_Get(data->characters,c);
		if(ch==NULL){ // build
			ch=TTFont_BuildChar(_this,c);

			if(ch==NULL){
				continue;
			}
		}

		//int end_y=ch->size.y-ch->bearing.y+data->ascender;
		// calcule p1 and p2 center
		Vector3f p1_3d=ViewPort_ScreenToWorldDimension2i(ch->bearing.x,-ch->bearing.y);
		Vector3f p2_3d=ViewPort_ScreenToWorldDimension2i(ch->bearing.x+ch->size.x,ch->size.y-ch->bearing.y);

		const float mesh_vertex []={
				_x3d+p1_3d.x, _y3d-p1_3d.y,0,  // bottom left
				_x3d+p2_3d.x, _y3d-p1_3d.y,0,  // bottom right
				_x3d+p1_3d.x, _y3d-p2_3d.y,0,   // top left
				_x3d+p2_3d.x, _y3d-p2_3d.y,0    // top right
		};

		TTFont_DrawCharacter(ch);

		Geometry_SetMeshVertex(data->geometry,mesh_vertex,ARRAY_SIZE(mesh_vertex));
		Geometry_Draw(data->geometry);

		_x3d += ViewPort_ScreenToWorldWidth(ch->advance_x >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
}

void	TTFont_OnDeleteNode(MapIntNode *node){
	TTFontCharacter * _font_character = node->val;
	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		TTFont_GL_DeleteChar(_font_character);
		break;
	}

	ZG_FREE(_font_character);
}

void	TTFont_Unload(TTFont *_this){

	TTFontData *data=_this->data;

	MapInt_Clear(data->characters);

	if(data->ft_face != NULL){
		FT_Done_Face(data->ft_face);
		data->ft_face=NULL;
	}


}

void	TTFont_Delete(TTFont *_this){

	TTFont_Unload(_this);
	TTFontData *data=_this->data;
	MapInt_Delete(data->characters);
	Geometry_Delete(data->geometry);
	ZG_FREE(_this);
	ZG_FREE(data);

}



