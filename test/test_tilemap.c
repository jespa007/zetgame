#include "ZetGame.h"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	TextureManager * texture_manager= TextureManager_New();
	TilemapManager *tilemap_manager=TilemapManager_New(texture_manager);
	Tilemap *tilemap=NULL;

	if(TilemapManager_Load(
			tilemap_manager
			,"../../../test/data/tilemap/mario3_world1.json"
		)){

		tilemap=TilemapManager_GetTilemap(tilemap_manager,"mario3_world1");
	}

	do{
		Graphics_BeginRender();


		if(tilemap != NULL){
			Tilemap_Draw(tilemap);
		}

	   Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	TilemapManager_Delete(tilemap_manager);
	TextureManager_Delete(texture_manager);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
