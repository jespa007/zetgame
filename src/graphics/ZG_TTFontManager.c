#include "@zg_graphics.h"

#include "ZG_TTFont.c"

typedef struct{
	ZG_MapString	* 	fonts;
	ZG_MapInt		* 	font_pointers;

	ZG_TTFont 		* 	default_font;
	const char 		*	font_resource_path;
	ZG_TTFont 		* 	font_embedded;
	char 				default_font_name[ZG_MAX_FONT_NAME];

}ZG_TTFontManagerData;

typedef struct{
	const char 			*id; // should be deallocated when remove
	ZG_TTFontManager 	*font_manager;
	ZG_TTFont			*font; // should be deallocated when remove
	int					ref_counf; // if ref_count > 0 some resources still using it.
}ZG_TTFontNode;

typedef struct{
	ZG_TTFontManager 	*font_manager;
	ZG_TTFont			**font_pointer;
	ZG_TTFontNode		*font_node;
	const char 			*source_file; // should be deallocated
	int 				source_line;
}ZG_TTFontPointerNode;

static ZG_TTFont 	*	g_font_embedded=NULL;
static FT_Library		g_ft_handler=NULL;


// prototype
void ZG_TTFontManager_OnDeleteNode(ZG_MapStringNode *node);



// STATIC
void	ZG_TTFontManager_Init(void){
	if(g_ft_handler == NULL){
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&g_ft_handler)){
			ZG_LOG_ERRORF("FREETYPE: Could not init FreeType Library");
		}
	}
}

FT_Library ZG_TTFontManager_GetFreeTypeHandler(void){
	return g_ft_handler;
}

ZG_TTFontInfo ZG_TTFontManager_GetEmbeddedFontInfo(void){
	return (ZG_TTFontInfo){
		.font_name=ZG_DEFAULT_FONT_FAMILY
		,.font_size=ZG_DEFAULT_FONT_SIZE
	};
}

ZG_TTFont * 		ZG_TTFontManager_GetEmbeddedFont(void){
	if(g_font_embedded == NULL){
		g_font_embedded=ZG_TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len);
	}
	return g_font_embedded;
}


static void	ZG_TTFontManager_DeleteFontNode(ZG_MapStringNode * _node){
	ZG_TTFontNode * node = _node->val;
	ZG_TTFontManager *_this=node->font_manager;

	if(node->ref_counf == 0){
		ZG_Free(node->id);
		ZG_TTFontManager_DeleteFont(_this,node->font);
	}else{
		LOG_ERROR("ZG_TTFontManager_OnDeleteFontNode : Cannot delete font because there's some references using");
	}

}

void	 ZG_TTFontManager_DeleteFontPointerNode(ZG_MapStringNode *_node){
	ZG_TTFontPointerNode * font_pointer_node = _node->val;
	ZG_TTFontNode * font_node = font_pointer_node->font_node;
	ZG_TTFontManager *_this=font_pointer_node->font_manager;
	if(font_node->ref_counf <= 0){
		LOG_ERROR("ZG_TTFontManager_DeleteFontPointerNode : Internal error unconsistent negative counting references");
		return;
	}

	// deallocate source file
	ZG_Free(font_pointer_node->source_file);

	font_node->ref_counf--;

	///ZG_TTFontManager_DeReference(_this,node->font_pointer);
	if(font_node->ref_counf == 0){
		ZG_TTFontManagerData *data=_this->data;
		ZG_MapString_Erase(data->font_pointers,font_node->id);
	}

}


