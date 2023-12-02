#include "@zg_graphics.h"

typedef struct{
	ZG_MapString	* 	fonts;
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

ZG_TTFont * 		ZG_TTFontManager_GetFont(ZG_TTFontManager *_this,const char * _filename){
	ZG_TTFontManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	ZG_TTFont * font=NULL,*new_font=NULL;
	//char filename[ZG_MAX_PATH]={0};
	uint8_t _font_size=32; // decide which resolution is better
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

	if((font=ZG_MapString_Get(data->fonts,ttf_font_file_to_lower,NULL))==NULL){
		if(ZG_STRCMP(ttf_font_file_to_lower,==,ZG_DEFAULT_FONT_FAMILY)){
			return ZG_TTFontManager_GetEmbeddedFont();//ZG_TTFont_GFromMemory(pf_arma_five_ttf,pf_arma_five_ttf_len,_font_size);
		}
		else{
			char filename[MAX_PATH]={0};

			strcpy(filename,_filename);

			if(ZG_File_Exists(filename) == false){
				sprintf(filename,"%s/%s",data->font_resource_path,_filename);
			}

			if(ZG_File_Exists(filename) == false){
				ZG_LOG_ERROR("TTFontManager_GetFont : Cannot find font '%s'. Please setup font path",_filename);
				return NULL;
			}

			if((new_font=ZG_TTFont_NewFromFile(filename,_font_size))==NULL){
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


void ZG_TTFontManager_Delete(ZG_TTFontManager *_this){
	ZG_TTFontManagerData *data=_this->data;

	ZG_MapString_Delete(data->fonts);
	data->fonts=NULL;

	ZG_FREE(data);
	data=NULL;
	ZG_FREE(_this);
}
