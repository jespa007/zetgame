#include "zg_graphics.h"

typedef struct{
	MapString 	* 	fonts;
	TTFont 		* 	default_font;
	const char 	*	font_resource_path;
	TTFont 		* 	font_embedded;
	char 			default_font_name[MAX_FONT_NAME];

}TTFontManagerData;

static TTFont 		*g_font_embedded=NULL;
static FT_Library	g_ft_handler=NULL;



// STATIC
void	TTFontManager_Init(void){
	if(g_ft_handler == NULL){
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&g_ft_handler)){
			Log_Error("FREETYPE: Could not init FreeType Library");
		}
	}
}

FT_Library TTFontManager_GetFreeTypeHandler(void){
	return g_ft_handler;
}

TTFont * 		TTFontManager_GetEmbeddedFont(void){
	if(g_font_embedded == NULL){
		g_font_embedded=TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,DEFAULT_FONT_SIZE);
	}
	return g_font_embedded;
}

void	* TTFontManager_OnDeleteNode(MapStringNode *node){
	TTFont * font = node->val;
	if(font!=NULL){
		TTFont_Delete(font);
	}
	return NULL;
}


void	TTFontManager_DeInit(void){

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

// MEMBERS
TTFontManager *	TTFontManager_New(void){
	TTFontManager *tfm=NEW(TTFontManager);
	TTFontManagerData *data=NEW(TTFontManagerData);

	data->fonts=NULL;
	data->default_font=NULL;
	data->font_resource_path=".";
	g_font_embedded=NULL;
	strcpy(data->default_font_name,DEFAULT_FONT_FAMILY);

	data->fonts = MapString_New();//new std::map<std::string,TTFont *>();

	data->fonts->on_delete=TTFontManager_OnDeleteNode;

	tfm->data=data;

	return tfm;
}

TTFont * 		TTFontManager_GetFontFromFontInfo(TTFontManager *_this, TTFontInfo * font_info){
	if(font_info==NULL){
		return TTFontManager_GetEmbeddedFont();
	}

	return TTFontManager_GetFontFromName(_this,font_info->font_name, font_info->font_size);
}


TTFontInfo 		TTFontManager_GetDefaultFontInfo(TTFontManager *_this){
	TTFontManagerData *data=_this->data;
	TTFontInfo font_info;
	TTFont *font=TTFontManager_GetDefaultFont(_this);

	font_info.font_name=data->default_font_name;
	font_info.font_size=font->font_size;

	return font_info;
}

TTFont * 		TTFontManager_GetDefaultFont(TTFontManager *_this){
	TTFontManagerData *data=_this->data;
	if(data->default_font == NULL){
		return TTFontManager_GetEmbeddedFont();
	}
	return data->default_font;
}

void 			TTFontManager_SetDefaultFont(TTFontManager *_this,TTFont * _font){
	TTFontManagerData *data=_this->data;
	if(_font!=NULL){
		data->default_font=_font;
	}
}


void 			TTFontManager_SetDefaultFontName(TTFontManager *_this,const char * _default_font_name){
	TTFontManagerData *data=_this->data;
	if(data->default_font_name!=NULL){
		strcpy(data->default_font_name,_default_font_name);
	}
}

const char *	TTFontManager_GetDefaultFontName(TTFontManager *_this){
	TTFontManagerData *data=_this->data;
	return data->default_font_name;
}


void 			TTFontManager_SetFontResourcePath(TTFontManager *_this,const char * path){
	TTFontManagerData *data=_this->data;
	data->font_resource_path=path;
}

const char * 	TTFontManager_GetFontResourcePath(TTFontManager *_this){
	TTFontManagerData *data=_this->data;
	return data->font_resource_path;
}

// MEMBERS

TTFont * 		TTFontManager_GetFontFromName(TTFontManager *_this,const char * _filename,uint8_t _font_size){
	TTFontManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	TTFont * font=NULL;
	//char filename[PATH_MAX]={0};
	char *ttf_font_file_to_lower=NULL;

	id_tmp=Path_GetFilenameWithoutExtension(_filename);

	if(id_tmp == NULL){ return NULL; }

	strcpy(id,id_tmp);
	free(id_tmp);

	// 1. get filename for absolute path...
	ttf_font_file_to_lower=StrUtils_ToLower(id);
	if(ttf_font_file_to_lower==NULL){
		return NULL;
	}

	sprintf(id,"%s_s%i_hl",ttf_font_file_to_lower,_font_size);
	free(ttf_font_file_to_lower);

	if((font=MapString_GetValue(data->fonts,id,NULL))==NULL){
		if(STRCMP(ttf_font_file_to_lower,==,DEFAULT_FONT_FAMILY)){
			font=TTFont_NewFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,_font_size);
		}
		else{
			char filename[PATH_MAX]={0};

			strcpy(filename,_filename);

			if(File_Exists(filename) == false){
				sprintf(filename,"%s/%s",data->font_resource_path,_filename);
			}

			if((font=TTFont_NewFromFile(filename,_font_size))!=NULL){
				MapString_SetValue(data->fonts,id,font);
			}
			else{
				font=TTFontManager_GetEmbeddedFont();
			}
		}

	}

	return font;
}

TTFont * 		TTFontManager_GetFontFromMemory(TTFontManager *_this, const uint8_t * ptr, unsigned int ptr_len,uint8_t font_size){
	TTFontManagerData *data=_this->data;
	char id[100]={0};
	TTFont * font=NULL;
	char *ttf_font_file_to_lower=NULL;
	bool exists=false;

	// 1. get filename for absolute path...
	char *allocated_int_str=StrUtils_IntToStr((intptr_t)ptr);
	ttf_font_file_to_lower=StrUtils_ToLower(allocated_int_str);

	sprintf(id,"%s_s%i_hl",ttf_font_file_to_lower,font_size);
	free(allocated_int_str);
	free(ttf_font_file_to_lower);

	font = MapString_GetValue(data->fonts,id,&exists);

	if(exists == false){
		font=TTFont_NewFromMemory(ptr,ptr_len,font_size);
		if(font!=NULL){
			MapString_SetValue(data->fonts,id,font);
		}
	}

	return font;
}


void TTFontManager_Delete(TTFontManager *_this){
	TTFontManagerData *data=_this->data;

	// erase all loaded fonts...
	if(data->fonts!=NULL){
		MapString_Delete(data->fonts);
		data->fonts=NULL;
	}

	MapString_Delete(data->fonts);
	FREE(data);
	data=NULL;
	FREE(_this);
}
