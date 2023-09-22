
#include "assets/image/silk_icons_preview.png.c"
#include "@zg_graphics.h"
//#include "../res/image/silk_icons_preview.gif.c"

#define DEFAULT_WIDTH_ICON_PACK 316
#define DEFAULT_ICONS_PER_ROW 	16
#define DEFAULT_ICON_WIDTH 		16
#define DEFAULT_ICON_HEIGHT		16
#define DEFAULT_OFFSET_X_GRID 	0
#define DEFAULT_OFFSET_Y_GRID 	0




ZG_IconManager *g_icon_manager_default=NULL;


// STATIC

void ZG_IconManager_Init(void){
	if(g_icon_manager_default != NULL){
		ZG_LOG_ERRORF("Icon manager already init");
	}

	g_icon_manager_default=IconManager_LoadFromMemory(
		 silk_icons_preview_png
		,silk_icons_preview_png_len
		,DEFAULT_ICON_WIDTH
		,DEFAULT_ICON_HEIGHT
		,DEFAULT_OFFSET_X_GRID
		,DEFAULT_OFFSET_Y_GRID
		,DEFAULT_ICONS_PER_ROW
	);
}

void ZG_IconManager_DeInit(void){
	if(	g_icon_manager_default == NULL){
		ZG_LOG_ERRORF("Icon manager not init");
	}

	ZG_IconManager_Delete(g_icon_manager_default);

	g_icon_manager_default=NULL;
}

ZG_IconManager * IconManager_GetDefault() {
	return g_icon_manager_default;
}

//------

// PUBLIC

ZG_IconManager * IconManager_LoadFromMemory(
		const uint8_t *ptr,
		size_t ptr_len,
		uint16_t icon_width,
		uint16_t icon_height,
		uint16_t icon_offset_x,
		uint16_t icon_offset_y,
		uint16_t icons_per_row
	) {

	ZG_IconManager *icon_manager=ZG_NEW(ZG_IconManager);



	SDL_Surface *img=SDL_LoadImageFromMemory(ptr,ptr_len,SDL_LOAD_IMAGE_POWER_OF_2_ORIGINAL_RESOLUTION,0);

	if(img != NULL){
		icon_manager->texture = ZG_Texture_NewFromSurface(img);

		icon_manager->icon_width = icon_width;
		icon_manager->icon_height = icon_height;
		icon_manager->icon_offset_x = icon_offset_x;
		icon_manager->icon_offset_y = icon_offset_y;

		icon_manager->icons_per_row = icons_per_row;
		icon_manager->icon_scanline_width = icon_manager->texture->width;
	}

	if(img != NULL){

		SDL_FreeSurface(img);
	}

	return icon_manager;
}

ZG_IconManager * IconManager_LoadFromFile(
		const char  * _filename,
		uint16_t icon_width,
		uint16_t icon_height,
		uint16_t icon_offset_x,
		uint16_t icon_offset_y,
		uint16_t icons_per_row
	) {

	ZG_BufferByte *buffer = ZG_File_Read(_filename);
	ZG_IconManager *icon_manager=NULL;
	if(buffer != NULL){
		icon_manager=IconManager_LoadFromMemory(
				buffer->ptr
				,buffer->len
				,icon_width
				,icon_height
				,icon_offset_x
				,icon_offset_y
				,icons_per_row
		);
		ZG_BufferByte_Delete(buffer);
	}

	return icon_manager;
}

void ZG_IconManager_DrawIcon(ZG_IconManager * _this,uint16_t idx_icon, int x,int y,uint16_t width, uint16_t height) { // from url: "../media/image/silk_icons_preview.png".

	if(_this != NULL) {

		Icon icon=ZG_IconManager_GetIcon(_this,idx_icon);


		if(icon.texture != NULL){

			ZG_Graphics_DrawTexturedRectangle4i(x,y,width,height,ZG_COLOR4F_WHITE,icon.texture,&icon.texture_crop);
		}
	}
}

void ZG_IconManager_DrawIconDefault(uint16_t idx_icon, int x,int y,uint16_t width, uint16_t height) { // from url: "../media/image/silk_icons_preview.png".

	ZG_IconManager_DrawIcon(g_icon_manager_default,idx_icon, x,y, width,  height);
}


Icon ZG_IconManager_GetIcon(ZG_IconManager *_this,uint16_t idx_icon){
	Icon icon;
	memset(&icon,0,sizeof(Icon));

	if(_this != NULL && _this->texture != NULL){

		/*if(idx_icon >= _this->n_icons){
			ZG_LOG_ERROR("Icon out of bounds. Max icons %i >= %i", idx_icon, _this->n_icons);
			idx_icon = 0;
			//return;
		}*/

		icon.texture_crop.u1 = (idx_icon%_this->icons_per_row)*(_this->icon_width +_this->icon_offset_x);
		icon.texture_crop.v1 = (idx_icon/_this->icons_per_row)*(_this->icon_height +_this->icon_offset_y);

		icon.texture_crop.u2 = icon.texture_crop.u1 + (_this->icon_width);
		icon.texture_crop.v2 = icon.texture_crop.v1 + (_this->icon_height);

		icon.texture_crop.u1 /= (float)_this->texture->width;
		icon.texture_crop.u2 /= (float)_this->texture->width;

		icon.texture_crop.v1 /= (float)_this->texture->height;
		icon.texture_crop.v2 /= (float)_this->texture->height;


		icon.texture = _this->texture;
		//return QIcon(iconpack->copy(x,y,ICON_SIZEX,ICON_SIZEY));
	}

	return icon;
}


Icon ZG_IconManager_GetIconDefault(uint16_t idx_icon){
	return ZG_IconManager_GetIcon(g_icon_manager_default,idx_icon);
}

void ZG_IconManager_Delete(ZG_IconManager * _this){
	if(_this == NULL) return;

	ZG_Texture_Delete(_this->texture);

	ZG_FREE(_this);
}


