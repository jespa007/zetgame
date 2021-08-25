#include "ZetGame.h"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	SpriteKeyFrameManager *skfm=SpriteKeyFrameManager_New();
	SpriteKeyFrame *skf_current=NULL;

	if(SpriteKeyFrameManager_Load(
			skfm
			,"mario_small"
			,"../../../test/data/sprites/mario_small.png"
			,"../../../test/data/sprites/mario_small.json"
			,NULL
			)){

		skf_current=GetSpriteKeyFrame(skfm,"mario_small_idle");
	}



	do{
		Graphics_BeginRender();


		if(skf_current != NULL){
			Graphics_DrawRectangleTexturedTranslate2i(
			0
			,0
			,64
			,64
			,COLOR4F_WHITE
			,skf_current->texture
			,&skf_current->frames[0].frame
			);

		}

		/*glBegin(GL_TRIANGLES); // draw something with the texture on
			glVertex2f(-1,1);
			glVertex2f(1,-1);
			glVertex2f(-1,-1);
	   glEnd();*/

	   Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	SpriteKeyFrameManager_Delete(skfm);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
