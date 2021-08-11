#include "Texture_GL.c"


typedef struct{
	MapString 	* 	textures;
	const char 	*	texture_resource_path;
	Texture		* 	texture_embedded;
}TextureVars;

// prototypes
static TextureVars * g_texture_vars=NULL;

void			* 	Texture_OnDeleteNode(MapStringNode *node);
Texture 		* 	Texture_GetDefault(void);

//--------------
// STATIC PUBLIC
void	 	Texture_Init(void){
	if(g_texture_vars !=NULL){
		Log_Error("Texture already initialized!");
		return;
	}

	g_texture_vars=NEW(TextureVars);

	g_texture_vars->textures=NULL;
	g_texture_vars->texture_resource_path=".";
	g_texture_vars->texture_embedded=NULL;

	g_texture_vars->textures = MapString_New();//new std::map<std::string,TTFont *>();

	g_texture_vars->textures->on_delete=Texture_OnDeleteNode;
}

void	 	Texture_DeInit(){
	if(g_texture_vars ==NULL){
		Log_Error("Texture not initialized!");
		return;
	}


	// erase all loaded fonts...
	if(g_texture_vars->texture_embedded != NULL){
		Texture_Delete(g_texture_vars->texture_embedded);
		g_texture_vars->texture_embedded = NULL;
	}

	if(g_texture_vars->textures!=NULL){
		MapString_Delete(g_texture_vars->textures);
		g_texture_vars->textures=NULL;
	}


	MapString_Delete(g_texture_vars->textures);
	FREE(g_texture_vars);
	g_texture_vars=NULL;

}


Texture * 		Texture_Get(const char * _filename){
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

	if((texture=MapString_GetValue(g_texture_vars->textures,id,NULL)) == NULL){
		char filename[PATH_MAX]={0};

		strcpy(filename,_filename);

		if(File_Exists(filename) == false){
			sprintf(filename,"%s/%s",g_texture_vars->texture_resource_path,_filename);
		}

		if((texture=Texture_LoadFromFile(filename))!=NULL){
			MapString_SetValue(g_texture_vars->textures,id,texture);
		}
		else{
			texture=Texture_GetDefault();
		}
	}
	return texture;
}

void 			Texture_SetTextureResourcePath(const char * path){
	g_texture_vars->texture_resource_path=path;
}

Texture * 	Texture_GetDefault(void){
	if(g_texture_vars->texture_embedded==NULL){
		g_texture_vars->texture_embedded=Texture_New();
		Texture_UpdateFromSurface(g_texture_vars->texture_embedded,SDL_GetDefaultImage());
	}
	return g_texture_vars->texture_embedded;
}

void Texture_Bind(Texture * _this){

	if(_this == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_Bind(_this);
		break;
	}
}
//--------------
// MEMBER PUBLIC
Texture * Texture_New(){
	Texture *text=NEW(Texture);
	memset(text,0,sizeof(Texture));

	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_New(text);
		break;
	}

	return text;
}
Texture * 	Texture_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	Texture *texture=Texture_New();
	SDL_Surface *srf=SDL_NewCircle(radius, fill_color, border_width, border_color);
	Texture_UpdateFromSurface(texture,srf);
	SDL_FreeSurface(srf);
	return texture;
}

/*Texture * 	Texture_NewRectangle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	Texture *texture=Texture_New();
	//SDL_Surface *srf=SDL_NewRectangle(radius, fill_color, border_width, border_color);
	//Texture_UpdateFromSurface(texture,srf);

	return texture;
}

Texture * 	Texture_NewCross(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	Texture *texture=Texture_New();
	//SDL_Surface *srf=SDL_NewCross(radius, fill_color, border_width, border_color);
	//Texture_UpdateFromSurface(texture,srf);

	return texture;
}

Texture * 	Texture_NewArrow(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	Texture *texture=Texture_New();
	//SDL_Surface *srf=SDL_NewArrow(radius, fill_color, border_width, border_color);
	//Texture_UpdateFromSurface(texture,srf);

	return texture;
}*/

Texture * 	Texture_LoadFromMemory(uint8_t *ptr, size_t ptr_len){
	Texture * text=NULL;
	SDL_Surface * srf=NULL;

	if((srf=SDL_LoadImageFromMemory(ptr,ptr_len,0,0))!=NULL){

		text=Texture_New();
		Texture_UpdateFromSurface(text,srf);
		SDL_FreeSurface(srf);
	}

	return text;
}

Texture * Texture_LoadFromFile(const char *_filename){
	BufferByte *buffer= NULL;
	Texture *texture = NULL;

	if(FileSystem_FileExists(_filename)){
		buffer=FileSystem_ReadFile(_filename);
	}else{ // try to load using texture_resource_path
		char tmp[MAX_PATH];
		sprintf(tmp,"%s/%s",g_texture_vars->texture_resource_path,_filename);
		buffer=FileSystem_ReadFile(tmp);
	}

	if(buffer != NULL){
		texture=Texture_LoadFromMemory(buffer->ptr,buffer->len);
		BufferByte_Delete(buffer);
	}

	return texture;

}

int		 	Texture_GetHandle(Texture *_this){

	if(_this == NULL) return false;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Texture_GL_GetHandle(_this);
		break;
	}

	return INVALID_TEXTURE_HANDLE;
}

void 	Texture_SetRepeatUV(Texture *_this, bool repeat_uv){

	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_SetRepeatUV(_this,repeat_uv);
		break;
	}

}

bool 	  Texture_Update(Texture * _this,void *_pixels, uint16_t _width, uint16_t _height, uint8_t _bytes_per_pixel){

	if(_this == NULL) return false;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Texture_GL_Update(_this,_pixels, _width, _height, _bytes_per_pixel);
		break;
	}

	return false;
}


bool Texture_UpdateFromSurface(Texture *_this, SDL_Surface *srf){

	if(_this == NULL) return false;

	if(srf ==NULL) {
		Log_Warning("Surface null");
		return false;
	}
	return Texture_Update(_this,srf->pixels,srf->w,srf->h,srf->format->BytesPerPixel);
}


void	* Texture_OnDeleteNode(MapStringNode *node){
	Texture * texture = node->val;
	if(texture!=NULL){
		Texture_Delete(texture);
	}
	return NULL;
}

void Texture_Delete(Texture *_this){

	if(_this==NULL) return;


	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Texture_GL_Delete(_this);
		break;
	}

	FREE(_this);
}

