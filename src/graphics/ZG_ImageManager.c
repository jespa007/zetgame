#include "@zg_graphics.h"

typedef struct{
	ZG_MapString 	* 	images;
	const char 	*	image_resource_path;
	ZG_Image	* 	image_embedded;
}ZG_ImageManagerData;

static ZG_Image		* 	g_default_image=NULL;

//-------------
// STATIC

ZG_Image * 	ZG_ImageManager_GetDefaultImage(void){
	if(g_default_image==NULL){
		g_default_image=ZG_Image_NewFromSurface(SDL_GetDefaultImage());
	}
	return g_default_image;
}

void	ZG_ImageManager_OnDeleteNode(ZG_MapStringNode *node){
	ZG_Image * image = node->val;
	if(image!=NULL){
		ZG_Image_Delete(image);
	}
}

void	 	ZG_ImageManager_DeInit(){
	if(g_default_image != NULL){
		ZG_Image_Delete(g_default_image);
		g_default_image = NULL;
	}
}

//----------------
// PUBLIC MEMBERS

ZG_ImageManager *ZG_ImageManager_New(void){
	ZG_ImageManager 		*tm=ZG_NEW(ZG_ImageManager);
	ZG_ImageManagerData 	*data=ZG_NEW(ZG_ImageManagerData);

	data->image_resource_path=".";
	data->images = ZG_MapString_New();//new std::map<std::string,ZG_TTFont *>();
	data->images->on_delete=ZG_ImageManager_OnDeleteNode;

	tm->data=data;

	return tm;
}

void 		ZG_ImageManager_SetImageResourcePath(ZG_ImageManager *_this,const char * path){
	ZG_ImageManagerData *data=_this->data;
	data->image_resource_path=path;
}

ZG_Image * 		ZG_ImageManager_Get(ZG_ImageManager *_this,const char * _filename){
	ZG_ImageManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	ZG_Image * image=NULL;
	//char filename[MAX_PATH];
	char *image_file_to_lower=NULL;

	id_tmp=ZG_Path_GetFilenameWithoutExtension(_filename);

	if(id_tmp == NULL) { return NULL;}

	strcpy(id,id_tmp);
	free(id_tmp);

	// 1. get filename for absolute path...
	image_file_to_lower=ZG_String_ToLower(id);
	if(image_file_to_lower==NULL){
		return NULL;
	}

	sprintf(id,"%s",image_file_to_lower);
	free(image_file_to_lower);

	// if image not set yet, try to load
	if((image=ZG_MapString_Get(data->images,id,NULL)) == NULL){
		char filename[ZG_PATH_MAX]={0};

		strcpy(filename,_filename);

		if(ZG_File_Exists(filename) == false){
			sprintf(filename,"%s/%s",data->image_resource_path,_filename);
		}

		if(ZG_File_Exists(filename) == false){
			ZG_LOG_ERROR("File '%s' not exist either in the given path or in the resource path",_filename);
			return NULL;
		}

		if((image=ZG_Image_NewFromFile(filename))!=NULL){
			ZG_MapString_Set(data->images,id,image);
		}
		else{
			image=ZG_ImageManager_GetDefaultImage();
		}
	}
	return image;
}

void			ZG_ImageManager_Delete(ZG_ImageManager *_this){

	ZG_ImageManagerData 	*data=_this->data;

	if(data->images!=NULL){
		ZG_MapString_Delete(data->images);
		data->images=NULL;
	}


	ZG_MapString_Delete(data->images);
	ZG_FREE(data);
	ZG_FREE(_this);
}