void	ZG_TTFontManager_DeInit(void){

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

// MEMBERS
ZG_TTFontManager *	ZG_TTFontManager_New(void){
	ZG_TTFontManager *tfm=ZG_NEW(ZG_TTFontManager);
	ZG_TTFontManagerData *data=ZG_NEW(ZG_TTFontManagerData);

	data->fonts=NULL;
	data->default_font=NULL;
	data->font_resource_path=".";
	g_font_embedded=NULL;
	strcpy(data->default_font_name,ZG_DEFAULT_FONT_FAMILY);

	data->fonts = ZG_MapString_New();//new std::map<std::string,ZG_TTFont *>();
	data->fonts->on_delete=ZG_TTFontManager_DeleteFontNode;

	data->font_pointers = ZG_MapInt_New();
	data->font_pointers->on_delete=ZG_TTFontManager_DeleteFontPointerNode;

	tfm->data=data;

	return tfm;
}

ZG_TTFont * 		ZG_TTFontManager_GetFontFromFontInfo(ZG_TTFontManager *_this, ZG_TTFontInfo * font_info){
	if(font_info==NULL){
		return ZG_TTFontManager_GetEmbeddedFont();
	}

	return ZG_TTFontManager_GetFont(_this,font_info->font_name, font_info->font_size);
}


ZG_TTFontInfo 		ZG_TTFontManager_GetDefaultFontInfo(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;
	ZG_TTFontInfo font_info;
	ZG_TTFont *font=ZG_TTFontManager_GetDefaultFont(_this);

	font_info.font_name=data->default_font_name;
	font_info.font_size=font->font_size;

	return font_info;
}

ZG_TTFont * 		ZG_TTFontManager_GetDefaultFont(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;
	if(data->default_font == NULL){
		return ZG_TTFontManager_GetEmbeddedFont();
	}
	return data->default_font;
}

void 			ZG_TTFontManager_SetDefaultFont(ZG_TTFontManager *_this,ZG_TTFont * _font){
	ZG_TTFontManagerData *data=_this->data;
	if(_font!=NULL){
		data->default_font=_font;
	}
}


void 			ZG_TTFontManager_SetDefaultFontName(ZG_TTFontManager *_this,const char * _default_font_name){
	ZG_TTFontManagerData *data=_this->data;
	if(_default_font_name!=NULL){
		strcpy(data->default_font_name,_default_font_name);
	}
}

const char *	ZG_TTFontManager_GetDefaultFontName(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;
	return data->default_font_name;
}


void 			ZG_TTFontManager_SetFontResourcePath(ZG_TTFontManager *_this,const char * path){
	ZG_TTFontManagerData *data=_this->data;
	data->font_resource_path=path;
}

const char * 	ZG_TTFontManager_GetFontResourcePath(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;
	return data->font_resource_path;
}

// MEMBERS
/*ZG_TTFont * 		ZG_TTFontManager_NewFont(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;
	ZG_TTFont *font=NULL;

	if(ZG_STRCMP(data->default_font_name,==,ZG_DEFAULT_FONT_FAMILY)){
		font=ZG_TTFont_New();
	}else{
		char filename[MAX_PATH]={0};
		strcpy(filename,data->default_font_name);

		if(ZG_File_Exists(filename) == false){
			sprintf(filename,"%s/%s",data->font_resource_path,data->default_font_name);
		}

		if(ZG_File_Exists(filename)){
			font=ZG_TTFont_NewFromFile(filename);
		}else{
			font=ZG_TTFont_New();
			ZG_LOG_ERROR("Default font '%s' not exist",data->default_font_name);
		}
	}

	ZG_List_Add(data->fonts,font);

	return font;

}*/

//----------------------------------------------------------------------------------------------------
//
// PRIVATE
//
/*
static ZG_TTFont * ZG_TTFont_NewFont(void){
    ZG_TTFont *font=ZG_NEW(ZG_TTFont);
    ZG_TTFontData *data=ZG_NEW(ZG_TTFontData);
    font->data=data;

    data->characters=ZG_MapInt_New();
    data->characters->on_delete=ZG_TTFont_DeleteNode;
    data->font_size=ZG_DEFAULT_FONT_SIZE;

    // data
    data->geometry=ZG_Geometry_NewTexturedRectangle2d();

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

static ZG_TTFont *ZG_TTFont_NewFromMemory(
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


// transform bold/italic
static void ZG_TTFont_SetTransformation(ZG_TTFont * _this, float _weight, float _shear)
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

void ZG_TTFont_SetStyle( ZG_TTFont * _this, ZG_TTFontStyle _style){
	ZG_TTFontData *data=_this->data;
	TTFont_SetTransformation(_this,(_style & ZG_TTFONT_STYLE_BOLD)?(ZG_BOLD_WEIGHT):(1.0), (_style & ZG_TTFONT_STYLE_ITALIC)?(ZG_ITALIC_SHEAR):(0.0));
	data->style = _style;
}

static void ZG_TTFont_LoadFromMemory(
		ZG_TTFontManager *_this
		,ZG_TTFont *_font
		,const uint8_t *_buffer
		, size_t _buffer_len
){
	ZG_TTFontData *data=_this->data;

	//TTFont_Unload(_font);

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

static void ZG_TTFont_LoadFromFile(
		ZG_TTFontManager *_this
		,ZG_TTFont *_font
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
}*/

void			ZG_TTFontManager_SetFontName(
		ZG_TTFontManager *_this
		, ZG_TTFont * _font
		, const char *_font_name
){
	ZG_TTFontManagerData *data=_this->data;
	char filename[MAX_PATH]={0};
	strcpy(filename,_font_name);

	if(ZG_File_Exists(filename) == false){
		sprintf(filename,"%s/%s",data->font_resource_path,_font_name);
	}

	if(ZG_File_Exists(filename)){
		ZG_TTFont_LoadFromFile(_font, filename);
	}else{
		ZG_LOG_ERROR("Font '%s' not exist",_font_name);
	}
}

// TODO: Restore GetFont.
// 1. Create a MapInt called 'font_pointers'
// 2. Change the following prototype as,
//
//    'void TTFontManager_GetFont(ZG_TTFontManager *_this,ZG_TTFont **_font,const char * _filename,uint8_t _font_size, const char * _src_file, int _line);
//
//    Description: Instead of returning a ZG_TTFont, the new function will pass a pointer of the pointer of the calling function.
//                 It has to pass also the _file and the _line that tells where the function was called.
//
// 3. Let's concatenate font name and size as '{font_name}_{size}', find requested '{font_name}_{size}' in the map string that contents the following structure,
//
//    typedef struct{
//       ZG_TTFont *font; // --> initialized as NULL
//       int ref_count; //--> initialized as 0
//    }FontInfo;
//
//    If not exist create a new slot with the ZG_TTFont instance and ref_count = 1
//
// 4. In the function we will find the pointer in the 'font_pointers'.
//	  If not exist it will create a new slot with the contents of ''{font_name}_{size}' string.
//    If exist it will find current data from '{font_name}_{size}' in the map and will decrease ref_count by -1
//    If ref_count == 0 it will delete slot and instance in the map
//    Find the new '{font_name}_{size}' in the map and increase ref_count by +1.
//

void ZG_TTFontManager_GetFont(
		ZG_TTFontManager *_this
		,ZG_TTFont **_font
		,  const char * _filename
		,uint8_t _font_size
		// ,uint8_t _font_style TODO
){
	ZG_TTFontManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	ZG_TTFont *font=NULL,*new_font=NULL;
	ZG_TTFontNode *font_node=NULL;
	ZG_TTFontPointerNode *font_pointer_node=NULL;
	//ZG_TTFont * font=NULL,*new_font=NULL;
	//char filename[MAX_PATH]={0};
	char *ttf_font_file_to_lower=NULL;

	id_tmp=ZG_Path_GetFilenameWithoutExtension(_filename);

	if(id_tmp == NULL){ return NULL; }

	strcpy(id,id_tmp);
	free(id_tmp);

	// 1. get filename for absolute path...
	ttf_font_file_to_lower=ZG_String_ToLower(id);
	if(ttf_font_file_to_lower==NULL){
		LOG_ERROR("TTFontManager_GetFont : Cannot convert to 'id' lower")
		return;
	}

	sprintf(id,"%s_s%i_hl",ttf_font_file_to_lower,_font_size);

	if((font_node=ZG_MapString_Get(data->fonts,id,NULL))==NULL){
		if(ZG_STRCMP(ttf_font_file_to_lower,==,ZG_DEFAULT_FONT_FAMILY)){
			new_font=ZG_TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,_font_size);
		}
		else{
			char filename[MAX_PATH]={0};

			strcpy(filename,_filename);

			if(ZG_File_Exists(filename) == false){
				sprintf(filename,"%s/%s",data->font_resource_path,_filename);
			}

			if((new_font=ZG_TTFontManager_NewFromFile(_this,filename,_font_size))==NULL){
				// load default font but configured by font size
				sprintf(id,"%s_s%i_hl",ZG_DEFAULT_FONT_FAMILY,_font_size);
				if((font=ZG_MapString_Get(data->fonts,id,NULL))==NULL){ // create new
					new_font=ZG_TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,_font_size);
				}
			}
		}

		if(new_font != NULL){
			font=new_font;
			ZG_MapString_Set(data->fonts,id,new_font);
		}

	}

	ZG_FREE(ttf_font_file_to_lower);

	return font;
}


