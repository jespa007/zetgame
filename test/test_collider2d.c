#include "ZetGame.h"


int main(int argc, char *argv[]){

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	Transform transform=Transform_New();

	do{
		Graphics_BeginRender();

		Geometry_Draw(Geometry_GetDefaultCircle());
		Geometry_Draw(Geometry_GetDefaultRectangle());

		Vector2i m=Input_GetMousePosition();
		float x=ViewPort_ScreenToWorldPositionX(m.x);
		float y=ViewPort_ScreenToWorldPositionY(m.y);

		transform.translate.x=x;
		transform.translate.y=y;

		Transform_Apply(&transform);
		Graphics_SetLineThickness(2);
		Geometry_Draw(Geometry_GetDefaultCircle());
		Transform_Restore(&transform);


		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
