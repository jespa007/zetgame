#include "zg_graphics.h"

typedef struct{
	MapString 	* 	textures;
	const char 	*	texture_resource_path;
	Texture		* 	texture_embedded;
}TextureManagerData;

static Texture		* 	g_default_texture=NULL;

//-------------
// STATIC

Texture * 	TextureManager_GetDefaultTexture(void){
	if(g_default_texture==NULL){
		g_default_texture=Texture_New();
		Texture_UpdateFromSurface(g_default_texture,0,0,SDL_GetDefaultImage());
	}
	return g_default_texture;
}

void	TextureManager_OnDeleteNode(MapStringNode *node){
	Texture * texture = node->val;
	if(texture!=NULL){
		Texture_Delete(texture);
	}
}

void	 	TextureManager_DeInit(){
	if(g_default_texture != NULL){
		Texture_Delete(g_default_texture);
		g_default_texture = NULL;
	}
}

//----------------
// PUBLIC MEMBERS

TextureManager *TextureManager_New(void){
	TextureManager 		*tm=NEW(TextureManager);
	TextureManagerData 	*data=NEW(TextureManagerData);

	data->texture_resource_path=".";
	data->textures = MapString_New();//new std::map<std::string,TTFont *>();
	data->textures->on_delete=TextureManager_OnDeleteNode;

	tm->data=data;

	return tm;
}

void 		TextureManager_SetTextureResourcePath(TextureManager *_this,const char * path){
	TextureManagerData *data=_this->data;
	data->texture_resource_path=path;
}

Texture * 		TextureManager_Get(TextureManager *_this,const char * _filename){
	TextureManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	Texture * texture=NULL;
	//char filename[MAX_PATH];
	char *texture_file_to_lower=NULL;

	id_tmp=Path_GetFilenameWithoutExtension(_filename);

	if(id_tmp == NULL) { return NULL;}

	strcpy(id,id_tmp);
	free(id_tmp);

	// 1. get filename for absolute path...
	texture_file_to_lower=StrUtils_ToLower(id);
	if(texture_file_to_lower==NULL){
		return NULL;
	}

	sprintf(id,"%s",texture_file_to_lower);
	free(texture_file_to_lower);

	// if texture not set yet, try to load
	if((texture=MapString_GetValue(data->textures,id,NULL)) == NULL){
		char filename[PATH_MAX]={0};

		strcpy(filename,_filename);

		if(File_Exists(filename) == false){
			sprintf(filename,"%s/%s",data->texture_resource_path,_filename);
		}

		if(File_Exists(filename) == false){
			Log_Error("File '%s' not exist either in the given path or in the resource path",_filename);
			return NULL;
		}

		if((texture=Texture_NewFromFile(filename))!=NULL){
			MapString_SetValue(data->textures,id,texture);
		}
		else{
			texture=TextureManager_GetDefaultTexture();
		}
	}
	return texture;
}

void			TextureManager_Delete(TextureManager *_this){

	TextureManagerData 	*data=_this->data;

	if(data->textures!=NULL){
		MapString_Delete(data->textures);
		data->textures=NULL;
	}


	MapString_Delete(data->textures);
	FREE(data);
	FREE(_this);
}