// Implement Dereference function
//
// 'void TTFontManager_Dereference(ZG_TTFontManager *_this,ZG_TTFont **_font, const char * _src_file, int _line);
//
// 1. In the function we will find the pointer in the 'font_pointers'.
//	  If not exist it will show an error that the pointer is not registered
//    If exist it will find current data from '{font_name}_{size}' in the map and will decrease ref_count by -1
//    If ref_count == 0 it will delete slot and instance in the map
void ZG_TTFontManager_Dereference(ZG_TTFontManager *_this, ZG_TTFont **_font){

	ZG_TTFontManagerData *data=_this->data;
	ZG_TTFontPointerNode *font_pointer_node=ZG_MapInt_Get(data->font_pointers,_font);

	// remove font pointer
	ZG_MapInt_Erase(data->font_pointers,font_pointer_node);
}




ZG_TTFont * 		ZG_TTFontManager_GetFontFromMemory(ZG_TTFontManager *_this, const uint8_t * ptr, unsigned int ptr_len,uint8_t font_size){
	ZG_TTFontManagerData *data=_this->data;
	char id[100]={0};
	ZG_TTFont * font=NULL;
	char *ttf_font_file_to_lower=NULL;
	bool exists=false;

	// 1. get filename for absolute path...
	char *allocated_int_str=ZG_String_IntToString((intptr_t)ptr);
	ttf_font_file_to_lower=ZG_String_ToLower(allocated_int_str);

	sprintf(id,"%s_s%i_hl",ttf_font_file_to_lower,font_size);
	free(allocated_int_str);
	free(ttf_font_file_to_lower);

	font = ZG_MapString_Get(data->fonts,id,&exists);

	if(exists == false){
		font=ZG_TTFont_NewFromMemory(ptr,ptr_len,font_size);
		if(font!=NULL){
			ZG_MapString_Set(data->fonts,id,font);
		}
	}

	return font;
}


void ZG_TTFontManager_Delete(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;

	// erase all loaded fonts...
	if(data->fonts!=NULL){
		for(int i=0; i < data->fonts->size; i++){
			ZG_TTFont *font=data->fonts->items[i];
			ZG_TTFont_Delete(font);
		}
	}

	ZG_List_Delete(data->fonts);
	data->fonts=NULL;

	ZG_FREE(data);
	data=NULL;
	ZG_FREE(_this);
}
