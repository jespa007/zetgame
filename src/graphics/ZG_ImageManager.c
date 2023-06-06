#include "zg_graphics.h"

typedef struct{
	MapString 	* 	images;
	const char 	*	image_resource_path;
	Image		* 	image_embedded;
}ImageManagerData;

static Image		* 	g_default_image=NULL;

//-------------
// STATIC

Image * 	ImageManager_GetDefaultImage(void){
	if(g_default_image==NULL){
		g_default_image=Image_NewFromSurface(SDL_GetDefaultImage());
	}
	return g_default_image;
}

void	ImageManager_OnDeleteNode(MapStringNode *node){
	Image * image = node->val;
	if(image!=NULL){
		Image_Delete(image);
	}
}

void	 	ImageManager_DeInit(){
	if(g_default_image != NULL){
		Image_Delete(g_default_image);
		g_default_image = NULL;
	}
}

//----------------
// PUBLIC MEMBERS

ImageManager *ImageManager_New(void){
	ImageManager 		*tm=ZG_NEW(ImageManager);
	ImageManagerData 	*data=ZG_NEW(ImageManagerData);

	data->image_resource_path=".";
	data->images = MapString_New();//new std::map<std::string,TTFont *>();
	data->images->on_delete=ImageManager_OnDeleteNode;

	tm->data=data;

	return tm;
}

void 		ImageManager_SetImageResourcePath(ImageManager *_this,const char * path){
	ImageManagerData *data=_this->data;
	data->image_resource_path=path;
}

Image * 		ImageManager_Get(ImageManager *_this,const char * _filename){
	ImageManagerData *data=_this->data;
	char *id_tmp=0;
	char id[100]={0};
	Image * image=NULL;
	//char filename[MAX_PATH];
	char *image_file_to_lower=NULL;

	id_tmp=Path_GetFilenameWithoutExtension(_filename);

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
	if((image=MapString_GetValue(data->images,id,NULL)) == NULL){
		char filename[ZG_PATH_MAX]={0};

		strcpy(filename,_filename);

		if(File_Exists(filename) == false){
			sprintf(filename,"%s/%s",data->image_resource_path,_filename);
		}

		if(File_Exists(filename) == false){
			Log_Error("File '%s' not exist either in the given path or in the resource path",_filename);
			return NULL;
		}

		if((image=Image_NewFromFile(filename))!=NULL){
			MapString_SetValue(data->images,id,image);
		}
		else{
			image=ImageManager_GetDefaultImage();
		}
	}
	return image;
}

void			ImageManager_Delete(ImageManager *_this){

	ImageManagerData 	*data=_this->data;

	if(data->images!=NULL){
		MapString_Delete(data->images);
		data->images=NULL;
	}


	MapString_Delete(data->images);
	ZG_FREE(data);
	ZG_FREE(_this);
}
