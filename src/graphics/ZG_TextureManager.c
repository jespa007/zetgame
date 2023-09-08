#include "@zg_graphics.h"

typedef struct{
	ZG_MapString 	* 	textures;
	const char 	*	texture_resource_path;
	ZG_Texture		* 	texture_embedded;
}ZG_TextureManagerData;

static ZG_Texture		* 	g_default_texture=NULL;

//-------------
// STATIC

ZG_Texture * 	ZG_TextureManager_GetDefaultTexture(void){
	if(g_default_texture==NULL){
		g_default_texture=ZG_Texture_NewFromSurface(SDL_GetDefaultImage());
	}
	return g_default_texture;
}

void	ZG_TextureManager_OnDeleteNode(ZG_MapStringNode *node){
	ZG_Texture * texture = node->val;
	if(texture!=NULL){
		ZG_Texture_Delete(texture);
	}
}

void	 	ZG_TextureManager_DeInit(){
	if(g_default_texture != NULL){
		ZG_Texture_Delete(g_default_texture);
		g_default_texture = NULL;
	}
}

//----------------
// PUBLIC MEMBERS

ZG_TextureManager *ZG_TextureManager_New(void){
	ZG_TextureManager 		*tm=ZG_NEW(ZG_TextureManager);
	ZG_TextureManagerData 	*data=ZG_NEW(ZG_TextureManagerData);

	data->texture_resource_path=".";
	data->textures = ZG_MapString_New();//new std::map<std::string,ZG_TTFont *>();
	data->textures->on_delete=ZG_TextureManager_OnDeleteNode;

	tm->data=data;

	return tm;
}

void 		ZG_TextureManager_SetTextureResourcePath(ZG_TextureManager *_this,const char * path){
	ZG_TextureManagerData *data=_this->data;
	data->texture_resource_path=path;
}

ZG_Texture * 		ZG_TextureManager_Get(ZG_TextureManager *_this,const char * _filename){

	if(_this==NULL){
		return NULL;
	}

	ZG_TextureManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	ZG_Texture * texture=NULL;
	//char filename[MAX_PATH];
	char *texture_file_to_lower=NULL;

	id_tmp=ZG_Path_GetFilenameWithoutExtension(_filename);

	if(id_tmp == NULL) { return NULL;}

	strcpy(id,id_tmp);
	free(id_tmp);

	// 1. get filename for absolute path...
	texture_file_to_lower=ZG_String_ToLower(id);
	if(texture_file_to_lower==NULL){
		return NULL;
	}

	sprintf(id,"%s",texture_file_to_lower);
	free(texture_file_to_lower);

	// if texture not set yet, try to load
	if((texture=ZG_MapString_Get(data->textures,id,NULL)) == NULL){
		char filename[ZG_PATH_MAX]={0};

		strcpy(filename,_filename);

		if(ZG_File_Exists(filename) == false){
			sprintf(filename,"%s/%s",data->texture_resource_path,_filename);
		}

		if(ZG_File_Exists(filename) == false){
			ZG_LOG_ERROR("File '%s' not exist either in the given path or in the resource path",_filename);
			return NULL;
		}

		if((texture=ZG_Texture_NewFromFile(filename))!=NULL){
			ZG_MapString_Set(data->textures,id,texture);
		}
		else{
			texture=ZG_TextureManager_GetDefaultTexture();
		}
	}
	return texture;
}

void			ZG_TextureManager_Delete(ZG_TextureManager *_this){

	ZG_TextureManagerData 	*data=_this->data;

	if(data->textures!=NULL){
		ZG_MapString_Delete(data->textures);
		data->textures=NULL;
	}


	ZG_MapString_Delete(data->textures);
	ZG_FREE(data);
	ZG_FREE(_this);
}
