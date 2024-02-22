#include "zetgame.h"


int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);

	do{
		// Event update
		ZG_Input_Update();

		// toggle fullscreen
		if(ZG_KP_F9){
			ZG_Graphics_ToggleFullscreen();
		}

		ZG_Graphics_BeginRender();

		ZG_Graphics_EndRender();



	}while(!ZG_KP_ESC);

	ZG_DeInit();

	return 0;
}
