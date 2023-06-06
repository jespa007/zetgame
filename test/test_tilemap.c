#include "zetgame.h"

#define TILEMAP_LAYER_NAME "mario3_world1"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);
	ZetGameSetupParams setup=(ZetGameSetupParams){
		.graphics_api=ZG_GRAPHICS_API_GL
		,.width=224
		,.height=144
		,.wcaption_title="Tilemap"
		,.wposx=SDL_WINDOWPOS_CENTERED
		,.wposy=SDL_WINDOWPOS_CENTERED
		,.graphic_properties=0 //MSK_GRAPHIC_PROPERTY_DESKTOP | MSK_GRAPHIC_PROPERTY_NO_SHOW_ICON_TASKBAR
	};
	ZetGame_Init(&setup);

	ZG_TextureManager * texture_manager= ZG_TextureManager_New();
	TilemapManager *tilemap_manager=TilemapManager_New(texture_manager);
	Tilemap *tilemap=NULL;

	if(TilemapManager_Load(
			tilemap_manager
			,"../../../test/data/tilemap/mario3_world1.json"
		)){

		if((tilemap=TilemapManager_Get(tilemap_manager,TILEMAP_LAYER_NAME))==NULL) {
			ZG_Log_Error("Cannot get layer '%s'",TILEMAP_LAYER_NAME);
		}
	}

	do{
		Graphics_BeginRender();


		if(tilemap != NULL){
			Tilemap_Update(tilemap);
			Tilemap_Draw(tilemap);
		}

		if(K_T){
			Graphics_ToggleFullscreen();
		}

	   Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	TilemapManager_Delete(tilemap_manager);
	ZG_TextureManager_Delete(texture_manager);


	ZetGame_DeInit();

	return 0;
}
