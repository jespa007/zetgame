#include "ZetGame.h"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	TextureManager *texture_manager = TextureManager_New();
	SpriteKeyFrameManager *skfm=SpriteKeyFrameManager_New(texture_manager);
	SpriteKeyFrame *skf_current=NULL;

	if(SpriteKeyFrameManager_Load(
			skfm
			,"mario_small"
			,"../../../test/data/sprites/mario_small.json"
	)){

		skf_current=SpriteKeyFrameManager_GetSpriteKeyFrame(skfm,"mario_small_idle");
	}



	do{
		Graphics_BeginRender();

		if(skf_current != NULL){
			Graphics_DrawRectangleTextured4i(
				32
				,32
				,64
				,64
				,COLOR4F_WHITE
				,skf_current->texture
				,&skf_current->frames[0].frame
			);
		}

	   Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	SpriteKeyFrameManager_Delete(skfm);
	TextureManager_Delete(texture_manager);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
