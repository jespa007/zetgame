#include "@zg_graphics.h"

typedef struct{
	ZG_List 		* 	fonts;
	ZG_TTFont 		* 	default_font;
	const char 		*	font_resource_path;
	ZG_TTFont 		* 	font_embedded;
	char 				default_font_name[ZG_MAX_FONT_NAME];

}ZG_TTFontManagerData;

static ZG_TTFont 	*	g_font_embedded=NULL;
static FT_Library		g_ft_handler=NULL;



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
		g_font_embedded=ZG_TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,ZG_DEFAULT_FONT_SIZE);
	}
	return g_font_embedded;
}

void	 ZG_TTFontManager_OnDeleteNode(ZG_MapStringNode *node){
	ZG_TTFont * font = node->val;
	if(font!=NULL){
		ZG_TTFont_Delete(font);
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

	data->fonts->on_delete=ZG_TTFontManager_OnDeleteNode;

	tfm->data=data;

	return tfm;
}

ZG_TTFont * 		ZG_TTFontManager_GetFontFromFontInfo(ZG_TTFontManager *_this, ZG_TTFontInfo * font_info){
	if(font_info==NULL){
		return ZG_TTFontManager_GetEmbeddedFont();
	}

	return TTFontManager_GetFont(_this,font_info->font_name, font_info->font_size);
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
ZG_TTFont * 		ZG_TTFontManager_NewFont(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;
	ZG_TTFont *font=NULL;

	if(ZG_STRCMP(data->default_font_name,==,ZG_DEFAULT_FONT_FAMILY)){
		font=ZG_TTFont_New();
	}else{
		char filename[ZG_MAX_PATH]={0};
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

}

void			ZG_TTFontManager_SetFontName(
		ZG_TTFontManager *_this
		, ZG_TTFont * _font
		, const char *_font_name
){
	ZG_TTFontManagerData *data=_this->data;
	char filename[ZG_MAX_PATH]={0};
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
/*
ZG_TTFont * 		TTFontManager_GetFont(ZG_TTFontManager *_this,const char * _filename,uint8_t _font_size){
	ZG_TTFontManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	ZG_TTFont * font=NULL,*new_font=NULL;
	//char filename[ZG_MAX_PATH]={0};
	char *ttf_font_file_to_lower=NULL;

	id_tmp=ZG_Path_GetFilenameWithoutExtension(_filename);

	if(id_tmp == NULL){ return NULL; }

	strcpy(id,id_tmp);
	free(id_tmp);

	// 1. get filename for absolute path...
	ttf_font_file_to_lower=ZG_String_ToLower(id);
	if(ttf_font_file_to_lower==NULL){
		return NULL;
	}

	sprintf(id,"%s_s%i_hl",ttf_font_file_to_lower,_font_size);

	if((font=ZG_MapString_Get(data->fonts,id,NULL))==NULL){
		if(ZG_STRCMP(ttf_font_file_to_lower,==,ZG_DEFAULT_FONT_FAMILY)){
			new_font=ZG_TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,_font_size);
		}
		else{
			char filename[ZG_MAX_PATH]={0};

			strcpy(filename,_filename);

			if(ZG_File_Exists(filename) == false){
				sprintf(filename,"%s/%s",data->font_resource_path,_filename);
			}

			if((new_font=ZG_TTFont_NewFromFile(filename,_font_size))==NULL){
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

ZG_TTFont * 		TTFontManager_GetFontFromMemory(ZG_TTFontManager *_this, const uint8_t * ptr, unsigned int ptr_len,uint8_t font_size){
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
}*/


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
