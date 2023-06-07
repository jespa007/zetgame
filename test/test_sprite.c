#include "zetgame.h"

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);

	ZG_TextureManager *texture_manager = ZG_TextureManager_New();
	SpriteKeyFrameManager *skfm=SpriteKeyFrameManager_New(texture_manager);
	SpriteKeyFrame *skf_current=NULL;
	Uint32 next_time=0;
	Uint32 idx_frame=0;

	if(SpriteKeyFrameManager_Load(
			skfm
			,"mario_small"
			,"../../../test/data/sprites/mario_small.json"
	)){

		skf_current=SpriteKeyFrameManager_Get(skfm,"mario_small_walk");
	}



	do{
		ZG_Graphics_BeginRender();

		if(skf_current != NULL){

			if(SDL_GetTicks()>next_time){

				idx_frame=(idx_frame+1)%skf_current->frames_len;

				next_time=SDL_GetTicks()+skf_current->frames[idx_frame].duration;

			}

			ZG_Graphics_DrawRectangleTextured4i(
				32
				,32
				,64
				,64
				,ZG_COLOR4F_WHITE
				,skf_current->texture
				,&skf_current->frames[idx_frame].frame
			);
		}

	   ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!ZG_KP_ESC);


	SpriteKeyFrameManager_Delete(skfm);
	ZG_TextureManager_Delete(texture_manager);


	ZG_DeInit();

	return 0;
}
