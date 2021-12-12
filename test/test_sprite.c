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


			/*	float uv_coords[]={
								0, 0, // bottom left
								1, 1, // bottom right
								0, 1, // top left
								1, 1  // top right

						};*/
		TextureRect tr=(TextureRect){
						.u1=0
						,.v1=0
						,.u2=1
						,.v2=1
					};

		if(skf_current != NULL){
			Graphics_DrawRectangleTextured4i(
			100
			,100
			,64
			,64
			,COLOR4F_WHITE
			,TextureManager_GetDefaultTexture()
			,NULL);//&tr);

		}


		if(skf_current != NULL){
			Graphics_DrawRectangleTextured4i(
			0
			,0
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
